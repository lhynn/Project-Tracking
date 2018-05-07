#include "videomanager.h"
#include <errno.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <linux/videodev2.h>
#include <fcntl.h>


static int xioctl(int fd, int request, void* argp)
{
    int r;
    do r = ioctl(fd, request, argp);
    while (-1 == r && EINTR == errno);

    return r;
}
 
int V4l2InitDevice(char *strDevName,PT_VideoDevice ptVideoDevice){
	struct stat st;
	int iFd;
	// stat file
	if (-1 == stat(strDevName, &st)) {
		fprintf(stderr, "Cannot identify '%s': %d, %s\n", strDevName, errno, strerror (errno));
		exit(EXIT_FAILURE);
	}
	// check if its device
	if (!S_ISCHR (st.st_mode)) {
		fprintf(stderr, "%s is no device\n",strDevName);
        exit(EXIT_FAILURE);
	}
	// open device
	iFd = open(strDevName, O_RDWR);
	if (-1 == iFd) {
		fprintf(stderr, "Cannot open '%s': %d, %s\n", strDevName, errno, strerror (errno));
        return -1;
	}
	ptVideoDevice->iFd = iFd;

	struct v4l2_format fmt;
	struct v4l2_input input;
	CLEAR(input);
	input.index = 0;
	int rtn = xioctl(iFd, VIDIOC_S_INPUT, &input);
	if (rtn < 0) {
		printf("VIDIOC_S_INPUT:rtn(%d)\n", rtn);
        close(iFd);
        return -1;
	}

	CLEAR (fmt);

	// v4l2_format
	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = IWIDTH;
	fmt.fmt.pix.height      = IHEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32;//V4L2_PIX_FMT_RGB32;//V4L2_PIX_FMT_NV12;//V4L2_PIX_FMT_RGB32;
        fmt.fmt.pix.field       = V4L2_FIELD_ANY;
	//fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;//V4L2_FIELD_NONE;

	if (-1 == xioctl(iFd, VIDIOC_S_FMT, &fmt)){
		close(iFd);
		return -1;
	}
	//	#if 1
	fmt.type = V4L2_BUF_TYPE_PRIVATE;
	if (-1 == xioctl(iFd, VIDIOC_S_FMT, &fmt))
	{
		printf("************** %s, line = %d\n", __FUNCTION__, __LINE__);
        close(iFd);
        return -1;
	}
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//	#endif
//	#if 1
	if (-1 == xioctl(iFd, VIDIOC_G_FMT, &fmt))
	{
		printf("************** %s, line = %d\n", __FUNCTION__, __LINE__);
        close(iFd);
        return -1;
	}
//	#endif
	/* Note VIDIOC_S_FMT may change width and height. */
	ptVideoDevice->iWidth =  fmt.fmt.pix.width;
	ptVideoDevice->iHeight = fmt.fmt.pix.height;

	struct v4l2_requestbuffers req;
	int i;

	CLEAR (req);
	req.count               = NB_BUFFER;
	req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory              = V4L2_MEMORY_MMAP;

	if (-1 == xioctl(iFd, VIDIOC_REQBUFS, &req)) {
		if (EINVAL == errno) {
			fprintf(stderr, "%s does not support memory mapping\n",strDevName);
            close(iFd);
            return -1;
		}
	}

	if (req.count < NB_BUFFER) {
			fprintf(stderr, "Insufficient buffer memory on %s\n", strDevName);
            close(iFd);
            return -1;
	}
		
	ptVideoDevice->iVideoBufCnt = req.count;

	for (i = 0; i < ptVideoDevice->iVideoBufCnt; i++) {
		struct v4l2_buffer buf;

		CLEAR (buf);

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;

		if (-1 == xioctl(iFd, VIDIOC_QUERYBUF, &buf)){
				printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
                close(iFd);
                return -1;
		}
		ptVideoDevice->iVideoBufMaxLen = buf.length;

        ptVideoDevice->pucVideoBuf[i] = (unsigned char *)mmap (NULL , buf.length, PROT_READ | PROT_WRITE , MAP_SHARED,iFd, buf.m.offset);

		if (MAP_FAILED == ptVideoDevice->pucVideoBuf[i]){
				printf("mmap error.\n");
                close(iFd);
                return -1;
			}
	}


	for (i = 0; i < ptVideoDevice->iVideoBufCnt; ++i)
    {
        struct v4l2_buffer buf;

        CLEAR (buf);
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = i;

        if (-1 == xioctl(iFd, VIDIOC_QBUF, &buf)){
			printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
            close(iFd);
            return -1;
		} 
	}
	return 0;
}

int V4l2ExitDevice(PT_VideoDevice ptVideoDevice){
	int i;
	for(i = 0; i < ptVideoDevice->iVideoBufCnt; i++){
		if(ptVideoDevice->pucVideoBuf[i]){
			munmap(ptVideoDevice->pucVideoBuf[i],ptVideoDevice->iVideoBufMaxLen);
			ptVideoDevice->pucVideoBuf[i] = NULL;
		}
	}
	close(ptVideoDevice->iFd);
	return 0;
}
static struct v4l2_buffer buf;

int V4l2ReadFarme(PT_VideoDevice ptVideoDevice,PT_VideoBuf ptVideoBuf){

	struct pollfd tFd[1];
	int iRet;
	tFd[0].fd = ptVideoDevice->iFd;
	tFd[0].events = POLLIN;

	iRet = poll(tFd, 1,-1);
	if(iRet < 0){
		printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
		return -1;
	}

	CLEAR (buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	if (-1 == xioctl(ptVideoDevice->iFd, VIDIOC_DQBUF, &buf)) {
		switch (errno) {
			case EAGAIN:
				return 0;
			case EIO:
			default:
				printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
		}
	}
	ptVideoDevice->iVideoBufCurIndex = buf.index;
	ptVideoBuf->iWidth				 = ptVideoDevice->iWidth;
	ptVideoBuf->iHeight				 = ptVideoDevice->iHeight;
	ptVideoBuf->iBpp				 = 4;
	ptVideoBuf->iLineBytes			 = ptVideoDevice->iWidth*ptVideoBuf->iBpp/8;
	ptVideoBuf->iTotalBytes			 = buf.bytesused;
	ptVideoBuf->aucPixelDatas		 = ptVideoDevice->pucVideoBuf[buf.index];
	return 0;
}
int V4l2PutFarme(PT_VideoDevice ptVideoDevice){
    if (-1 == xioctl(ptVideoDevice->iFd, VIDIOC_QBUF, &buf))
        printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__);
	return 0;
}

int V4l2StartCapture(PT_VideoDevice ptVideoDevice){
	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int iError;

	iError = xioctl(ptVideoDevice->iFd, VIDIOC_STREAMON, &type);
	if(iError){
		printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__); 
		return -1;
	}

	return 0;
}

 int V4l2StopCapture(PT_VideoDevice ptVideoDevice){

	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int iError;
	iError = xioctl(ptVideoDevice->iFd, VIDIOC_STREAMON, &type);
	if(iError){
	printf("function:<%s>:line-%d\n",__FUNCTION__,__LINE__); 
	return -1;
	}	
	return 0;
}

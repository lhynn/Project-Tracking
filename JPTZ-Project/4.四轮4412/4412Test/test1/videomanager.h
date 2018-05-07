#ifndef __VIDEOMANAGER_H__
#define __VIDEOMANAGER_H__


#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define NB_BUFFER		 2
#define     IWIDTH      720
#define     IHEIGHT     576

typedef struct VideoDevice T_VideoDevice,*PT_VideoDevice;

typedef struct VideoBuf{
    int iWidth;				/*宽度,一行有多少个像素*/
    int iHeight;			/*高度,一列有多少个像素*/
    int iBpp;				/*一个像素用多少为表示*/
    int iLineBytes;			/*一行数据有多少个字节*/
    int iTotalBytes;		/*所有字节数*/
    unsigned char *aucPixelDatas;	/*像素数据存储的地方*/
}T_VideoBuf,*PT_VideoBuf;

struct VideoDevice
{
    int iFd;
    int iWidth;
    int iHeight;

    int iVideoBufCnt;
    int iVideoBufMaxLen;
    int iVideoBufCurIndex;
    unsigned char* pucVideoBuf[NB_BUFFER];
    /*function*/
};

int V4l2InitDevice(char *strDevName,PT_VideoDevice ptVideoDevice);

int V4l2StartCapture(PT_VideoDevice ptVideoDevice);

int V4l2ReadFarme(PT_VideoDevice ptVideoDevice,PT_VideoBuf ptVideoBuf);
int V4l2PutFarme(PT_VideoDevice ptVideoDevice);

int V4l2StopCapture(PT_VideoDevice ptVideoDevice);

int V4l2ExitDevice(PT_VideoDevice ptVideoDevice);
#endif

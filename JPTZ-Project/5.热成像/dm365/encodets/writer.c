/*
 * writer.c
 *
 * This source file has the implementations for the writer thread
 * functions implemented for 'DVSDK encode demo' on DM365 platform
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>

#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include <xdc/std.h>

#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/BufTab.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <ti/sdo/dmai/Rendezvous.h>

#include "writer.h"
#include "../demo.h"
#include <sys/socket.h>
#include <netinet/in.h> 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../ctrl.h"


//#include <libavutil/opt.h>
//#include <libavutil/mathematics.h>
//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libswresample/swresample.h>

#include <dirent.h>
#include <sys/vfs.h>


/* Number of buffers in writer pipe */
#define NUM_WRITER_BUFS         5//10


//unsigned int   gl_frame_count;
//unsigned char  idr_frame_promote = 0;

char gl_file[64];
char gl_gps_file[64];



unsigned int create_default_filename(char* filestr, char* gpsfilestr)
{
	DIR *dir;
	struct dirent *ptr;

	unsigned int file_max_count = 0;
	unsigned int current_file_count = 0;
	char filename[64];
	char strbuf[10];
	
	dir = opendir("/home/root/media/");
	while( (ptr = readdir(dir)) != NULL)
	{
		memset(filename, 0, 64);
		memcpy(filename, ptr->d_name, 64); 

		memset(strbuf, 0, 10);
		memcpy(strbuf, filename, 2);

		if( strcmp("V_", strbuf) == 0) //test first 8 byte of filename
		{
			current_file_count = (filename[2]-'0')*10000 + (filename[3]-'0')*1000 + (filename[4]-'0')*100 + (filename[5]-'0')*10+(filename[6]-'0');

			if(current_file_count > file_max_count)
			{
				file_max_count = current_file_count;
			}
		}
	}
	close(dir);

	file_max_count += 1;

	sprintf(filename, "/home/root/media/V_%05d.264", file_max_count);
	memcpy(filestr, filename, strlen(filename));

	sprintf(filename, "/home/root/media/V_%05d.txt", file_max_count);
	memcpy(gpsfilestr, filename, strlen(filename));

}



#define   MP4_FILE_LENGTH           54000//46500//(15500*3) //10min x 60 sec x 30frame = 18000    //(15000)//10min x 60 sec x 25frame = 15000
#define   MP4_FILE_LENGTH_OVERFLOW  (MP4_FILE_LENGTH + 1000)


/******************************************************************************
 * writerThrFxn
 ******************************************************************************/
Void *writerThrFxn(Void *arg)
{
    WriterEnv          *envp            = (WriterEnv *) arg;
    Void               *status          = THREAD_SUCCESS;
    FILE               *outFile         = NULL;
    Buffer_Attrs        bAttrs          = Buffer_Attrs_DEFAULT;
    BufTab_Handle       hBufTab         = NULL;
    Buffer_Handle       hOutBuf;
    Int                 fifoRet;
    Int                 bufIdx;
	pthread_mutex_t mutex_buffer;

	char *buf;
	unsigned int Num = 0;
	struct  tm *times;
	time_t t;

	pthread_mutex_init(&mutex_buffer, NULL);

	gl_frame_count = MP4_FILE_LENGTH_OVERFLOW;   //set 16000 > 15000,to create mp4 file //10min x 60 sec x 25frame = 15000
	envp->writeDisabled = 1;

    /*
     * Create a table of buffers for communicating buffers to
     * and from the video thread.
     */
    hBufTab = BufTab_create(NUM_WRITER_BUFS, envp->outBufSize, &bAttrs);

    if (hBufTab == NULL) {
        ERR("Failed to allocate contiguous buffers\n");
        cleanup(THREAD_FAILURE);
    }

    /* Send all buffers to the video thread to be filled with encoded data */
    for (bufIdx = 0; bufIdx < NUM_WRITER_BUFS; bufIdx++) 
	{
        if (Fifo_put(envp->hOutFifo, BufTab_getBuf(hBufTab, bufIdx)) < 0) {
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }
    }

    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);
    while (TRUE) {
		
        /* Get an encoded buffer from the video thread */
		
        fifoRet = Fifo_get(envp->hInFifo, &hOutBuf);
        if (fifoRet < 0) 
		{
            ERR("Failed to get buffer from video thread\n");
            cleanup(THREAD_FAILURE);
        }

        /* Did the video thread flush the fifo? */
        if (fifoRet == Dmai_EFLUSH) {
            cleanup(THREAD_SUCCESS);
        }
		extern unsigned char uart_command;
		if(gl_cam_time/60 == 10){
					gl_cam_time = 0;
					goto OPENFILE;
		}	
        if( (gl_system_status == STATUS_REC) && (video_command == CMD_STOP) )
		{
			video_command = CMD_NONE;
			gl_system_status = STATUS_STOP;		
			envp->writeDisabled = 1; //do not write 
			
			printf("stop rec command\r\n");
		//	system("sync");  //file write sync control!!!!!!
		//	pthread_mutex_lock(&mutex_buffer);

		//	memset(gl_file, 0, 64);
		//	memset(gl_gps_file, 0, 64);
		//	create_default_filename(gl_file, gl_gps_file);
		//	pthread_mutex_unlock(&mutex_buffer);

 
		//	if (outFile) 
		//	{	
		//		fflush(outFile);
		//		fsync(outFile);
		//		fclose(outFile);
		//	} 
		
		}
		else if( (gl_system_status == STATUS_STOP) && (video_command == CMD_REC) )
	 	{
			video_command = CMD_NONE;
			gl_system_status = STATUS_REC;	
			printf("rec REC \n");
			envp->writeDisabled = 0; //start write
			goto OPENFILE;
		}

        if (!envp->writeDisabled) 
		{
            /* Store the encoded frame to disk */
            if (Buffer_getNumBytesUsed(hOutBuf)) 
			{
				uint8_t* ptr = Buffer_getUserPtr(hOutBuf);
				uint32_t length = Buffer_getNumBytesUsed(hOutBuf);
		           
//				CheckoutDiskSpace();

			    if( (gl_frame_count >= MP4_FILE_LENGTH) )
			    {	
					system("sync");  //file write sync control!!!!!!						
					pthread_mutex_lock(&mutex_buffer);

					memset(gl_file, 0, 64);
					memset(gl_gps_file, 0, 64);
	 				create_default_filename(gl_file, gl_gps_file);
					pthread_mutex_unlock(&mutex_buffer);

					if (outFile) 
					{
						fflush(outFile);
						fsync(outFile);
						fclose(outFile);
					}					
OPENFILE:
	
				memset(gl_file, 0, 64);
				memset(gl_gps_file, 0, 64);
				create_default_filename(gl_file, gl_gps_file);
					outFile = fopen(gl_file, "w");
					
					gl_frame_count = 0;
					idr_frame_promote = 10;

			    }

				if(idr_frame_promote < 10)
				{
		            if (fwrite(ptr, length, 1, outFile) != 1)
					{
						printf("============write error===========\r\n");
		            }

					fflush(outFile);
					fsync(outFile);
					gl_frame_count ++;
				}
            }
            else {
                printf("Warning, writer received 0 byte encoded frame\n");
            }
        }

        /* Return buffer to capture thread */
        if (Fifo_put(envp->hOutFifo, hOutBuf) < 0) {
            ERR("Failed to send buffer to display thread\n");
            cleanup(THREAD_FAILURE);
        }
    }

cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);
    Fifo_flush(envp->hOutFifo);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (outFile) 
	{
	
		fflush(outFile);
		fsync(outFile);

        fclose(outFile);
    }

    if (hBufTab) {
        BufTab_delete(hBufTab);
    }
    return status;
}

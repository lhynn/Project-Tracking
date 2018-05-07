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
#include <dirent.h>
#include <sys/vfs.h>
#include "../ctrl.h"
#include "avilib.h"


/* Number of buffers in writer pipe */
#define NUM_WRITER_BUFS         10




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
	avi_t*              avifile         = NULL;
	char videofile[64];


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
        if (fifoRet < 0) {
            ERR("Failed to get buffer from video thread\n");
            cleanup(THREAD_FAILURE);
        }
        /* Did the video thread flush the fifo? */
        if (fifoRet == Dmai_EFLUSH) {
            cleanup(THREAD_SUCCESS);
        }

        if( (gl_system_status == STATUS_REC) && (video_command == CMD_STOP) ) //while rec and get stop command
		{
			video_command = CMD_NONE;
			gl_system_status = STATUS_STOP;

			printf("stop rec command\r\n");
			if (avifile != NULL )
			{
				AVI_close(avifile);
				system("sync");  //file write sync control!!!!!!
			}
			avifile = NULL;
			envp->writeDisabled = 1; //do not write 
		}
		else if( (gl_system_status == STATUS_STOP) && (video_command == CMD_REC) )
	 	{
			video_command = CMD_NONE;
			gl_system_status = STATUS_REC;

//			printf("start rec command\r\n");	
			envp->writeDisabled = 0; //allow write file flag
			goto OPENFILE;
		}

        if (!envp->writeDisabled) 
		{
            /* Store the encoded frame to disk */
            if (Buffer_getNumBytesUsed(hOutBuf)) 
			{
				uint8_t* ptr = Buffer_getUserPtr(hOutBuf);
				uint32_t length = Buffer_getNumBytesUsed(hOutBuf);
		           
			    if( (gl_frame_count >= VIDEO_FILE_LENGTH) )   ////a h264 file is written larger enough
			    {	
					system("sync");  //file write sync control!!!!!!	
					if (avifile != NULL )
					{
						printf("close avi file\r\n");
						AVI_close(avifile);
						avifile  = NULL;
					}
					system("sync");  //file write sync control!!!!!!	


OPENFILE:           //create another AVI file
					avifile = NULL;
					memset(videofile, 0, 64);
					create_filename(videofile, "avi");

					if(avifile == NULL)
					{
						avifile = AVI_open_output_file(videofile);
						printf("open avi file: %s\r\n", videofile);
						if (avifile == NULL ) 
						{
						    printf(stderr,"Error opening avifile %s\n", videofile);
						}
						else 
						{
						    AVI_set_video(avifile, AVI_WIDTH, AVI_HEIGHT, AVI_FRAMERATE, "H264");   ///width ,height .framerate
						}
					
					}
					gl_frame_count = 0;
					idr_frame_promote = 10;
			    }

				if(idr_frame_promote < 5)
				{
					if (avifile != NULL )
					{
						AVI_write_frame(avifile, (char *)ptr, length, gl_frame_count);
					}
					gl_frame_count ++;
				}
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
		AVI_close(avifile);
    }

    if (hBufTab) {
        BufTab_delete(hBufTab);
    }
    return status;
}

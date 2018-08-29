/*
 * audio.c
 *
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2009
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>

#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Sound.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/Loader.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/ce/Aenc1.h>


#include "audio.h"
#include "../demo.h"
#include "avilib.h"

extern avi_t* avifile;
extern pthread_mutex_t avmutex;

uint8_t* aacData;
long aacSize;


/*
Sound_Handle	hSound = NULL;

void decode_snd_pause()
{
	if(hSound != NULL)
	{
		printf("sound pause\r\n");
		Sound_alsa_pause(hSound, 1);
	}
}

void decode_snd_resume()
{
	if(hSound != NULL)
	{
		printf("sound play\r\n");
		Sound_alsa_pause(hSound, 0);
	}
}

*/
/******************************************************************************
 * audioThrFxn
 ******************************************************************************/
Void *audioThrFxn(Void *arg)
{
    AudioEnv               *envp                = (AudioEnv *) arg;
    Void                   *status              = THREAD_SUCCESS;
    Sound_Attrs             sAttrs              = Sound_Attrs_MONO_DEFAULT;
    Buffer_Attrs            bAttrs              = Buffer_Attrs_DEFAULT;
    AUDENC1_Params          defaultParams       = Aenc1_Params_DEFAULT;
    AUDENC1_DynamicParams   defaultDynParams    = Aenc1_DynamicParams_DEFAULT;
    Engine_Handle           hEngine             = NULL;
    Sound_Handle            hSound              = NULL;
    Aenc1_Handle            hAe1                = NULL;
    Buffer_Handle           hOutBuf             = NULL;
    Buffer_Handle           hInBuf              = NULL;
    Buffer_Handle           hEncInBuf           = NULL;
    FILE                   *outFile             = NULL;
    AUDENC1_Params         *params;
    AUDENC1_DynamicParams  *dynParams;
    static Int offset = 0;
    static Int wmacodec = 0;

#if 0
    /* Open the output file for writing */
    outFile = fopen(envp->audioFile, "w");

    if (outFile == NULL) {
        ERR("Failed to open %s for writing\n", envp->audioFile);
        cleanup(THREAD_FAILURE);
    }
#endif

#if 0
    /* Open the codec engine */
    hEngine = Engine_open(envp->engineName, NULL, NULL);

    if (hEngine == NULL) {
        ERR("Failed to open codec engine %s\n", envp->engineName);
        cleanup(THREAD_FAILURE);
    }

    /* Use supplied params if any, otherwise use defaults */
    params = envp->params ? envp->params : &defaultParams;
    dynParams = envp->dynParams ? envp->dynParams : &defaultDynParams;

    params->sampleRate = dynParams->sampleRate = envp->sampleRate;
    params->bitRate = dynParams->bitRate = envp->soundBitRate;
    
    /* Create the audio encoder */
    hAe1 = Aenc1_create(hEngine, envp->audioEncoder, params, dynParams);

    if (hAe1 == NULL) {
        ERR("Failed to create audio encoder: %s\n", envp->audioEncoder);
        cleanup(THREAD_FAILURE);
    }

    /* Ask the codec how much space it needs for output data */
    hOutBuf = Buffer_create(Aenc1_getOutBufSize(hAe1), &bAttrs);

    /* Buffer specifically for WMA to read 8KBytes of data*/

    hEncInBuf = Buffer_create(Aenc1_getInBufSize(hAe1)/4, &bAttrs);
 /* Ask the codec how much input data it needs */
    hInBuf = Buffer_create(Aenc1_getInBufSize(hAe1), &bAttrs);
   
    if (hInBuf == NULL || hOutBuf == NULL || hEncInBuf == NULL) {
        ERR("Failed to allocate audio buffers\n");
        cleanup(THREAD_FAILURE);
    }
#endif

  

    /* Set the sample rate for the user interface */
 //   gblSetSamplingFrequency(envp->sampleRate);


	hInBuf = Buffer_create(4096, &bAttrs);

    /* Create the sound device */
    sAttrs.sampleRate = 16000;            //envp->sampleRate;
    sAttrs.soundInput = Sound_Input_LINE; //Sound_Input_MIC;           //envp->soundInput;
	sAttrs.channels   = 1;
    sAttrs.mode       = Sound_Mode_INPUT;

    hSound = Sound_create(&sAttrs);

    if (hSound == NULL) {
        ERR("Failed to create audio device\n");
        cleanup(THREAD_FAILURE);
    }

//	Sound_alsa_pause(hSound, 0);

    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);

extern packetcnt;

    while (!gblGetQuit()) {
        /* Pause processing? */
        Pause_test(envp->hPauseProcess);
 
		/* Read samples from the Sound device */
		if (Sound_read(hSound, hInBuf) < 0) {
			ERR("Failed to write audio buffer\n");
			cleanup(THREAD_FAILURE);
		}

        /* Write encoded buffer to the speech file */
        if (!envp->writeDisabled) 
        {
			if (Buffer_getNumBytesUsed(hInBuf)) 
			{
				pthread_mutex_lock(&avmutex);
				if (avifile != NULL )
				{
					AVI_write_audio(avifile, Buffer_getUserPtr(hInBuf), Buffer_getNumBytesUsed(hInBuf));
				}
				pthread_mutex_unlock(&avmutex);
			}
            else 
			{
                printf("Warning, zero bytes audio encoded\n");
            }
        }
/*
   		if (Sound_write(hSound, hInBuf) < 0) 
		{
			ERR("Failed to write audio buffer\n");
			cleanup(THREAD_FAILURE);
		}
*/
    }

#if 0
    while (!gblGetQuit()) {
        /* Pause processing? */
        Pause_test(envp->hPauseProcess);
 
		/* Read samples from the Sound device */
		if (Sound_read(hSound, hInBuf) < 0) {
			ERR("Failed to write audio buffer\n");
			cleanup(THREAD_FAILURE);
		}
		/* Encode the audio buffer */
		if (Aenc1_process(hAe1, hInBuf, hOutBuf) < 0) {
			ERR("Failed to encode audio buffer\n");
			cleanup(THREAD_FAILURE);
		}
      
        /* Write encoded buffer to the speech file */
        if (!envp->writeDisabled) 
        {

			if (Buffer_getNumBytesUsed(hOutBuf)) 
			{
//printf("aac Buffer_getNumBytesUsed:%d\n", Buffer_getNumBytesUsed(hOutBuf)); 

              if (fwrite(Buffer_getUserPtr(hOutBuf),
                       Buffer_getNumBytesUsed(hOutBuf), 1, outFile) != 1) {
                    ERR("Error writing the encoded data to speech file.\n");
                    cleanup(THREAD_FAILURE);
                }

//				aacData = Buffer_getUserPtr(hOutBuf);
//				aacSize = Buffer_getNumBytesUsed(hOutBuf);
//				printf("aacsize = %d--------------------j =-----%d\r\n\r\n\r\n",aacSize, j);
				if (avifile != NULL )
				{
//printf("avi Buffer_getNumBytesUsed:%d\n", Buffer_getNumBytesUsed(hOutBuf)); 
					pthread_mutex_lock(&avmutex);
					AVI_write_audio(avifile, Buffer_getUserPtr(hOutBuf), Buffer_getNumBytesUsed(hOutBuf));
					pthread_mutex_unlock(&avmutex);
				}
			}
            else 
			{
                printf("Warning, zero bytes audio encoded\n");
            }
        }

       printf("Buffer_getNumBytesUsed:%d\n", Buffer_getNumBytesUsed(hOutBuf)); 

        /* Increment the number of bytes encoded for the user interface */
        gblIncSoundBytesProcessed(Buffer_getNumBytesUsed(hOutBuf));
    }
#endif
cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Pause_off(envp->hPauseProcess);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);

    /* Clean up the thread before exiting */
    if (hAe1) {
        Aenc1_delete(hAe1);
    }

    if (hSound) {
        Sound_delete(hSound);
    }

    if (hInBuf) {
        Buffer_delete(hInBuf);
    }
    if (hEncInBuf) {
        Buffer_delete(hEncInBuf);
    }

    if (hOutBuf) {
        Buffer_delete(hOutBuf);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }

    if (avifile) {
        AVI_close(avifile);
    }
/*
    if (outFile) {
        fclose(outFile);
    }
*/
    return status;
}

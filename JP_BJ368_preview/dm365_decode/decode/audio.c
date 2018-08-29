/*
 * audio.c
 *
 * This source file has the implementations for the Audio Thread implemented 
 * for the decode demo on DM6467 platform.
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
 *
*/

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>

#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Sound.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/ce/Adec1.h>

#include "../demo.h"
#include "audio.h"
#include "avilib.h"
#include "../ctrl.h"

extern pthread_mutex_t avmutex;
extern avi_t* avifile;
extern int frames;
extern int gl_videoend;
char audiobuf[4096];
int audiolen;
static int frame;
unsigned long audioframe;

Sound_Handle	hSound = NULL;

void decode_snd_pause()
{
	if(hSound != NULL)
	{
		printf("sound pause\r\n");
		//Sound_alsa_pause(hSound, 1);
	}
}

void decode_snd_resume()
{
	if(hSound != NULL)
	{
		printf("sound play\r\n");
	//	Sound_alsa_pause(hSound, 0);
	}
}

void decode_snd_start()
{
	if (hSound) 
	{
	//	Sound_alsa_start(hSound);
	}
}


/******************************************************************************
 * audioThrFxn
 ******************************************************************************/
Void *audioThrFxn(Void *arg)
{
    AudioEnv               *envp                = (AudioEnv *) arg;
    Void                   *status              = THREAD_SUCCESS;
    Sound_Attrs             sAttrs              = Sound_Attrs_MONO_DEFAULT;
    Buffer_Attrs            bAttrs              = Buffer_Attrs_DEFAULT;
    AUDDEC1_Params          defaultParams       = Adec1_Params_DEFAULT;
    AUDDEC1_DynamicParams   defaultDynParams    = Adec1_DynamicParams_DEFAULT;
    Engine_Handle           hEngine             = NULL;
//    Sound_Handle            hSound              = NULL;
    Adec1_Handle            hAd1                = NULL;
    Buffer_Handle           hOutBuf             = NULL;
    Buffer_Handle           hInBuf				= NULL;
    AUDDEC1_Params         *params;
    AUDDEC1_DynamicParams  *dynParams;
    Int                     ret;

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

    params->dataEndianness = XDM_LE_16;

    /* Create the audio decoder */
    hAd1 = Adec1_create(hEngine, envp->audioDecoder, params, dynParams);

    if (hAd1 == NULL) {
        ERR("Failed to create audio decoder %s\n", envp->audioDecoder);
        cleanup(THREAD_FAILURE);
    }

    /* Create an output buffer for decoded data */
    hOutBuf = Buffer_create(Adec1_getOutBufSize(hAd1), &bAttrs);

	hInBuf = Buffer_create(Adec1_getInBufSize(hAd1), &bAttrs);//amob 

    printf("*********************out size =%d\r\n", Adec1_getInBufSize(hAd1) );
    if (hOutBuf == NULL) {
        ERR("Failed to allocate output buffer\n");
        cleanup(THREAD_FAILURE);
    }
#endif


	hInBuf = Buffer_create(4096, &bAttrs);//amob  4096 is enough
    if (hInBuf == NULL) 
	{
        ERR("Failed to allocate output buffer\n");
        cleanup(THREAD_FAILURE);
    }


    /* Signal that initialization is done and wait for other threads */
    Rendezvous_meet(envp->hRendezvousInit);

    if ((ret == Dmai_EFAIL)||
        (ret == Dmai_EBITERROR && Buffer_getNumBytesUsed(hInBuf) == 0)) {
            ERR("Failed to decode audio buffer. Stream may have ended \
                with partial frame.\n");
            cleanup(THREAD_FAILURE);
    }

//	system("amixer sset 'Mono DAC' 70");

    /* Create the sound device */
    sAttrs.sampleRate = 16000;
	sAttrs.channels   = 1;
	sAttrs.mode = Sound_Mode_FULLDUPLEX;
//    sAttrs.leftGain  = 127;
//    sAttrs.rightGain = 127;
    hSound = Sound_create(&sAttrs);

    if (hSound == NULL) {
        ERR("Failed to create audio device\n");
        cleanup(THREAD_FAILURE);
    }

	audioframe = 0;

//	decode_snd_start();

	while (!gblGetQuit() && avifile != NULL) 
    {	
		/* Pause processing? */
		Pause_test(envp->hPauseProcess);
	
		pthread_mutex_lock(&avmutex);				
		audiolen = AVI_audio_size(avifile, audioframe ++);
		AVI_read_audio(avifile, (char *)audiobuf, audiolen);
		pthread_mutex_unlock(&avmutex);

//			printf("read audiolen in audiothread is %d\r\n",audiolen);	

		Buffer_setUserPtr(hInBuf, audiobuf);
		Buffer_setSize(hInBuf, audiolen);
		Buffer_setNumBytesUsed(hInBuf, audiolen);

		if (Sound_write(hSound, hInBuf) < 0) 
		{
			ERR("Failed to write audio buffer\n");
			cleanup(THREAD_FAILURE);
		}


		if(gl_videoend)
           break;
	}

cleanup:
    /* Make sure the other threads aren't waiting for us */
    Rendezvous_force(envp->hRendezvousInit);
    Rendezvous_force(envp->hRendezvousLoop);
    Pause_off(envp->hPauseProcess);

    /* Meet up with other threads before cleaning up */
    Rendezvous_meet(envp->hRendezvousCleanup);


    if (hAd1) {
        Adec1_delete(hAd1);
    }

    if (hSound) {
        Sound_delete(hSound);
    }

    if (hOutBuf) {
        Buffer_delete(hOutBuf);
    }

    if (hInBuf) {
        Buffer_delete(hInBuf);
    }

    if (hEngine) {
        Engine_close(hEngine);
    }

    return status;

}


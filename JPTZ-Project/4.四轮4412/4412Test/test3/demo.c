#include "demo.h"

static void drawDynamicData(OsdData *op){
	struct timeval        tv;
	struct tm            *timePassed;
	unsigned long 	     newTime;
	unsigned long 	     deltaTime;
	time_t		     spentTime;
	int 		     fps;
	int 		     fpsf;	
	op->time = -1;

	if (gettimeofday(&tv, NULL) == -1) {
		ERR("Failed to get os time\n");
		return;
	}
	newTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (!op->firstTime) {
		op->firstTime = newTime;
		op->prevTime = newTime;
		return;
	}
	/* Only update user interface every second */
	deltaTime = newTime - op->prevTime;
	if (deltaTime <= 1000) {
		return;
	}
	op->prevTime = newTime;

	spentTime = (newTime - op->firstTime) / 1000;
	if (spentTime <= 0) {
		return;
	}

	op->time     = spentTime;
	fpsf         = gblGetAndResetFrames() * 1000.0 / deltaTime;
	fps          = fpsf + 0.5;
	printf(">>>>>>>>>>>>>>>>FPS:%d\n",fps);
}

GlobalData gbl = GBL_DATA_INIT;

void *PrintAllData(void *arg){

	OsdData                 osdData             = OSD_DATA_INIT;

	pthread_mutex_init(&gbl.mutex, NULL);
	while(1){
		drawDynamicData(&osdData);		
		usleep(50000);
	}
	pthread_mutex_destroy(&gbl.mutex);
}




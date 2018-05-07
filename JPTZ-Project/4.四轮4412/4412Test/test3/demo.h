#ifndef __DEMO_H__
#define __DEMO_H__
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ERR(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
#define OSD_DATA_INIT        { -1, 0, 0, 0, 0, 0 }
#define GBL_DATA_INIT { 0 }

typedef struct OsdData {
    int            time;
    unsigned long  firstTime;
    unsigned long  prevTime;
    int            samplingFrequency;
    int            imageWidth;
    int            imageHeight;
} OsdData;

/* Global data structure */
typedef struct GlobalData {
    int             frames;              /* Video frame counter */
    pthread_mutex_t mutex;               /* Mutex to protect the global data */
} GlobalData;

extern GlobalData gbl;

static inline void gblIncFrames(void)
{
    pthread_mutex_lock(&gbl.mutex);
    gbl.frames++;
    pthread_mutex_unlock(&gbl.mutex);
}

static inline int gblGetAndResetFrames(void)
{
    int frames;

    pthread_mutex_lock(&gbl.mutex);
    frames = gbl.frames;
    gbl.frames = 0;
    pthread_mutex_unlock(&gbl.mutex);

    return frames;
}


void *PrintAllData(void *arg);
#endif



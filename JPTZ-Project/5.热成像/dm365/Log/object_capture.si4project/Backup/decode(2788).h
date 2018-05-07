
#ifndef _DECODE_H
#define _DECODE_H

#include <xdc/std.h>

#include <ti/sdo/dmai/Fifo.h>
#include <ti/sdo/dmai/Pause.h>
#include <ti/sdo/dmai/Rendezvous.h>
#include <ti/sdo/dmai/VideoStd.h>
#include <ti/sdo/dmai/Display.h> 

#define DECODEPRORUN			1
#define DECODEPROSTOP			0
#define DECODEEXIT				2
int 	       			 ProState;


struct myDecodeArg{
	Display_Handle  handle;
	char 		   *filename;
};

/* Thread function prototype */
extern Void *decodeThrFxn(Void *arg);

#endif /* _DECODE_H */


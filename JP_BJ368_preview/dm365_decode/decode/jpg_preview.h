#ifndef __JPG_PREVIEW__
#define __JPG_PREVIEW__
#include <stdio.h>
#include <string.h>

#include <xdc/std.h>

#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/CERuntime.h>

#include <ti/sdo/dmai/Dmai.h>
#include <ti/sdo/dmai/Buffer.h>
#include <ti/sdo/dmai/ColorSpace.h>
#include <ti/sdo/dmai/ce/Idec1.h>
#include <ti/sdo/dmai/BufferGfx.h>
#include <xdc/std.h>
#include "../ctrl.h"

#include <ti/sdo/dmai/ColorSpace.h>

#define 		I_JPG_WID			736
#define 		I_JPG_HID			576

#define			Jpg_zoom_WID   		304//304
#define 		Jpg_zoom_HID   		224//240

/*视频解码处理所需的宏*/
#define 		i_OFFSETY				48				//解码后Buffer偏移
#define 		i_OFFSETX				24

#define 		i_HEIGHT 				576		
#define 		i_LINELEN 				800

#define			i_AFTZOOMH				224//240
#define			i_AFTZOOMW				330//330
#define			i_DISZOOMW				308//308
#define 	 	CIRCULARADIUS			 44				

/*JPG_DATA_FORMAT:
 *			JPG缩放后的数据格式
 */
typedef  unsigned char * 	JPG_DATA_FORMAT;	


/*After_Jpg_Date_To_Diplay:
 *		Show JPG to Screen
 *		Parameters：
 *					[in] dis_Dstbuf				:Display Buffer
 *		Return values:
 *					Void
 */
void After_Jpg_Date_To_Diplay(Buffer_Handle hSrcBuf);

/*After_Zoom_To_Display_jpg:
 *		Show JPG to Screen after zoom 
 *		Parameters：
 *					[in] hSrcBuf				:Display Buffer
 *					[in] ZoomPtr				:need show date
 *					[in] offsetx				:relative screen level offset
 *					[in] offsety				:relative screen vertical offset
 *		Return values:
 *					Void
 */
Void After_Zoom_To_Display_jpg(Buffer_Handle hSrcBuf,JPG_DATA_FORMAT ZoomPtr,int offsetx,int offsety);



/*JPG_DATA_FORMAT:
 *		Init jpegdec Engine
 *		Parameters：
 *						Void
 *		Return values:
 *				-1 				:Faild
 *				 0				:Successful
 */
Int  JPG_Decode_Engine_Init	  			(Void);
/*JPG_Decode_Engine_Destory:
 *		free jpegdec Engine
 *		Parameters：
 *						Void
 *		Return values:
 *						Void
 */
Void JPG_Decode_Engine_Destory(Void);


/*JPG_Decode_File_Open:
 *		Open jpg file and decode
 *		Parameters：
 *				[in] filepath			:Need decode JPG file path
 *		Return values:
 *				-1 						:Faild
 *				 0						:Successful
 */
Int  JPG_Decode_File_Open (unsigned char * filepath);
/*JPG_Decode_File_Close:
 *		close jpg file 
 *		Parameters：
 *						Void
 *		Return values:
 *						Void
 */
Void JPG_Decode_File_Close(Void);


/*JPG_Get_Zoom_Decode_Date:
 *		Get zoom date 
 *		Parameters：
 *					[in] DstWidth			:Need yuv420sp date width after Zoom
 *					[in] DstHeight			:Need yuv420sp date height after Zoom
 *		Return values:
 *					JPG_DATA_FORMAT			:Get yuv420sp date pointer.
 *					NULL					:Get date error.
 */
JPG_DATA_FORMAT  JPG_Get_Zoom_Decode_Date(Int DstWidth,Int DstHeight);
/*JPG_Destory_Zoom_Decode_Date:
 *		Free zoom date 
 *		Parameters：
 *					[in] Data				:Need freed date 
 *		Return values:
 *					Void
 */
Void JPG_Destory_Zoom_Decode_Date(JPG_DATA_FORMAT Data);


#endif


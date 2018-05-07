/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-u17
 */

#define __nested__
#define __config__

#include <xdc/std.h>

/*
 * ======== GENERATED SECTIONS ========
 *     
 *     MODULE INCLUDES
 *     
 *     <module-name> INTERNALS
 *     <module-name> INHERITS
 *     <module-name> VTABLE
 *     <module-name> PATCH TABLE
 *     <module-name> DECLARATIONS
 *     <module-name> OBJECT OFFSETS
 *     <module-name> TEMPLATES
 *     <module-name> INITIALIZERS
 *     <module-name> FUNCTION STUBS
 *     <module-name> PROXY BODY
 *     <module-name> OBJECT DESCRIPTOR
 *     <module-name> SYSTEM FUNCTIONS
 *     <module-name> PRAGMAS
 *     
 *     INITIALIZATION ENTRY POINT
 *     PROGRAM GLOBALS
 *     CLINK DIRECTIVES
 */


/*
 * ======== MODULE INCLUDES ========
 */



/*
 * ======== xdc.cfg.Program TEMPLATE ========
 */

/*
 *  ======== __ASM__ ========
 *  Define absolute path to program's assembly directory
 */
xdc__META(__ASM__, "@(#)__ASM__ = /mnt/hgfs/share/dvsdk_dm365_4_02_00_06/dvsdk-demos_4_02_00_01/dm365/encodets/encodets_config/package/cfg/encodets_xv5T");

/*
 *  ======== __ISA__ ========
 *  Define the ISA of this executable.  This symbol is used by platform
 *  specific "exec" commands that support more than one ISA; e.g., gdb
 */
xdc__META(__ISA__, "@(#)__ISA__ = v5T");

/*
 *  ======== __PLAT__ ========
 *  Define the name of the platform that can run this executable.  This
 *  symbol is used by platform independent "exec" commands
 */
xdc__META(__PLAT__, "@(#)__PLAT__ = ti.platforms.evmDM365");

/*
 *  ======== __TARG__ ========
 *  Define the name of the target used to build this executable.
 */
xdc__META(__TARG__, "@(#)__TARG__ = gnu.targets.arm.GCArmv5T");


/*
 * ======== ti.sdo.ce.ipc.Settings TEMPLATE ========
 */


/*
 * ======== ti.sdo.ce.osal.Global TEMPLATE ========
 */



/*
 *  ======== Global Configuration ========
 *  Do not modify the following; it is automatically generated from the template
 *  Server.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */
#include <ti/sdo/ce/osal/Log.h>

Log_Handle ti_sdo_ce_dvtLog = NULL; 

Bool Memory_skipVirtualAddressTranslation = FALSE;


/* List of all packages and their versions and paths, for diagnostics */
String Global_buildInfo[] = {
    "    package gnu.targets.arm.rtsv5T (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdctools_3_16_03_36/packages/gnu/targets/arm/rtsv5T/) [1, 0, 0, 0]",
    "    package ti.sdo.linuxutils.edma (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/linuxutils_2_26_01_02/packages/ti/sdo/linuxutils/edma/) [1, 0, 0]",
    "    package ti.sdo.linuxutils.cmem (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/linuxutils_2_26_01_02/packages/ti/sdo/linuxutils/cmem/) [2, 2, 0]",
    "    package ti.sdo.fc.ires (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/) [1, 0, 0]",
    "    package ti.sdo.fc.global (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/global/) [1, 0, 0]",
    "    package ti.sdo.fc.memutils (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/memutils/) [1, 0, 0]",
    "    package ti.xdais (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdais_6_26_01_03/packages/ti/xdais/) [1, 2.0, 1]",
    "    package ti.sdo.fc.ires.addrspace (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/addrspace/) [1, 0, 0]",
    "    package ti.sdo.fc.utils (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/utils/) [1, 0, 2]",
    "    package ti.sdo.fc.ires.memtcm (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/memtcm/) [1, 0, 0]",
    "    package ti.sdo.linuxutils.vicp (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/linuxutils_2_26_01_02/packages/ti/sdo/linuxutils/vicp/) [1, 0, 0]",
    "    package ti.sdo.fc.ires.hdvicp (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/hdvicp/) [1, 0, 1]",
    "    package ti.sdo.utils.trace (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/utils/trace/) [1, 0, 0]",
    "    package ti.sdo.fc.hdvicpsync (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/hdvicpsync/) [1, 0, 0]",
    "    package ti.sdo.codecs.mpeg4enc_hdvicp (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg4enc_hdvicp/) [1, 0, 0]",
    "    package ti.sdo.codecs.h264enc (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/h264enc/) [1, 0, 0]",
    "    package ti.sdo.codecs.mpeg2enc (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg2enc/) [1, 0, 0]",
    "    package ti.sdo.codecs.mpeg4dec_hdvicp (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg4dec_hdvicp/) [1, 0, 0]",
    "    package ti.sdo.codecs.h264dec (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/h264dec/) [1, 0, 0]",
    "    package ti.sdo.codecs.mpeg2dec (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg2dec/) [1, 0, 0]",
    "    package ti.sdo.fc.ires.nullresource (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/nullresource/) [1, 0, 0]",
    "    package ti.sdo.fc.rman (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/rman/) [2, 0, 0]",
    "    package ti.sdo.fc.edma3 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/edma3/) [3, 0, 0]",
    "    package ti.sdo.fc.ires.edma3chan (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/edma3chan/) [1, 0, 0]",
    "    package ti.sdo.fc.ires.grouputils (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/grouputils/) [1, 0, 0]",
    "    package ti.sdo.fc.ires.vicp (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/ires/vicp/) [1, 0, 1]",
    "    package ti.sdo.fc.vicpsync (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/vicpsync/) [1, 0, 0]",
    "    package ti.sdo.ce.global (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/global/) [1, 0, 0]",
    "    package ti.xdais.dm (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdais_6_26_01_03/packages/ti/xdais/dm/) [1, 0, 5]",
    "    package ti.sdo.ce.utils.xdm (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/utils/xdm/) [1, 0, 2]",
    "    package ti.sdo.ce.node (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/node/) [1, 0, 0]",
    "    package ti.sdo.fc.dman3 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/dman3/) [1, 0, 4]",
    "    package ti.sdo.fc.acpy3 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/framework-components_2_26_00_01/packages/ti/sdo/fc/acpy3/) [1, 0, 4]",
    "    package gnu.targets (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdctools_3_16_03_36/packages/gnu/targets/) [1, 0, 1]",
    "    package gnu.targets.arm (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdctools_3_16_03_36/packages/gnu/targets/arm/) [1, 0, 0, 0]",
    "    package ti.catalog.arm (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdctools_3_16_03_36/packages/ti/catalog/arm/) [1, 0, 1, 0]",
    "    package ti.catalog (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdctools_3_16_03_36/packages/ti/catalog/) [1, 0, 0]",
    "    package ti.platforms.evmDM365 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/xdctools_3_16_03_36/packages/ti/platforms/evmDM365/) [1, 0, 0]",
    "    package ti.sdo.ce.osal (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/osal/) [2, 0, 2]",
    "    package ti.sdo.ce.osal.linux (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/osal/linux/) [2, 0, 1]",
    "    package ti.sdo.ce.ipc (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/ipc/) [2, 0, 1]",
    "    package ti.sdo.ce.ipc.linux (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/ipc/linux/) [2, 0, 1]",
    "    package ti.sdo.ce.alg (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/alg/) [1, 0, 1]",
    "    package ti.sdo.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/) [1, 0, 6]",
    "    package ti.sdo.ce.video1 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/video1/) [1, 0, 2]",
    "    package ti.sdo.codecs.mpeg4enc_hdvicp.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg4enc_hdvicp/ce/) [1, 0, 0]",
    "    package ti.sdo.codecs.h264enc.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/h264enc/ce/) [1, 0, 0]",
    "    package ti.sdo.codecs.mpeg2enc.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg2enc/ce/) [1, 0, 0]",
    "    package ti.sdo.ce.video2 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/video2/) [1, 0, 2]",
    "    package ti.sdo.codecs.mpeg4dec_hdvicp.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg4dec_hdvicp/ce/) [1, 0, 0]",
    "    package ti.sdo.codecs.h264dec.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/h264dec/ce/) [1, 0, 0]",
    "    package ti.sdo.codecs.mpeg2dec.ce (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codecs-dm365_4_02_00_00/packages/ti/sdo/codecs/mpeg2dec/ce/) [1, 0, 0]",
    "    package ti.sdo.ce.speech (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/speech/) [1, 0, 2]",
    "    package ti.sdo.ce.speech1 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/speech1/) [1, 0, 1]",
    "    package ti.sdo.ce.audio (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/audio/) [1, 0, 2]",
    "    package ti.sdo.ce.audio1 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/audio1/) [1, 0, 1]",
    "    package ti.sdo.ce.video (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/video/) [1, 0, 3]",
    "    package ti.sdo.ce.image (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/image/) [1, 0, 3]",
    "    package ti.sdo.ce.image1 (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/codec-engine_2_26_02_11/packages/ti/sdo/ce/image1/) [1, 0, 1]",
    "    package ti.sdo.dmai (/home/qxd/dm365/dvsdk_dm365_4_02_00_06/dmai_2_20_00_15/packages/ti/sdo/dmai/) [1, 0, 0]",
    "    package encodets_config (/mnt/hgfs/share/dvsdk_dm365_4_02_00_06/dvsdk-demos_4_02_00_01/dm365/encodets/encodets_config/) []",
    NULL
};

/*
 * ======== ti.sdo.ce.Engine TEMPLATE ========
 */

/*
 *  ======== Engine Configuration ========
 *  Do not modify following; it is automatically generated from the template
 *  Engine.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */
#include <ti/sdo/ce/Engine.h>
#include <ti/xdais/ialg.h>

#include <ti/xdais/idma3.h>

#include <ti/xdais/ires.h>

/* type tables for algorithms */
extern IALG_Fxns H264VENC_TI_IH264VENC;
static String typeTab_H264VENC_TI_IH264VENC[] = {
    "ti.sdo.ce.video1.IVIDENC1",
    NULL
};

extern IALG_Fxns H264VDEC_TI_IH264VDEC;
static String typeTab_H264VDEC_TI_IH264VDEC[] = {
    "ti.sdo.ce.video2.IVIDDEC2",
    NULL
};

extern IALG_Fxns MPEG4VDEC_TI_IMPEG4VDEC;
static String typeTab_MPEG4VDEC_TI_IMPEG4VDEC[] = {
    "ti.sdo.ce.video2.IVIDDEC2",
    NULL
};

extern IALG_Fxns MPEG4VENC_TI_IMPEG4VENC;
static String typeTab_MPEG4VENC_TI_IMPEG4VENC[] = {
    "ti.sdo.ce.video1.IVIDENC1",
    NULL
};

extern IALG_Fxns MPEG2VENC_TI_IMPEG2VENC;
static String typeTab_MPEG2VENC_TI_IMPEG2VENC[] = {
    "ti.sdo.ce.video1.IVIDENC1",
    NULL
};

extern IALG_Fxns MPEG2VDEC_TI_IMPEG2VDEC;
static String typeTab_MPEG2VDEC_TI_IMPEG2VDEC[] = {
    "ti.sdo.ce.video2.IVIDDEC2",
    NULL
};


/* tables of engine algorithms */
extern IALG_Fxns MPEG4VENC_TI_IMPEG4VENC;
extern IRES_Fxns MPEG4VENC_TI_IRES;
extern IALG_Fxns H264VENC_TI_IH264VENC;
extern IRES_Fxns H264VENC_TI_IRES;
extern IALG_Fxns MPEG2VENC_TI_IMPEG2VENC;
extern IRES_Fxns MPEG2VENC_TI_IRES;
extern IALG_Fxns MPEG4VDEC_TI_IMPEG4VDEC;
extern IRES_Fxns MPEG4VDEC_TI_IRES;
extern IALG_Fxns H264VDEC_TI_IH264VDEC;
extern IRES_Fxns H264VDEC_TI_IRES;
extern IALG_Fxns MPEG2VDEC_TI_IMPEG2VDEC;
extern IRES_Fxns MPEG2VDEC_TI_IRES;

/* algorithm-specific stub/skeleton config data objects */
extern struct ti_sdo_codecs_mpeg4enc_hdvicp_ce_MPEG4ENC_HDVICP_CodecClassConfig ti_sdo_codecs_mpeg4enc_hdvicp_ce_MPEG4ENC_HDVICP_codecClassConfig;
extern struct ti_sdo_codecs_h264enc_ce_H264ENC_CodecClassConfig ti_sdo_codecs_h264enc_ce_H264ENC_codecClassConfig;
extern struct ti_sdo_codecs_mpeg2enc_ce_MPEG2ENC_CodecClassConfig ti_sdo_codecs_mpeg2enc_ce_MPEG2ENC_codecClassConfig;
extern struct ti_sdo_codecs_mpeg4dec_hdvicp_ce_MPEG4DEC_HDVICP_CodecClassConfig ti_sdo_codecs_mpeg4dec_hdvicp_ce_MPEG4DEC_HDVICP_codecClassConfig;
extern struct ti_sdo_codecs_h264dec_ce_H264DEC_CodecClassConfig ti_sdo_codecs_h264dec_ce_H264DEC_codecClassConfig;
extern struct ti_sdo_codecs_mpeg2dec_ce_MPEG2DEC_CodecClassConfig ti_sdo_codecs_mpeg2dec_ce_MPEG2DEC_codecClassConfig;

static Engine_AlgDesc engineAlgs0[] = {
    {
        "mpeg4enc",       /* name */
        {2805129948U},   /* uuid */
        &MPEG4VENC_TI_IMPEG4VENC,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_MPEG4VENC_TI_IMPEG4VENC,              /* typeTab */
        TRUE,          /* isLocal */
        1,        /* groupId */
        0,      /* protocol */
        &MPEG4VENC_TI_IRES,         /* iresFxns */
        &ti_sdo_codecs_mpeg4enc_hdvicp_ce_MPEG4ENC_HDVICP_codecClassConfig, /* stub/skel config params */
        Engine_USECACHEDMEM_DEFAULT,    /* memType */
        "ti.sdo.ce.video1.IVIDENC1",
    },
    {
        "h264enc",       /* name */
        {972753848U},   /* uuid */
        &H264VENC_TI_IH264VENC,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_H264VENC_TI_IH264VENC,              /* typeTab */
        TRUE,          /* isLocal */
        1,        /* groupId */
        0,      /* protocol */
        &H264VENC_TI_IRES,         /* iresFxns */
        &ti_sdo_codecs_h264enc_ce_H264ENC_codecClassConfig, /* stub/skel config params */
        Engine_USECACHEDMEM_DEFAULT,    /* memType */
        "ti.sdo.ce.video1.IVIDENC1",
    },
    {
        "mpeg2enc",       /* name */
        {2779951992U},   /* uuid */
        &MPEG2VENC_TI_IMPEG2VENC,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_MPEG2VENC_TI_IMPEG2VENC,              /* typeTab */
        TRUE,          /* isLocal */
        1,        /* groupId */
        0,      /* protocol */
        &MPEG2VENC_TI_IRES,         /* iresFxns */
        &ti_sdo_codecs_mpeg2enc_ce_MPEG2ENC_codecClassConfig, /* stub/skel config params */
        Engine_USECACHEDMEM_DEFAULT,    /* memType */
        "ti.sdo.ce.video1.IVIDENC1",
    },
    {
        "mpeg4dec",       /* name */
        {2798854612U},   /* uuid */
        &MPEG4VDEC_TI_IMPEG4VDEC,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_MPEG4VDEC_TI_IMPEG4VDEC,              /* typeTab */
        TRUE,          /* isLocal */
        1,        /* groupId */
        0,      /* protocol */
        &MPEG4VDEC_TI_IRES,         /* iresFxns */
        &ti_sdo_codecs_mpeg4dec_hdvicp_ce_MPEG4DEC_HDVICP_codecClassConfig, /* stub/skel config params */
        Engine_USECACHEDMEM_DEFAULT,    /* memType */
        "ti.sdo.ce.video2.IVIDDEC2",
    },
    {
        "h264dec",       /* name */
        {2701911697U},   /* uuid */
        &H264VDEC_TI_IH264VDEC,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_H264VDEC_TI_IH264VDEC,              /* typeTab */
        TRUE,          /* isLocal */
        1,        /* groupId */
        0,      /* protocol */
        &H264VDEC_TI_IRES,         /* iresFxns */
        &ti_sdo_codecs_h264dec_ce_H264DEC_codecClassConfig, /* stub/skel config params */
        Engine_USECACHEDMEM_DEFAULT,    /* memType */
        "ti.sdo.ce.video2.IVIDDEC2",
    },
    {
        "mpeg2dec",       /* name */
        {2974277761U},   /* uuid */
        &MPEG2VDEC_TI_IMPEG2VDEC,             /* fxns */
        NULL,        /* idma3Fxns */
        typeTab_MPEG2VDEC_TI_IMPEG2VDEC,              /* typeTab */
        TRUE,          /* isLocal */
        1,        /* groupId */
        0,      /* protocol */
        &MPEG2VDEC_TI_IRES,         /* iresFxns */
        &ti_sdo_codecs_mpeg2dec_ce_MPEG2DEC_codecClassConfig, /* stub/skel config params */
        Engine_USECACHEDMEM_DEFAULT,    /* memType */
        "ti.sdo.ce.video2.IVIDDEC2",
    },
    {NULL},
};
static Engine_AlgDesc engineAlgs1[] = {
    {NULL},
};

/* table of all engines available in this application */
static Engine_Desc engineTab[] = {
    {"encodedecode",	/* engine name */
      engineAlgs0,	/* alg table */
      NULL,		/* (optional) server name */
      NULL,        /* (optional) Link config ID */
      6 /* number of algs in alg table */
    },
    {"local",	/* engine name */
      engineAlgs1,	/* alg table */
      NULL,		/* (optional) server name */
      NULL,        /* (optional) Link config ID */
      0 /* number of algs in alg table */
    },
    {NULL, NULL, NULL, 0}	/* NULL-terminate the engine table */
};

Engine_Config Engine_config = {
    engineTab,		    /* table of all engines */
    "local"   /* local RMS engine name */
};

Bool ti_sdo_ce_Engine_noCommName = FALSE;

/*
 * ======== ti.sdo.ce.CERuntime TEMPLATE ========
 */


#include <ti/sdo/ce/osal/Global.h>
#include <ti/sdo/ce/ipc/Comm.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/osal/Queue.h>
#include <ti/sdo/ce/osal/Sem.h>
#include <ti/sdo/ce/osal/SemMP.h>
#include <ti/sdo/ce/osal/Thread.h>
#include <ti/sdo/ce/ipc/Processor.h>
#include <ti/sdo/ce/alg/Algorithm.h>
#include <ti/sdo/ce/osal/Lock.h>
#include <ti/sdo/ce/osal/LockMP.h>
#include <ti/sdo/ce/Server.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/rms.h>
#include <ti/sdo/ce/utils/xdm/XdmUtils.h>

#include <ti/sdo/utils/trace/gt.h>

/* This symbol is referenced by Server_init() */
Int Global_useLinkArbiter = FALSE;

/*
 *  ======== CERuntime_init ========
 */
Void CERuntime_init(Void)
{

    GT_init();


    /* if CE_DEBUG is set, turn on tracing and DSP auto trace collection */
    if (Global_getenv("CE_DEBUG") != NULL) {
        extern Bool   Engine_alwaysCollectDspTrace;
        extern String Engine_ceDebugDspTraceMask;

        Engine_alwaysCollectDspTrace = TRUE;



        if (Global_getenv("CE_DEBUG")[0] == '1') {
            GT_set("*+67,CE-3,GT_time=0,GT_prefix=1235");
            Engine_ceDebugDspTraceMask = "*+67,GT_prefix=1235,GT_time=3";
        }
        else if (Global_getenv("CE_DEBUG")[0] == '2') {
            GT_set(
                "*+01234567,CE-3,ti.sdo.ce.osal.SemMP=67,OG=467,OM=4567,OC=67,GT_time=0,GT_prefix=1235");
            Engine_ceDebugDspTraceMask =
                "*+01234567,CR=67,ti.sdo.fc.dman3-2,ti.sdo.fc.dskt2-2,GT_prefix=1235,GT_time=3";
        } else {
            GT_set("*+01234567,CE-3,GT_time=0,GT_prefix=12345");
            Engine_ceDebugDspTraceMask = "*+01234567,GT_prefix=12345,GT_time=3";
        }
    }

    if (Global_getenv("CE_CHECK") != NULL) {
        extern Bool VISA_checked;

        /*
         * Currently just change _this_ processor's value... perhaps we should
         * enable remote processors as well?
         */
        if (Global_getenv("CE_CHECK")[0] == '1') {
            VISA_checked = TRUE;
            /* turn on all GT_7CLASS trace (errors) */
            GT_set("*+7");
        } else if (Global_getenv("CE_CHECK")[0] == '0') {
            VISA_checked = FALSE;
        } else {
            /* leave it whatever it was... maybe we should drop a warning? */
        }
    }

    /* allow user to over-ride via CE_TRACE. */
    GT_set(Global_getenv("CE_TRACE"));
    Global_init();

    Sem_init();
    SemMP_init();


    Memory_init();
    Queue_init();
    Comm_init();
    Thread_init();
    Processor_init();
    LockMP_init();  /* Must be called before DMAN3_init() */
    Algorithm_init();
    XdmUtils_init();
    Lock_init();

    Engine_init();
    Server_init();

}

/*
 *  ======== CERuntime_exit ========
 */
Void CERuntime_exit(Void)
{
    Global_exit();
}

/* for backward compatibility with xdc-m based tools */
Void ti_sdo_ce_CERuntime_init__F(Void) {
    CERuntime_init();
}

/*
 * ======== ti.sdo.ce.Settings TEMPLATE ========
 */

/*
 *  ======== ti.sdo.ce Settings Configuration ========
 *  Do not modify following; it is automatically generated from the template
 *  Settings.xdt in the ti.sdo.ce package and will be overwritten next time the
 *  executable is configured
 */
Bool VISA_checked = FALSE;

/*
 * ======== ti.sdo.ce.video1.VIDENC1Config TEMPLATE ========
 */


/* this data structure has codec-specific names and values, but class-specific
 * fields -- it's indentical to struct IVIDENC1_CodecClassConfig from videnc1.h.
 * Its name doesn't matter, only that the address of data is captured in the
 * "codecClassConfig" field of the algorithm description
 */    
struct ti_sdo_codecs_mpeg2enc_ce_MPEG2ENC_CodecClassConfig {
    Bool manageInBufsCache   [ XDM_MAX_IO_BUFFERS ];  
    Bool manageOutBufsCache  [ XDM_MAX_IO_BUFFERS ];  
} ti_sdo_codecs_mpeg2enc_ce_MPEG2ENC_codecClassConfig = {
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE}
};
/* this data structure has codec-specific names and values, but class-specific
 * fields -- it's indentical to struct IVIDENC1_CodecClassConfig from videnc1.h.
 * Its name doesn't matter, only that the address of data is captured in the
 * "codecClassConfig" field of the algorithm description
 */    
struct ti_sdo_codecs_mpeg4enc_hdvicp_ce_MPEG4ENC_HDVICP_CodecClassConfig {
    Bool manageInBufsCache   [ XDM_MAX_IO_BUFFERS ];  
    Bool manageOutBufsCache  [ XDM_MAX_IO_BUFFERS ];  
} ti_sdo_codecs_mpeg4enc_hdvicp_ce_MPEG4ENC_HDVICP_codecClassConfig = {
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE}
};
/* this data structure has codec-specific names and values, but class-specific
 * fields -- it's indentical to struct IVIDENC1_CodecClassConfig from videnc1.h.
 * Its name doesn't matter, only that the address of data is captured in the
 * "codecClassConfig" field of the algorithm description
 */    
struct ti_sdo_codecs_h264enc_ce_H264ENC_CodecClassConfig {
    Bool manageInBufsCache   [ XDM_MAX_IO_BUFFERS ];  
    Bool manageOutBufsCache  [ XDM_MAX_IO_BUFFERS ];  
} ti_sdo_codecs_h264enc_ce_H264ENC_codecClassConfig = {
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE}
};

/*
 * ======== ti.sdo.ce.video1.VIDDEC1Config TEMPLATE ========
 */



/*
 * ======== ti.sdo.ce.video2.VIDDEC2Config TEMPLATE ========
 */


/* this data structure has codec-specific names and values, but class-specific
 * fields -- it's indentical to struct IVIDDEC2_CodecClassConfig from viddec2.h.
 * Its name doesn't matter, only that the address of data is captured in the
 * "codecClassConfig" field of the algorithm description
 */    
struct ti_sdo_codecs_mpeg4dec_hdvicp_ce_MPEG4DEC_HDVICP_CodecClassConfig {
    Bool manageInBufsCache     [ XDM_MAX_IO_BUFFERS ];  
    Bool manageOutBufsCache    [ XDM_MAX_IO_BUFFERS ];  
} ti_sdo_codecs_mpeg4dec_hdvicp_ce_MPEG4DEC_HDVICP_codecClassConfig = {
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE}
};
/* this data structure has codec-specific names and values, but class-specific
 * fields -- it's indentical to struct IVIDDEC2_CodecClassConfig from viddec2.h.
 * Its name doesn't matter, only that the address of data is captured in the
 * "codecClassConfig" field of the algorithm description
 */    
struct ti_sdo_codecs_mpeg2dec_ce_MPEG2DEC_CodecClassConfig {
    Bool manageInBufsCache     [ XDM_MAX_IO_BUFFERS ];  
    Bool manageOutBufsCache    [ XDM_MAX_IO_BUFFERS ];  
} ti_sdo_codecs_mpeg2dec_ce_MPEG2DEC_codecClassConfig = {
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE}
};
/* this data structure has codec-specific names and values, but class-specific
 * fields -- it's indentical to struct IVIDDEC2_CodecClassConfig from viddec2.h.
 * Its name doesn't matter, only that the address of data is captured in the
 * "codecClassConfig" field of the algorithm description
 */    
struct ti_sdo_codecs_h264dec_ce_H264DEC_CodecClassConfig {
    Bool manageInBufsCache     [ XDM_MAX_IO_BUFFERS ];  
    Bool manageOutBufsCache    [ XDM_MAX_IO_BUFFERS ];  
} ti_sdo_codecs_h264dec_ce_H264DEC_codecClassConfig = {
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE},
    {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE}
};

/*
 * ======== ti.sdo.ce.video.VIDENCConfig TEMPLATE ========
 */



/*
 * ======== ti.sdo.ce.video.VIDDECConfig TEMPLATE ========
 */



/*
 * ======== ti.sdo.ce.image.IMGDECConfig TEMPLATE ========
 */



/*
 * ======== ti.sdo.ce.image.IMGENCConfig TEMPLATE ========
 */



/*
 * ======== ti.sdo.ce.image1.IMGDEC1Config TEMPLATE ========
 */



/*
 * ======== ti.sdo.ce.image1.IMGENC1Config TEMPLATE ========
 */



/*
 * ======== ti.sdo.fc.rman.RMAN TEMPLATE ========
 */

/* C code contribution from ti/sdo/fc/rman/RMAN.xdt */
 
#include <ti/sdo/fc/rman/rman.h>


        /* declaration of user-supplied RMAN.persistentAllocFxn */
        extern Bool _ALG_allocMemory(IALG_MemRec *memTab, Int numRecs);
        extern Void _ALG_freeMemory(IALG_MemRec *memTab, Int numRecs);

/* Table of user-configured size to store the registration entries */ 
__FAR__ IRESMAN_Fxns * RMAN_TABLE[11];

/* Table of user-configured size to store the freed entries */ 
__FAR__ short RMAN_FREE_ENTRIES[11];

/*
 * Number of entries statically configured for the Resource Registry table
 */
__FAR__ short RMAN_numRegistryEntries  = 0;
 
/*
 * Static configuration entries for the table
 */
__FAR__ IRESMAN_Fxns ** RMAN_registryEntries = NULL;
 
/*
 * Initialization arguments for the entries to be registered
 */
__FAR__ IRESMAN_Params ** RMAN_registryResmanArgs = NULL;

/*
 *   ========== RMAN_PARAMS ===========
 *   Module configuration for RMAN implementation
 *   It is set at design time by system integrator to adjust the behavior of 
 *   module to be optimal for its execution environment
 */
RMAN_Params RMAN_PARAMS = {
 
    /* size of the registry table */ 
    11,

    /* persistentAllocFxn */
    _ALG_allocMemory,
 
    /* persistentFreeFxn */
    _ALG_freeMemory,
  
    /* yieldFxn */
    NULL,

    /* flag indicating if yield to same priority is allowed */
    FALSE,
};

extern IRESMAN_Fxns * HDVICP_getIresmanFxns();
extern IRESMAN_Params * HDVICP_getDefaultParams();
extern IRESMAN_Fxns * VICP2_getIresmanFxns();
extern IRESMAN_Params * VICP2_getDefaultParams();
extern IRESMAN_Fxns * ADDRSPACE_getIresmanFxns();
extern IRESMAN_Params * ADDRSPACE_getDefaultParams();
extern IRESMAN_Fxns * EDMA3CHAN_getIresmanFxns();
extern IRESMAN_Params * EDMA3CHAN_getDefaultParams();
extern IRESMAN_Fxns * MEMTCM_getIresmanFxns();
extern IRESMAN_Params * MEMTCM_getDefaultParams();
extern IRESMAN_Fxns * GROUPUTILS_getIresmanFxns();
extern IRESMAN_Params * GROUPUTILS_getDefaultParams();


static __FAR__ IRESMAN_Fxns * _entries[6 + 1];
static __FAR__ IRESMAN_Params * _params[6 + 1];

IRES_Status RMAN_autoRegister()
{
    /* Populate the RMAN_registryEntries etc tables, to be able to do 
       static registration of these arguments */ 
    IRES_Status status = IRES_OK;
    
    RMAN_numRegistryEntries = 6;


    _entries[0] = HDVICP_getIresmanFxns(); 
    _params[0] = HDVICP_getDefaultParams(); 

    _params[0]->allocFxn = RMAN_PARAMS.allocFxn;
    _params[0]->freeFxn = RMAN_PARAMS.freeFxn;


    _entries[1] = VICP2_getIresmanFxns(); 
    _params[1] = VICP2_getDefaultParams(); 

    _params[1]->allocFxn = RMAN_PARAMS.allocFxn;
    _params[1]->freeFxn = RMAN_PARAMS.freeFxn;


    _entries[2] = ADDRSPACE_getIresmanFxns(); 
    _params[2] = ADDRSPACE_getDefaultParams(); 

    _params[2]->allocFxn = RMAN_PARAMS.allocFxn;
    _params[2]->freeFxn = RMAN_PARAMS.freeFxn;


    _entries[3] = EDMA3CHAN_getIresmanFxns(); 
    _params[3] = EDMA3CHAN_getDefaultParams(); 

    _params[3]->allocFxn = RMAN_PARAMS.allocFxn;
    _params[3]->freeFxn = RMAN_PARAMS.freeFxn;


    _entries[4] = MEMTCM_getIresmanFxns(); 
    _params[4] = MEMTCM_getDefaultParams(); 

    _params[4]->allocFxn = RMAN_PARAMS.allocFxn;
    _params[4]->freeFxn = RMAN_PARAMS.freeFxn;


    _entries[5] = GROUPUTILS_getIresmanFxns(); 
    _params[5] = GROUPUTILS_getDefaultParams(); 

    _params[5]->allocFxn = RMAN_PARAMS.allocFxn;
    _params[5]->freeFxn = RMAN_PARAMS.freeFxn;


    RMAN_registryEntries = (IRESMAN_Fxns **)(&_entries);
    RMAN_registryResmanArgs = (IRESMAN_Params **)(&_params);

    return (status);
}

/*
 * ======== ti.sdo.fc.vicpsync.VICPSYNC TEMPLATE ========
 */

 

/* Device is TMS320DM365 */
Bool _VICPSYNC_dm355 = 0; 
Bool _VICPSYNC_dm365 = 1; 

/*
 * ======== ti.sdo.fc.edma3.Settings TEMPLATE ========
 */



/* C code contribution from ti/sdo/fc/edma3/Settings.xdt */

__FAR__ Uns ti_sdo_fc_edma3_EDMA3_physicalId = 0;


#include <xdc/std.h>
#include <ti/sdo/fc/rman/rman.h>
#include <ti/sdo/fc/edma3/edma3_config.h>

/*
 * Declare alloc/Free functions before using them
 */
extern Bool _ALG_allocMemory(IALG_MemRec *memTab, Int numRecs);
extern Void _ALG_freeMemory(IALG_MemRec *memTab, Int numRecs);

/*
 *  ======== EDMA3_PARAMS ========
 *  EDMA3_PARAMS structure is created using the configured values
 */
EDMA3_Params EDMA3_PARAMS = {
   1,
    0,
   _ALG_allocMemory,
   _ALG_freeMemory,
    NULL,
    NULL,
    0,
   1,
};

/* Base key value for DMAN3 Linux IPC objects */
UInt32 _EDMA3_ipcKeyBase = 0x33414d44;

/*
 * Maximum size of resource table used to store requests for each scratch grp. 
 */
__FAR__ Int ti_sdo_fc_edma3_EDMA3_MAXEDMAREQUESTS = 64;


/*
 * ======== ti.sdo.fc.ires.hdvicp.HDVICP TEMPLATE ========
 */

 

/* Generated code from the HDVICP module */
unsigned int _HDVICP_REGBASEADDR = (unsigned int)31457280; 
unsigned int _HDVICP_MEMBASEADDR = (unsigned int)32505856; 
unsigned int _HDVICP_NUMRESOURCES = (unsigned int)1;

#ifdef xdc_target__os_Linux
/* Base key value for HDVICP Linux IPC objects */
UInt32 _HDVICP_ipcKeyBase = 0x49564448;
#endif


/*
 * ======== ti.sdo.fc.ires.vicp.VICP2 TEMPLATE ========
 */



/* Generated code from ti.sdo.fc.ires.vicp.VICP2.xdt */
unsigned int _VICP2_imcopConfigBase = (unsigned int)30015488;

/* Base key value for VICP2 Linux IPC objects */
UInt32 _VICP2_ipcKeyBase = 0x50434956;

/* Device is TMS320DM365 */
Bool _VICP2_dm355 = 0;
Bool _VICP2_dm365 = 1;

/*
 * ======== ti.sdo.fc.ires.addrspace.ADDRSPACE TEMPLATE ========
 */

    
Bool ADDRSPACE_noTranslation = FALSE;

/*
 * ======== ti.sdo.fc.ires.edma3chan.EDMA3CHAN TEMPLATE ========
 */

 

/*
 * ======== ti.sdo.fc.ires.memtcm.MEMTCM TEMPLATE ========
 */

/* Generated code from ti.sdo.fc.ires.memtcm.MEMTCM.xdt */
UInt32 MEMTCM_blockId = 1;

/* TODO, figure out if we shouldn't insmod CMEM from 0x0 */ 
UInt32 MEMTCM_size = 0x7000; 

/* Base key value for VICP2 Linux IPC objects */
UInt32 MEMTCM_ipcKeyBase = 0x4d43544d;

/*
 * ======== ti.sdo.fc.global.Settings TEMPLATE ========
 */

Bool ti_sdo_fc_multiProcess = TRUE;

/*
 * ======== ti.sdo.fc.ires.grouputils.GROUPUTILS TEMPLATE ========
 */



/* Base key value for  GROUPUTILS Linux IPC objects */
UInt32 _GROUPUTILS_ipcKeyBase = 0x534c4954;

#include <ti/sdo/fc/ires/grouputils/ires_grouputils.h>


extern Void Algorithm_acquireLock(Int groupId, IALG_Handle alg);
extern Void Algorithm_releaseLock(Int groupId, IALG_Handle alg);

ti_sdo_fc_ires_grouputils_LockFxn ti_sdo_fc_ires_grouputils_lockGroup = 
        (ti_sdo_fc_ires_grouputils_LockFxn)(Algorithm_acquireLock);
ti_sdo_fc_ires_grouputils_LockFxn ti_sdo_fc_ires_grouputils_unlockGroup = 
        (ti_sdo_fc_ires_grouputils_LockFxn)(Algorithm_releaseLock);

/*
 * Declare semaphore related functions before using them
 */

/* Need to declare these functions only if they haven't been declared before */

#include <xdc/std.h>
#include <ti/sdo/fc/rman/rman.h>

typedef struct Sem_Obj *_GROUPUTILS_SemHandle;
 
extern _GROUPUTILS_SemHandle Sem_create(Int key, Int count);
extern Void Sem_delete(_GROUPUTILS_SemHandle sem);
extern Int Sem_pend(_GROUPUTILS_SemHandle sem, UInt32 count);
extern Void Sem_post(_GROUPUTILS_SemHandle sem);

__FAR__ ti_sdo_fc_ires_grouputils_GROUPUTILS_SemCreateFxn 
        ti_sdo_fc_ires_grouputils_GROUPUTILS_semCreateFxn = 
        (ti_sdo_fc_ires_grouputils_GROUPUTILS_SemCreateFxn)Sem_create;
__FAR__ ti_sdo_fc_ires_grouputils_GROUPUTILS_SemDeleteFxn 
        ti_sdo_fc_ires_grouputils_GROUPUTILS_semDeleteFxn = 
        (ti_sdo_fc_ires_grouputils_GROUPUTILS_SemDeleteFxn)Sem_delete;
__FAR__ ti_sdo_fc_ires_grouputils_GROUPUTILS_SemPendFxn  
        ti_sdo_fc_ires_grouputils_GROUPUTILS_semPendFxn = 
        (ti_sdo_fc_ires_grouputils_GROUPUTILS_SemPendFxn)Sem_pend;
__FAR__ ti_sdo_fc_ires_grouputils_GROUPUTILS_SemPostFxn 
        ti_sdo_fc_ires_grouputils_GROUPUTILS_semPostFxn = 
        (ti_sdo_fc_ires_grouputils_GROUPUTILS_SemPostFxn)Sem_post;


/*
 * ======== ti.sdo.utils.trace.GT TEMPLATE ========
 */




/*
 * C code contribution from ti/sdo/utils/GT.xdt 
 */
#include <xdc/std.h>
#include <ti/sdo/utils/trace/gt.h>

extern Void GTConfig_tracePrintf(String fmt, ...); 
extern Ptr GTConfig_libcMalloc(Int size);
 
extern Int GTConfig_threadId(void);
extern void GTConfig_error(String fmt, ...);
extern void GTConfig_libcFree(Ptr, Int);
extern UInt32 GTConfig_gettimeofday(void);
extern Int GTConfig_pthreadLock(void);
extern Int GTConfig_pthreadUnlock(void);
extern Int GTConfig_posixInit(void);

GT_Config _ti_sdo_utils_trace_GT_params = {
    GTConfig_tracePrintf,
    NULL,
    GTConfig_threadId,
    GTConfig_error,
    GTConfig_libcMalloc,
    GTConfig_libcFree,
    GTConfig_gettimeofday,
    GTConfig_pthreadLock,
    GTConfig_pthreadUnlock,
    GTConfig_posixInit    
};

GT_Config *GT = &_ti_sdo_utils_trace_GT_params;

/*
 * ======== ti.sdo.ce.ipc.linux.Ipc TEMPLATE ========
 */



Void IPC_generatedInit()
{
}

/*
 * ======== ti.sdo.ce.alg.Settings TEMPLATE ========
 */

unsigned int ti_sdo_ce_alg_ALG_useHeap = FALSE;
unsigned int ti_sdo_ce_alg_ALG_useCache = FALSE;

unsigned int ti_sdo_ce_alg_ALG_maxGroups = 20; 

UInt32 ti_sdo_ce_alg_ipcKey = 0x4f474c41;

int _ALG_groupUsed[20] = {
     0,
     1,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
};

#include <ti/sdo/ce/osal/SemMP.h>
#include <ti/xdais/ialg.h>

SemMP_Handle _ALG_sems[20];
int _ALG_groupRefCount[20] = {
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
};

IALG_Handle _Algorithm_lockOwner[20] = {
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
};

// No IDMA3 using algs in the system, and DMAN3 is not configured into the
// system.  Stub out these fxn calls.
Void DMAN3_CE_init()
{
}
Void DMAN3_CE_exit()
{
}

int DMAN3_grantDmaChannels(int groupId, void *algHandles, void *dmaFxns,
        int numAlgs)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}

int DMAN3_releaseDmaChannels(void *algHandles, void * dmaFxns, int numAlgs)
{
    /* TODO:M  Should assert(false) here.  Should never be called */
    return (0);
}


/*
 * ======== ti.sdo.ce.osal.linux.Settings TEMPLATE ========
 */

UInt32 ti_sdo_ce_osal_linux_SemMP_ipcKey = 0x4c41534f;

UInt32 Memory_maxCbListSize = 100;


/*
 * ======== PROGRAM GLOBALS ========
 */


/*
 * ======== CLINK DIRECTIVES ========
 */

#ifdef __ti__
#define PRAGMA(x) _Pragma(#x)
#define xdc__clink_constant(var) \
PRAGMA(DATA_SECTION(var, ".const:" #var))  \
asm("   .sect \".const:" #var "\"\n"  \
  "   .clink\n"  \
  "   .sect \"[0].const:" #var "\"\n"  \
  "   .clink\n"  \
  "   .sect \"[1].const:" #var "\"\n"  \
  "   .clink")

#endif


#include "osdmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#define STD_D1_WIDTH        720     /**< Width in pixels of D1 */
#define STD_D1_NTSC_HEIGHT  480     /**< Height in pixels of D1 NTSC */
#define STD_D1_PAL_HEIGHT   576     /**< Height in pixels of D1 PAL */

#define STD_480P_WIDTH      720     /**< Width in pixels for 480P */
#define STD_480P_HEIGHT     480     /**< Height in pixels for 480P */

#define STD_720P_WIDTH      1280    /**< Width in pixels of 720P */
#define STD_720P_HEIGHT     720     /**< Height in pixels of 720P */

#define STD_1080I_WIDTH     1920    /**< Width in pixels of 1080I */
#define STD_1080I_HEIGHT    1080    /**< Height in pixels of 1080I */

#define DISPLAY_DEVICE           "/dev/fb0"
#define NUM_DISPLAY_BUFS         2

#define SYSFS_OUTPUT            "/sys/class/davinci_display/ch0/output"
#define SYSFS_MODE              "/sys/class/davinci_display/ch0/mode"
#define ATTR_DEVICE             "/dev/fb2"
#define NUM_ATTR_BUFS           1
#define FBDEV_OSD               "/dev/fb0"

#define MAX_TRANSPARENCY            0x77
#define MIN_TRANSPARENCY            0x0
#define NORMAL_TRANSPARENCY         0x55
#define INC_TRANSPARENCY            0x11
#define MIN_VISIBLE_TRANSPARENCY    0x11

/* Strings for sysfs video output variables */
static char * outputStrings[] = {
    (char *)"COMPOSITE",
    (char *)"COMPONENT",
    (char *)"LCD",
};

/* Strings for sysfs video mode variables */
static char * modeStrings[] = {
    (char *)"NTSC",
    (char *)"PAL",
    (char *)"480P-60",
    (char *)"720P-60",
    (char *)"720P-50",
    (char *)"VGA",
    (char *)"1080I-30",
};

static void cleanup(int fd, char * virtPtr,
                    struct fb_var_screeninfo * varInfo,
                    struct fb_fix_screeninfo * fixInfo)
{
    if (virtPtr != NULL) {
        munmap(virtPtr, fixInfo->line_length * varInfo->yres_virtual);
    }
    ::close(fd);
}


#define OMAP_DSS_GLOBAL_ALPHA \
    "/sys/devices/platform/omapdss/overlay0/global_alpha"

OSDManager::OSDManager()
{
    m_x = 0;
    m_y = 0;
    m_width = STD_480P_WIDTH;
    m_height = STD_480P_HEIGHT;
}

bool OSDManager::setFormat(char * deviceNode, int numBufs, int * fd, int * size,
                           char **virtPtr, DisplayMode mode)
{
    struct fb_var_screeninfo varInfo;
    struct fb_fix_screeninfo fixInfo;
    *virtPtr = NULL;

    *fd = ::open(deviceNode, O_RDWR);

    if (*fd == -1) {
        qFatal("Failed to open fb device %s (%s)\n", deviceNode,
               strerror(errno));
        return false;
    }

    /* Get fixed screen info */
    if (ioctl(*fd, FBIOGET_FSCREENINFO, &fixInfo) == -1) {
        qFatal("Failed FBIOGET_FSCREENINFO on %s (%s)\n",
               deviceNode, strerror(errno));
        cleanup(*fd, NULL, NULL, NULL);
        return false;
    }

    /* Get virtual screen info */
    if (ioctl(*fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        qFatal("Failed FBIOGET_VSCREENINFO on %s (%s)\n",
               deviceNode, strerror(errno));
        cleanup(*fd, NULL, NULL, NULL);
        return false;
    }
    varInfo.xoffset         = 0;
    varInfo.yoffset         = 0;

    switch (mode) {
    case MODE_PAL:
        varInfo.xres        = STD_D1_WIDTH;
        varInfo.yres        = STD_D1_PAL_HEIGHT;
        break;
    case MODE_NTSC:
    case MODE_480P_60:
        varInfo.xres        = STD_D1_WIDTH;
        varInfo.yres        = STD_D1_NTSC_HEIGHT;
        break;
    case MODE_720P_60:
    case MODE_720P_50:
        varInfo.xres        = STD_720P_WIDTH;
        varInfo.yres        = STD_720P_HEIGHT;
        break;
    case MODE_1080I_30:
        varInfo.xres        = STD_1080I_WIDTH;
        varInfo.yres        = STD_1080I_HEIGHT;
        break;
    default:
        qFatal("Unsupported standard\n");
        cleanup(*fd, NULL, NULL, NULL);
        return false;
    }

    varInfo.xres_virtual    = varInfo.xres;
    varInfo.yres_virtual    = varInfo.yres * numBufs;

    /* Set video display format */

    if (ioctl(*fd, FBIOPUT_VSCREENINFO, &varInfo) == -1) {
        qFatal("Failed FBIOPUT_VSCREENINFO on %s (%s)\n",
               deviceNode, strerror(errno));
        cleanup(*fd, NULL, NULL, NULL);
        return false;
    }

    if (ioctl(*fd, FBIOGET_FSCREENINFO, &fixInfo) == -1) {
        qFatal("Failed FBIOGET_FSCREENINFO on %s (%s)\n",
               deviceNode, strerror(errno));
        cleanup(*fd, NULL, NULL, NULL);
        return false;
    }

    *virtPtr = (char *) mmap(NULL,
                             fixInfo.line_length * varInfo.yres * numBufs,
                             PROT_READ | PROT_WRITE,
                             MAP_SHARED,
                             *fd, 0);

    if (*virtPtr == MAP_FAILED) {
        qFatal("Failed mmap on %s (%s)\n", deviceNode,
               strerror(errno));
        cleanup(*fd, NULL, NULL, NULL);
        return false;
    }

    memset(*virtPtr, 0, fixInfo.line_length * varInfo.yres);

    if (ioctl(*fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        qFatal("Failed FBIOGET_VSCREENINFO (%s)\n", strerror(errno));
        cleanup(*fd, *virtPtr, &varInfo, &fixInfo);
        return false;
    }

    varInfo.yoffset = 0;

    if (ioctl(*fd, FBIOPAN_DISPLAY, &varInfo) == -1) {
        qFatal("Failed FBIOPAN_DISPLAY (%s)\n", strerror(errno));
        cleanup(*fd, *virtPtr, &varInfo, &fixInfo);
        return false;
    }

    if (ioctl(*fd, FBIOBLANK, 0)) {
        qFatal("Error enabling %s\n", deviceNode);
        cleanup(*fd, *virtPtr, &varInfo, &fixInfo);
        return false;
    }

    *size = fixInfo.line_length * varInfo.yres;

    cleanup(*fd, *virtPtr, &varInfo, &fixInfo);
    return true;
}

bool OSDManager::setDisplayFormat(DisplayMode mode)
{
    /* Setup display device */
    if (!setFormat((char *)DISPLAY_DEVICE, NUM_DISPLAY_BUFS, &m_fdDisplay,
                   &m_sizeDisplay, &m_virtPtrDisplay, mode)) {
        return false;
    }

    /* Setup attribute device */
    if (!setFormat((char *)ATTR_DEVICE, NUM_ATTR_BUFS, &m_fdAttr, &m_sizeAttr,
                   &m_virtPtrAttr, mode)) {
        return false;
    }

    return true;
}

bool OSDManager::setDisplayMode(DisplayMode mode)
{
    char sysFsFileName[] = SYSFS_MODE;

    m_mode = mode;
    if (writeSysFs(sysFsFileName, modeStrings[mode])
            < 0) {
        qFatal("Could not write display mode to sysfs\n");
        return false;
    }

    if (!setDisplayFormat(mode)) {
        qFatal("Could not set display format in interface\n");
        return false;
    }

    return true;
}

bool OSDManager::setupOsd(int width, int height)
{
    int fd;
    struct fb_var_screeninfo varInfo;

    fd = open(FBDEV_OSD, O_RDWR);

    if (fd == -1) {
        qFatal("Failed to open %s\n", FBDEV_OSD);
        return false;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        qFatal("Failed FBIOGET_VSCREENINFO on /dev/fb0 (%s)\n",
               strerror(errno));
        close(fd);
        return false;
    }

    /* Change the resolution */
    varInfo.xres            = width;
    varInfo.yres            = height;
    varInfo.xres_virtual    = width;
    varInfo.yres_virtual    = height;

    /* Set up ARGB8888 */

    varInfo.red.length      = 8;
    varInfo.green.length    = 8;
    varInfo.blue.length     = 8;
    varInfo.transp.length   = 8;
    varInfo.transp.offset   = 24;
    varInfo.red.offset      = 16;
    varInfo.green.offset    = 8;
    varInfo.blue.offset     = 0;
    varInfo.bits_per_pixel  = 32;


    if (ioctl(fd, FBIOPUT_VSCREENINFO, &varInfo) < 0) {
        printf("Failed FBIOPUT_VSCREENINFO on /dev/fb0 (%s)\n",
               strerror(errno));
        return false;
    }


    return true;
}

bool OSDManager::readSysFs(char *fileName, char *val, int length)
{
    FILE *fp;
    int ret;
    int len;
    char *tok;

    fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Failed to open %s for reading\n", fileName);
        return false;
    }

    memset(val, '\0', length);
    ret = fread(val, 1, length, fp);
    if (ret < 1) {
        printf("Failed to read sysfs variable from %s\n", fileName);
        return false;
    }

    tok = strtok(val, "\n");
    len = tok ? strlen(tok) : strlen(val);
    val[len] = '\0';

    fclose(fp);

    return true;
}

bool OSDManager::setDisplayOutput(DisplayOutput output)
{
    char sysFsFileName[] = SYSFS_OUTPUT;

    if (writeSysFs(sysFsFileName, outputStrings[output])
            < 0) {
        qFatal("Could not write display output to sysfs\n");
        return false;
    }

    return true;
}

bool OSDManager::writeSysFs(char *fileName, char *val)
{
    FILE *fp;
    char *valString;

    valString = (char *)malloc(strlen(val) + 1);

    if (valString == NULL) {
        qFatal("Failed to allocate memory for temporary string\n");
        return false;
    }

    fp = fopen(fileName, "w");

    if (fp == NULL) {
        qFatal("Failed to open %s for writing\n", fileName);
        free(valString);
        return false;
    }

    if (fwrite(val, strlen(val) + 1, 1, fp) != 1) {
        qFatal("Failed to write sysfs variable %s to %s\n",
               fileName, val);
        fclose(fp);
        free(valString);
        return false;
    }

    fclose(fp);

    free(valString);

    return true;
}

bool OSDManager::writeTransValue()
{
    return writeTransFbdev(m_transValue, m_x, m_y, m_width, m_height);
    return false;
}


bool OSDManager::writeTransSysfs(int val)
{
    FILE *fptr;
    char str[4];

    sprintf(str, "%d", val);

    fptr = fopen(OMAP_DSS_GLOBAL_ALPHA, "w");

    if (fptr == NULL) {
        qFatal("Failed to open handle");
        return false;
    }

    fwrite(str, 4, 1, fptr);
    fclose(fptr);

    return true;
}

bool OSDManager::writeTransFbdev(int val, int x, int y, int width, int height)
{
    struct fb_var_screeninfo    vScreenInfo;
    struct fb_fix_screeninfo    fScreenInfo;
    unsigned short              *attrDisplay;
    int attrSize;
    int bitsPerPixel;
    int offset;

    int fd = open("/dev/fb2", O_RDWR);
    if (fd < 0) {
        qFatal("Failed to open /dev/fb2");
        return false;
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &fScreenInfo) == -1) {
        ::close(fd);
        qFatal("Failed to get fixed screen info");
        return false;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vScreenInfo) == -1) {
        ::close(fd);
        qFatal("Failed to get virtual screen info");
        return false;
    }

    attrSize = fScreenInfo.line_length * vScreenInfo.yres;
    bitsPerPixel = 8 * fScreenInfo.line_length / vScreenInfo.xres;

    attrDisplay = (unsigned short *) mmap(NULL, attrSize,
                                          PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (attrDisplay == MAP_FAILED) {
        qFatal("Failed to map the attribute window");
        ::close(fd);
        return false;
    }
    memset(attrDisplay, MIN_TRANSPARENCY, attrSize);

    // Write transparency value to rectangle defined by function arguments
    for (int i = y; i < y + height; i++) {
        offset = ((x * bitsPerPixel) / 8 + (fScreenInfo.line_length * i)) /
                sizeof(unsigned short);
        memset(attrDisplay + offset, val, bitsPerPixel * width / 8);
    }
    munmap(attrDisplay, attrSize);
    ::close(fd);

    return true;
}


bool OSDManager::incTrans()
{
    if (m_transValue + INC_TRANSPARENCY <= MAX_TRANSPARENCY) {
        m_transValue += INC_TRANSPARENCY;
    }
    else{
        m_transValue = MAX_TRANSPARENCY;
    }
    return writeTransValue();
}

bool OSDManager::decTrans()
{
    /*
     * We make sure the OSD does not completely disappear when decrementing
     * transparency
     */
    if (m_transValue - INC_TRANSPARENCY > MIN_VISIBLE_TRANSPARENCY) {
        m_transValue -= INC_TRANSPARENCY;
    }
    else {
        m_transValue = MIN_VISIBLE_TRANSPARENCY;
    }
    return writeTransValue();
}

bool OSDManager::showTransOSD()
{
    m_transValue = NORMAL_TRANSPARENCY;
    return writeTransValue();
}

bool OSDManager::hideOSD()
{
    m_transValue = MIN_TRANSPARENCY;
    return writeTransValue();
}

bool OSDManager::showOSD()
{
    m_transValue = MAX_TRANSPARENCY;
    return writeTransValue();
}

void OSDManager::setBoxDimensions(int x, int y, int width, int height)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
}

OSDManager::~OSDManager()
{
    showOSD();
}


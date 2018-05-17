#ifndef OSDMANAGER_H
#define OSDMANAGER_H

#include <QtGui>

class OSDManager : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        OUTPUT_COMPOSITE,
        OUTPUT_COMPONENT,
        OUTPUT_LCD,
        OUTPUT_DVI,
        OUTPUT_SVIDEO,
        DisplayOutput_COUNT
    } DisplayOutput;

    typedef enum {
        MODE_NTSC,
        MODE_PAL,
        MODE_480P_60,
        MODE_720P_60,
        MODE_720P_50,
        MODE_VGA,
        MODE_1080I_30,
        DisplayMode_COUNT
    } DisplayMode;

    OSDManager();
    ~OSDManager();
    bool setDisplayOutput(DisplayOutput output);
    bool setDisplayMode(DisplayMode mode);
    bool showOSD();
    bool hideOSD();
    bool showTransOSD();
    bool incTrans();
    bool decTrans();
    void setBoxDimensions(int x, int y, int width, int height);

private:
    bool setupOsd(char *displayEntry);
    bool setupOsd(int width, int height);
    bool setOsdTransparency(int transparency);
    bool writeSysFs(char *fileName, char *val);
    bool readSysFs(char *fileName, char *val, int length);
    bool setDisplayFormat(DisplayMode mode);
    bool setFormat(char * deviceNode, int numBufs, int * fd, int * size,
        char ** virtPtr, DisplayMode mode);
    bool writeTransValue();
    bool writeTransFbdev(int val, int x, int y, int width, int height);
    bool writeTransSysfs(int val);

    DisplayMode     m_mode;
    int             m_fdDisplay;
    int             m_fdAttr;
    char *          m_virtPtrDisplay;
    char *          m_virtPtrAttr;
    int             m_sizeDisplay;
    int             m_sizeAttr;
    int             m_transValue;
    int             m_x;
    int             m_y;
    int             m_width;
    int             m_height;
};

#endif // OSDMANAGER_H

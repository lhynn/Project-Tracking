#ifndef IMAGEDILOG_H
#define IMAGEDILOG_H

#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QHBoxLayout>

class Imagedilog : public QWidget                   //我们需要给它重写一些方法，比如mouseDoubleClickEvent事件等
{
    Q_OBJECT
public:
    explicit Imagedilog(void);          //基类的构造函数
    ~Imagedilog();

public:
    void HideLable(void);
    void ShowLable(void);
    void SetImageFrame();
    void DelImageFrame();

    QLabel *ImageWindow;
    QLabel *NameWindow;
    QVBoxLayout *vLayOut;

private:
    int Iwidth;
    int Iheight;

    int Nwidth;
    int Nheight;

    QHBoxLayout *hNameLayout;
    QHBoxLayout *hImageLayout;

};
#endif // IMAGEDILOG_H

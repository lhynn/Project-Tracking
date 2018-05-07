#include "Imagedilog.h"ss

Imagedilog::Imagedilog(void)
{
    Iwidth  = 250;
    Iheight = 180;

    Nwidth  = 60;
    Nheight = 30;

    ImageWindow = new QLabel;
    ImageWindow->setFixedSize(Iwidth,Iheight);
    ImageWindow->setScaledContents(true);
    ImageWindow->setStyleSheet("color: rgb(0, 0, 0);");

    NameWindow  = new QLabel;
    NameWindow->setStyleSheet("color: rgb(0, 0, 0);");
    NameWindow->setFixedSize(Nwidth,Nheight);
    //NameWindow->setStyleSheet("background-color: rgb(0, 85, 255);");
    QSpacerItem *space1 = new QSpacerItem(40, 20);
    QSpacerItem *space2 = new QSpacerItem(200, 0);

    hImageLayout = new QHBoxLayout;
    hImageLayout->addSpacerItem(space1);
    hImageLayout->addWidget(ImageWindow);
   // hImageLayout->addSpacerItem(space1);


    hNameLayout = new QHBoxLayout;
    hNameLayout->addSpacerItem(space2);
    hNameLayout->addWidget(NameWindow);
    hNameLayout->addSpacerItem(space2);

    vLayOut = new QVBoxLayout;
    vLayOut->addLayout(hImageLayout);
    vLayOut->addLayout(hNameLayout);

}

Imagedilog::~Imagedilog(){
    delete ImageWindow;
    delete NameWindow;
}

void Imagedilog::HideLable(){
    ImageWindow->hide();
    NameWindow->hide();
}
void Imagedilog::ShowLable(){
    ImageWindow->show();
    NameWindow->show();
}
void Imagedilog::SetImageFrame(){
    ImageWindow->setStyleSheet("border-width: 2px;border-style: solid;border-color: rgb(0,255,0);");
}
void Imagedilog::DelImageFrame(){
    ImageWindow->setStyleSheet("color: rgb(0, 0, 0);");
}



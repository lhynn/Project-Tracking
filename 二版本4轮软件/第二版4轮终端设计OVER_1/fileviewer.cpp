#include "fileviewer.h"

FileViewer::FileViewer( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{
    lastvalidrow=0;
    setWindowTitle(tr("File Browser"));
    DirName = new QLabel();
    QFont font("JPTZ", 26, 55);    //( “Microsoft YaHei”, 10, 75);
    DirName->setFont(font);
    DirName->setText("存储文件列表");
    ListWidgetFile = new QListWidget();
    ListWidgetFile->setIconSize(QSize(40,26));
    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    vbLayout->addWidget(DirName);
    vbLayout->addWidget(ListWidgetFile);

    ListStatus = new QLabel();
    QFont font1("JPTZ", 26, 55);    //( “Microsoft YaHei”, 10, 75);
    ListStatus->setFont(font1);
    vbLayout->addWidget(ListStatus);
    currentrow = 0;
}


FileViewer::~FileViewer()
{

}
char *FileViewer::GetFileName(){
    if(gl_maxfilecnt > 0)
      return ListWidgetFile->item(currentrow)->text().toLatin1().data();
    else
      return NULL;
}

void FileViewer::nextrow()
{
    if(currentrow < gl_maxfilecnt-1)
    {
        currentrow ++;
        ListWidgetFile->setCurrentRow(currentrow);
        showFileInfoList(10);
    }
}

void FileViewer::prevrow()
{
    if(currentrow > 0)
    {
        currentrow --;
        ListWidgetFile->setCurrentRow(currentrow);
    }
}

void FileViewer::iDirFileToLink(QString Type){
    QByteArray  ba = Type.toLatin1(); // must
    char *str = ba.data();
    fileType = Type;
    //printf("str:%s\n",str);
    //fflush(NULL);
    gl_MyBmpLList = mySearchDirFun("/home/root/media/",str);
    if(gl_MyBmpLList == NULL){
        printf("Search Dir error.\n");
    }
    ShowImageNode = gl_MyBmpLList->head.next;
    gl_MyBmpLList->getMemSum(gl_MyBmpLList,&gl_maxfilecnt);
}

/*****************************************************
文件列表窗口ChuangJian
*****************************************************/
void FileViewer::showFileInfoList(int num){
    QString Tmp,str;
    QFont font("JPTZ", 28, 55);
    PT_MyFileSt data = NULL;
    for(int i = 0;i < num;i++){
        if(ShowImageNode  != &gl_MyBmpLList->head){
            data = (PT_MyFileSt)ShowImageNode->data;
            Tmp = QString(QLatin1String(data->name));
            QIcon icon("/dm368/icon/file.png");
            QListWidgetItem *lst = new QListWidgetItem(icon,Tmp + fileType);
            lst->setSizeHint(QSize(60,60));  //每次改变Item的高度
            lst->setFont(font);
            ListWidgetFile->addItem(lst);
            ShowImageNode = ShowImageNode->next;
            gl_MenCount++;
         }
        str.sprintf("[%d条记录]", gl_maxfilecnt);
        ListStatus->setText(str);
    }
    ListWidgetFile->setCurrentRow(currentrow);
    printf("gl_maxfilecnt %d gl_MenCount:%d\n",gl_maxfilecnt,gl_MenCount);
}

void FileViewer::FileInfoListDestory(){
    int index;
    for(index = 0;index < gl_MenCount ;index++){
        QListWidgetItem *item = ListWidgetFile->takeItem(0);
        delete item;
    }
    printf("SecFileListDestory gl_MenCount:%d\n",gl_MenCount);
    gl_MenCount = 0;
    gl_maxfilecnt = 0;
    llist_destroy(gl_MyBmpLList);
}



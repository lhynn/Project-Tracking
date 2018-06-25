#include "fileviewer.h"

FileViewer::FileViewer( QWidget *parent, Qt::WindowFlags  f )
    : QDialog( parent, f )
{
    lastvalidrow=0;
    setWindowTitle(tr("File Browser"));
    DirName = new QLabel();
    QFont font("JPTZ", 22, 55);    //( “Microsoft YaHei”, 10, 75);
    DirName->setFont(font);
    DirName->setText("存储文件列表");
    ListWidgetFile = new QListWidget();
    ListWidgetFile->setIconSize(QSize(40,26));
    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    vbLayout->addWidget(DirName);
    vbLayout->addWidget(ListWidgetFile);

    ListStatus = new QLabel();
    QFont font1("JPTZ", 20, 55);    //( “Microsoft YaHei”, 10, 75);
    ListStatus->setFont(font1);
    vbLayout->addWidget(ListStatus);
    currentrow = 0;
}


FileViewer::~FileViewer()
{

}

int FileViewer::findcurrentItemPos()
{
    int y;
    int x = 100;
    int row;
    QListWidgetItem* pItem;

    for(y=0; y<576; y++)
    {
        pItem = ListWidgetFile->itemAt(x, y);
        row = ListWidgetFile->row(pItem);
        if(row == currentrow)
        {
            //printf("this y=%d, row=%d, current=%d\r\n", y, row, currentrow);
            return y;
        }
    }
}

void FileViewer::nextrow()
{
    if(currentrow < gl_maxfilecnt-1)
    {
        currentrow ++;
        ListWidgetFile->setCurrentRow(currentrow);
        showFileInfoList(1);
    }
    else
    {
        currentrow = 0;
        ListWidgetFile->setCurrentRow(currentrow);
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
    QFont font("JPTZ", 24, 55);
    PT_MyFileSt data = NULL;
    for(int i = 0;i < num;i++){
        if(ShowImageNode  != &gl_MyBmpLList->head){
            data = (PT_MyFileSt)ShowImageNode->data;
           // printf("data->name :%s\n",data->name);
         //   fflush(NULL);
            Tmp = QString(QLatin1String(data->name));
            QIcon icon("/dm368/icon/file.png");
            QListWidgetItem *lst = new QListWidgetItem(icon,"    " + Tmp + fileType);
            lst->setSizeHint(QSize(60, 60) );  //每次改变Item的高度
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

#if 0

void FileViewer::showFileInfoList()
{
    unsigned int i;
    QString str;
    QStringList string;
    string << "*" ;

    QString root = "/home/root/media";
    QDir rootDir(root);

    QStringList filters;
    filters<<QString("*.AVI")<<QString("*.jpg");
    rootDir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    rootDir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式（后缀为.jpeg等图片格式）

    QFileInfoList list = rootDir.entryInfoList(filters);

    QFont font("WenQuanYi Micro Hei", 20, 55);    //  ( “Microsoft YaHei”, 10, 75);

    ListWidgetFile->clear();
    filenumber = list.count();

    for(i=filenumber; i>0; i--)
    {
        QFileInfo tmpFileInfo = list.at(i-1);
        if(tmpFileInfo.isFile())
        {
            QIcon icon("/dm368/icon/file.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *pItem = new QListWidgetItem (icon, fileName);
            pItem->setSizeHint(QSize(60, 30) );  //每次改变Item的高度
            pItem->setFont(font);
            ListWidgetFile->addItem(pItem);
        }
    }
    str.sprintf("[%d条记录]", filenumber);
    ListStatus->setText(str);
}
#endif

#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include <QtGui>
#include "searchdir.h"
#include "llist.h"

class FileViewer : public QDialog		// file information dialogs widget
{
    Q_OBJECT
public:
    FileViewer( QWidget *parent=0, Qt::WindowFlags  f=0 );
    ~FileViewer();
public:
    QLabel* DirName;
    QListWidget* ListWidgetFile;
    QLabel* ListStatus;
    unsigned int lastvalidrow;
    unsigned int currentrow;

    int gl_MenCount;
    int gl_maxfilecnt;
    LLIST *gl_MyBmpLList;
    struct llist_node_st *ShowImageNode;
    QString fileType;

public:
    int findcurrentItemPos();
    void nextrow();
    void prevrow();

    void showFileInfoList(int num);
    void FileInfoListDestory(void);
    void iDirFileToLink(QString Type);
};


#endif // FILEVIEWER_H

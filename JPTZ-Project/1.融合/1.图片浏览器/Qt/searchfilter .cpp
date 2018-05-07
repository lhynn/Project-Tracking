#include "searchfilter.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QDir>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug>

SearchFilter::SearchFilter(QWidget *parent) : QWidget(parent)
{
    //输入框
    m_input_edit = new QLineEdit();
    m_input_edit->setMinimumHeight(35);
    //文件列表
    m_file_list_view = new QListView();
    
    m_string_list_model = new QStringListModel();
    m_proxy_model = new QSortFilterProxyModel();
    
    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(m_input_edit);
    main_layout->addWidget(m_file_list_view);
    
    this->setLayout(main_layout);
    
    connect(m_input_edit, SIGNAL(textChanged(QString)), this, SLOT(textChanged_input_edit(QString)));
    connect(m_file_list_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClick_listView(QModelIndex)));
    
    //这里绑定了选中变化时发生事件
    connect(m_file_list_view->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectChanged_listView(QItemSelection)));
}

//设置搜索过滤目录和文件后缀
void SearchFilter::Init(const QString &dir_str, const QStringList &filter_list)
{
    m_dir_str = dir_str;
    
    QDir* dir = new QDir(dir_str);
    dir->setNameFilters(filter_list);
    
    QStringList file_list = dir->entryList();
    
    m_string_list_model->setStringList(file_list);
    m_proxy_model->setSourceModel(m_string_list_model);
    
    m_file_list_view->setModel(m_proxy_model);
    m_file_list_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    //设置当前选中行 0行 0列
    QModelIndex index = m_proxy_model->index(0, 0);
    m_file_list_view->setCurrentIndex(index);
}

//选上一个
void SearchFilter::SetCurrentSelectFile_Prev()
{
    QModelIndex curr_index = m_file_list_view->currentIndex();
    int curr_row = curr_index.row();
    
    if (curr_row == 0) {
        QMessageBox::information(this, tr("提示"), tr("前面没有了"), QMessageBox::Ok);
        return ;
    }
    
    curr_row = curr_row - 1;
    
    curr_index = m_proxy_model->index(curr_row, 0);
    m_file_list_view->setCurrentIndex(curr_index);
    
    //当前选中
    curr_index = m_file_list_view->currentIndex();
    QString picture_name = curr_index.data().toString();
    QString picture_path = m_dir_str + picture_name;
    
    emit signal_current_select_file(picture_path);
}

//选中下一个
void SearchFilter::SetCurrentSelectFile_Next()
{
    QModelIndex curr_index = m_file_list_view->currentIndex();
    int curr_row = curr_index.row();
    
    int rows = m_proxy_model->rowCount();
    
    if (curr_row == (rows - 1)) {
        QMessageBox::information(this, tr("提示"), tr("后面没有了"), QMessageBox::Ok);
        return ;
    }
    
    curr_row = curr_row + 1;
    
    curr_index = m_proxy_model->index(curr_row, 0);
    m_file_list_view->setCurrentIndex(curr_index);
    
    //当前选中
    curr_index = m_file_list_view->currentIndex();
    QString picture_name = curr_index.data().toString();
    QString picture_path = m_dir_str + picture_name;
    
    emit signal_current_select_file(picture_path);
}

//QLineEdit输入发生变化事件
void SearchFilter::textChanged_input_edit(const QString &text)
{
    //正则方式
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(
                QRegExp::FixedString);
    QRegExp regExp(text, Qt::CaseInsensitive, syntax);
    m_proxy_model->setFilterRegExp(regExp);
}

//listview双击事件
void SearchFilter::onDoubleClick_listView(const QModelIndex &index)
{
    QString picture_name = index.data().toString();
    QString picture_path = m_dir_str + picture_name;
    
    emit signal_current_select_file(picture_path);
}

//选中发生变化时  不知道为什么这里没有触发（如果哪位前辈知道原因，指导一下）
void SearchFilter::selectChanged_listView(const QItemSelection &selection)
{
    QModelIndex index = m_file_list_view->currentIndex();
    QString picture_name = index.data().toString();
    QString picture_path = m_dir_str + picture_name;

　　emit signal_current_select_file(picture_path);
}
#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QWidget>

class QLineEdit;
class QListView;
class QStringListModel;
class QSortFilterProxyModel;
class QItemSelection;

class SearchFilter : public QWidget
{
    Q_OBJECT
public:
    explicit SearchFilter(QWidget *parent = 0);
    void Init(const QString &dir_str, const QStringList &filter_list);
    void SetCurrentSelectFile_Prev();
    void SetCurrentSelectFile_Next();
    
signals:
    //发送当前选中的文件名
    void signal_current_select_file(const QString& file_name);
    
private slots:
    void textChanged_input_edit(const QString& text);
    void onDoubleClick_listView(const QModelIndex& index);
    void selectChanged_listView(const QItemSelection& selection);
    
private:
    QString m_dir_str;
    QLineEdit* m_input_edit;
    QListView* m_file_list_view;
    QStringListModel* m_string_list_model;
    QSortFilterProxyModel* m_proxy_model;
};

#endif // SEARCHFILTER_H
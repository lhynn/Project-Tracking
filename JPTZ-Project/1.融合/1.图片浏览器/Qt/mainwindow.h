#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ImageViewer;
class SearchFilter;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_signal_next();
    void on_signal_prev();
    void on_signal_show_image(const QString& picture_path);
    
private:
    Ui::MainWindow *ui;
    
    QString dir_str;  //目录
    
    SearchFilter* m_widget_search_filter;
    ImageViewer* m_widget_image_viewr;
};

#endif // MAINWINDOW_H
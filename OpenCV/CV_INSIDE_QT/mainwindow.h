#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void Mat2QImage(Mat src);


private slots:
    void on_open_triggered();

private:
    Ui::MainWindow *ui;
    QImage *image;
};

#endif // MAINWINDOW_H

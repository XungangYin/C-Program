#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QColor>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //在QMainWindow中，下边两个同时开启才能在不按下鼠标的情况下追踪鼠标移动事件
    QMainWindow::setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
    if(image)
    {
        delete image;
        image = nullptr;
    }
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);

    if(!image){
        return ;
    }
    painter.drawImage(QPoint(0,0),*image);

}
//打开图片文件
void MainWindow::on_open_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open File"),"/home/yxg",
                                                tr("Image (*.png *jpg *.tiff)"));
    if(path.isEmpty()){
        return ;
    }
    Mat src = imread(path.toStdString());

    Mat2QImage(src);
}

//Mat 转化为QImag图像
void MainWindow::Mat2QImage(Mat src){
    // Since OpenCV uses BGR order, we need to convert it to RGB
    if(src.channels() == 3)
        cv::cvtColor(src, src, CV_BGR2RGB);

//    _image = new QImage(img.size().width, img.size().height, QImage::Format_RGB888);
    image = new QImage(src.size().width,src.size().height,QImage::Format_RGB888);
    mempcpy(image->scanLine(0),(unsigned char *)src.data,image->width()*image->height()*src.channels());

    // Resize the window to fit video dimensions
    resize(src.size().width, src.size().height);

//    QWidget::setMouseTracking(true);

    update();
}
//状态栏显示位置和当前位置的像素值
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if(!image)
        return;

    QPoint pos = event->pos();
    if(pos.x() <0 || pos.x()>image->width())
        return;
    if(pos.y() <0 || pos.y() > image->height())
        return;
    QString pos_x;
    pos_x = pos_x.setNum(pos.x());

    QString pos_y;
    pos_y = pos_y.setNum(pos.y());

    QColor pixel = image->pixelColor(pos);
    int r = pixel.red();
    int g = pixel.green();
    int b = pixel.blue();

    QString pixel_info = pos_x + "," + pos_y + " R:" + QString().setNum(r) +
                                               " G:" + QString().setNum(g) +
                                               " B:" + QString().setNum(b);
    ui->statusBar->showMessage(pixel_info);
    qDebug()<<pixel_info;
}

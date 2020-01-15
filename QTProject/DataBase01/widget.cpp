#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //打印QT支持的数据库驱动
    qDebug()<<QSqlDatabase::drivers();

    //添加Mysql数据库
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
       //链接数据库
//    db.setHostName("127.0.0.1");//数据库的ip
//    db.setUserName("root"); //数据库的用户名
//    db.setPassword("123"); //密码
//    db.setDatabaseName("YXG"); //使用哪个数据库

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./db.db");
    if( !db.open()){ //如果打开失败
        QMessageBox::warning(this,"错误", db.lastError().text());
        return;
    }
    QSqlQuery query(db);
    query.exec("CREATE TABLE student(int_col INT,float_col REAL,string_col TEXT;");


}

Widget::~Widget()
{
    delete ui;
}

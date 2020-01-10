#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>
#include <QPushButton>

#include "stone.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);

    //判断选中的棋子在上还是下
    bool isBottomSide(int &id);
    //红方在下 true
    bool beSide ;
    //void reDrawBoard(boo side);

    //绘制
    virtual void paintEvent(QPaintEvent *ev);
    void drawBorad(QPainter &painter,int d);

    //void drawStone(QPainter &paniter,bool bRedSisde);
    //点击
    void mousePressEvent();
    //释放
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    //传入鼠标点击位置
    void click(QPoint &pt);
    //鼠标点击棋子的id和其所在的行列
    virtual void click(int clickdeId,int row,int col);

    //获取点击位置棋子的id,否则返回-1；
    int getStoneId(const int &row ,const int &col);
    void drawStone(QPainter &painter,int id);
    //返回棋盘行列对应的像素坐标
    QPoint center(int row,int col);
    QPoint center(int id);
    //返回像素坐标对应的棋盘行列
    bool getRowCol(QPoint pt,int &row,int &col);
    //判斷某特定行列坐標是否存在活着的棋子
    bool isExistStone(int row,int col);
    //判断某行或某列是否存在棋子，并返回棋子个数
    int  countOfLineExistStone(int &selectId,int &row,int &col);
    //行走规则：红色先手，且交互执行
    void moveRule();


    //移动棋子
    bool canMove(int &selectId,int row,int col,int killId);
    //移动将、帅棋子
    bool canMoveJiang(int &selectId,int &row,int &col,int &killId);
    //士
    bool canMoveShi(int &selectId,int &row,int &col,int &killId);
    //相
    bool canMoveXiang(int &selectId,int &row,int &col,int &killId);
    //馬
    bool canMoveMa(int &selectId,int &row,int &col,int &killId);
    //兵
    bool canMoveBing(int &selectId,int &row,int &col,int &killId);
    //炮
    bool canMovePao(int &selectId,int &row,int &col,int &killId);
    //车
    bool canMoveChe(int &selectId,int &row,int &col,int &killId);
signals:

public slots:

private:
    //棋子
    Stone _s[32];
    //棋子的直径
    int _d;
    int _selectId;
    QPushButton *button;
};

#endif // BOARD_H

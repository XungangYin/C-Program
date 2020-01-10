#include "board.h"
#include <QPixmap>

Board::Board(QWidget *parent) : QWidget(parent)
{
    button = new QPushButton("悔棋",this);
    button->setGeometry(40*10,100,100,60);
    this->setGeometry(0,0,500,500);
    _d = 40;
    beSide = true;
    for(int i = 0; i< 32;++i){
        _s[i].init(i,beSide);
    }
    _selectId = -1;
}

void Board::drawBorad(QPainter &painter,int d){
    d = _d;
    //绘制十条横线
    for(int i = 1 ;i <=10; ++i){
        painter.drawLine(QPoint(d,i * d),QPoint(9*d,i*d) );
    }

    //绘制九条竖线
    for(int i = 1 ;i <=9; ++i){
        if(1 ==i || 9 == i)
            painter.drawLine(QPoint(i*d, d),QPoint(i*d,10*d) );
        else{
            painter.drawLine(QPoint(i*d, d),QPoint(i*d,5*d) );
            painter.drawLine(QPoint(i*d, 6*d),QPoint(i*d,10*d) );
        }
    }
    //绘制九宫格
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(6*d,d),QPoint(4*d,3*d));
    painter.drawLine(QPoint(4*d,10*d),QPoint(6*d,8*d));
    painter.drawLine(QPoint(6*d,10*d),QPoint(4*d,8*d));

}
void Board::paintEvent(QPaintEvent *ev){
    QPainter painter(this);

    //绘制背景图
    painter.drawPixmap(rect(),QPixmap(":/new/prefix1/timg.jpeg"));

    drawBorad(painter,_d);

    //绘制32颗棋子
    for(int i = 0; i< 32;++i){
        drawStone(painter,i);
    }

}

void Board::drawStone(QPainter &painter, int id){

    if(_s[id]._dead)
        return;

    QPoint c = center(id);
    QRect rect = QRect(c.x() - _d/2,c.y() - _d/2,_d,_d);

    painter.setPen(Qt::black);

    if(id == _selectId)
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(QColor(255,255,0)));

    if(_s[id]._red)
        painter.setPen(Qt::red);

    painter.drawEllipse(center(id),_d/2,_d/2);
    painter.setFont(QFont("system",_d/2,200));
    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));
}

QPoint Board::center(int id){
    return center(_s[id]._row,_s[id]._col);
}
QPoint Board::center(int row, int col){
    QPoint point;
    point.rx() = (col +1)*_d;
    point.ry() = (row +1)*_d;
    return point;
}


bool Board::canMove(int &selectId, int row,int col,int killId){
    /**
      1、判断是否有棋子选中且同色(前16个棋子是红色，后16为黑色)
      2、若同色且两枚棋子位置不同，则killId表示切换选择
    */
    if(killId != -1 && _s[selectId]._red == _s[killId]._red){
        selectId = killId;
        update();
        return false;
    }

    /**
      1 制定每種棋子的行走規則
    */
    switch (_s[selectId]._type) {
    case Stone::JIANG:
        return canMoveJiang(selectId,row,col,killId);
        break;
    case Stone::SHI:
        return canMoveShi(selectId,row,col,killId);
        break;
    case Stone::PAO:
        return canMovePao(selectId,row,col,killId);
        break;
    case Stone::MA:
        return canMoveMa(selectId,row,col,killId);
        break;
    case Stone::XIANG:
        return canMoveXiang(selectId,row,col,killId);
        break;
    case Stone::BING:
        return canMoveBing(selectId,row,col,killId);
        break;
    case Stone::CHE:
        return canMoveChe(selectId,row,col,killId);
        break;
    default:
        break;
    }

}

/**
  将棋移动规则：
  只能在九宫内，且每次只能移动一个格子，当将帅会面时，后一个吃掉前一个
*/
bool Board::canMoveJiang(int &selectId, int &row, int &col, int &killId)
{
//    if(_s[selectId]._red){
//        if(row > 2) return false;
//    }else {
//        if(row < 7) return false;
//    }

    if(isBottomSide(selectId)){  //上方
        if(row > 2) return false;
    }else {
        if(row < 7) return false;
    }
    if(col < 3 ||col >5)
        return false;
    /**
      走棋規則:
      將 只能在九宮內,且直線行走,所以dr或者dc必有一個值爲1
    */
    int dr = row - _s[selectId]._row;
    int dc = col - _s[selectId]._col;
    int d = abs(dr)*10 + abs(dc); //將帥:1 || 10,
    if(d == 1 || d == 10)
        return true;
    return false;
}

//士
bool Board::canMoveShi(int &selectId,int &row,int &col,int &killId){
    if(isBottomSide(selectId)){  //上方
        if(row > 2) return false;
    }else {
        if(row < 7) return false;
    }

    if(col < 3 ||col >5)
        return false;
    /**
      走棋規則:
      士: 只能在九宮內,且對角線行走,所以dr與dc值爲1
    */
    int dr = row - _s[selectId]._row;
    int dc = col - _s[selectId]._col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 11)
        return true;
    return false;
}
//象:不能過河
bool Board::canMoveXiang(int &selectId,int &row,int &col,int &killId){
    //不能過河
    if(isBottomSide(selectId)){  //上方
        if(row > 4) return false;
    }else {
        if(row < 5) return false;
    }

    //象所在的列一定是在偶數列上
    if(col %2 != 0)
        return false;
    /**
        1 找到象眼位置 (共有16個)
            方法:判斷點擊位置和 象 位置中間點坐標上是否有棋子
        2 判斷該象對應的象眼位置是否有棋子
    */

    /**
      走棋規則:
      象: 走田字格,所以dr與dc值 必爲2
    */
    int dr = row - _s[selectId]._row;
    int dc = col - _s[selectId]._col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 22){
        int rowYan = (_s[selectId]._row + row) /2;
        int colYan = (_s[selectId]._col + col) /2;
        if(!isExistStone(rowYan,colYan)) //象眼不不存在棋子
             return true;
    }
    return false;
}
//馬, 搬馬腿
bool Board::canMoveMa(int &selectId,int &row,int &col,int &killId){
    /**
      走棋規則:d == 21表示竖着走，否则横着走
      馬: 走日字格,所以dr與dc值差值爲2或者1 必爲21或者12
    */
    int dr = row - _s[selectId]._row;
    int dc = col - _s[selectId]._col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 21){
        //马腿位置
        int rowM = (_s[selectId]._row + row)/2;
        int colM = _s[selectId]._col;
        if(!isExistStone(rowM,colM))
            return true;
        else
            return false;
    }
    if( d == 12){
        //马腿位置
        int rowM = _s[selectId]._row ;
        int colM = _s[selectId]._col + col;
        if(!isExistStone(rowM,colM/2))
            return true;
        else
            return false;
    }
    return false;
}
//兵:不能走回頭路,且在過河前只能向前行走
bool Board::canMoveBing(int &selectId,int &row,int &col,int &killId){
    /**
      走棋規則:不能回頭
              過河前不能橫走
      兵: 走直線,類似將,所以d=1 || 10

       if(isBottomSide(selectId)){  //上方
    */

    if(!beSide){

        if(_s[selectId]._red){
            //不能後退
            if(_s[selectId]._row > row )
                return false;
            //上方,過河前不能橫走
            if(isBottomSide(selectId)){
                if(abs(_s[selectId]._col - col) >= 1)
                    return false;
            }
        }else{
             if(_s[selectId]._row < row ) return false;
             if(_s[selectId]._row >= 5){
                 if(abs(_s[selectId]._col - col) >= 1)
                     return false;
             }
        }
    }else{
        if(_s[selectId]._red){
            //不能後退
            if(_s[selectId]._row < row )
                return false;
            //上方,過河前不能橫走
            if(_s[selectId]._row >=5){
                if(abs(_s[selectId]._col - col) >= 1)
                    return false;
            }
        }else{
             if(_s[selectId]._row > row ) return false;
             if(_s[selectId]._row <=3){
                 if(abs(_s[selectId]._col - col) >= 1)
                     return false;
             }
        }

    }
    int dr = row - _s[selectId]._row;
    int dc = col - _s[selectId]._col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 1 || d == 10)
        return true;
    return false;
}

//炮:只能走直线，若直线上有棋子则不能走，吃子规则为隔子吃子
bool Board::canMovePao(int &selectId, int &row, int &col, int &killId){
   if(_s[selectId]._col != col && _s[selectId]._row != row)  //必须在同一行或者列
       return false;
   else{
        if(0 == countOfLineExistStone(selectId,row,col) && -1 == killId){  //走棋
            return true;
        }
        if(1 == countOfLineExistStone(selectId,row,col) && -1 != killId)
            return true;
   }
    return false;
}

//车：直线，且吃掉直线上任意对方的棋子
bool Board::canMoveChe(int &selectId, int &row, int &col, int &killId){
    if(_s[selectId]._col != col && _s[selectId]._row != row)  //必须在同一行或者列
        return false;
    else{
         if(0 == countOfLineExistStone(selectId,row,col)){  //走棋
             return true;
         }
         if(-1 != killId && 0 == countOfLineExistStone(selectId,row,col)){  //如果点击的位置有棋子且中间没有棋子阻碍，则吃掉
               return true;
         }
    }
     return false;
}

int Board::getStoneId(const int &row, const int &col){
    int clickdeId = -1;
    for(int i = 0; i< 32; ++i){
        if(_s[i]._col == col && _s[i]._row == row && (!_s[i]._dead )){
            clickdeId = i;
            return clickdeId;
        }
    }
    return clickdeId;
}

void Board::click(int clickdeId, int row, int col){
    //如果之前没有棋子被选中
    if(-1 == _selectId){
       if(clickdeId != -1){
           _selectId = clickdeId;
           update();
       }

    }else{           //如果之前有棋子被选中，则移动棋子
        if(canMove(_selectId,row,col,clickdeId)){   //该棋子能否移动
            _s[_selectId]._row = row;
            _s[_selectId]._col = col;
            if(-1 != clickdeId)
                _s[clickdeId]._dead = true;
            _selectId = -1;
            update();
        }
    }
}
void Board::click(QPoint &pt){
    //将pt转化为棋盘的行列值
    //判断该行列位置是否有棋子，并获取点击到的行列值
    int row =  0,col = 0;
    bool bRet = getRowCol(pt,row,col);
    if(!bRet)   //点到棋盘外边
        return;

    //标记本次点击的棋子
    int clickdeId = getStoneId(row,col);

    click(clickdeId,row,col);
}

void Board::mouseReleaseEvent(QMouseEvent *ev){

    if(ev->button() != Qt::LeftButton)
        return ;

    QPoint pt = ev->pos();
    click(pt);
}

bool Board::getRowCol(QPoint pt, int &row, int &col){
    //比较慢的方法
    for(row =0; row <= 9;row++){
        for(col =0 ;col <=8; col++){
            QPoint p = center(row,col);
            int dx = p.x() - pt.x();
            int dy = p.y() - pt.y();
            int dist = dx*dx + dy * dy;
            if(dist < (_d/2)*(_d/2))
                return true;
        }
    }
    return false;
}

//判斷方法:32課棋子所在的位置是否與提供的行列相同.(也可以返回棋子的ID，便知道该棋子的类型了车马炮兵等)
bool Board::isExistStone(int row,int col){
    for(int i = 0; i < 32; ++i){
        if(_s[i]._row == row && _s[i]._col == col && !_s[i]._dead)
            return true;
    }
    return false;
}

//判断row col位置和当前选中棋子之间是否有棋子
//获取点击位置和selectedId之间所有的行列位置坐标
int Board::countOfLineExistStone(int &selectId, int &row, int &col){
    int count  = 0;
    if(_s[selectId]._row == row){   //若同行，则左右

        int min = _s[selectId]._col > col ? col : _s[selectId]._col;
        int max = _s[selectId]._col > col ? _s[selectId]._col : col;
        for(int i = min+1; i < max; i++){
            if (isExistStone(row,i))
                    count++;
        }
        return count;
    }
    if(_s[selectId]._col == col){
        int min = _s[selectId]._row > row ? row : _s[selectId]._row;
        int max = _s[selectId]._row > row ? _s[selectId]._row : row;
        for(int i = min+1; i < max; i++){
            if (isExistStone(i,col))
                    count++;;
        }
        return count;
        /*
        if(_s[selectId]._row >= row){
            for(int i = row;i<_s[selectId]._row;i++){
                if (isExistStone(i,col))
                    return true;
            }
        }else{
            for(int i = _s[selectId]._row +1;i<= row;i++){
                if (isExistStone(i,col))
                    return true;
            }
        } */
    }
    return count;
}

bool Board::isBottomSide(int &id){
    if(_s[id]._row <= 4)return true;
    else return false;
}























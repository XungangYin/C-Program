#include "stone.h"

Stone::Stone()
{

}

QString Stone::getText(){

    switch (this->_type) {
    case CHE:
        return "车";
    case JIANG:
        return "将";
    case PAO:
        return "炮";
    case MA:
        return "马";
    case BING:
        return "兵";
    case XIANG:
        return "相";
    case SHI:
        return "士";
    default:
        return "错误";
    }
}
void Stone::init(int id, bool side){
    _id = id;
    _dead = false;
    if(side){
       // _red = id >= 16; //每16个为一类

        if(id < 16){
            _row = pos[id].row;
            _col = pos[id].col;
            _type = pos[id].type;
            _red = false;
        }else{
            _row = 9 - pos[id-16].row;
            _col = 8 - pos[id-16].col;
            _type = pos[id-16].type;
            _red = true;
        }
    }else{
        //_red = id < 16; //每16个为一类

        if(id < 16){
            _row = pos[id].row;
            _col = pos[id].col;
            _type = pos[id].type;
            _red = true;
        }else{
            _row = 9 - pos[id-16].row;
            _col = 8 - pos[id-16].col;
            _type = pos[id-16].type;
            _red = false;
        }
    }
}

void Stone::rotate(){
    _row = 9 - _row;
    _col = 8 - _col;
}

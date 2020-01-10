#ifndef STEP_H
#define STEP_H

#include <QObject>

/**
    保存操作记录的类
*/

class Step : public QObject
{
    Q_OBJECT
public:
    explicit Step(QObject *parent = nullptr);
    //最后一次操作的对象ID；
    int _preId;
    //最后操作的对象是否发生了位移（若只是点击选中则返回false）
    bool _bMoved;

private:

};

#endif // STEP_H

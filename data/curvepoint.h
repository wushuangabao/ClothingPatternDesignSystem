#ifndef CURVEPOINT_H
#define CURVEPOINT_H

#include <QPointF>

class CurvePoint
{
public:
    CurvePoint(qreal x,qreal y,CurvePoint *pre=nullptr);
    ~CurvePoint();

    CurvePoint *pre=nullptr;
    CurvePoint *next=nullptr;

    qreal x=0;
    qreal y=0;

    bool isFirst;
    bool isLast;
    bool isCtrlPoint;

    void setFirst();
    void setFirst(CurvePoint *pre); //设置为曲线的第一个点，且有控制点
    void setLast();
    void setLast(CurvePoint *next); //设置为曲线最后一个点，且有控制点
};

#endif // CURVEPOINT_H

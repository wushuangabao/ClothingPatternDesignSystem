#ifndef CURVEPOINT_H
#define CURVEPOINT_H

#include <QPointF>

class CurvePoint
{
public:
    CurvePoint(int id,CurvePoint *pre=nullptr);
    ~CurvePoint();

    CurvePoint *pre=nullptr;
    CurvePoint *next=nullptr;

    int id;

    bool isFirst; //起点
    bool isLast; //终点
    bool isCtrlPoint;

    void setFirst();
    void setFirst(CurvePoint *pre); //设置为曲线的第一个点，且有控制点
    void setLast();
    void setLast(CurvePoint *next); //设置为曲线最后一个点，且有控制点
};

#endif // CURVEPOINT_H

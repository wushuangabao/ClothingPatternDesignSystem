#include "curvepoint.h"

CurvePoint::CurvePoint(qreal x,qreal y,CurvePoint* pre)
{
    this->x=x;
    this->y=y;
    isFirst=false;
    isLast=false;
    isCtrlPoint=false;
    if(pre!=nullptr)
    {
        pre->next=this;
        this->pre=pre;
    }
}

CurvePoint::~CurvePoint()
{
}

void CurvePoint::setFirst()
{
    isFirst = true;
}

void CurvePoint::setLast()
{
    isLast = true;
}

void CurvePoint::setFirst(CurvePoint *pre)
{
    isFirst = true;
    if(pre!=nullptr)
    {
        pre->next=this;
        this->pre=pre;
        pre->isCtrlPoint=true;
    }
}

void CurvePoint::setLast(CurvePoint *next)
{
    isLast = true;
    if(next!=nullptr)
    {
        next->pre=this;
        this->next=next;
        next->isCtrlPoint=true;
    }
}

#include "curvepoint.h"

/**
 * @brief
 *
 * @param id
 * @param pre
 */
CurvePoint::CurvePoint(int id,CurvePoint* pre)
{
    this->id=id;
    isFirst=false;
    isLast=false;
    isCtrlPoint=false;
    if(pre!=nullptr)
    {
        pre->next=this;
        this->pre=pre;
    }
}

/**
 * @brief
 *
 */
CurvePoint::~CurvePoint()
{
}

/**
 * @brief
 *
 */
void CurvePoint::setFirst()
{
    isFirst = true;
}

/**
 * @brief
 *
 */
void CurvePoint::setLast()
{
    isLast = true;
}

/**
 * @brief 将自身设置为起点
 *
 * @param pre 指向前一个点（bezier的控制点）的指针
 */
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

/**
 * @brief 将自身设置为终点
 *
 * @param next
 */
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

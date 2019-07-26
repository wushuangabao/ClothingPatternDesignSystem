#include "curvepoint.h"

/**
 * @brief 构造函数
 *
 * @param id 点在数组中的索引
 * @param pre 前一个点的指针
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
 * @brief 拷贝构造函数
 * @param copyObj
 */
CurvePoint::CurvePoint(const CurvePoint *copyObj)
{
    id = copyObj->id;
    isFirst = copyObj->isFirst;
    isLast = copyObj->isLast;
    isCtrlPoint = copyObj->isCtrlPoint;
}

CurvePoint::~CurvePoint()
{
}

/**
 * @brief 将自身设置为起点
 *
 * @param pre 指向前一个点（也是bezier的控制点）的指针
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
 * @param next 指向后一个点（也是bezier的控制点）的指针
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

#ifndef CURVEPOINT_H
#define CURVEPOINT_H

#include <QPointF>

/**
 * @brief
 *
 */
class CurvePoint
{
public:
    CurvePoint(int id,CurvePoint *pre=nullptr);

    ~CurvePoint();

    CurvePoint *pre=nullptr; /**< 前指针 */
    CurvePoint *next=nullptr; /**< 后指针 */

    int id; /**< 点在pointData数组中的id */

    bool isFirst; /**< 是否为起点 */
    bool isLast; /**< 是否为终点 */
    bool isCtrlPoint; /**< 是否为贝塞尔曲线的控制点 */

    void setFirst(CurvePoint *pre=nullptr);
    void setLast(CurvePoint *next=nullptr);
};

#endif // CURVEPOINT_H

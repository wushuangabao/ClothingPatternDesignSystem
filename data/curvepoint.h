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
    /**
     * @brief
     *
     * @param id
     * @param pre
     */
    CurvePoint(int id,CurvePoint *pre=nullptr);
    /**
     * @brief
     *
     */
    ~CurvePoint();

    CurvePoint *pre=nullptr; /**< TODO: describe */
    CurvePoint *next=nullptr; /**< TODO: describe */

    int id; /**< TODO: describe */

    bool isFirst; //起点 /**< TODO: describe */
    bool isLast; //终点 /**< TODO: describe */
    bool isCtrlPoint; /**< TODO: describe */

    /**
     * @brief
     *
     */
    void setFirst();
    /**
     * @brief
     *
     * @param pre
     */
    void setFirst(CurvePoint *pre); //设置为曲线的第一个点，且有控制点
    /**
     * @brief
     *
     */
    void setLast();
    /**
     * @brief
     *
     * @param next
     */
    void setLast(CurvePoint *next); //设置为曲线最后一个点，且有控制点
};

#endif // CURVEPOINT_H

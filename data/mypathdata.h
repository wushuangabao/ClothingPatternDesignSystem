#ifndef MYPATHDATA_H
#define MYPATHDATA_H

#include <QPointF>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QMap>
#include <QString>
#include "curvepoint.h"

class QPainterPath;

/**
 * @brief
 *
 */
struct PathData{
    int id; /**< TODO: describe */
    bool isLine; /**< TODO: describe */
    CurvePoint* startPoint; //起点指针 /**< TODO: describe */
    CurvePoint* endPoint;  //终点指针 /**< TODO: describe */
    QPainterPath *path; //原始路径数据 /**< TODO: describe */
};

/**
 * @brief
 *
 */
class MyPathData
{
public:
    /**
     * @brief
     *
     * @param name
     */
    MyPathData(QString name);
    /**
     * @brief
     *
     */
    ~MyPathData();

    QString name=""; /**< TODO: describe */
    QFile file; /**< TODO: describe */

    const static int MAX_NUM_POINT=250; /**< TODO: describe */
    const static int MAX_NUM_PATH=100; /**< TODO: describe */
    int numberPath=0; /**< TODO: describe */
    int numberPoint=0; /**< TODO: describe */
    PathData pathData[MAX_NUM_PATH]; /**< TODO: describe */
    QPointF pointData[MAX_NUM_POINT]; /**< TODO: describe */
    QMap<QString,int> pointMap; //记录点的名称，用于查找特殊点，如smallCroPoint /**< TODO: describe */

    /**
     * @brief
     *
     * @param endPoint
     * @param idStartPoint
     * @return bool
     */
    bool addLineTo(QPointF endPoint,int idStartPoint=-1);
    /**
     * @brief
     *
     * @param startPoint
     * @param endPoint
     */
    void addLine(QPointF startPoint,QPointF endPoint);
    /**
     * @brief
     *
     * @param points
     * @param ctrlPoint1
     * @param ctrlPoint2
     * @param path
     */
    void addCurve(QList<QPointF> points,QPointF ctrlPoint1,QPointF ctrlPoint2,QPainterPath path);
    /**
     * @brief
     *
     * @param point
     * @param name
     * @return int
     */
    int addPoint(QPointF point,QString name=""); //将point加入pointData数组（若已存在就不会添加），返回在数组中的索引
    /**
     * @brief
     *
     * @param ctrlPoint
     * @return int
     */
    int addCtrlPoint(QPointF ctrlPoint); //将ctrlPoint加入pointData数组（不管是否已存在同位置的点），返回在数组中的索引
    /**
     * @brief
     *
     * @param point
     * @param addIfNotFind
     * @return int
     */
    int findPoint(QPointF point,bool addIfNotFind=false); //在pointData数组中寻找point，返回它的索引。addIfNotFind表示若没有找到，自动添加进数组。返回-1表示没找到也没添加。
    /**
     * @brief
     *
     * @param point
     * @return bool
     */
    bool pointMapHas(QPointF point);
    /**
     * @brief
     *
     * @param name
     * @return QPointF
     */
    QPointF findPoint(QString name);
//    QString findName(QPointF point);
//    QString findName(qreal x,qreal y);
    /**
     * @brief
     *
     * @param id
     * @return QString
     */
    QString findName(int id);
    /**
     * @brief
     *
     * @param point
     * @return QString
     */
    QString stringOf(CurvePoint *point);
    /**
     * @brief
     *
     * @param point
     * @return QString
     */
    QString stringsOf(CurvePoint *point);
//    CurvePoint pointOf(QString str);
//    CurvePoint pointsOf(QString str);

    /**
     * @brief
     *
     * @param path
     * @return bool
     */
    bool saveTo(QString path);
//    static bool equal(QPointF p1,CurvePoint *p2,qreal E); //判断两点是否相等（对应坐标相差不超过变量(E)mm）
    /**
     * @brief
     *
     */
    void clear(); //清空所有数据

private:
    const qreal E=0.1; /**< TODO: describe */
    int idCurrentPoint=0; //记录当前点在pointData数组中的索引 /**< TODO: describe */
//    bool equal(CurvePoint *p1,CurvePoint *p2); //判断两点是否相等（对应坐标相差不超过常量(E)mm）
    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @return bool
     */
    bool equal(QPointF p1,QPointF p2);
};

#endif // MYPATHDATA_H

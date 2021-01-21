#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QtMath>
#include <QPainterPath>
#include <QList>

class PainterArea;
class MyRule;
class MyPathData;
struct Path;

static const qreal g_minUnit = 0.01; /**< 比较实数时的精度 */

class MyPainter
{
public:
    MyPainter();
    ~MyPainter();

    QPainterPath *myPath; /**< 绘图路径 */
    MyPathData *myData;   /**< 绘图路径数据 */
    QPointF *startPoint;  /**< 坐标系中的位置参照点 */

    // 设置位置参照点
    void setStartPoint(qreal x,qreal y);
    void setStartPoint(QPointF point);
    // 画曲线
    void curve(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);
    // 使用 Path（路径类型）数据来构造 myPath、myData，目前仅供 MyRule 使用
    void parsePathCode(Path path);

    QPainterPath drawByPathData(MyPathData* pathData, int astmTag = 0, QList<QPointF>* posList = nullptr, QList<QString>* nameList = nullptr);
    QPainterPath drawPointsByData(MyPathData* pathData);

    // 辅助函数
    static inline bool equal(qreal v1,qreal v2);
    static inline bool equal(QPointF p1,QPointF p2);
    static bool currentPositionequal(QPainterPath path,QPointF p);
    static qreal distanceBetween(QPointF p1,QPointF p2);
    static QPointF getPointOnLine(QPointF p1,QPointF p2,qreal proportion); //线段上某等分点
    static QPointF getSymmetryPoint(QPointF point,QPointF center); //点point关于点center的对称点
    static QPointF convertPoint(QPointF pFromRule,int n=10); //将点的坐标值放大n倍

private:
    bool addPointByRule(QList<QPointF>* points, MyRule* rule, QString* pathCode);
    void addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints);
    QString getConnectType(QString code);

    // 画路径并添加到myPathData中
    void addPointData(QPointF point,QString name="");
    void brokenLineThrough(QList<QPointF> points);
    void brokenLineThrough(QPainterPath brokenLine);
    void curveThrough(QList<QPointF> points);
    void curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);
};

#endif // MYPAINTER_H

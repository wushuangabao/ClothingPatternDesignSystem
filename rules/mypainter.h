#ifndef MYPAINTER_H
#define MYPAINTER_H

#include <QtMath>
#include <QPoint>
#include <QPainterPath>
#include <QList>

class PainterArea;
class MyRule;

class MyPainter
{
public:
    MyPainter(PainterArea *parent);
    ~MyPainter();
    PainterArea *parent;  /**< 绘图区指针 */
    QPainterPath *myPath; /**< 绘图路径 */
    QPointF *startPoint;  /**< 坐标系中的位置参照点 */

    void setStartPoint(qreal x,qreal y);
    void setStartPoint(QPointF point);

    void parseCode(MyRule* rule, QString pathCode);

private:
    const qreal minUnit = 0.01; /**< 比较实数时的精度 */

    // 辅助函数
    bool equal(qreal v1,qreal v2);
    bool equal(QPointF p1,QPointF p2);
    bool currentPositionequal(QPainterPath path,QPointF p);
    bool addPointByRule(QList<QPointF>* points, MyRule* rule, QString namePoint);
    qreal distanceBetween(QPointF p1,QPointF p2);
    QPointF getPointOnLine(QPointF p1,QPointF p2,qreal proportion);
    QPointF getSymmetryPoint(QPointF point,QPointF center);
    void addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints);
    void curve(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);
    QString connectType(QString code);

    // 画路径并添加到myPathData中
    void addPointData(QPointF point,QString name="");
    void brokenLineThrough(QList<QPointF> points);
    void brokenLineThrough(QPainterPath brokenLine);
    void curveThrough(QList<QPointF> points);
    void curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);

private slots:

};

#endif // MYPAINTER_H

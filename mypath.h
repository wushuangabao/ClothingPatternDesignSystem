#ifndef MYPATH_H
#define MYPATH_H

#include <QtMath>
#include <QPoint>
#include <QPainterPath>
#include <QList>

class PainterArea;
class CurvePoint;

class MyPath
{
public:
    MyPath(PainterArea *parent);
    ~MyPath();
    PainterArea *parent;
    QPainterPath *myPath;
    QPointF *startPoint;

    void setStartPoint(qreal x,qreal y);
    void setStartPoint(QPointF point);
    void initializeSize();

    QPainterPath auxiliaryLinesH_1();
    QPainterPath auxiliaryLinesH_2();
    QPainterPath outLines_data();

    void drawWaistLine1(int intCase);
    void drawWaistLine2(int intCase);

    void drawWaist1(int wCase,int sangCase);
    void drawWaist2(int wCase,int sangCase);
    QList<QPointF> drawSang1(QPointF pointSang,QPointF vertexSang,qreal sang);
    QList<QPointF> drawSang2(QPointF pointSang,QPointF vertexSang,qreal sang);

    void drawOutline1(int typeSang);
    void drawOutline2(int typeSang);
    void curveThrough_data(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);

private:
    int pantsHeight;
    int pantsL;
    int pantsW;
    int pantsH;
    int pantsCrotchH;

    int w_h_height;
    int knee_height;
    int hWidth1;
    int hWidth2;
    int feetWidth;
    int smallCro;
    int halfCroWidth1;
    int bigCro;
    int xMidVerticalLine2;
    qreal sang1;
    qreal sang2;

    int rightWaist1 = 7;
    int downWaist1 = 9;
    int leftCro1 = 5;

    int leftH2;
    int downCro2 = 15;
    int leftCro2 = 12;
    int upWaist2 = 28;

    int deltaKneeWidth = 10;
    int waistBandWidth = 30;

    QPointF smallCroPoint;
    QPointF bigCroPoint;
    QPointF leftUpPoint1;
    QPointF leftUpPoint2;
    QPointF rightUpPoint1;
    QPointF rightUpPoint2;
    QPointF leftHPoint2;
    QPointF getSymmetryPoint(QPointF point,QPointF center);
    QPointF getIntersection(qreal x1,qreal y1,qreal x2,qreal y2,qreal k,qreal b);
    QPointF getIntersection(QPointF p1,QPointF p2,qreal k,qreal b);
    QPointF getIntersection(QPointF p1,QPointF p2,QPointF p3,QPointF p4);
    QPointF getPointOnRadial(qreal x1,qreal y1,qreal x2,qreal y2,qreal l);
    QPointF getPointOnRadial(QPointF p1,QPointF p2,qreal l);
    QPointF getPointOnRadial_Down(qreal x1,qreal y1,qreal k,qreal l);
    QPointF getPointOnRadial_Down(QPointF p,qreal k,qreal l);
    QPointF getPointOnRadial_Up(qreal x1,qreal y1,qreal k,qreal l);
    QPointF getPointOnRadial_Up(QPointF p,qreal k,qreal l);
    QPointF getPointOfSang(QPointF vertex,QPointF leftUp,QPointF rightUp);
    QPointF getPointOfSang_D(QPointF p1,QPointF p2,qreal distance);
    QPointF getPointOfSang_D(qreal x1,qreal y1,qreal x2,qreal y2,qreal distance);
    QPointF getPointOfSang_P(QPointF p1,QPointF p2,qreal proportion);
    QPointF getPointOfSang_P(qreal x1,qreal y1,qreal x2,qreal y2,qreal proportion);
    QPointF getVertexOfSang_Up(QPointF p1,QPointF ps,qreal ls);
    QPointF getVertexOfSang_Down(QPointF p1,QPointF ps,qreal ls);
    QPointF getIntersection_L(QPointF p,qreal l,qreal k,qreal b);
    QPointF getIntersection_R(QPointF p,qreal l,qreal k,qreal b);

    qreal distanceBetween(QPointF p1,QPointF p2);
    //    qreal distanceBetween(QPointF p1,QPointF p2,QPointF p3,QPointF p4);

    QPainterPath lineThrough2P(QPointF startPoint,QPointF endPoint);
    void brokenLineThrough(QList<QPointF> points);
    void brokenLineThrough(QPainterPath brokenLine);
    QPainterPath curveThrough3P(QPointF A,QPointF B,QPointF C,QPointF ctrl1,QPointF ctrl4);
    void curveThrough(QList<QPointF> points);
    void curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);

    void addPoint(QPointF point,QString name="");
    void addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints);
    bool currentPositionequal(QPainterPath path,QPointF p);

private slots:

};

#endif // MYPATH_H

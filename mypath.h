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

    constexpr static const qreal minUnit = 0.1; /**< 比较实数时的精度 */
    static bool currentPositionequal(QPainterPath path,QPointF p);
    static bool equal(qreal v1,qreal v2);
    static bool equal(QPointF p1,QPointF p2);
    static bool pointIsOnSegment(QPointF p,QPointF sp1,QPointF sp2);
    static qreal distanceBetween(QPointF p1,QPointF p2);
    static qreal distanceBetween(QPointF p,QPointF p1,QPointF p2);
    static QPointF getSymmetryPoint(QPointF point,QPointF center);
    static QPointF getIntersection(qreal x1,qreal y1,qreal x2,qreal y2,qreal k,qreal b);
    static QPointF getIntersection(QPointF p1,QPointF p2,qreal k,qreal b);
    static QPointF getIntersection(QPointF p1,QPointF p2,QPointF p3,QPointF p4);
    static QPointF getIntersection_L(QPointF p,qreal l,qreal k,qreal b);
    static QPointF getIntersection_R(QPointF p,qreal l,qreal k,qreal b);
    static QPointF getPointOnRadial(qreal x1,qreal y1,qreal x2,qreal y2,qreal l);
    static QPointF getPointOnRadial(QPointF p1,QPointF p2,qreal l);
    static QPointF getPointOnRadial_Down(qreal x1,qreal y1,qreal k,qreal l);
    static QPointF getPointOnRadial_Down(QPointF p,qreal k,qreal l);
    static QPointF getPointOnRadial_Up(qreal x1,qreal y1,qreal k,qreal l);
    static QPointF getPointOnRadial_Up(QPointF p,qreal k,qreal l);
    static QPointF getPointOfSang(QPointF vertex,QPointF leftUp,QPointF rightUp);
    static QPointF getPointOfSang_D(QPointF p1,QPointF p2,qreal distance);
    static QPointF getPointOfSang_D(qreal x1,qreal y1,qreal x2,qreal y2,qreal distance);
    static QPointF getPointOfSang_P(QPointF p1,QPointF p2,qreal proportion);
    static QPointF getPointOfSang_P(qreal x1,qreal y1,qreal x2,qreal y2,qreal proportion);
    static QPointF getVertexOfSang_Up(QPointF p1,QPointF ps,qreal ls);
    static QPointF getVertexOfSang_Down(QPointF p1,QPointF ps,qreal ls);

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

    QPainterPath lineThrough2P(QPointF startPoint,QPointF endPoint);
    void brokenLineThrough(QList<QPointF> points);
    void brokenLineThrough(QPainterPath brokenLine);
    QPainterPath curveThrough3P(QPointF A,QPointF B,QPointF C,QPointF ctrl1,QPointF ctrl4);
    void curveThrough(QList<QPointF> points);
    void curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);

    void addPoint(QPointF point,QString name="");
    void addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints);

private slots:

};

#endif // MYPATH_H

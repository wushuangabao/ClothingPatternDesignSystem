#ifndef MYPATH_H
#define MYPATH_H

#include <QtMath>
#include <QPoint>
#include <QPainterPath>
#include <QList>

class PainterArea;
class CurvePoint;

/**
 * @brief
 *
 */
class MyPath
{
public:
    /**
     * @brief
     *
     * @param parent
     */
    MyPath(PainterArea *parent);
    /**
     * @brief
     *
     */
    ~MyPath();
    PainterArea *parent; /**< TODO: describe */
    QPainterPath *myPath; /**< TODO: describe */
    QPointF *startPoint; /**< TODO: describe */

    /**
     * @brief
     *
     * @param x
     * @param y
     */
    void setStartPoint(qreal x,qreal y);
    /**
     * @brief
     *
     * @param point
     */
    void setStartPoint(QPointF point);
    /**
     * @brief
     *
     */
    void initializeSize();

    /**
     * @brief
     *
     * @return QPainterPath
     */
    QPainterPath auxiliaryLinesH_1();
    /**
     * @brief
     *
     * @return QPainterPath
     */
    QPainterPath auxiliaryLinesH_2();
    /**
     * @brief
     *
     * @return QPainterPath
     */
    QPainterPath outLines_data();

    /**
     * @brief
     *
     * @param intCase
     * @param points
     * @return QPainterPath
     */
    QPainterPath sang_1(int intCase,QList<QPointF> *points);
    /**
     * @brief
     *
     * @param intCase
     * @param points
     * @return QPainterPath
     */
    QPainterPath sang_2(int intCase,QList<QPointF> *points);
    /**
     * @brief
     *
     * @param wCase
     * @param sangCase
     * @param points
     * @return QPainterPath
     */
    QPainterPath waist_1(int wCase,int sangCase,QList<QPointF> *points);
    /**
     * @brief
     *
     * @param wCase
     * @param sangCase
     * @param points
     * @return QPainterPath
     */
    QPainterPath waist_2(int wCase,int sangCase,QList<QPointF> *points);
    /**
     * @brief
     *
     * @param wCase
     * @param sangCase
     * @return QPainterPath
     */
    QPainterPath drawWaist1(int wCase,int sangCase);
    /**
     * @brief
     *
     * @param wCase
     * @param sangCase
     * @return QPainterPath
     */
    QPainterPath drawWaist2(int wCase,int sangCase);
    /**
     * @brief
     *
     * @param pointSang
     * @param vertexSang
     * @param ls
     * @param sang
     * @param points
     * @return QPainterPath
     */
    QPainterPath drawSang1(QPointF pointSang,QPointF vertexSang,qreal ls,qreal sang,QList<QPointF> *points);
    /**
     * @brief
     *
     * @param pointSang
     * @param vertexSang
     * @param ls
     * @param sang
     * @param points
     * @return QPainterPath
     */
    QPainterPath drawSang2(QPointF pointSang,QPointF vertexSang,qreal ls,qreal sang,QList<QPointF> *points);

    /**
     * @brief
     *
     * @param typeSang
     */
    void drawOutline1(int typeSang);
    /**
     * @brief
     *
     * @param typeSang
     */
    void drawOutline2(int typeSang);
    /**
     * @brief
     *
     * @param points
     * @param firstCtrlPoint
     * @param lastCtrlPoint
     */
    void curveThrough_data(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);

    /**
     * @brief
     *
     * @param path1
     * @param path2
     * @return QPainterPath
     */
    static QPainterPath cutPath(QPainterPath path1,QPainterPath path2);//类似于path1-path2，区别在于返回的QPainterPath不闭合
    /**
     * @brief
     *
     * @param path1
     * @param path2
     * @return QPainterPath
     */
    static QPainterPath intersectPath(QPainterPath path1,QPainterPath path2);//类似于path1&path2，区别在于返回的QPainterPath不闭合
private:
    int pantsHeight; /**< TODO: describe */
    int pantsL; /**< TODO: describe */
    int pantsW; /**< TODO: describe */
    int pantsH; /**< TODO: describe */
    int pantsCrotchH; /**< TODO: describe */

    int w_h_height; /**< TODO: describe */
    int knee_height; /**< TODO: describe */
    int hWidth1; /**< TODO: describe */
    int hWidth2; /**< TODO: describe */
    int feetWidth; /**< TODO: describe */
    int smallCro; /**< TODO: describe */
    int halfCroWidth1; /**< TODO: describe */
    int bigCro; /**< TODO: describe */
    int xMidVerticalLine2; /**< TODO: describe */
    qreal sang1; /**< TODO: describe */
    qreal sang2; /**< TODO: describe */

    int rightWaist1 = 7; /**< TODO: describe */
    int downWaist1 = 9; /**< TODO: describe */
    int leftCro1 = 5; /**< TODO: describe */

    int leftH2; /**< TODO: describe */
    int downCro2 = 15; /**< TODO: describe */
    int leftCro2 = 12; /**< TODO: describe */
    int upWaist2 = 28; /**< TODO: describe */

    int deltaKneeWidth = 10; /**< TODO: describe */
    int waistBandWidth = 30; /**< TODO: describe */

    QPointF smallCroPoint; /**< TODO: describe */
    QPointF bigCroPoint; /**< TODO: describe */
    QPointF leftUpPoint1; /**< TODO: describe */
    QPointF leftUpPoint2; /**< TODO: describe */
    QPointF rightUpPoint1; /**< TODO: describe */
    QPointF rightUpPoint2; /**< TODO: describe */
    QPointF leftHPoint2; /**< TODO: describe */
    /**
     * @brief
     *
     * @param point
     * @param center
     * @return QPointF
     */
    QPointF getSymmetryPoint(QPointF point,QPointF center);
    /**
     * @brief
     *
     * @param nA1
     * @param x1
     * @param y1
     * @param nA2
     * @param x2
     * @param y2
     * @param scale
     * @return QPointF
     */
    QPointF getIntersection(qreal nA1,qreal x1,qreal y1,qreal nA2,qreal x2,qreal y2,qreal scale);
    /**
     * @brief
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param k
     * @param b
     * @return QPointF
     */
    QPointF getIntersection(qreal x1,qreal y1,qreal x2,qreal y2,qreal k,qreal b);
    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @param k
     * @param b
     * @return QPointF
     */
    QPointF getIntersection(QPointF p1,QPointF p2,qreal k,qreal b);
    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @param p3
     * @param p4
     * @return QPointF
     */
    QPointF getIntersection(QPointF p1,QPointF p2,QPointF p3,QPointF p4);
    /**
     * @brief
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param l
     * @return QPointF
     */
    QPointF getPointOnRadial(qreal x1,qreal y1,qreal x2,qreal y2,qreal l);
    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @param l
     * @return QPointF
     */
    QPointF getPointOnRadial(QPointF p1,QPointF p2,qreal l);
    /**
     * @brief
     *
     * @param x1
     * @param y1
     * @param k
     * @param l
     * @return QPointF
     */
    QPointF getPointOnRadial_Down(qreal x1,qreal y1,qreal k,qreal l);
    /**
     * @brief
     *
     * @param p
     * @param k
     * @param l
     * @return QPointF
     */
    QPointF getPointOnRadial_Down(QPointF p,qreal k,qreal l);
    /**
     * @brief
     *
     * @param x1
     * @param y1
     * @param k
     * @param l
     * @return QPointF
     */
    QPointF getPointOnRadial_Up(qreal x1,qreal y1,qreal k,qreal l);
    /**
     * @brief
     *
     * @param p
     * @param k
     * @param l
     * @return QPointF
     */
    QPointF getPointOnRadial_Up(QPointF p,qreal k,qreal l);
    /**
     * @brief
     *
     * @param vertex
     * @param leftUp
     * @param rightUp
     * @return QPointF
     */
    QPointF getPointOfSang(QPointF vertex,QPointF leftUp,QPointF rightUp);
    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @param distance
     * @return QPointF
     */
    QPointF getPointOfSang_D(QPointF p1,QPointF p2,qreal distance);
    /**
     * @brief
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param distance
     * @return QPointF
     */
    QPointF getPointOfSang_D(qreal x1,qreal y1,qreal x2,qreal y2,qreal distance);
    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @param proportion
     * @return QPointF
     */
    QPointF getPointOfSang_P(QPointF p1,QPointF p2,qreal proportion);
    /**
     * @brief
     *
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @param proportion
     * @return QPointF
     */
    QPointF getPointOfSang_P(qreal x1,qreal y1,qreal x2,qreal y2,qreal proportion);
    /**
     * @brief
     *
     * @param p1
     * @param ps
     * @param ls
     * @return QPointF
     */
    QPointF getVertexOfSang_Up(QPointF p1,QPointF ps,qreal ls);
    /**
     * @brief
     *
     * @param p1
     * @param ps
     * @param ls
     * @return QPointF
     */
    QPointF getVertexOfSang_Down(QPointF p1,QPointF ps,qreal ls);
    /**
     * @brief
     *
     * @param p
     * @param l
     * @param k
     * @param b
     * @return QPointF
     */
    QPointF getIntersection_L(QPointF p,qreal l,qreal k,qreal b);
    /**
     * @brief
     *
     * @param p
     * @param l
     * @param k
     * @param b
     * @return QPointF
     */
    QPointF getIntersection_R(QPointF p,qreal l,qreal k,qreal b);

    /**
     * @brief
     *
     * @param p1
     * @param p2
     * @return qreal
     */
    qreal distanceBetween(QPointF p1,QPointF p2);
//    qreal distanceBetween(QPointF p1,QPointF p2,QPointF p3,QPointF p4);

    /**
     * @brief
     *
     * @param startPoint
     * @param endPoint
     * @return QPainterPath
     */
    QPainterPath lineThrough2P(QPointF startPoint,QPointF endPoint);
    /**
     * @brief
     *
     * @param points
     */
    void brokenLineThrough(QList<QPointF> points);
    /**
     * @brief
     *
     * @param A
     * @param B
     * @param C
     * @param ctrl1
     * @param ctrl4
     * @return QPainterPath
     */
    QPainterPath curveThrough3P(QPointF A,QPointF B,QPointF C,QPointF ctrl1,QPointF ctrl4);
    /**
     * @brief
     *
     * @param points
     */
    void curveThrough(QList<QPointF> points);
    /**
     * @brief
     *
     * @param points
     * @param firstCtrlPoint
     * @param lastCtrlPoint
     */
    void curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint);

    /**
     * @brief
     *
     * @param point
     * @param name
     */
    void addPoint(QPointF point,QString name="");
    /**
     * @brief
     *
     * @param A
     * @param B
     * @param C
     * @param ctrlPoints
     */
    void addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints);
    /**
     * @brief
     *
     * @param path
     * @param p
     * @return bool
     */
    bool currentPositionequal(QPainterPath path,QPointF p);

private slots:

};

#endif // MYPATH_H

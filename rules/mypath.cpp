#include "mypath.h"
#include "painterarea.h"
#include "mainwindow.h"
#include "data/mypathdata.h"
#include <QPainter>
#include <QDebug>

MyPath::~MyPath()
{
    delete myPath;
    delete startPoint;
}

/**
 * @brief 构造和初始化
 *
 * @param parent
 */
MyPath::MyPath(PainterArea *parent)
{
    this->parent = parent;
    myPath = new QPainterPath;
    startPoint = new QPointF(0,0);
    initializeSize();
}

/**
 * @brief 初始化尺寸
 *
 */
void MyPath::initializeSize()
{
    pantsHeight = parent->pantsHeight;
    pantsL = parent->pantsL;
    pantsW = parent->pantsW;
    pantsH = parent->pantsH;
    pantsCrotchH = parent->pantsCrotchH;

    w_h_height = qRound(pantsCrotchH*2.0/3);
    knee_height = (pantsL+ w_h_height)/2 - 40;

    hWidth1 = pantsH/4-10;
    hWidth2 = hWidth1+20;
    smallCro = pantsH/20 - 5;
    bigCro = pantsH/10 + 5;
    halfCroWidth1 = (hWidth1+smallCro)/2;
    xMidVerticalLine2 = (smallCro+hWidth2)/2;
    feetWidth = pantsH/5-20;
    sang1 = (hWidth1-rightWaist1)/2.0-pantsW/8.0;
    sang2 = (hWidth2-pantsW/4.0)/3.0*2;
}

/**
 * @brief 设置位置参照点
 *
 * @param x
 * @param y
 */
void MyPath::setStartPoint(qreal x,qreal y)
{
    startPoint->setX(x);
    startPoint->setY(y);
}

/**
 * @brief 设置位置参照点
 *
 * @param point
 */
void MyPath::setStartPoint(QPointF point)
{
    startPoint->setX(point.x());
    startPoint->setY(point.y());
}

/**
 * @brief 辅助线 H型 前片
 *
 * @return QPainterPath
 */
QPainterPath MyPath::auxiliaryLinesH_1()
{
    qreal sx = startPoint->x(),
            sy = startPoint->y();
    smallCroPoint = QPointF(sx-smallCro,sy+pantsCrotchH);
    leftUpPoint1 = QPointF(sx+rightWaist1,sy+downWaist1);
    rightUpPoint1 = QPointF(sx+hWidth1-sang1,sy);

    qreal x = sx+hWidth1, y = sy;
    QPainterPath path;
    path.moveTo(sx,sy);
    path.lineTo(x,y);

    y = sy+w_h_height;
    QPointF point(sx,y);
    path.moveTo(point);
    point.setX(x);
    path.lineTo(point);

    path.moveTo(smallCroPoint);
    point = QPointF(x,smallCroPoint.y());
    path.lineTo(point);

    y = sy+knee_height;
    point = QPointF(sx,y);
    path.moveTo(sx,y);
    point.setX(x);
    path.lineTo(x,y);

    y = sy+pantsL;
    point = QPointF(sx,y);
    path.moveTo(point);
    point.setX(x);
    path.lineTo(point);

    path.moveTo(sx,sy);
    path.lineTo(sx,y);
    x = sx-smallCro+halfCroWidth1;
    path.moveTo(x,sy);
    path.lineTo(x,y);
    x = sx+hWidth1;
    path.moveTo(x,sy);
    path.lineTo(x,y);

    return path;
}

/**
 * @brief 辅助线 H型 后片
 *
 * @return QPainterPath
 */
QPainterPath MyPath::auxiliaryLinesH_2()
{
    qreal sx = startPoint->x(),
            sy = startPoint->y();
    bigCroPoint = QPointF(sx+hWidth2+bigCro,sy+pantsCrotchH+downCro2);
    addPoint(bigCroPoint,"theCro2");

    QPainterPath myPath;
    qreal x = sx+hWidth2;
    myPath.moveTo(sx,sy);
    myPath.lineTo(x,sy);

    qreal y = sy+w_h_height;
    myPath.moveTo(sx,y);
    myPath.lineTo(x,y);

    myPath.moveTo(sx,bigCroPoint.y());
    myPath.lineTo(bigCroPoint);

    y = sy+knee_height;
    myPath.moveTo(sx,y);
    myPath.lineTo(x,y);

    y = sy+pantsL;
    myPath.moveTo(sx,y);
    myPath.lineTo(x,y);

    myPath.moveTo(sx,sy);
    myPath.lineTo(sx,y);
    x = sx+xMidVerticalLine2;
    myPath.moveTo(x,sy);
    myPath.lineTo(x,y);
    x = sx+hWidth2;
    myPath.moveTo(x,sy);
    myPath.lineTo(x,y);

    return myPath;
}



/**
 * @brief 增添点的数据
 *
 * @param point
 * @param name 名称可以不填
 */
void MyPath::addPoint(QPointF point,QString name)
{
    parent->myPathData->addPoint(point,name);
}

/**
 * @brief 画轮廓线 前片
 *
 * @param typeSang
 */
void MyPath::drawOutline1(int typeSang)
{
    addPoint(smallCroPoint,"theCro1");
    addPoint(leftUpPoint1,"leftUp1");
    addPoint(rightUpPoint1,"rightUp1");
    qreal sx = startPoint->x(),
            sy = startPoint->y();
    QList<QPointF> points;

    points.clear();
    points.append(rightUpPoint1);
    QPointF point(sx+hWidth1,sy+w_h_height);
    QPointF point2 = rightUpPoint1;
    point2 = getPointOfSang_P(point2,point,0.5);
    point2 = getVertexOfSang_Up(point,point2,3);
    addPoint(point2,"rightUtH1");
    points.append(point2); //"rightUtH1"
    points.append(point);  //"rightH1"
    addPoint(point,"rightH1");
    point = QPointF(point.x()-leftCro1,sy+pantsCrotchH);
    addPoint(point,"rightCro1");
    points.append(point);
    point2 = QPointF(sx+hWidth1-halfCroWidth1+feetWidth/2.0+deltaKneeWidth,sy+knee_height);
    addPoint(point2,"rightMid1");
    point = getPointOfSang_P(point,point2,2.0/3.0);
    point = getVertexOfSang_Up(point2,point,2);
    addPoint(point,"rightCtM1");
    points<<point<<point2;
    curveThrough(points);

    points.clear();
    points.append(point2); //"rightMid1"
    point = QPointF(point2.x()-deltaKneeWidth,sy+pantsL);
    addPoint(point,"rightDown1");
    points.append(point);
    point.setX(point.x()-feetWidth);
    addPoint(point,"leftDown1");
    points.append(point);
    point2.setX(point.x()-deltaKneeWidth);
    addPoint(point2,"leftMid1");
    points.append(point2);
    brokenLineThrough(points);

    points.clear();
    point = getPointOfSang_P(point2,smallCroPoint,1.0/3.0);
    point = getVertexOfSang_Up(point2,point,4);
    addPoint(point,"leftMtC11");
    points<<point2<<point;
    point = getPointOfSang_P(point2,smallCroPoint,2.0/3.0);
    point = getVertexOfSang_Up(point2,point,5);
    addPoint(point,"leftMtC21");
    points<<point<<smallCroPoint;
    curveThrough(points);

    points.clear();
    point = QPointF(sx,sy+w_h_height); //"leftH1"
    addPoint(point,"leftH1");
    point2 = QPointF(sx-11.5*sqrt(2.0),sy+pantsCrotchH-11.5*sqrt(2.0));
    addPoint(point2,"CtH1");
    points<<smallCroPoint<<point2<<point;
    curveThrough(points,QPointF(smallCroPoint.x()-6,smallCroPoint.y()),getPointOfSang_P(point,leftUpPoint1,0.05));

    points.clear();
    points<<point<<leftUpPoint1;
    brokenLineThrough(points);

    drawWaist1(1,typeSang);
}

/**
 * @brief 画轮廓线 后片
 *
 * @param typeSang
 */
void MyPath::drawOutline2(int typeSang)
{
    qreal sx = startPoint->x(),
            sy = startPoint->y();
    QList<QPointF> points;
    rightUpPoint2 = QPointF(sx+xMidVerticalLine2/2.0+hWidth2/2.0,sy-upWaist2);
    addPoint(rightUpPoint2,"rightUp2");
    QPointF tempPoint = getIntersection_L(rightUpPoint2,hWidth2,0,sy);
    QPointF leftUpLeftPoint = tempPoint;
    leftUpPoint2 = getPointOnRadial(tempPoint,rightUpPoint2,sang2/2.0);
    addPoint(leftUpPoint2,"leftUp2");
    tempPoint = QPointF(sx+hWidth2-leftCro2,sy+pantsCrotchH);
    QPointF point = getIntersection(rightUpPoint2,tempPoint,0,sy+w_h_height);
    addPoint(point,"rightH2");
    leftH2 = qRound(sx)+hWidth2-qRound(point.x());//长度类型，对应结构图上的小圆圈
    points<<rightUpPoint2<<point;
    brokenLineThrough(points);

    points.clear();
    points.append(point);
    bigCroPoint = QPointF(sx+hWidth2+bigCro,sy+pantsCrotchH+downCro2);
    addPoint(bigCroPoint,"theCro2");
    tempPoint = getIntersection(rightUpPoint2,tempPoint,0,bigCroPoint.y());
    qreal tempL = 14.75*sqrt(2.0);
    point = QPointF(tempPoint.x()+tempL,tempPoint.y()-tempL);
    addPoint(point,"HtC2");
    points<<point<<bigCroPoint;
    curveThrough(points);

    points.clear();
    points.append(bigCroPoint);
    point = QPointF(sx+xMidVerticalLine2+(feetWidth+60)/2.0,sy+knee_height);
    addPoint(point,"rightMid2");
    tempPoint = getPointOfSang_P(bigCroPoint,point,4.0/10.0);
    tempPoint = getVertexOfSang_Up(bigCroPoint,tempPoint,12);
    addPoint(tempPoint,"rightCtM12");
    points<<tempPoint;
    tempPoint = getPointOfSang_P(bigCroPoint,point,8.0/10.0);
    tempPoint = getVertexOfSang_Up(bigCroPoint,tempPoint,9.8);
    addPoint(tempPoint,"rightCtM22");
    points<<tempPoint<<point;
    curveThrough(points);

    points.clear();
    points.append(point);
    tempPoint = QPointF(point.x()-deltaKneeWidth,sy+pantsL);
    addPoint(tempPoint,"rightDown2");
    points.append(tempPoint);
    tempPoint.setX(tempPoint.x()-(feetWidth+40));
    addPoint(tempPoint,"leftDown2");
    points.append(tempPoint);
    point.setX(tempPoint.x()-deltaKneeWidth);
    addPoint(point,"leftMid2");
    points.append(point);
    brokenLineThrough(points);

    tempPoint = QPointF(sx-leftH2,sy+w_h_height);
    addPoint(tempPoint,"leftH2");
    leftHPoint2 = tempPoint;
    tempPoint = getIntersection(leftUpLeftPoint,tempPoint,0,sy+pantsCrotchH+downCro2);
    addPoint(tempPoint,"leftCro1");
    QPointF leftCroPoint = tempPoint;
    tempPoint = getPointOfSang_P(point,leftCroPoint,1.0/3.0);
    tempPoint = getVertexOfSang_Up(point,tempPoint,2.5);
    addPoint(tempPoint,"leftCtM2");

    points.clear();
    points<<point<<tempPoint<<leftCroPoint;
    curveThrough(points,point,getPointOfSang_P(leftCroPoint,leftHPoint2,1.0/3.0));

    //从leftUpPoint2和leftHPoint2连线上1/2点往左取3mm。
    qreal x1=leftUpPoint2.x(), x2=leftHPoint2.x(),
            y1=leftUpPoint2.y(), y2=leftHPoint2.y();
    if(x1<x2)
        point = getVertexOfSang_Down(leftUpPoint2,getPointOfSang_P(leftUpPoint2,leftHPoint2,1.0/2.0),3);
    else if(x1>x2)
        point = getVertexOfSang_Up(leftUpPoint2,getPointOfSang_P(leftUpPoint2,leftHPoint2,1.0/2.0),3);
    else
        point = QPointF(x1-3,(y1+y2)/2.0);
    addPoint(point,"leftUtH2");

    points.clear();
    points<<leftCroPoint<<leftHPoint2<<point<<leftUpPoint2;
    curveThrough(points);
    //curveThrough(points,getPointOfSang_P(leftHPoint2,leftCroPoint,1.0/4.0),leftUpPoint2);

    drawWaist2(1,typeSang);
}

/**
 * @brief 画前片的腰部
 *
 * @param wCase 腰位
 * @param sangCase 省褶类型
 * @return QPainterPath
 */
void MyPath::drawWaist1(int wCase,int sangCase)
{
    drawWaistLine1(sangCase);
    //    path1 = this->waist_1(wCase,sangCase,points);
    //    path.addPath(path1);

    //    qreal l=distanceBetween(leftUpPoint1,rightUpPoint1), scale = 1.0*waistBandWidth/l;
    //    qreal dy=scale*sqrt(l*l-downWaist1*downWaist1);
    //    qreal dx=scale*downWaist1;
    //    path2 = path1; path2.translate(dx,dy);
    //    QPainterPath waistBottom = cutPath(path2,path);
    //    path.addPath(waistBottom);

    //    //画门襟
    //    QPainterPath::Element e=waistBottom.elementAt(0);
    //    QPointF p1(e.x,e.y);
    //    e=waistBottom.elementAt(waistBottom.elementCount()-1);
    //    QPointF p2(e.x,e.y);
    //    QPointF cubicStartPoint(startPoint->x(),startPoint->y()+w_h_height);
    //    QPointF leftWPoint1=getIntersection(p1,p2,leftUpPoint1,cubicStartPoint);
    //    addPoint(leftWPoint1,"leftW1");
    //    QPointF point=getPointOnRadial(p1,p2,30);
    //    QPainterPath door;
    //    door.moveTo(leftWPoint1);
    //    door.lineTo(point);
    //    point=getPointOnRadial(leftWPoint1,cubicStartPoint,88);
    //    point.setX(point.x()+29.66);
    //    door.lineTo(point);
    //    QList<QPointF> doorPoints;
    //    doorPoints<<point<<QPointF(cubicStartPoint.x()+21.96,cubicStartPoint.y()-9.77)<<cubicStartPoint;
    //    MyPath myTempPath(parent);
    //    myTempPath.curveThrough_data(doorPoints,point,cubicStartPoint);
    //    door.addPath(*(myTempPath.myPath));
    //    path.addPath(door);

    //    //画斜插袋
    //    QPointF rightWPoint1=QPointF(rightUpPoint1.x()+4.9,rightUpPoint1.y()+30);
    //    point=getPointOnRadial(rightWPoint1,p2,30);
    //    path.moveTo(rightWPoint1);
    //    path.lineTo(point);
    //    path.lineTo(QPointF(startPoint->x()+hWidth1-0.1,startPoint->y()+w_h_height+27.34));
}

/**
 * @brief 画后片的腰部
 *
 * @param pCase 口袋类型
 * @param sangCase 省褶类型
 * @return QPainterPath
 */
void MyPath::drawWaist2(int pCase,int sangCase)
{
    QPointF midHPoint,midPocketPoint;
    qreal angle,dx,dy,scale;
    QList<QPointF> points;
    QPainterPath path1,path2;
    QPainterPath::Element e;

    drawWaistLine2(sangCase);

    // 画单嵌线口袋
    if(pCase==1)
    {
        midHPoint = QPointF(leftHPoint2.x()+0.51*rightUpPoint2.x()-0.49*leftUpPoint2.x(),leftHPoint2.y()+(rightUpPoint2.y()-leftUpPoint2.y())/2.0);
        //midHPoint = QPointF(leftHPoint2.x()+(rightUpPoint2.x()-leftUpPoint2.x())/2.0,leftHPoint2.y()+(rightUpPoint2.y()-leftUpPoint2.y())/2.0);
        midPocketPoint = getVertexOfSang_Up(leftHPoint2,midHPoint,55);
        angle = qAsin(1.0*upWaist2/hWidth2);
        dx = qCos(angle)*60; dy = qSin(angle)*60;
        path1.moveTo(QPointF(midPocketPoint.x()-dx,midPocketPoint.y()+dy));
        path1.lineTo(QPointF(midPocketPoint.x()+dx,midPocketPoint.y()-dy));
        scale = 7.5/hWidth2;    dy = scale*sqrt(hWidth2*hWidth2+upWaist2*upWaist2);    dx = scale*upWaist2;
        path2 = path1;    path2.translate(dx,dy);   path1.translate(-dx,-dy);
        e=path1.elementAt(0);   points<<QPointF(e.x,e.y);
        e=path1.elementAt(1);   points<<QPointF(e.x,e.y);
        e=path2.elementAt(1);   points<<QPointF(e.x,e.y);
        e=path2.elementAt(0);   points<<QPointF(e.x,e.y);
        e=path1.elementAt(0);   points<<QPointF(e.x,e.y);
        brokenLineThrough(points);   points.clear();
    }

    // 画腰头
}

/**
 * @brief 画省、腰线 前片
 *
 * @param intCase
 */
void MyPath::drawWaistLine1(int intCase){
    QPointF sangA,sangB,tempPoint;
    qreal l1=w_h_height/2.0, l0=l1+32, l2=l1-15;
    QList<QPointF> waistPoints, curvePoints;
    waistPoints<<leftUpPoint1;

    switch (intCase) {
    case 0: //单省
        tempPoint = QPointF(rightUpPoint1.x()+sang1-halfCroWidth1,rightUpPoint1.y()+l0);
        sangA = getPointOfSang(tempPoint,leftUpPoint1,rightUpPoint1);
        waistPoints<<drawSang1(sangA,tempPoint,sang1);
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.first();
        curveThrough(curvePoints);
        curvePoints.clear();
        break;
    case 1: //双省
        tempPoint = QPointF(rightUpPoint1.x()+sang1-halfCroWidth1,rightUpPoint1.y()+l1);
        sangA = getPointOfSang(tempPoint,leftUpPoint1,rightUpPoint1);
        sangB = getPointOfSang_P(sangA,rightUpPoint1,1.0/2);
        waistPoints<<drawSang1(sangA,tempPoint,sang1/3.0*2);
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.first();
        curveThrough(curvePoints);
        curvePoints.clear();
        waistPoints<<drawSang1(sangB,getVertexOfSang_Down(leftUpPoint1,sangB,l2),sang1/3.0);
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.first();
        curveThrough(curvePoints);
        curvePoints.clear();
        break;
    case 2: //褶
        break;
    }
    waistPoints<<rightUpPoint1;
    curveThrough(waistPoints);
}

/**
 * @brief 画省、腰线 后片
 *
 * @param intCase
 */
void MyPath::drawWaistLine2(int intCase){
    QPointF sangA,sangB;
    qreal l0=w_h_height/2.0+35, l1=l0-10, l2=l0;
    QList<QPointF> waistPoints, curvePoints;
    waistPoints<<leftUpPoint2;

    if(true){ //有单嵌线口袋存在
        l0 = 110; l1 = 110; l2 = 110; // 待计算
    }

    switch (intCase) {
    case 0: //单省
        sangA = getPointOfSang_P(rightUpPoint2,leftUpPoint2,1/2.0);
        waistPoints<<drawSang2(sangA,getVertexOfSang_Down(leftUpPoint2,sangA,l0),sang2);
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.first();
        curveThrough(curvePoints);
        curvePoints.clear();
        break;
    case 1: //双省
        sangA = getPointOfSang_P(rightUpPoint2,leftUpPoint2,2/3.0);
        sangB = getPointOfSang_P(rightUpPoint2,leftUpPoint2,1/3.0);
        waistPoints<<drawSang2(sangA,getVertexOfSang_Down(leftUpPoint2,sangA,l1),sang2/2.0);
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.first();
        curveThrough(curvePoints);
        curvePoints.clear();
        waistPoints<<drawSang2(sangB,getVertexOfSang_Down(leftUpPoint2,sangB,l2),sang2/2.0);
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.takeFirst();
        curvePoints<<waistPoints.first();
        curveThrough(curvePoints);
        curvePoints.clear();
        break;
    }
    waistPoints<<rightUpPoint2;
    curveThrough(waistPoints);
}

/**
 * @brief 画省、腰线 前片
 *
 * @param point 省的位置
 * @param vertex 省尖点
 * @param sang 省量
 * @return QList<QPointF>
 */
QList<QPointF> MyPath::drawSang1(QPointF point,QPointF vertex,qreal sang)
{
    QPointF p1=getPointOnRadial(point,leftUpPoint1,sang),
            p4=getPointOnRadial(point,rightUpPoint1,sang),
            pMid=getPointOnRadial(vertex,point,distanceBetween(p1,vertex)),
            p2=getPointOfSang(vertex,pMid,p1),
            p3=getPointOfSang(vertex,pMid,p4);

    QList<QPointF> pList;
    pList<<p2<<vertex<<p3;
    brokenLineThrough(pList);

    // 返回画腰线需要用到的点（从左往右）
    pList.clear();
    pList<<p2<<pMid<<p3;
    return pList;
}
/**
 * @brief 画省、腰线 后片
 *
 * @param point 省的位置
 * @param vertex 省尖点
 * @param sang 省量
 * @return QList<QPointF>
 */
QList<QPointF> MyPath::drawSang2(QPointF point,QPointF vertex,qreal sang)
{
    QPointF p1=getPointOnRadial(point,leftUpPoint2,sang), //省位向腰线左端点方向 移一个sang 的位置
            p4=getPointOnRadial(point,rightUpPoint2,sang),
            pMid=getPointOnRadial(vertex,point,distanceBetween(p1,vertex)), //省尖点向省位方向 移一个“p1到省尖距离” 的位置
            p2=getPointOfSang(vertex,pMid,p1), //省尖点到“pMid、p1连线”的垂点(根据“等腰三角形三线合一”，这也是“pMid、p1连线”的中点）
            p3=getPointOfSang(vertex,pMid,p4);

    QList<QPointF> pList;
    pList<<p2<<vertex<<p3;
    brokenLineThrough(pList);

    // 返回画腰线需要用到的点（从左往右）
    pList.clear();
    pList<<p2<<pMid<<p3;
    return pList;
}





/**
 * @brief 画过3点的曲线（贝塞尔插值法
 *
 * @param A
 * @param B
 * @param C
 * @param ctrl1
 * @param ctrl4
 * @return QPainterPath
 */
QPainterPath MyPath::curveThrough3P(QPointF A,QPointF B,QPointF C,QPointF ctrl1,QPointF ctrl4)
{
    QPointF E((A.x()+B.x())/2.0,(A.y()+B.y())/2.0);
    QPointF F((B.x()+C.x())/2.0,(B.y()+C.y())/2.0);
    qreal d1 = distanceBetween(A,B),    d2 = distanceBetween(B,C),    scale = d1/(d1+d2);
    QPointF D = getPointOfSang_P(E,F,scale);
    qreal dx = B.x()-D.x(),    dy = B.y()-D.y();
    QPointF ctrl2(E.x()+dx,E.y()+dy);
    QPointF ctrl3(F.x()+dx,F.y()+dy);
    QPainterPath path;
    path.moveTo(A);    path.cubicTo(ctrl1,ctrl2,B);    path.cubicTo(ctrl3,ctrl4,C);
    return path;
}

/**
 * @brief 向3点曲线中加入中间2个控制点
 *
 * @param A
 * @param B
 * @param C
 * @param ctrlPoints
 */
void MyPath::addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints)
{
    QPointF E((A.x()+B.x())/2.0,(A.y()+B.y())/2.0);
    QPointF F((B.x()+C.x())/2.0,(B.y()+C.y())/2.0);
    qreal d1 = distanceBetween(A,B),    d2 = distanceBetween(B,C),    scale = d1/(d1+d2);
    QPointF D = getPointOfSang_P(E,F,scale);
    qreal dx = B.x()-D.x(),    dy = B.y()-D.y();
    QPointF ctrl2(E.x()+dx,E.y()+dy);
    QPointF ctrl3(F.x()+dx,F.y()+dy);
    ctrlPoints->insert(1,ctrl2);
    ctrlPoints->insert(2,ctrl3);
}

/**
 * @brief 根据经过的一系列点，画曲线
 *
 * @param points
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 */
void MyPath::curveThrough_data(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    int num=points.size(), i=3;
    QList<QPointF> *ctrlPoints = new QList<QPointF>;
    ctrlPoints->append(getSymmetryPoint(firstCtrlPoint,points.at(0)));
    ctrlPoints->append(getSymmetryPoint(lastCtrlPoint,points.at(num-1)));
    if(num>0){
        if(!currentPositionequal(*myPath,points.at(0)))
            myPath->moveTo(points.at(0));
        while(i<=num){
            addCtrlPoints(points.at(0),points.at(1),points.at(2),ctrlPoints);
            myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1)); //绘制到第2个端点的三次贝塞尔曲线
            points.takeAt(0); //删除第1个端点
            i++;
        }
        myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1));
    }
    delete ctrlPoints;
}



/**
 * @brief 画曲线并添加数据（无控制点
 *
 * @param points 曲线经过点的列表
 */
void MyPath::curveThrough(QList<QPointF> points)
{
    if(!points.isEmpty())
        curveThrough(points,points.first(),points.last());
    else
        return;
}


/**
 * @brief 用myPath画折线，并添加数据到myPathData数组
 *
 * @param brokenLine 折线(务必确认不含曲线）
 */
void MyPath::brokenLineThrough(QPainterPath brokenLine)
{
    QList<QPointF> points;
    QPainterPath::Element e;
    int len = brokenLine.elementCount();
    if(len<2) return;
    for(int i=0;i<len;i++){
        e = brokenLine.elementAt(i);
        if(e.type>2) return;
        points<<QPointF(e.x,e.y);
    }
    brokenLineThrough(points);
}

/**
 * @brief 两点间距
 *
 * @param p1
 * @param p2
 * @return qreal
 */
qreal MyPath::distanceBetween(QPointF p1,QPointF p2)
{
    qreal x1=p1.x(), y1=p1.y(), x2=p2.x(),y2=p2.y();
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

/**
 * @brief 点p到直线间距
 * @param p
 * @param p1 直线上一点
 * @param p2 直线上另一点
 * @return
 */
qreal MyPath::distanceBetween(QPointF p, QPointF p1, QPointF p2)
{
    QPointF pl, pr, pChui;
    if(p1.x()<p2.x()){
        pl=p1; pr=p2;
    }else if(p1.x()>p2.x()){
        pl=p2; pr=p1;
    }else return -1;
    pChui = getPointOfSang(p,pl,pr);
    return distanceBetween(p,pChui);
}

/**
 * @brief 位于p1(x1,y1)出发、经过p2(x2,y2)的射线上，并距离(x1,y1)距离为l的点
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param l
 * @return QPointF
 */
QPointF MyPath::getPointOnRadial(qreal x1,qreal y1,qreal x2,qreal y2,qreal l)
{
    qreal k = (y1-y2)/(x1-x2), b = y1-k*x1, x;
    qreal aa = k*k+1, bb = -2*x1*aa, cc = aa*x1*x1-l*l;
    if(x1<x2)
        x = (-bb+sqrt(bb*bb-4*aa*cc))/(2.0*aa);
    else
        x = (-bb-sqrt(bb*bb-4*aa*cc))/(2.0*aa);
    return QPointF(x,k*x+b);
}

/**
 * @brief 从p1出发，经过p2的射线上，距离p1位置为l的点
 *
 * @param p1
 * @param p2
 * @param l
 * @return QPointF
 */
QPointF MyPath::getPointOnRadial(QPointF p1,QPointF p2,qreal l)
{
    return getPointOnRadial(p1.x(),p1.y(),p2.x(),p2.y(),l);
}

/**
 * @brief 位于(x1,y1)出发、斜率为k的向下的射线上，并到(x1,y1)距离为l的点
 *
 * @param x1
 * @param y1
 * @param k
 * @param l
 * @return QPointF
 */
QPointF MyPath::getPointOnRadial_Down(qreal x1,qreal y1,qreal k,qreal l)
{
    qreal b = y1-k*x1, x;
    qreal aa = k*k+1, bb = -2*x1*aa, cc = aa*x1*x1-l*l;
    if(k>0)
        x = (-bb+sqrt(bb*bb-4*aa*cc))/(2.0*aa);
    else
        x = (-bb-sqrt(bb*bb-4*aa*cc))/(2.0*aa);
    return QPointF(x,k*x+b);
}
/**
 * @brief 位于点p出发、斜率为k的向下的射线上，并到p距离为l的点
 *
 * @param p
 * @param k
 * @param l
 * @return QPointF
 */
QPointF MyPath::getPointOnRadial_Down(QPointF p,qreal k,qreal l)
{
    return getPointOnRadial_Down(p.x(),p.y(),k,l);
}
/**
 * @brief 位于(x1,y1)出发、斜率为k的向上的射线上，并到(x1,y1)距离为l的点
 *
 * @param x1
 * @param y1
 * @param k
 * @param l
 * @return QPointF
 */
QPointF MyPath::getPointOnRadial_Up(qreal x1,qreal y1,qreal k,qreal l)
{
    qreal b = y1-k*x1, x;
    qreal aa = k*k+1, bb = -2*x1*aa, cc = aa*x1*x1-l*l;
    if(k<0)
        x = (-bb+sqrt(bb*bb-4*aa*cc))/(2.0*aa);
    else
        x = (-bb-sqrt(bb*bb-4*aa*cc))/(2.0*aa);
    return QPointF(x,k*x+b);
}
/**
 * @brief 位于点p出发、斜率为k的向上的射线上，并到p距离为l的点
 *
 * @param p
 * @param k
 * @param l
 * @return QPointF
 */
QPointF MyPath::getPointOnRadial_Up(QPointF p,qreal k,qreal l)
{
    return getPointOnRadial_Up(p.x(),p.y(),k,l);
}

/**
 * @brief “点(x1,y1)出发、经过(x2,y2)的射线”与“直线y=kx+b”的交点
 *
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param k
 * @param b
 * @return QPointF
 */
QPointF MyPath::getIntersection(qreal x1,qreal y1,qreal x2,qreal y2,qreal k,qreal b)
{
    qreal k1 = (y1-y2)/(x1-x2), b1 = y1-k1*x1;
    qreal x = (b-b1)/(k1-k);
    return QPointF(x,k*x+b);
}
/**
 * @brief “点p1出发、经过p2的射线”与“直线y=kx+b”的交点
 *
 * @param p1
 * @param p2
 * @param k
 * @param b
 * @return QPointF
 */
QPointF MyPath::getIntersection(QPointF p1,QPointF p2,qreal k,qreal b)
{
    qreal x1 = p1.x(), x2 = p2.x(), y1 = p1.y(), y2 = p2.y();
    qreal k1 = (y1-y2)/(x1-x2), b1 = y1-k1*x1;
    qreal x = (b-b1)/(k1-k);
    return QPointF(x,k*x+b);
}
/**
 * @brief “经过p1,p2的直线”与“经过p3,p4的直线”的交点
 *
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 * @return QPointF
 */
QPointF MyPath::getIntersection(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    qreal x1=p1.x(), x2=p2.x(), y1=p1.y(), y2=p2.y();
    if(equal(x1,x2))
    {
        qreal x3=p3.x(), x4=p4.x(), y3=p3.y(), y4=p4.y();
        qreal k=(y3-y4)/(x3-x4), b=y3-k*x3;
        return QPointF(x1,x1*k+b);
    }
    else
    {
        qreal k=(y1-y2)/(x1-x2), b=y1-k*x1;
        return getIntersection(p3,p4,k,b);
    }
}

/**
 * @brief 省的位置
 *
 * @param p1 腰线端点1
 * @param p2 腰线端点2
 * @param distance 省位到端点1的距离
 * @return QPointF
 */
QPointF MyPath::getPointOfSang_D(QPointF p1,QPointF p2,qreal distance)
{
    qreal dx = p1.x()-p2.x(), dy = p1.y()-p2.y();
    qreal l = sqrt(dx*dx+dy*dy), proportion = 1.0*distance/l;
    return getPointOfSang_P(p1,p2,proportion);
}
/**
 * @brief 省的位置
 *
 * @param x1 腰线端点1横坐标
 * @param y1 腰线端点1纵坐标
 * @param x2 腰线端点2横坐标
 * @param y2 腰线端点2纵坐标
 * @param distance 省位到端点1的距离
 * @return QPointF
 */
QPointF MyPath::getPointOfSang_D(qreal x1,qreal y1,qreal x2,qreal y2,qreal distance)
{
    qreal dx = x1-x2, dy = y1-y2;
    qreal l = sqrt(dx*dx+dy*dy), proportion = 1.0*distance/l;
    return getPointOfSang_P(x1,y1,x2,y2,proportion);
}

/**
 * @brief 省的位置
 *
 * @param p1 腰线端点1
 * @param p2 腰线端点2
 * @param proportion “省位到端点1的距离”占“腰线长度”的比例
 * @return QPointF
 */
QPointF MyPath::getPointOfSang_P(QPointF p1,QPointF p2,qreal proportion)
{
    qreal x1=p1.x(), y1=p1.y(), dx=x1-p2.x(), dy=y1-p2.y();
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}
/**
 * @brief 省的位置
 *
 * @param x1 腰线端点1横坐标
 * @param y1 腰线端点1纵坐标
 * @param x2 腰线端点2横坐标
 * @param y2 腰线端点2纵坐标
 * @param proportion “省位到端点1的距离”占“腰线长度”的比例
 * @return QPointF
 */
QPointF MyPath::getPointOfSang_P(qreal x1,qreal y1,qreal x2,qreal y2,qreal proportion)
{
    qreal dx=x1-x2, dy=y1-y2;
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}

/**
 * @brief 省尖点（沿垂直于p1、ps连线方向，从ps向下取ls长的点
 *
 * @param p1 腰线上一点
 * @param ps 省位
 * @param ls 省长
 * @return QPointF
 */
QPointF MyPath::getVertexOfSang_Down(QPointF p1,QPointF ps,qreal ls)
{
    qreal k2=(p1.x()-ps.x())/(ps.y()-p1.y());
    return getPointOnRadial_Down(ps,k2,ls);
}
/**
 * @brief 沿垂直于p1、ps连线方向，从ps向上取ls长的点
 *
 * @param p1 直线上一点
 * @param ps 直线的垂点
 * @param ls 到垂点的距离
 * @return QPointF
 */
QPointF MyPath::getVertexOfSang_Up(QPointF p1,QPointF ps,qreal ls)
{
    qreal k2=(p1.x()-ps.x())/(ps.y()-p1.y());
    return getPointOnRadial_Up(ps,k2,ls);
}

/**
 * @brief 根据省尖点位置，求与腰线的垂点
 *
 * @param vertex 省尖点
 * @param leftUp 腰线左端点
 * @param rightUp 腰线右端点
 * @return QPointF
 */
QPointF MyPath::getPointOfSang(QPointF vertex,QPointF leftUp,QPointF rightUp)
{
    qreal x1=leftUp.x(), y1=leftUp.y(), x2=rightUp.x(),y2=rightUp.y();
    qreal k=(y1-y2)/(x1-x2), b=y1-k*x1;
    if(k==0.0)
        return QPointF(vertex.x(),b);
    else
    {
        qreal kk=-1.0/k, bb=vertex.y()-kk*vertex.x();
        qreal x=(bb-b)/(k-kk), y=kk*x+bb;
        return QPointF(x,y);
    }
}

/**
 * @brief 直线上，到点p固定距离，在点p左侧的点
 *
 * @param p
 * @param l 到点p的距离
 * @param k 直线斜率
 * @param b 直线截距
 * @return QPointF
 */
QPointF MyPath::getIntersection_L(QPointF p,qreal l,qreal k,qreal b)
{
    qreal x=p.x(), y=p.y();
    qreal aa=k*k+1, bb=2*(k*b-k*y-x), cc=b*b-2*y*b+x*x+y*y-l*l;
    qreal xx=-(sqrt(bb*bb-4*aa*cc)+bb)/(2.0*aa), yy=k*xx+b;
    return QPointF(xx,yy);
}
/**
 * @brief 直线上，到点p固定距离，在点p右侧的点
 *
 * @param p
 * @param l 到点p的距离
 * @param k 直线斜率
 * @param b 直线截距
 * @return QPointF
 */
QPointF MyPath::getIntersection_R(QPointF p,qreal l,qreal k,qreal b)
{
    qreal x=p.x(), y=p.y();
    qreal aa=k*k+1, bb=2*(k*b-k*y-x), cc=b*b-2*y*b+x*x+y*y-l*l;
    qreal xx=sqrt(bb*bb-4*aa*cc)-bb/(2.0*aa), yy=k*xx+b;
    return QPointF(xx,yy);
}

/**
 * @brief 点point关于点center的对称点
 *
 * @param point
 * @param center
 * @return QPointF
 */
QPointF MyPath::getSymmetryPoint(QPointF point,QPointF center)
{
    qreal px=point.x(), py=point.y(),
            cx=center.x(), cy=center.y(),
            x=2*cx-px, y=2*cy-py;
    return QPointF(x,y);
}

/**
 * @brief path的currentPosition是否和p的位置相符
 *
 * @param path
 * @param p
 * @return bool
 */
bool MyPath::currentPositionequal(QPainterPath path,QPointF p)
{
    QPointF pp = path.currentPosition();
    return equal(pp,p);
}

/**
 * @brief 2个实数是否相等
 * @param v1 实数1
 * @param v2 实数2
 * @return bool
 */
bool MyPath::equal(qreal v1, qreal v2)
{
    qreal d = v1-v2;
    if(d>-minUnit && d<minUnit)
        return true;
    else
        return false;
}

/**
 * @brief 2个点是否相等
 * @param p1
 * @param p2
 * @return bool
 */
bool MyPath::equal(QPointF p1,QPointF p2)
{
    if(equal(p1.x(),p2.x()) && equal(p1.y(),p2.y()))
        return true;
    else
        return false;
}

/**
 * @brief 点p是否位于线段上
 * @param p
 * @param sp1 线段端点
 * @param sp2 线段另一端点
 * @return bool
 */
bool MyPath::pointIsOnSegment(QPointF p, QPointF sp1, QPointF sp2)
{
    qreal x=p.x(), y=p.y(), x1=sp1.x(), x2=sp2.x(), y1=sp1.y(), y2=sp2.y();
    if(((x>x1 && x>x2)||(x<x1 && x<x2))||((y>y1 && y>y2)||(y<y1 && y<y2)))
        return false;
    else{
        if(equal(x1,x2) && equal(x1,x))
            return true;
        else{
            qreal k1 = (y2-y)/(x2-x),  k2 = (y-y1)/(x-x1);
            if(equal(k1,k2))
                return true;
            else return false;
        }
    }
}

#include "mypath.h"
#include "painterarea.h"
#include "mainwindow.h"
#include "data/mypathdata.h"
#include <QPainter>
#include <QDebug>

/**
 * @brief
 *
 */
MyPath::~MyPath()
{
    delete myPath;
    delete startPoint;
}

/**
 * @brief
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
 * @brief 根据myPathData画轮廓线
 *
 * @return QPainterPath
 */
QPainterPath MyPath::outLines_data()
{
    MyPath path(parent);
    MyPathData *data = parent->myPathData;
    int numPaths = data->numberPath, i;
    for(i=0;i<numPaths;++i)
    {
        PathData pathData = data->pathData[i];
        QPointF startPoint = data->pointData[pathData.startPoint->id];
        if(!currentPositionequal(*(path.myPath),startPoint))
            path.myPath->moveTo(startPoint);
        if(pathData.isLine)
        {
            QPointF endPoint = data->pointData[pathData.endPoint->id];
            path.myPath->lineTo(endPoint);
        }
        else //如果pathData表示曲线
        {
            QList<QPointF> points;
            CurvePoint *p = pathData.startPoint->pre;
            QPointF firstCtrlPoint = data->pointData[p->id];
            while(p->next->isCtrlPoint!=true)
            {
                p=p->next; points<<data->pointData[p->id];
            }
            p=p->next;
            QPointF lastCtrlPoint = data->pointData[p->id];
            path.curveThrough_data(points,firstCtrlPoint,lastCtrlPoint);
        }
    }
    return *(path.myPath);
}

/**
 * @brief
 *
 * @param point
 * @param name
 */
void MyPath::addPoint(QPointF point,QString name)
{
    parent->myPathData->addPoint(point,name);
}

/**
 * @brief
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
//    qreal nAngle1=qAtan(1.0*smallCro/(pantsCrotchH-w_h_height)); //单位为弧度
//    qreal nAngle2=M_PI-nAngle1;
//    nAngle1=M_PI_2-nAngle1;
//    point2 = getIntersection(nAngle1,sx,point.y(),
//                             nAngle2,sx,smallCroPoint.y(),
//                             1.0/3);
//    myPath->addEllipse(point2,1,1);
//    以上方法画的辅助点不适合用贝塞尔插值来画曲线
    point2 = QPointF(sx-11.5*sqrt(2.0),sy+pantsCrotchH-11.5*sqrt(2.0));
    addPoint(point2,"CtH1");
    points<<smallCroPoint<<point2<<point;
    curveThrough(points,QPointF(smallCroPoint.x()-6,smallCroPoint.y()),getPointOfSang_P(point,leftUpPoint1,0.05));

    points.clear();
    points<<point<<leftUpPoint1;
    brokenLineThrough(points);

//    myPath->addPath(this->drawWaist1(1,typeSang));
    this->drawWaist1(1,typeSang);
}

/**
 * @brief
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
    leftH2 = qRound(sx)+hWidth2-qRound(point.x());//长度类型，对应图上的小圆圈
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

    myPath->addPath(this->drawWaist2(1,typeSang));
}

/**
 * @brief 前片腰线
 *
 * @param wCase
 * @param sangCase
 * @param points
 * @return QPainterPath
 */
QPainterPath MyPath::waist_1(int wCase,int sangCase,QList<QPointF> *points)
{
    QPainterPath path;
    QList<QPointF>::iterator i=points->begin();
    QPointF p1 = (*i);
    ++i;
    path.moveTo(leftUpPoint1);
    path.cubicTo(p1,p1,(*i));
    ++i;
    path.lineTo(*i);
    ++i;
    path.lineTo(*i);
    ++i;
    if(sangCase==1)
    {
        p1 = (*i);
        ++i; QPointF p2 = (*i);
        ++i; path.cubicTo(p1,p2,(*i));
        ++i; path.lineTo(*i);
        i++; path.lineTo(*i);
        i++;
    }
    path.cubicTo((*i),(*i),rightUpPoint1);

    return path;
}

/**
 * @brief 画前片的腰
 *
 * @param wCase 腰位
 * @param sangCase 省褶类型
 * @return QPainterPath
 */
QPainterPath MyPath::drawWaist1(int wCase,int sangCase)
{
    QPainterPath path,path1,path2;
    path.addPath(this->sang_1(sangCase)); //画省，并添加腰线关键点到points
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

    return path;
}

/**
 * @brief
 *
 * @param wCase
 * @param sangCase
 * @param points
 * @return QPainterPath
 */
QPainterPath MyPath::waist_2(int wCase,int sangCase,QList<QPointF> *points)
{
    QPainterPath path;
    QList<QPointF>::iterator i=points->begin();
    QPointF p1 = (*i);
    ++i;
    path.moveTo(leftUpPoint2);
    path.cubicTo(p1,p1,(*i));
    ++i;
    path.lineTo(*i);
    ++i;
    path.lineTo(*i);
    ++i;
    if(sangCase==1)
    {
        p1 = (*i);
        ++i; QPointF p2 = (*i);
        ++i; path.cubicTo(p1,p2,(*i));
        ++i; path.lineTo(*i);
        i++; path.lineTo(*i);
        i++;
    }
    path.cubicTo((*i),(*i),rightUpPoint2);
    return path;
}

/**
 * @brief
 *
 * @param pCase
 * @param sangCase
 * @return QPainterPath
 */
QPainterPath MyPath::drawWaist2(int pCase,int sangCase)
{
    QPointF midHPoint,midPocketPoint;
    qreal angle,dx,dy,scale;
    QPainterPath path,path1,path2;
    QList<QPointF> *points = new QList<QPointF>;

//    //画单嵌线口袋
//    if(pCase==1)
//    {
//        midHPoint = QPointF(leftHPoint2.x()+0.51*rightUpPoint2.x()-0.49*leftUpPoint2.x(),leftHPoint2.y()+(rightUpPoint2.y()-leftUpPoint2.y())/2.0);
////        midHPoint = QPointF(leftHPoint2.x()+(rightUpPoint2.x()-leftUpPoint2.x())/2.0,leftHPoint2.y()+(rightUpPoint2.y()-leftUpPoint2.y())/2.0);
//        midPocketPoint = getVertexOfSang_Up(leftHPoint2,midHPoint,55);
//        angle = qAsin(1.0*upWaist2/hWidth2);
//        dx = qCos(angle)*60; dy = qSin(angle)*60;
//        path1.moveTo(QPointF(midPocketPoint.x()-dx,midPocketPoint.y()+dy));
//        path1.lineTo(QPointF(midPocketPoint.x()+dx,midPocketPoint.y()-dy));
//        scale = 7.5/hWidth2;    dy = scale*sqrt(hWidth2*hWidth2+upWaist2*upWaist2);    dx = scale*upWaist2;
//        path2 = path1;    path2.translate(dx,dy);
//        path1.translate(-dx,-dy);
//        QPainterPath::Element e=path2.elementAt(path2.elementCount()-1);
//        path1.lineTo(e.x,e.y);
//        e = path2.elementAt(0);
//        path1.lineTo(e.x,e.y);
//        e = path1.elementAt(0);
//        path1.lineTo(e.x,e.y);
//        path.addPath(path1);
//    }

    path.addPath(this->sang_2(sangCase,points)); //画省，并添加腰线关键点到points
    path1 = this->waist_2(pCase,sangCase,points);
    path.addPath(path1);
//    scale = 1.0*waistBandWidth/hWidth2;
//    dy = scale*sqrt(hWidth2*hWidth2+upWaist2*upWaist2);
//    dx = scale*upWaist2;
//    path2 = path1; path2.translate(dx,dy);
////    QPainterPath waistBottom = cutPath(path2,path);
////    path.addPath(waistBottom);   有bug
//    path.addPath(path2);

    delete points;
    return path;
}

/**
 * @brief 省 前片
 *
 * @param intCase
 * @return QPainterPath
 */
QPainterPath MyPath::sang_1(int intCase){
    QPainterPath path;
    QPointF sangA,sangB,tempPoint;
    qreal l1=w_h_height/2.0, l0=l1+32, l2=l1-15;
    switch (intCase) {
    case 0: //单省
        tempPoint = QPointF(rightUpPoint1.x()+sang1-halfCroWidth1,rightUpPoint1.y()+l0);
        sangA = getPointOfSang(tempPoint,leftUpPoint1,rightUpPoint1);
        path.addPath(drawSang1(sangA,tempPoint,l0,sang1));
        break;
    case 1: //双省
        tempPoint = QPointF(rightUpPoint1.x()+sang1-halfCroWidth1,rightUpPoint1.y()+l1);
        sangA = getPointOfSang(tempPoint,leftUpPoint1,rightUpPoint1);
        sangB = getPointOfSang_P(sangA,rightUpPoint1,1.0/2);
        path.addPath(drawSang1(sangA,tempPoint,l1,sang1/3.0*2));
        path.addPath(drawSang1(sangB,getVertexOfSang_Down(leftUpPoint1,sangB,l2),l2,sang1/3.0));
        break;
    case 2: //褶
        break;
    }
    return path;
}

//省：后片
/**
 * @brief
 *
 * @param intCase
 * @param points
 * @return QPainterPath
 */
QPainterPath MyPath::sang_2(int intCase,QList<QPointF> *points){
    QPainterPath path;
    QPointF sangA,sangB;
    qreal l0=w_h_height/2.0+35, l1=l0-10, l2=l0;
    if(true) //有单嵌线口袋存在
    {
        l0 = 110; l1 = 110; l2 = 110; // 待计算
    }
    switch (intCase) {
    case 0: //单省
        sangA = getPointOfSang_P(rightUpPoint2,leftUpPoint2,1/2.0);
        path.addPath(drawSang2(sangA,getVertexOfSang_Down(leftUpPoint2,sangA,l0),l0,sang2,points));
        break;
    case 1: //双省
        sangA = getPointOfSang_P(rightUpPoint2,leftUpPoint2,2/3.0);
        sangB = getPointOfSang_P(rightUpPoint2,leftUpPoint2,1/3.0);
        path.addPath(drawSang2(sangA,getVertexOfSang_Down(leftUpPoint2,sangA,l1),l1,sang2/2.0,points));
        path.addPath(drawSang2(sangB,getVertexOfSang_Down(leftUpPoint2,sangB,l2),l2,sang2/2.0,points));
        break;
    }
    return path;
}

/**
 * @brief 画前省、腰线
 *
 * @param point 省的位置
 * @param vertex 省尖点
 * @param ls 省长
 * @param sang 省量
 * @return QPainterPath
 */
QPainterPath MyPath::drawSang1(QPointF point,QPointF vertex,qreal ls,qreal sang)
{
    QPainterPath path;
    QPointF p1=getPointOnRadial(point,leftUpPoint1,sang),
            p4=getPointOnRadial(point,rightUpPoint1,sang),
            pMid=getPointOnRadial(vertex,point,distanceBetween(p1,vertex)),
            p2=getPointOfSang(vertex,pMid,p1),
            p3=getPointOfSang(vertex,pMid,p4);

    path.moveTo(p2);
    path.lineTo(vertex);
    path.lineTo(p3);

    QList<QPointF> pList;
    pList<<p2<<vertex<<p3;
    brokenLineThrough(pList);
    pList.clear();
    pList<<leftUpPoint1<<p2<<pMid;
    curveThrough(pList);
    pList.clear();
    pList<<pMid<<p3<<rightUpPoint1;
    curveThrough(pList);

    return path;
}
/**
 * @brief
 *
 * @param point
 * @param vertex
 * @param ls
 * @param sang
 * @param points
 * @return QPainterPath
 */
QPainterPath MyPath::drawSang2(QPointF point,QPointF vertex,qreal ls,qreal sang,QList<QPointF> *points)
{
    QPainterPath path;
    QPointF p1=getPointOnRadial(point,leftUpPoint2,sang),
            p4=getPointOnRadial(point,rightUpPoint2,sang),
            pMid=getPointOnRadial(vertex,point,distanceBetween(p1,vertex)),
            p2=getPointOfSang(vertex,pMid,p1),
            p3=getPointOfSang(vertex,pMid,p4);
    path.moveTo(p2);
    path.lineTo(vertex);
    path.lineTo(p3);
    points->append(p1);
    points->append(p2);
    points->append(pMid);
    points->append(p3);
    points->append(p4);
    return path;
}

/**
 * @brief
 *
 * @param A
 * @param B
 * @return QPainterPath
 */
QPainterPath MyPath::lineThrough2P(QPointF A,QPointF B)
{
    QPainterPath path;
    path.moveTo(A);
    path.lineTo(B);
    parent->myPathData->addLine(A,B);
    return path;
}

/**
 * @brief 用myPath画折线，并添加数据到myPathData数组
 *
 * @param points 有序的折线顶点列表
 */
void MyPath::brokenLineThrough(QList<QPointF> points)
{
    if(points.size()<2)
        return;
    QPointF point;
    point = points.takeAt(0);
    qreal dx=point.x()-myPath->currentPosition().x(),
          dy=point.y()-myPath->currentPosition().y();
    if(dx>0.1 || dx<-0.1 || dy<-0.1 ||dy>0.1)
        myPath->moveTo(point);
    myPath->lineTo(points.at(0));
    parent->myPathData->addLine(point,points.takeAt(0));
    while(!points.isEmpty())
    {
        point = points.takeAt(0);
        myPath->lineTo(point);
        parent->myPathData->addLineTo(point);
    }
}

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
QPainterPath MyPath::curveThrough3P(QPointF A,QPointF B,QPointF C,QPointF ctrl1,QPointF ctrl4)
{
    QPointF E((A.x()+B.x())/2.0,(A.y()+B.y())/2.0);
    QPointF F((B.x()+C.x())/2.0,(B.y()+C.y())/2.0);
    qreal d1 =  distanceBetween(A,B),    d2 = distanceBetween(B,C),    scale = d1/(d1+d2);
    QPointF D = getPointOfSang_P(E,F,scale);
    qreal dx = B.x()-D.x(),    dy = B.y()-D.y();
    QPointF ctrl2(E.x()+dx,E.y()+dy);
    QPointF ctrl3(F.x()+dx,F.y()+dy);
    QPainterPath path;
    path.moveTo(A);    path.cubicTo(ctrl1,ctrl2,B);    path.cubicTo(ctrl3,ctrl4,C);
    return path;
}

/**
 * @brief
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
    qreal d1 =  distanceBetween(A,B),    d2 = distanceBetween(B,C),    scale = d1/(d1+d2);
    QPointF D = getPointOfSang_P(E,F,scale);
    qreal dx = B.x()-D.x(),    dy = B.y()-D.y();
    QPointF ctrl2(E.x()+dx,E.y()+dy);
    QPointF ctrl3(F.x()+dx,F.y()+dy);
    ctrlPoints->insert(1,ctrl2);
    ctrlPoints->insert(2,ctrl3);
}

/**
 * @brief
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
        qreal dx = myPath->currentPosition().x()-points.at(0).x(),
              dy = myPath->currentPosition().y()-points.at(0).y();
        if(dx<-0.1 || dx>0.1 || dy<-0.1 || dy>0.1)
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
 * @brief
 *
 * @param points
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 */
void MyPath::curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    curveThrough_data(points,firstCtrlPoint,lastCtrlPoint);
    MyPath path(parent);
    path.curveThrough_data(points,firstCtrlPoint,lastCtrlPoint);
    parent->myPathData->addCurve(points,firstCtrlPoint,lastCtrlPoint,*(path.myPath));
}

/**
 * @brief
 *
 * @param points
 */
void MyPath::curveThrough(QList<QPointF> points)
{
    if(!points.isEmpty())
        curveThrough(points,points.first(),points.last());
    else
        return;
}

/**
 * @brief
 *
 * @param path1
 * @param path2
 * @return QPainterPath
 */
QPainterPath MyPath::cutPath(QPainterPath path1,QPainterPath path2)
{
    QPainterPath closedPath=path1-path2;
    int n=closedPath.elementCount(), i=0;
    qreal x,y;
    QPainterPath unclosedPath;
    for(;i<n;i++)
    {
        QPainterPath::Element e=closedPath.elementAt(i);
        if(e.isMoveTo()){
            x=e.x; y=e.y;
            unclosedPath.moveTo(x,y);
        }
        else if(e.isLineTo()){
            qreal dx=e.x-x, dy=e.y-y, E=0.005;
            //如果unclosedPath的点在path1上，且这个点不是起点,就画线
            if(path1.contains(QPointF(e))&&(dx<-E||dx>E||dy<-E||dy>E))
                unclosedPath.lineTo(e.x,e.y);
        }
        else if(e.type==2){
            i++;
            QPainterPath::Element e2=closedPath.elementAt(i);
            i++;
            QPainterPath::Element e3=closedPath.elementAt(i);
            unclosedPath.cubicTo(e.x,e.y,e2.x,e2.y,e3.x,e3.y);
        }
    }
    return unclosedPath;
}

/**
 * @brief
 *
 * @param path1
 * @param path2
 * @return QPainterPath
 */
QPainterPath MyPath::intersectPath(QPainterPath path1,QPainterPath path2)//有bug
{
    QPainterPath closedPath=path1&path2;
    int n=closedPath.elementCount(), i=0;
    qreal x,y;
    QPainterPath unclosedPath;
    for(;i<n;i++)
    {
        QPainterPath::Element e=closedPath.elementAt(i);
        if(e.isMoveTo()){
            x=e.x; y=e.y;
            unclosedPath.moveTo(x,y);
        }
        else if(e.isLineTo()){
            qreal dx=e.x-x, dy=e.y-y, E=0.005;
            //如果unclosedPath的点在path1上，且这个点不是起点,就画线
            if(path1.contains(QPointF(e))&&(dx<-E||dx>E||dy<-E||dy>E)
               &&path2.contains(QPointF(e)))
                unclosedPath.lineTo(e.x,e.y);
        }
        else if(e.type==2){
            i++;
            QPainterPath::Element e2=closedPath.elementAt(i);
            i++;
            QPainterPath::Element e3=closedPath.elementAt(i);
            unclosedPath.cubicTo(e.x,e.y,e2.x,e2.y,e3.x,e3.y);
        }
    }
    return unclosedPath;
}

//画小裆：求辅助点函数（弃用）
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
QPointF MyPath::getIntersection(qreal nA1,qreal x1,qreal y1,qreal nA2,qreal x2,qreal y2,qreal scale)
{
    qreal k1=-qTan(nA1), k2=-qTan(nA2); //注意因为坐标系，要给斜率添加负号
    qreal b1=y1-k1*x1,b2=y2-k2*x2;
    qreal xx=1.0*(b2-b1)/(k1-k2);
    qreal yy=k1*xx+b1;
    return QPointF(xx+(x2-xx)*scale,yy+(y2-yy)*scale);
}

//求长度：两点间距
/**
 * @brief
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

//求长度：两平行线间距
//qreal MyPath::distanceBetween(QPointF p1,QPointF p2,QPointF p3,QPointF p4)
//{
//}

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

//求点：位于p(x1,y1)出发，斜率为k的向下/上的射线上，并距离(x1,y1)距离为l的点
/**
 * @brief
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
 * @brief
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
 * @brief
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
 * @brief
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

//求点：“点p1(x1,y1)出发、经过p2(x2,y2)的射线”与“直线y=kx+b”的交点
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
QPointF MyPath::getIntersection(qreal x1,qreal y1,qreal x2,qreal y2,qreal k,qreal b)
{
    qreal k1 = (y1-y2)/(x1-x2), b1 = y1-k1*x1;
    qreal x = (b-b1)/(k1-k);
    return QPointF(x,k*x+b);
}
/**
 * @brief
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
//求两直线的交点。其中一条直线经过p1,p2,另一条经过p3,p4。
/**
 * @brief
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
    if(x1==x2)
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

//求点（省）：根据腰线顶点p1、p2以及到顶点p1的距离distance确定省的位置
/**
 * @brief
 *
 * @param p1
 * @param p2
 * @param distance
 * @return QPointF
 */
QPointF MyPath::getPointOfSang_D(QPointF p1,QPointF p2,qreal distance)
{
    qreal dx = p1.x()-p2.x(), dy = p1.y()-p2.y();
    qreal l = sqrt(dx*dx+dy*dy), proportion = 1.0*distance/l;
    return getPointOfSang_P(p1,p2,proportion);
}
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
QPointF MyPath::getPointOfSang_D(qreal x1,qreal y1,qreal x2,qreal y2,qreal distance)
{
    qreal dx = x1-x2, dy = y1-y2;
    qreal l = sqrt(dx*dx+dy*dy), proportion = 1.0*distance/l;
    return getPointOfSang_P(x1,y1,x2,y2,proportion);
}

///
/// \brief 求点（省）：根据腰线顶点p1、p2以及到顶点p1的距离占腰线长度的比例proportion确定省的位置
///
/**
 * @brief
 *
 * @param p1
 * @param p2
 * @param proportion
 * @return QPointF
 */
QPointF MyPath::getPointOfSang_P(QPointF p1,QPointF p2,qreal proportion)
{
    qreal x1=p1.x(), y1=p1.y(), dx=x1-p2.x(), dy=y1-p2.y();
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}
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
QPointF MyPath::getPointOfSang_P(qreal x1,qreal y1,qreal x2,qreal y2,qreal proportion)
{
    qreal dx=x1-x2, dy=y1-y2;
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}

//求点（省尖）：根据腰线上点p1、省点ps、省长ls求省尖点
/**
 * @brief
 *
 * @param p1
 * @param ps
 * @param ls
 * @return QPointF
 */
QPointF MyPath::getVertexOfSang_Down(QPointF p1,QPointF ps,qreal ls)
{
    qreal k2=(p1.x()-ps.x())/(ps.y()-p1.y());
    return getPointOnRadial_Down(ps,k2,ls);
}
/**
 * @brief
 *
 * @param p1
 * @param ps
 * @param ls
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
 * @param vertex
 * @param leftUp
 * @param rightUp
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

//求点：距离点p的距离为l，且位于直线y=kx+b上的点。该点在p左侧。
/**
 * @brief
 *
 * @param p
 * @param l
 * @param k
 * @param b
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
 * @brief
 *
 * @param p
 * @param l
 * @param k
 * @param b
 * @return QPointF
 */
QPointF MyPath::getIntersection_R(QPointF p,qreal l,qreal k,qreal b)
{
    qreal x=p.x(), y=p.y();
    qreal aa=k*k+1, bb=2*(k*b-k*y-x), cc=b*b-2*y*b+x*x+y*y-l*l;
    qreal xx=sqrt(bb*bb-4*aa*cc)-bb/(2.0*aa), yy=k*xx+b;
    return QPointF(xx,yy);
}

//求点：点point关于点center的对称点
/**
 * @brief
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

//path的currentPosition是否和p的位置相符
/**
 * @brief
 *
 * @param path
 * @param p
 * @return bool
 */
bool MyPath::currentPositionequal(QPainterPath path,QPointF p)
{
    QPointF pp = path.currentPosition();
    qreal ppx=pp.x(), ppy=pp.y(),
          dx=ppx-p.x(), dy=ppy-p.y();
    if(dx>-0.1 && dx<0.1 && dy>-0.1 && dy<0.1)
        return true;
    else
        return false;
}

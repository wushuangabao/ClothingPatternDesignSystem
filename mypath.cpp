#include "mypath.h"
#include "painterarea.h"
#include "mainwindow.h"
#include "data/mypathdata.h"
#include <QPainter>
#include <QDebug>

MyPath::MyPath(PainterArea *parent)
{
    this->parent = parent;
    myPath = new QPainterPath;

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
    sang1 = (hWidth1-rightWaist)/2.0-pantsW/8.0;
    sang2 = (hWidth2-pantsW/4.0)/3.0*2;
}

//辅助线：H型前片
QPainterPath MyPath::auxiliaryLinesH_1(const QPointF startPoint)
{
    smallCroPoint = QPointF(startPoint.x()-smallCro,startPoint.y()+pantsCrotchH);
    leftUpPoint1 = QPointF(startPoint.x()+rightWaist,startPoint.y()+downWaist);
    rightUpPoint1 =QPointF(startPoint.x()+hWidth1-sang1,startPoint.y());

    qreal x = startPoint.x()+hWidth1, y = startPoint.y();
    QPainterPath myPath;
    myPath.moveTo(startPoint);
    myPath.lineTo(x,y);
    y = startPoint.y()+w_h_height;
    myPath.moveTo(startPoint.x(),y);
    myPath.lineTo(x,y);
    myPath.moveTo(smallCroPoint);
    myPath.lineTo(x,smallCroPoint.y());
    y = startPoint.y()+knee_height;
    myPath.moveTo(startPoint.x(),y);
    myPath.lineTo(x,y);
    y = startPoint.y()+pantsL;
    myPath.moveTo(startPoint.x(),y);
    myPath.lineTo(x,y);
    x = startPoint.x();
    myPath.moveTo(startPoint);
    myPath.lineTo(x,y);
    x = x-smallCro+halfCroWidth1;
    myPath.moveTo(x,startPoint.y());
    myPath.lineTo(x,y);
    x = startPoint.x()+hWidth1;
    myPath.moveTo(x,startPoint.y());
    myPath.lineTo(x,y);

    return myPath;
}

//辅助线：H型后片
QPainterPath MyPath::auxiliaryLinesH_2(const QPointF startPoint)
{
    qreal x = startPoint.x(), y = startPoint.y();
    bigCroPoint = QPointF(x+hWidth2+bigCro,y+pantsCrotchH+downCro);

    QPainterPath myPath;
    x += hWidth2;
    myPath.moveTo(startPoint);
    myPath.lineTo(x,y);
    y = startPoint.y()+w_h_height;
    myPath.moveTo(startPoint.x(),y);
    myPath.lineTo(x,y);
    myPath.moveTo(startPoint.x(),bigCroPoint.y());
    myPath.lineTo(bigCroPoint);
    y = startPoint.y()+knee_height;
    myPath.moveTo(startPoint.x(),y);
    myPath.lineTo(x,y);
    y = startPoint.y()+pantsL;
    myPath.moveTo(startPoint.x(),y);
    myPath.lineTo(x,y);
    x = startPoint.x();
    myPath.moveTo(startPoint);
    myPath.lineTo(x,y);
    x = x+xMidVerticalLine2;
    myPath.moveTo(x,startPoint.y());
    myPath.lineTo(x,y);
    x = startPoint.x()+hWidth2;
    myPath.moveTo(x,startPoint.y());
    myPath.lineTo(x,y);

    return myPath;
}

//轮廓线：H型前片
QPainterPath MyPath::outlineH_1(const QPointF startPoint,int typeSang)
{
    QPainterPath wPath;
    QPointF tempPoint1 = rightUpPoint1;
    QPointF tempPoint2(startPoint.x()+hWidth1,startPoint.y()+w_h_height);
    qreal tempL = knee_height-w_h_height;

    wPath.moveTo(rightUpPoint1);
    //右边
    wPath.cubicTo(tempPoint1,QPointF(tempPoint2.x(),tempPoint2.y()-w_h_height*0.25),tempPoint2);
    tempPoint1.setX(tempPoint2.x()-halfCroWidth1+feetWidth/2.0+deltaKneeWidth);
    tempPoint1.setY(startPoint.y()+knee_height);
    wPath.cubicTo(QPointF(tempPoint2.x(),tempPoint2.y()+tempL*0.3183),QPointF(tempPoint1.x(),tempPoint1.y()-tempL*0.2957),tempPoint1);
    tempPoint2.setX(tempPoint1.x()-deltaKneeWidth);
    tempPoint2.setY(startPoint.y()+pantsL);
    wPath.lineTo(tempPoint2);
    //底边
    tempPoint1.setX(tempPoint2.x()-feetWidth);
    tempPoint1.setY(tempPoint2.y());
    wPath.lineTo(tempPoint1);
    //左边
    tempPoint2.setX(tempPoint1.x()-deltaKneeWidth);
    tempPoint2.setY(startPoint.y()+knee_height);
    wPath.lineTo(tempPoint2);
    tempL = knee_height-pantsCrotchH;
    tempPoint2.setY(tempPoint2.y()-tempL*0.17);
    tempPoint1 = QPointF(smallCroPoint.x()+(tempPoint2.x()-smallCroPoint.x())*0.395,smallCroPoint.y()+tempL*0.22);
    wPath.cubicTo(tempPoint1,tempPoint2,smallCroPoint);

    wPath.addPath(this->smallCroCurve(startPoint));
    wPath.addPath(this->drawWaist1(1,typeSang));

    return wPath;
}

//轮廓线：H型后片
QPainterPath MyPath::outlineH_2(const QPointF startPoint,int typeSang)
{
    QPainterPath wPath;
    rightUpPoint2 = QPointF(startPoint.x()+xMidVerticalLine2/2.0+hWidth2/2.0,startPoint.y()-upWaist);
    leftUpPoint2 = QPointF(getIntersection_L(rightUpPoint2,hWidth2,0,startPoint.y()));
    leftUpPoint2 = getPointOnRadial(leftUpPoint2,rightUpPoint2,sang2/2.0);
    QPointF vertexPoint(startPoint.x()+hWidth2-leftCro,startPoint.y()+pantsCrotchH);
    //wPath.addEllipse(vertexPoint,2,2);
    cubicStartPoint2 = getIntersection(rightUpPoint2,vertexPoint,0,startPoint.y()+w_h_height);
    bigCroPoint = QPointF(startPoint.x()+hWidth2+bigCro,startPoint.y()+pantsCrotchH+downCro);
    leftH = qRound(startPoint.x())+hWidth2-qRound(cubicStartPoint2.x());
    qreal tempL = knee_height-w_h_height;

    wPath.moveTo(leftUpPoint2);
    //左边
    QPointF tempPoint1(leftUpPoint2.x()-2,leftUpPoint2.y()+8); //数值待定
    QPointF tempPoint2(startPoint.x()-leftH,startPoint.y()+w_h_height);
    leftHPoint2 = tempPoint2;
    wPath.cubicTo(tempPoint1,tempPoint2,tempPoint2);
    tempPoint1 = QPointF(tempPoint2.x(),tempPoint2.y()+tempL*0.17143);
    tempPoint2 = QPointF(startPoint.x()+(xMidVerticalLine2-(feetWidth+60)/2),startPoint.y()+knee_height);
    wPath.cubicTo(tempPoint1,QPointF(tempPoint2.x(),tempPoint2.y()-tempL*0.22857),tempPoint2);
    tempPoint1 = QPointF(tempPoint2.x()+deltaKneeWidth,startPoint.y()+pantsL);
    wPath.lineTo(tempPoint1);
    //底边
    tempPoint1.setX(tempPoint1.x()+feetWidth+40);
    wPath.lineTo(tempPoint1);
    //右边
    tempPoint2.setX(tempPoint1.x()+deltaKneeWidth);
    wPath.lineTo(tempPoint2);
    tempL = knee_height-pantsCrotchH-downCro;
    tempPoint1 = QPointF(tempPoint2.x(),tempPoint2.y()-tempL*0.2349);
    tempPoint2 = QPointF(bigCroPoint.x()-bigCro*0.34965,bigCroPoint.y()+tempL*0.18078);
    wPath.cubicTo(tempPoint1,tempPoint2,bigCroPoint);

    wPath.addPath(this->bigCroCurve());
    wPath.addPath(this->drawWaist2(1,typeSang));

    return wPath;
}

QPainterPath MyPath::smallCroCurve(const QPointF startPoint)
{
    QPointF cubicStartPoint(startPoint.x(),startPoint.y()+w_h_height);
    qreal nAngle1=qAtan(1.0*smallCro/(pantsCrotchH-w_h_height)); //单位为弧度
    qreal nAngle2=M_PI-nAngle1;
    nAngle1=M_PI_2-nAngle1;
    QPointF cubicMidPoint=getIntersection(nAngle1,cubicStartPoint.x(),cubicStartPoint.y(),
                                          nAngle2,cubicStartPoint.x(),smallCroPoint.y(),
                                          1.0/3);
    QPainterPath myPath;
    myPath.moveTo(leftUpPoint1);
    myPath.lineTo(cubicStartPoint);

    QPointF ctrlPoint1 = QPointF(startPoint.x(),startPoint.y()+qRound((pantsCrotchH+w_h_height)/2.0)-3);
    QPointF ctrlPoint2 = QPointF(qRound(cubicMidPoint.x()+11),qRound(cubicMidPoint.y()-9));
    QPointF ctrlPoint3 = QPointF(cubicMidPoint.x()-7,qRound(cubicMidPoint.y()+7));
    QPointF ctrlPoint4 = QPointF(qRound(smallCroPoint.x()+8),qRound(smallCroPoint.y()-3));
    myPath.cubicTo(ctrlPoint1,ctrlPoint2,cubicMidPoint);
    myPath.cubicTo(ctrlPoint3,ctrlPoint4,smallCroPoint);

    return myPath;
}

QPainterPath MyPath::bigCroCurve()
{
    qreal lx = bigCroPoint.x()-cubicStartPoint2.x();
    qreal ly = bigCroPoint.y()-cubicStartPoint2.y();
    QPainterPath myPath;
    myPath.moveTo(rightUpPoint2);
    myPath.lineTo(cubicStartPoint2);
    myPath.cubicTo(QPointF(cubicStartPoint2.x()+lx*0.13,cubicStartPoint2.y()+ly*0.68),
                   QPointF(bigCroPoint.x()-lx*0.75,bigCroPoint.y()),
                   bigCroPoint);
    return myPath;
}

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

QPainterPath MyPath::drawWaist1(int wCase,int sangCase)
{
    QPainterPath path,path1,path2;
    QList<QPointF> *points = new QList<QPointF>;
    path.addPath(this->sang_1(sangCase,points)); //画省，并添加腰线关键点到points
    path1 = this->waist_1(wCase,sangCase,points);
    path.addPath(path1);
    qreal l=distanceBetween(leftUpPoint1,rightUpPoint1), scale = 1.0*waistBandWidth/l;
    qreal dy=scale*sqrt(l*l-downWaist*downWaist);
    qreal dx=scale*downWaist;
    path2 = path1; path2.translate(dx,dy);

    QPainterPath intersection = path2-path;
    //qDebug()<<"intersection="<<intersection<<intersection.elementAt(0)<<intersection.elementAt(2);
    path.addPath(intersection);

    //path.addPath(path2);
    return path;
}

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

QPainterPath MyPath::drawWaist2(int pCase,int sangCase)
{
    QPointF midHPoint,midPocketPoint;
    qreal angle,dx,dy,scale;
    QPainterPath path,path1,path2;
    QList<QPointF> *points = new QList<QPointF>;

    //画单嵌线口袋
    if(pCase==1)
    {
        midHPoint = QPointF(leftHPoint2.x()+(rightUpPoint2.x()-leftUpPoint2.x())/2.0,leftHPoint2.y()+(rightUpPoint2.y()-leftUpPoint2.y())/2.0);
        midPocketPoint = getVertexOfSang_Up(leftHPoint2,midHPoint,55);
        angle = qAsin(1.0*upWaist/hWidth2);
        dx = qCos(angle)*60; dy = qSin(angle)*60;
        path1.moveTo(QPointF(midPocketPoint.x()-dx,midPocketPoint.y()+dy));
        path1.lineTo(QPointF(midPocketPoint.x()+dx,midPocketPoint.y()-dy));
        path.addPath(path1);
        scale = 7.5/hWidth2;    dy = scale*sqrt(hWidth2*hWidth2+upWaist*upWaist);    dx = scale*upWaist;
        path2 = path1;    path2.translate(dx,dy);    path.addPath(path2);
        path1.translate(-dx,-dy);    path.addPath(path1);
    }

    path.addPath(this->sang_2(sangCase,points)); //画省，并添加腰线关键点到points
    path1 = this->waist_2(pCase,sangCase,points);
    path.addPath(path1);
    scale = 1.0*waistBandWidth/hWidth2;
    dy = scale*sqrt(hWidth2*hWidth2+upWaist*upWaist);
    dx = scale*upWaist;
    path2 = path1; path2.translate(dx,dy);
    path.addPath(path2);

    return path;
}

//省：前片
QPainterPath MyPath::sang_1(int intCase,QList<QPointF> *points){
    QPainterPath path;
    QPointF sangA,sangB,tempPoint;
    qreal l1=w_h_height/2.0, l0=l1+32, l2=l1-15;
    switch (intCase) {
    case 0: //单省
        tempPoint = QPointF(rightUpPoint1.x()+sang1-halfCroWidth1,rightUpPoint1.y()+l0);
        sangA = getPointOfSang(tempPoint,leftUpPoint1,rightUpPoint1);
        path.addPath(drawSang1(sangA,tempPoint,l0,sang1,points));
        break;
    case 1: //双省
        tempPoint = QPointF(rightUpPoint1.x()+sang1-halfCroWidth1,rightUpPoint1.y()+l1);
        sangA = getPointOfSang(tempPoint,leftUpPoint1,rightUpPoint1);
        sangB = getPointOfSang_P(sangA,rightUpPoint1,1.0/2);
        path.addPath(drawSang1(sangA,tempPoint,l1,sang1/3.0*2,points));
        path.addPath(drawSang1(sangB,getVertexOfSang_Down(leftUpPoint1,sangB,l2),l2,sang1/3.0,points));
        break;
    case 2: //褶
        break;
    }
    return path;
}

//省：后片
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

//根据省的位置point、省尖点vertex、省长ls、省量sang来画前省
QPainterPath MyPath::drawSang1(QPointF point,QPointF vertex,qreal ls,qreal sang,QList<QPointF> *points)
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
    points->append(p1);
    points->append(p2);
    points->append(pMid);
    points->append(p3);
    points->append(p4);
    return path;
}
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

QPainterPath MyPath::lineThrough2P(QPointF A,QPointF B)
{
    QPainterPath path;
    path.moveTo(A);
    path.lineTo(B);
    parent->myPathData->addLine(A,B);
    return path;
}

void MyPath::brokenLineThrough(QList<QPointF> points)
{
    if(points.size()<2)
        return;
    QPointF point;
    point = points.takeAt(0);
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

void MyPath::curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    QList<QPointF> *ctrlPoints = new QList<QPointF>;
    ctrlPoints->append(firstCtrlPoint);
    ctrlPoints->append(lastCtrlPoint);
    int num=points.size(), i=3;
    if(num>0){
        myPath->moveTo(points.at(0));
        while(i<=num){
            addCtrlPoints(points.at(0),points.at(1),points.at(2),ctrlPoints);
            myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1)); //绘制到第2个端点的三次贝塞尔曲线
            points.takeAt(0); //删除第1个端点
            i++;
        }
        myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1));
        points.takeAt(0);
    }
    delete ctrlPoints;

    parent->myPathData->addCurve(points,firstCtrlPoint,lastCtrlPoint);
}

void MyPath::curveThrough(QList<QPointF> points,QPointF firstCtrlPoint)
{
    if(!points.isEmpty())
        curveThrough(points,firstCtrlPoint,points.last());
    else
        return;
}

void MyPath::curveThrough(QList<QPointF> points)
{
    if(!points.isEmpty())
        curveThrough(points,points.first(),points.last());
    else
        return;
}

//画小裆：求辅助点函数
QPointF MyPath::getIntersection(qreal nA1,qreal x1,qreal y1,qreal nA2,qreal x2,qreal y2,qreal scale)
{
    qreal k1=-qTan(nA1), k2=-qTan(nA2); //注意因为坐标系，要给斜率添加负号
    qreal b1=y1-k1*x1,b2=y2-k2*x2;
    qreal xx=1.0*(b2-b1)/(k1-k2);
    qreal yy=k1*xx+b1;
    return QPointF(xx+(x2-xx)*scale,yy+(y2-yy)*scale);
}

//求长度：两点间距
qreal MyPath::distanceBetween(QPointF p1,QPointF p2)
{
    qreal x1=p1.x(), y1=p1.y(), x2=p2.x(),y2=p2.y();
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

//求长度：两平行线间距
//qreal MyPath::distanceBetween(QPointF p1,QPointF p2,QPointF p3,QPointF p4)
//{
//}

//求点：位于p1(x1,y1)出发、经过p2(x2,y2)的射线上，并距离(x1,y1)距离为l的点
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
QPointF MyPath::getPointOnRadial(QPointF p1,QPointF p2,qreal l)
{
    return getPointOnRadial(p1.x(),p1.y(),p2.x(),p2.y(),l);
}

//求点：位于p(x1,y1)出发，斜率为k的向下/上的射线上，并距离(x1,y1)距离为l的点
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
QPointF MyPath::getPointOnRadial_Down(QPointF p,qreal k,qreal l)
{
    return getPointOnRadial_Down(p.x(),p.y(),k,l);
}
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
QPointF MyPath::getPointOnRadial_Up(QPointF p,qreal k,qreal l)
{
    return getPointOnRadial_Up(p.x(),p.y(),k,l);
}

//求点：“点p1(x1,y1)出发、经过p2(x2,y2)的射线”与“直线y=kx+b”的交点
QPointF MyPath::getIntersection(qreal x1,qreal y1,qreal x2,qreal y2,qreal k,qreal b)
{
    qreal k1 = (y1-y2)/(x1-x2), b1 = y1-k1*x1;
    qreal x = (b-b1)/(k1-k);
    return QPointF(x,k*x+b);
}
QPointF MyPath::getIntersection(QPointF p1,QPointF p2,qreal k,qreal b)
{
    qreal x1 = p1.x(), x2 = p2.x(), y1 = p1.y(), y2 = p2.y();
    qreal k1 = (y1-y2)/(x1-x2), b1 = y1-k1*x1;
    qreal x = (b-b1)/(k1-k);
    return QPointF(x,k*x+b);
}

//求点（省）：根据腰线顶点p1、p2以及到顶点p1的距离distance确定省的位置
QPointF MyPath::getPointOfSang_D(QPointF p1,QPointF p2,qreal distance)
{
    qreal dx = p1.x()-p2.x(), dy = p1.y()-p2.y();
    qreal l = sqrt(dx*dx+dy*dy), proportion = 1.0*distance/l;
    return getPointOfSang_P(p1,p2,proportion);
}
QPointF MyPath::getPointOfSang_D(qreal x1,qreal y1,qreal x2,qreal y2,qreal distance)
{
    qreal dx = x1-x2, dy = y1-y2;
    qreal l = sqrt(dx*dx+dy*dy), proportion = 1.0*distance/l;
    return getPointOfSang_P(x1,y1,x2,y2,proportion);
}

//求点（省）：根据腰线顶点p1、p2以及到顶点p1的距离占腰线长度的比例proportion确定省的位置
QPointF MyPath::getPointOfSang_P(QPointF p1,QPointF p2,qreal proportion)
{
    qreal x1=p1.x(), y1=p1.y(), dx=x1-p2.x(), dy=y1-p2.y();
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}
QPointF MyPath::getPointOfSang_P(qreal x1,qreal y1,qreal x2,qreal y2,qreal proportion)
{
    qreal dx=x1-x2, dy=y1-y2;
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}

//求点（省尖）：根据腰线上点p1、省点ps、省长ls求省尖点
QPointF MyPath::getVertexOfSang_Down(QPointF p1,QPointF ps,qreal ls)
{
    qreal k2=(p1.x()-ps.x())/(ps.y()-p1.y());
    return getPointOnRadial_Down(ps,k2,ls);
}
QPointF MyPath::getVertexOfSang_Up(QPointF p1,QPointF ps,qreal ls)
{
    qreal k2=(p1.x()-ps.x())/(ps.y()-p1.y());
    return getPointOnRadial_Up(ps,k2,ls);
}

//求点（省）：根据省尖点位置，求与腰线的垂点
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
QPointF MyPath::getIntersection_L(QPointF p,qreal l,qreal k,qreal b)
{
    qreal x=p.x(), y=p.y();
    qreal aa=k*k+1, bb=2*(k*b-k*y-x), cc=b*b-2*y*b+x*x+y*y-l*l;
    qreal xx=-(sqrt(bb*bb-4*aa*cc)+bb)/(2.0*aa), yy=k*xx+b;
    return QPointF(xx,yy);
}
QPointF MyPath::getIntersection_R(QPointF p,qreal l,qreal k,qreal b)
{
    qreal x=p.x(), y=p.y();
    qreal aa=k*k+1, bb=2*(k*b-k*y-x), cc=b*b-2*y*b+x*x+y*y-l*l;
    qreal xx=sqrt(bb*bb-4*aa*cc)-bb/(2.0*aa), yy=k*xx+b;
    return QPointF(xx,yy);
}


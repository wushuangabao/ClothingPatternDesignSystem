#include "mypathdata.h"
#include <QtDebug>
//#include <QTextCodec>

MyPathData::MyPathData(QString name)
{
    this->name = name;
}

bool MyPathData::saveTo(QString filePath)
{
//    QFile file(QDir::currentPath()+"/data.txt");
    QFile file(filePath);
    QString content = "";

    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QByteArray str = content.toUtf8();
    file.write(str);

    file.close();

//    QTextStream out(&file);
//    QTextCodec *code = QTextCodec::codecForName("utf8");
//    out.setCodec(code);
//    out<<"123\nabc"<<endl<<"sd"; //这里不能用中文（原因不明）

    return true;
}

MyPathData::~MyPathData()
{
    file.close();
    for(int i=0;i<numberPoint;i++)
    {
        CurvePoint *curvePoint=pointData[i].point, *curvePoint2=curvePoint;
        while(curvePoint->pre!=nullptr)
        {
            curvePoint=curvePoint->pre;
            delete  curvePoint->next;
        }
        delete curvePoint;
        while(curvePoint2->next!=nullptr)
        {
            curvePoint2=curvePoint2->next;
            delete  curvePoint2->pre;
        }
        delete curvePoint2;
    }
}

void MyPathData::addLineTo(QPointF endPoint)
{
    CurvePoint point(endPoint.x(),endPoint.y());
    PointData pointDataStruct={
        numberPoint,
        &point
    };
    pointData[numberPoint]=pointDataStruct;
    PathData pathDataStruct={
        numberPath,
        true,
        numberPoint-1,
        numberPoint,
    };
    pathData[numberPath]=pathDataStruct;
    numberPath++;
    numberPoint++;
}

void MyPathData::addLine(QPointF startPoint,QPointF endPoint)
{
    CurvePoint curvepoint(startPoint.x(),startPoint.y());
    if(numberPoint>0 && equal(pointData[numberPoint-1].point,&curvepoint))
        addLineTo(endPoint); //如果起点与pointData[numberPoint-1]是同一点，就不添加进pointData数组了
    else
    {
        PointData pointDataStruct={
            numberPoint,
            &curvepoint
        };
        pointData[numberPoint]=pointDataStruct;
        numberPoint++;
        addLineTo(endPoint);
    }
}

void MyPathData::addCurve(QList<QPointF> points)
{
    CurvePoint *pFirstCPoint = new CurvePoint;
    CurvePoint *pCPoint = new CurvePoint;
    PointData pointDataStruct;
    //取出并删除points中的第一个点
    QPointF point = points.takeAt(0);
    CurvePoint curvePoint1(point.x(),point.y());
    pFirstCPoint = &curvePoint1;
    pCPoint = &curvePoint1;
    //将points中其余的点在之前取出的第一个点后面串成链表
    int numOfPoints = points.size();
    for(int i=0;i<numOfPoints;i++)
    {
        CurvePoint curvePoint2(points.at(i).x(),points.at(i).y(),pCPoint);
        pCPoint = &curvePoint2;
    }
    pFirstCPoint->setFirst();
    pCPoint->setLast();
    //将起点加入pointData数组（不管数组中是否已经存在同一点）
    pointDataStruct={
        numberPoint,
        pFirstCPoint
    };
    pointData[numberPoint]=pointDataStruct;
    numberPoint++;
    //将终点加入pointData数组
    pointDataStruct={
        numberPoint,
        pCPoint
    };
    pointData[numberPoint]=pointDataStruct;
    numberPoint++;
    //创建新的PathData，加入pathData数组
    PathData pathDataStruct={
        numberPath,
        false,
        numberPoint-2,
        numberPoint-1
    };
    pathData[numberPath]=pathDataStruct;
    numberPath++;
}

void MyPathData::addCurve(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    CurvePoint *pFirstCPoint = new CurvePoint;
    CurvePoint *pCPoint = new CurvePoint;
    CurvePoint *ctrlPoint1 = new CurvePoint;
    CurvePoint *ctrlPoint2 = new CurvePoint;
    CurvePoint curvePoint(firstCtrlPoint.x(),firstCtrlPoint.y());
    ctrlPoint1 = &curvePoint;
    curvePoint = CurvePoint(lastCtrlPoint.x(),lastCtrlPoint.y());
    ctrlPoint2 = &curvePoint;
    PointData pointDataStruct;
    //取出并删除points中的第一个点
    QPointF point = points.takeAt(0);
    CurvePoint curvePoint1(point.x(),point.y());
    pFirstCPoint = &curvePoint1;
    pCPoint = &curvePoint1;
    //将points中其余的点在之前取出的第一个点后面串成链表
    int numOfPoints = points.size();
    for(int i=0;i<numOfPoints;i++)
    {
        CurvePoint curvePoint2(points.at(i).x(),points.at(i).y(),pCPoint);
        pCPoint = &curvePoint2;
    }
    pFirstCPoint->setFirst(ctrlPoint1);
    pCPoint->setLast(ctrlPoint2);
    //将起点加入pointData数组（不管数组中是否已经存在同一点）
    pointDataStruct={
        numberPoint,
        pFirstCPoint
    };
    pointData[numberPoint]=pointDataStruct;
    numberPoint++;
    //将终点加入pointData数组
    pointDataStruct={
        numberPoint,
        pCPoint
    };
    pointData[numberPoint]=pointDataStruct;
    numberPoint++;
    //创建新的PathData，加入pathData数组
    PathData pathDataStruct={
        numberPath,
        false,
        numberPoint-2,
        numberPoint-1
    };
    pathData[numberPath]=pathDataStruct;
    numberPath++;
}

bool MyPathData::equal(CurvePoint *p1,CurvePoint *p2)
{
    qreal dx=p1->x-p2->x, dy=p1->y-p2->y;
    if(dx<E && dx>-E && dy<E && dy>-E)
        return true;
    else
        return false;
}

bool MyPathData::equal(CurvePoint p1,CurvePoint p2)
{
    qreal dx=p1.x-p2.x, dy=p1.y-p2.y;
    if(dx<E && dx>-E && dy<E && dy>-E)
        return true;
    else
        return false;
}

void MyPathData::addPoint(QPointF point,QString name)
{
    CurvePoint curvePoint(point.x(),point.y());
    PointData pointDataS={
        numberPoint,
        &curvePoint
    };
    pointData[numberPoint]=pointDataS;
    if(name!="")
        pointMap.insert(name,numberPoint);
    numberPoint++;
}

QPointF MyPathData::findPoint(QString name)
{
    QMap<QString,int>::iterator i;
    i = pointMap.find(name);
    if(i==pointMap.end())
        return QPointF(0,0); //如果没有找到对应点，就返回(0,0)
    else
    {
        int id = i.value();
        qreal xx = pointData[id].point->x;
        qreal yy = pointData[id].point->y;
        return QPointF(xx,yy);
    }
}

#include "mypathdata.h"
#include <QtDebug>
//#include <QTextCodec>

MyPathData::MyPathData(QString name)
{
    this->name = name;
}

QString MyPathData::stringOf(CurvePoint *point)
{
    qreal x = point->x,
          y = point->y;

    QString str = findName(QPointF(x,y))+"("+QString::number(point->x,'f',2)+","+QString::number(point->y,'f',2)+")";
    if(point->isCtrlPoint)
    {
        if(point->next!=nullptr)
            str = str+"--";
    }
    else if(point->isLast)
    {
        if(point->next!=nullptr)
            str = str+"--";
    }
    else if(point->next!=nullptr)
    {
        str = str+"->";
    }
    return str;
}

QString MyPathData::stringsOf(CurvePoint *point)
{
    QString str="";
    if(point->isFirst)
    {
        if(point->pre!=nullptr)
            str=stringOf(point->pre);
        str = str+stringOf(point);
        while(point->next!=nullptr)
        {
            point = point->next;
            str = str+stringOf(point);
        }
    }
    else
    {
        str=stringOf(point);
    }
    return str;
}

CurvePoint MyPathData::pointOf(QString str)
{
    int id1=str.indexOf("("),
        id2=str.indexOf(","),
        id3=str.indexOf(")"),
        n1=id2-id1+1,
        n2=id3-id2+1;
    QString xs=str.mid(id1,n1),
            ys=str.mid(id2,n2);
    qreal x=xs.toDouble(),
          y=ys.toDouble();
    CurvePoint point(x,y);
    return point;
}

bool MyPathData::saveTo(QString filePath)
{
    //QFile file(QDir::currentPath()+"/pathData.txt");
    QFile file(filePath);
    QString content = "--paths--\n";
    int i;
    for(i=0;i<numberPath;i++)
    {
        QString id = QString::number(pathData[i].id);
        QString isLine = "false";
        if(pathData[i].isLine)
            isLine = "true";
        QString p1 = QString::number(pathData[i].startPoint);
        QString p2 = QString::number(pathData[i].endPoint);
        content = content + id +" "+ isLine +" "+p1+" "+p2+"\n";
    }
    content = content+"--points--\n";
    for(i=0;i<numberPoint;i++)
    {
        QString id = QString::number(pointData[i].id);
        content = content + id +" "+ stringsOf(pointData[i].point) +"\n";
    }

    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QByteArray str = content.toUtf8();
    file.write(str);

    //file.close();已经在析构函数里面调用了

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
    CurvePoint *point = new CurvePoint(endPoint.x(),endPoint.y());
    PointData pointDataStruct={
        numberPoint,
        point
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
    CurvePoint *curvepoint = new CurvePoint(startPoint.x(),startPoint.y());
    if(numberPoint>0 && equal(pointData[numberPoint-1].point,curvepoint))
        addLineTo(endPoint); //如果起点与pointData[numberPoint-1]是同一点，就不添加进pointData数组了
    else
    {
        PointData pointDataStruct={
            numberPoint,
            curvepoint
        };
        pointData[numberPoint]=pointDataStruct;
        numberPoint++;
        addLineTo(endPoint);
    }
}

void MyPathData::addCurve(QList<QPointF> points)
{
    CurvePoint *pFirstCPoint = new CurvePoint(0,0);
    CurvePoint *pCPoint = new CurvePoint(0,0);
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
//    //将终点加入pointData数组
//    pointDataStruct={
//        numberPoint,
//        pCPoint
//    };
//    pointData[numberPoint]=pointDataStruct;
//    numberPoint++;
    //创建新的PathData，加入pathData数组
    PathData pathDataStruct={
        numberPath,
        false,
        numberPoint-1,
        -1 //没有记录终点
        //numberPoint-2,
        //numberPoint-1
    };
    pathData[numberPath]=pathDataStruct;
    numberPath++;
}

void MyPathData::addCurve(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    CurvePoint *ctrlPoint1 = new CurvePoint(firstCtrlPoint.x(),firstCtrlPoint.y());
    CurvePoint *ctrlPoint2 = new CurvePoint(lastCtrlPoint.x(),lastCtrlPoint.y());
    PointData pointDataStruct;
    //取出并删除points中的第一个点
    QPointF point = points.takeAt(0);
    CurvePoint *pFirstCPoint = new CurvePoint(point.x(),point.y());
    CurvePoint *pCPoint = pFirstCPoint;
    //将points中其余的点在之前取出的第一个点后面串成链表
    int numOfPoints = points.size();
    for(int i=0;i<numOfPoints;i++)
    {
        CurvePoint *curvePoint2=new CurvePoint(points.at(i).x(),points.at(i).y(),pCPoint);
        pCPoint = curvePoint2;
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
//    //将终点加入pointData数组
//    pointDataStruct={
//        numberPoint,
//        pCPoint
//    };
//    pointData[numberPoint]=pointDataStruct;
//    numberPoint++;
    //创建新的PathData，加入pathData数组
    PathData pathDataStruct={
        numberPath,
        false,
        numberPoint-1,
        -1 //没有记录终点
        //numberPoint-2,
        //numberPoint-1
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
    if(name!="")
        pointMap.insert(name,point);
}

bool MyPathData::pointMapHas(QPointF point)
{
    qreal x=point.x(), y=point.y();
    QMap<QString,QPointF>::iterator i;
    for( i=pointMap.begin(); i!=pointMap.end(); ++i)
    {
        qreal dx = i.value().x()-x,
              dy = i.value().y()-y;
        if(dx<E && dx>-E && dy<E && dy>-E)
            return true;
    }
    return false;
}

QString MyPathData::findName(QPointF point)
{
    qreal x=point.x(), y=point.y();
    QMap<QString,QPointF>::iterator i;
    for( i=pointMap.begin(); i!=pointMap.end(); ++i)
    {
        qreal dx = i.value().x()-x,
              dy = i.value().y()-y;
        if(dx<E && dx>-E && dy<E && dy>-E)
            return i.key();
    }
    return "";
}

QPointF MyPathData::findPoint(QString name)
{
    QMap<QString,QPointF>::iterator i;
    i = pointMap.find(name);
    if(i==pointMap.end())
        return QPointF(0,0); //如果没有找到对应点，就返回(0,0)
    else
    {
        return i.value();
    }
}

void MyPathData::clear()
{
    numberPath=0; numberPoint=0;
    pointMap.clear();
}

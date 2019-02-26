#include "mypathdata.h"
#include <QtDebug>
#include <QPainterPath>
//#include <QTextCodec>

MyPathData::MyPathData(QString name)
{
    this->name = name;
}

QString MyPathData::stringOf(CurvePoint *point)
{
    QPointF p=pointData[point->id];
    qreal x = p.x(),
          y = p.y();
    QString str = findName(point->id)+"("+QString::number(x,'f',2)+","+QString::number(y,'f',2)+")";
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
        str=stringOf(point)+stringOf(point->next);
    }
    return str;
}

//CurvePoint MyPathData::pointOf(QString str)
//{
//    int id1=str.indexOf("("),
//        id2=str.indexOf(","),
//        id3=str.indexOf(")"),
//        n1=id2-id1+1,
//        n2=id3-id2+1;
//    QString xs=str.mid(id1,n1),
//            ys=str.mid(id2,n2);
//    qreal x=xs.toDouble(),
//          y=ys.toDouble();
//    CurvePoint point(x,y);
//    return point;
//}

bool MyPathData::saveTo(QString filePath)
{
    //QFile file(QDir::currentPath()+"/pathData.txt");
    QFile file(filePath);
    QString content = "--paths--\n";
    int i;
    for(i=0;i<numberPath;i++)
    {
        QString id = QString::number(pathData[i].id),
                isLine = "false";
        if(pathData[i].isLine)
            isLine = "true";
        CurvePoint *sp=pathData[i].startPoint;
        QString p1 = QString::number(sp->id),
                p2 = QString::number(pathData[i].endPoint->id);
        content = content + id +" "+ isLine +" "+p1+" "+p2+"\n"+stringsOf(sp)+"\n";
    }
    content = content+"--points--\n";
    for(i=0;i<numberPoint;i++)
    {
        QString x=QString::number(pointData[i].x()),
                y=QString::number(pointData[i].y());
        content = content+QString::number(i)+" ("+x+","+y+")\n";
    }

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
    for(int i=0;i<numberPath;i++)
    {
        delete pathData[i].path;
        CurvePoint *curvePoint=pathData[i].startPoint;
        if(curvePoint->pre!=nullptr)
            delete  curvePoint->pre;
        while(curvePoint->next!=nullptr)
        {
            CurvePoint *p = curvePoint;
            curvePoint=curvePoint->next;
            delete  p;
        }
        delete curvePoint;
    }
}

bool MyPathData::addLineTo(QPointF endPoint,int idSP)
{
    if(numberPoint<1 || idSP<-1 || idSP>=numberPoint)
        return false;
    if(idSP==-1) //表示起点为myPath路径的最后一个点
        idSP = idCurrentPoint;
    int idEP = findPoint(endPoint,true);
    CurvePoint *startCPoint = new CurvePoint(idSP);
    CurvePoint *endCPoint = new CurvePoint(idEP);
    startCPoint->next = endCPoint;
    QPainterPath *path = new QPainterPath;
    path->moveTo(pointData[idSP]);
    path->lineTo(pointData[idEP]);
    PathData pathDataStruct={
        numberPath,
        true,
        startCPoint,
        endCPoint,
        path
    };
    pathData[numberPath] = pathDataStruct;
    numberPath++;
    idCurrentPoint = endCPoint->id;
    return true;
}

void MyPathData::addLine(QPointF startPoint,QPointF endPoint)
{
    addLineTo(endPoint,findPoint(startPoint,true));
}

void MyPathData::addCurve(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint,QPainterPath path)
{
    CurvePoint *ctrlPoint1 = new CurvePoint(addCtrlPoint(firstCtrlPoint));
    CurvePoint *ctrlPoint2 = new CurvePoint(addCtrlPoint(lastCtrlPoint));
    QPainterPath *pathPointer = new QPainterPath(path);
    //取出并删除points中的第一个点
    QPointF point = points.takeAt(0);
    CurvePoint *pFirstCPoint = new CurvePoint(findPoint(point,true));
    CurvePoint *pCPoint = pFirstCPoint;
    //将points中其余的点在之前取出的第一个点后面串成链表
    int numOfPoints = points.size();
    for(int i=0;i<numOfPoints;i++)
    {
        CurvePoint *curvePoint2=new CurvePoint(findPoint(points.at(i)),pCPoint);
        pCPoint = curvePoint2;
    }
    pFirstCPoint->setFirst(ctrlPoint1);
    pCPoint->setLast(ctrlPoint2);
    //创建新的PathData，加入pathData数组
    PathData pathDataStruct={
        numberPath,
        false,
        pFirstCPoint,
        pCPoint,
        pathPointer
    };
    pathData[numberPath]=pathDataStruct;
    numberPath++;
    idCurrentPoint = pCPoint->id;
}

//bool MyPathData::equal(CurvePoint *p1,CurvePoint *p2)
//{
//    qreal dx=p1->x-p2->x, dy=p1->y-p2->y;
//    if(dx<E && dx>-E && dy<E && dy>-E)
//        return true;
//    else
//        return false;
//}

//bool MyPathData::equal(QPointF p1,CurvePoint *p2,qreal E)
//{
//    qreal dx=p1.x()-p2->x, dy=p1.y()-p2->y;
//    if(dx<E && dx>-E && dy<E && dy>-E)
//        return true;
//    else
//        return false;
//}

bool MyPathData::equal(QPointF p1,QPointF p2)
{
    qreal dx=p1.x()-p2.x(), dy=p1.y()-p2.y();
    if(dx<E && dx>-E && dy<E && dy>-E)
        return true;
    else
        return false;
}

int MyPathData::addPoint(QPointF point,QString name)
{
    int i=0;
    for(;i<numberPoint;i++)
        if(equal(pointData[i],point))
            break;
    if(i==numberPoint)
    {
        numberPoint++;
        pointData[i]=point;
    }
    if(name!="")
        pointMap.insert(name,i);
    return i;
}

int MyPathData::addCtrlPoint(QPointF ctrlPoint)
{
    int i=numberPoint;
    numberPoint++;
    pointData[i]=ctrlPoint;
    return i;
}

int MyPathData::findPoint(QPointF point,bool addIfNotFind)
{
    int i=0;
    if(numberPoint<1)
    {
        if(addIfNotFind)
        {
            i=numberPoint;
            pointData[i]=point;
            numberPoint++;
            return i;
        }
        else
            return -1;
    }
    for(;i<numberPoint;i++)
        if(equal(point,pointData[i]))
            return i;
    if(i==numberPoint)
    {
        if(addIfNotFind)
        {
            i=numberPoint;
            pointData[i]=point;
            numberPoint++;
            return i;
        }
        else
            return -1;
    }
}

bool MyPathData::pointMapHas(QPointF point)
{
    int id=findPoint(point);
    if(id==-1)
        return false;
    QMap<QString,int>::iterator i;
    for( i=pointMap.begin(); i!=pointMap.end(); ++i)
    {
        if(id==i.value())
            return true;
    }
    return false;
}

//QString MyPathData::findName(QPointF point)
//{
//    int id = findPoint(point);
//    if(id==-1)
//        return "";
//    else
//        return findName(id);
//}
//QString MyPathData::findName(qreal x,qreal y)
//{
//    return findName(QPointF(x,y));
//}
QString MyPathData::findName(int id)
{
    QMap<QString,int>::iterator i;
    for(i=pointMap.begin(); i!=pointMap.end(); ++i)
    {
        if(id==i.value())
            return i.key();
    }
    return "";
}

QPointF MyPathData::findPoint(QString name)
{
    QMap<QString,int>::iterator i;
    i = pointMap.find(name);
    if(i==pointMap.end())
        return QPointF(0,0); //如果没有找到对应点，就返回(0,0)
    else
        return pointData[i.value()];
}

void MyPathData::clear()
{
    numberPath=0; numberPoint=0;
    pointMap.clear();
}

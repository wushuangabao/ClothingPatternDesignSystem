#include "mypathdata.h"
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <QPainterPath>
#include <QDateTime>
#include "../mypath.h"

/**
 * @brief 构造函数
 *
 * @param name 名称
 * @param parent
 */
MyPathData::MyPathData(QString name,PainterArea* parent)
{
    this->name = name;
    this->parent=parent;
}

/**
 * @brief
 *
 * @param point
 * @return QString
 */
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

/**
 * @brief
 *
 * @param point
 * @return QString
 */
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

/**
 * @brief myPathData保存为txt文件
 *
 * @param filePath
 * @return bool
 */
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
    file.write(content.toUtf8());

    file.close();

    return true;
}

/**
 * @brief 保存为ASTM标准
 * @return 是否成功
 */
bool MyPathData::writeASTM(QString filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QString content = "999\nANSI/AAMA\n0\nSECTION\n2\nBLOCKS\n0\nBLOCK\n8\n1\n2\n"+name+"\n70\n0\n10\n0.0\n20\n0.0\n";
    file.write(content.toUtf8());

    // 经向线
    content=" 0\nLINE\n  8\n7\n"+stringPoint(590,655)+" 11\n590.0\n 21\n-455.0\n";
    file.write(content.toUtf8());

    // 样片名称和号型
    writeText(&file,"Piece Name: noname");
    writeText(&file,"Size: M",0,15.0);

    // 边界线、内部线和剪口
    writeBoundary(&file);
    writeInLines(&file);
    writeNotches(&file);

    content="  0\nENDBLK\n  0\nENDSEC\n";
    file.write(content.toUtf8());
    content="  0\nSECTION\n  2\nENTITIES\n  0\nINSERT\n  8\n1\n  2\n-M\n"+stringPoint(0.0,0.0);
    file.write(content.toUtf8());
    writeText(&file,"Style Name: style");
    content=QDateTime::currentDateTime().toString("MM-dd-yyyy");
    writeText(&file,"Creation Date: "+content);
    content="wushuangBaOY_WHA";
    writeText(&file,"Author: "+content);
    writeText(&file,"Sample Size: M");
    writeText(&file,"Grading Rule Table: "+name);
    writeText(&file,"Units: METRIC");
    content="  0\nENDSEC\n  0\nEOF";

    file.close();
    return true;
}

/**
 * @brief 析构函数
 */
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

/**
 * @brief 增加直线段的记录
 *
 * @param endPoint 终点
 * @param idSP 起点的序号
 * @return bool
 */
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

/**
 * @brief 增加直线段的记录
 *
 * @param startPoint
 * @param endPoint
 */
void MyPathData::addLine(QPointF startPoint,QPointF endPoint)
{
    addLineTo(endPoint,findPoint(startPoint,true));
}

/**
 * @brief 增加曲线段的记录
 *
 * @param points
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 * @param path
 */
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

/**
 * @brief 两个点是否相等
 *
 * @param p1
 * @param p2
 * @return bool
 */
bool MyPathData::equal(QPointF p1,QPointF p2)
{
    qreal dx=p1.x()-p2.x(), dy=p1.y()-p2.y();
    if(dx<E && dx>-E && dy<E && dy>-E)
        return true;
    else
        return false;
}

/**
 * @brief 向ASTM标准的文件中写入Text元素
 * @param file 文件指针
 * @param str 文本内容
 * @param x 横坐标
 * @param y 纵坐标
 * @param layer 所在图层
 */
void MyPathData::writeText(QFile *file, QString str, qreal x, qreal y, int layer)
{
    QString content="  0\nTEXT\n  8\n"+QString::number(layer)
            +"\n 10\n"+stringReal(x)+"\n 20\n"+stringReal(-y)
            +"\n 40\n10.0\n 50\n0.0\n  1\n"+str+"\n";
    file->write(content.toUtf8());
}

/**
 * @brief ASTM中写入一系列多段线顶点
 * @param file 文件指针
 * @param path 想要写入的路径数据
 * @param layer 所在图层
 */
void MyPathData::writePolyL(QFile *file, PathData path, int layer)
{
    QString content, vertex="  0\nVERTEX\n  8\n"+QString::number(layer)+"\n";
    int idSP=path.startPoint->id, idEP=path.endPoint->id;
    if(path.isLine){
        QPainterPath painterPath;
        painterPath.moveTo(pointData[idSP]);
        painterPath.lineTo(pointData[idEP]);
        writePolyL(file,painterPath,layer);
    }
    else{
        MyPath myPath(parent);
        QList<QPointF> points;
        CurvePoint *p = path.startPoint->pre;
        QPointF firstCtrlPoint = pointData[p->id];
        while(p->next->isCtrlPoint!=true){
            p=p->next; points<<pointData[p->id];
        }
        p=p->next;
        QPointF lastCtrlPoint = pointData[p->id];
        myPath.curveThrough_data(points,firstCtrlPoint,lastCtrlPoint);
        writePolyL(file,*myPath.myPath,layer);
        //        CurvePoint* cp=path.startPoint;
        //        while(cp->isLast==false){
        //            content=vertex+stringPoint(pointData[cp->id]);
        //            file->write(content.toUtf8());
        //            cp=cp->next;
        //        }
        //        content=vertex+stringPoint(pointData[cp->id]);
        //        file->write(content.toUtf8());
    }
}

/**
 * @brief ASTM中写入一系列多段线顶点
 * @param file 文件指针
 * @param path 想要写入的绘图路径（它将被微分）
 * @param layer 所在图层
 */
void MyPathData::writePolyL(QFile *file, const QPainterPath &path, int layer)
{
    QString content, vertex="  0\nVERTEX\n  8\n"+QString::number(layer)+"\n";
    qreal t=0, dt;
    if(path.length()<175)
        dt=0.02;
    else if(path.length()<350)
        dt=0.04;
    else dt=0.1;
    while(t<0.99)
    {
        QPointF p=path.pointAtPercent(t);
        content=vertex+stringPoint(p);
        file->write(content.toUtf8());
        t=t+dt;
    }
    content=vertex+stringPoint(path.pointAtPercent(1));
    file->write(content.toUtf8());
}

/**
 * @brief ASTM中写入boundary(纸样轮廓）
 * @param file
 */
void MyPathData::writeBoundary(QFile *file)
{
    // TODO：采取某种手段，确保某路径属于边界线
    // 目前从第一条路径开始，默认它是边界线，而且往后首尾衔接的路径都是边界线。
    // 目前默认所有线在绘制时是按顺时针方向的。

    if(numberPath<=0) return;
    int i=0, idSP=pathData[i].startPoint->id;
    QString content;
    writePolyLHead(file);

    while(true){
        int idEP=pathData[i].endPoint->id;
        writePolyL(file,pathData[i]);
        if(idEP==idSP) break;
        else i=findPathBySP(idEP);
        if(i==-1){qDebug()<<"ERROR!找不到下一个路径！";break;}
    }

    content="  0\nSEQEND\n  8\n1\n";
    file->write(content.toUtf8());
}

/**
 * @brief ASTM中写入剪口信息
 * @param file
 */
void MyPathData::writeNotches(QFile *file)
{
    // TODO：采取某种手段，确保某点属于剪口

    QString content;
    int id = pathData[7].startPoint->id;
    content="  0\nPOINT\n  8\n4\n"+stringPoint(pointData[id])+" 30\n0.2\n";
    file->write(content.toUtf8());
    id = pathData[8].endPoint->id;
    content="  0\nPOINT\n  8\n4\n"+stringPoint(pointData[id])+" 30\n0.2\n";
    file->write(content.toUtf8());
}

/**
 * @brief ASTM中写入内部线(Internal lines)信息
 * @param file
 */
void MyPathData::writeInLines(QFile *file)
{
    QString content;
    writePolyLHead(file,8);

    // TODO：采取某种手段，确保某路径属于内部线

    int id=7;
    writePolyL(file,pathData[id],8);
    id=8;
    writePolyL(file,pathData[id],8);

    content="  0\nSEQEND\n  8\n8\n";
    file->write(content.toUtf8());
}

/**
 * @brief 添加PolyLine（多段线）的头字符串到ASTM
 * @param file 文件指针
 * @param layer PolyLine所在图层
 */
void MyPathData::writePolyLHead(QFile *file, int layer)
{
    file->write(("  0\nPOLYLINE\n  8\n"+QString::number(layer)+"\n 66\n1\n 70\n0\n").toUtf8());
}

/**
 * @brief 将实数转化为字符串，精度为2
 * @param n
 * @return
 */
QString MyPathData::stringReal(qreal n)
{
    return QString::number(n,'f',2);
}

/**
 * @brief 将点的坐标转化为ASTM标准写法
 * @param p
 * @return
 */
QString MyPathData::stringPoint(QPointF p)
{
    return " 10\n"+stringReal(p.x())+"\n 20\n"+stringReal(-p.y())+"\n";
}

/**
 * @brief 将xy坐标转化为ASTM标准写法
 * @param x
 * @param y
 * @return
 */
QString MyPathData::stringPoint(qreal x, qreal y)
{
    return " 10\n"+stringReal(x)+"\n 20\n"+stringReal(-y)+"\n";
}

/**
 * @brief 增添点的数据
 *
 * @param point
 * @param name
 * @return int 点在数组中的下标
 */
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

/**
 * @brief 加入曲线尽头的控制点（无论是否已经有相同的点存在）
 *
 * @param ctrlPoint
 * @return int 点在数组中的下标
 */
int MyPathData::addCtrlPoint(QPointF ctrlPoint)
{
    int i=numberPoint;
    numberPoint++;
    pointData[i]=ctrlPoint;
    return i;
}

/**
 * @brief 找点的id
 *
 * @param point
 * @param addIfNotFind 如果没找到，是否加入到点的数组
 * @return int 如既没有找到也没有新加入，返回-1
 */
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

/**
 * @brief map中是否存在点
 *
 * @param point
 * @return bool
 */
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

/**
 * @brief 点的名称
 *
 * @param id 点的序号
 * @return QString
 */
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

/**
 * @brief 找点
 *
 * @param name 名称
 * @return QPointF 如没找到，返回(0,0)
 */
QPointF MyPathData::findPoint(QString name)
{
    QMap<QString,int>::iterator i;
    i = pointMap.find(name);
    if(i==pointMap.end())
        return QPointF(0,0); //如果没有找到对应点，就返回(0,0)
    else
        return pointData[i.value()];
}

/**
 * @brief 清除所有记录
 */
void MyPathData::clear()
{
    numberPath=0; numberPoint=0;
    pointMap.clear();
}

/**
 * @brief 根据起点找到路径
 * @param idSP 起点在数组中的的序号
 * @return 路径在数组中的序号。-1表示没找到。
 */
int MyPathData::findPathBySP(int idSP)
{
    // TODO:有多条路径起点相同的情况，如何处理？

    int i=0;
    for(;i<numberPath;i++){
        if(pathData[i].startPoint->id==idSP)
            return i;
    }
    return -1;
}

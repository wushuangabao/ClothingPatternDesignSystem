#include "mypainter.h"
#include "myrule.h"
#include "../data/mypathdata.h"
#include <QPainter>

MyPainter::MyPainter()
{
    myPath = new QPainterPath;
    myData = new MyPathData("未命名");
    startPoint = new QPointF(0,0);
}

MyPainter::~MyPainter()
{
    delete myPath;
    delete myData;
    delete startPoint;
}

void MyPainter::setStartPoint(qreal x, qreal y)
{
    startPoint->setX(x);
    startPoint->setY(y);
}

void MyPainter::setStartPoint(QPointF point)
{
    *startPoint = point;
}

/**
 * @brief 解析路径代码，添加数据到myData
 * @param path path.str形如"e 连接 i 连接 j 圆顺 a ..."
 */
void MyPainter::parsePathCode(Path path)
{
    // 记录 astm 标志和 name 属性
    myData->astmTag = path.astmTag;
    myData->pathName = path.name;
    // 去除pathCode中的所有空格
    QString pathCode = path.str.remove(' ');
    if(pathCode.contains("以及")){
        // 将pathCode根据"以及"操作符，拆分成几段
        QStringList sl = pathCode.split("以及");
        foreach(const QString &code, sl){
            // todo: 这段 sonPath 赋值的代码应该有问题
            Path sonPath = {
                path.rule,
                code,
                path.astmTag,
                ""
            };
            parsePathCode(sonPath);
        }
        return;
    }
    int n = 1;
    QList<QPointF> points;
    QString type = connectType(pathCode);
    if(type == ""){
        // case: pathCode 本身表示一条直线
        if(path.rule->getTypeOf(pathCode) == "直线"){
            Line l = path.rule->line(pathCode);
            points << convertPoint(l.p1) << convertPoint(l.p2);
            brokenLineThrough(points);
            return;
        }
        // case: pathCode 本身表示一条曲线
        else if(path.rule->getTypeOf(pathCode) == "曲线"){
            Curve c = path.rule->curve(pathCode);
            for(int i = 0; i < c.points.size(); ++i)
                points.append(convertPoint(c.points[i]));
            curveThrough(points);
            return;
        }
        // case: pathCode 本身表示一个路径
        else if(path.rule->getTypeOf(pathCode) == "路径"){
            Path p = path.rule->path(pathCode);
            int oldAstmTag = path.astmTag;
            QString oldName = path.name;
            parsePathCode(p);
            myData->astmTag = oldAstmTag;
            myData->pathName = oldName;
            return;
        }
        // case: 无法识别的语法
        else{
            path.rule->info(pathCode+"无法识别！");
            return;
        }
    }
    // 删除第一个点和type，将点加入points队列
    if(!addPointByRule(&points, path.rule, &pathCode))
        return;
    while(pathCode.length()>0){
        QString nextType = connectType(pathCode);
        // case: 进行绘图操作
        if(nextType != type && nextType != ""){
            if(!addPointByRule(&points, path.rule, &pathCode))
                return;
            // 用points中的n+1个点画路径
            if(type == "圆顺"){
                if(points.size()>2) curveThrough(points);
                else brokenLineThrough(points);
            }
            else brokenLineThrough(points);
            // 删除points的前n个点
            for(int i=0; i<n; ++i)
                points.removeFirst();
            type = nextType;
            n = 1;
        }
        // case: 到达末尾，剩最后一个点
        else if(nextType == ""){
            bool ok = true;
            QPointF point = convertPoint(path.rule->point(pathCode,&ok));
            if(ok){
                addPointData(point,pathCode); // 添加点数据到myData
                points << point;
                if(type == "圆顺") {
                    if(points.size()>2) curveThrough(points);
                    else brokenLineThrough(points);
                }
                else brokenLineThrough(points);
                return;
            }
            else{
                path.rule->info(pathCode+"无法解析！");
                return;
            }
        }
        // case: 提取下一个点
        else{
            if(addPointByRule(&points, path.rule, &pathCode))
                n++;
            else return;
        }
    }
}

/**
 * @brief 根据经过的一系列点，画曲线
 * @param points
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 */
void MyPainter::curve(QList<QPointF> points, QPointF firstCtrlPoint, QPointF lastCtrlPoint)
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
#ifdef Q_OS_DARWIN
            myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(0),points.at(1));
#else
            myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1)); //绘制到第2个端点的三次贝塞尔曲线
#endif
            points.takeAt(0); //删除第1个端点
            i++;
        }
#ifdef Q_OS_DARWIN
        myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(0),points.at(1));
#else
        myPath->cubicTo(ctrlPoints->takeAt(0),ctrlPoints->takeAt(1),points.at(1));
#endif
    }
    ctrlPoints->clear();
    delete ctrlPoints;
}

/**
 * @brief 下一个连接类型是“连接”还是“圆顺”
 * @param code
 * @return 返回""表示找不到新的连接了
 */
QString MyPainter::connectType(QString code)
{
    int id = code.indexOf(QRegExp("连接|圆顺"));
    if(id != -1)
        return code.mid(id,2);
    else
        return "";
}

/**
 * @brief 用myPath画折线，并添加数据到myPathData数组
 * @param points 有序的折线顶点列表
 */
void MyPainter::brokenLineThrough(QList<QPointF> points)
{
    if(points.size()<2)
        return;
    QPointF point;
    point = points.takeAt(0);
    qreal d = distanceBetween(point,myPath->currentPosition());
    if(!equal(d,0.0))
        myPath->moveTo(point);
    myPath->lineTo(points.at(0));
    myData->addLine(point,points.takeAt(0));
    while(!points.isEmpty())
    {
        point = points.takeAt(0);
        myPath->lineTo(point);
        myData->addLineTo(point);
    }
}

/**
 * @brief 根据路径画折线，并添加数据到myPathData数组
 * @param brokenLine 折线(务必确认不含曲线）
 */
void MyPainter::brokenLineThrough(QPainterPath brokenLine)
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
 * @brief 画曲线并添加数据（有首尾控制点）
 * @param points 曲线经过点的列表
 * @param firstCtrlPoint
 * @param lastCtrlPoint
 */
void MyPainter::curveThrough(QList<QPointF> points,QPointF firstCtrlPoint,QPointF lastCtrlPoint)
{
    curve(points,firstCtrlPoint,lastCtrlPoint);
    MyPainter path;
    path.curve(points,firstCtrlPoint,lastCtrlPoint);
    myData->addCurve(points,firstCtrlPoint,lastCtrlPoint);
}

/**
 * @brief 根据MyPathData画QPainterPath
 * @param pathData
 * @param astmTag
 * @param pos 用于输出显示样片名称的位置
 * @return
 */
QPainterPath MyPainter::drawByPathData(MyPathData *data, int astmTag, QList<QPointF>* posList, QList<QString>* nameList)
{
    MyPainter painter;
    QPainterPath* path = painter.myPath;
    int numPaths = data->numberPath, i;
    for(i=0;i<numPaths;++i){
        PathData pathData = data->pathData[i];
        if(pathData.astmTag != astm(astmTag))
            continue;
        QPointF startPoint = data->pointData[pathData.startPoint->id];
        if(!currentPositionequal(*path, startPoint)){
            // 此处为断点（需要另起一点）
            path->moveTo(startPoint);
        }
        // 如果pathData表示直线
        if(pathData.isLine){
            QPointF endPoint = data->pointData[pathData.endPoint->id];
            path->lineTo(endPoint);
            // 如果传入了指针，且当前直线为经向线，并且有名字
            if(astmTag == 2 && posList != nullptr && nameList != nullptr){
                if(pathData.name != ""){
                    posList->append(getPointOnLine(startPoint, endPoint, 0.5));
                    nameList->append(pathData.name);
                }
            }
        }
        // 如果pathData表示曲线
        else{
            QList<QPointF> points;
            CurvePoint *p = pathData.startPoint->pre;
            QPointF firstCtrlPoint = data->pointData[p->id];
            while(p->next->isCtrlPoint!=true){
                p=p->next; points<<data->pointData[p->id];
            }
            p=p->next;
            QPointF lastCtrlPoint = data->pointData[p->id];
            painter.curve(points,firstCtrlPoint,lastCtrlPoint);
        }
    }
    return *path;
}

/**
 * @brief 根据MyPathData画点
 * @param pathData
 * @return
 */
QPainterPath MyPainter::drawPointsByData(MyPathData *data)
{
    MyPainter painter;
    QPainterPath* path = painter.myPath;
    int numPaths = data->numberPoint;
    for(int i = 0; i < numPaths; ++i){
        QPointF center = data->pointData[i];
        path->addEllipse(center.x()-1, center.y()-1, 2, 2);
    }
    return *path;
}

/**
 * @brief 画曲线并添加数据（无控制点）
 * @param points 曲线经过点的列表
 */
void MyPainter::curveThrough(QList<QPointF> points)
{
    if(points.length() >= 3)
        curveThrough(points,points.first(),points.last());
    else
        return;
}

/**
 * @brief 点point关于点center的对称点
 * @param point
 * @param center
 * @return QPointF
 */
QPointF MyPainter::getSymmetryPoint(QPointF point, QPointF center)
{
    qreal px=point.x(), py=point.y(),
            cx=center.x(), cy=center.y(),
            x=2*cx-px, y=2*cy-py;
    return QPointF(x,y);
}

/**
 * @brief 将MyRule中点的坐标值放大10倍
 * @param pFromRule
 * @return
 */
QPointF MyPainter::convertPoint(QPointF pFromRule)
{
    qreal x = pFromRule.x() * 10,
          y = pFromRule.y() * 10;
    return QPointF(x, y);
}

/**
 * @brief path的currentPosition是否和p的位置相符
 * @param path
 * @param p
 * @return
 */
bool MyPainter::currentPositionequal(QPainterPath path, QPointF p)
{
    QPointF pp = path.currentPosition();
    return equal(pp,p);
}

/**
 * @brief 将pathCode中下一个“操作符”之前的点加入points，并删除pathCode中的点名和“操作符”
 * @param points
 * @param rule
 * @param pathCode
 * @return
 */
bool MyPainter::addPointByRule(QList<QPointF> *points, MyRule *rule, QString* pathCode)
{
    int id;
    bool ok = true;
    QString type = connectType(*pathCode);
    if(type == ""){
        rule->info(*pathCode+"无法解析");
        return false;
    }
    else id = pathCode->indexOf(type);
    QString namePoint = pathCode->left(id);
    QPointF point = convertPoint(rule->point(namePoint, &ok));
    if(ok){
        points->append(point); // 将点加入points队末
        pathCode->remove(0,id+type.length()); // 将namePoint和type从字符串中删除
        addPointData(point,namePoint); // 添加点数据到myData
        return true;
    }
    else{
        rule->info("点"+namePoint+"无法解析");
        return false;
    }
}

qreal MyPainter::distanceBetween(QPointF p1, QPointF p2)
{
    qreal x1=p1.x(), y1=p1.y(), x2=p2.x(),y2=p2.y();
    if(equal(x1,x2) && equal(y1,y2))
        return 0.0;
    qreal dx=x1-x2, dy=y1-y2;
    return sqrt(dx*dx + dy*dy);
}

/**
 * @brief 线段上某等分点
 * @param p1 线段端点1
 * @param p2 线段端点2
 * @param proportion 点到端点1的距离占线段长度的比例
 * @return QPointF
 */
QPointF MyPainter::getPointOnLine(QPointF p1,QPointF p2,qreal proportion)
{
    qreal x1=p1.x(), y1=p1.y(), dx=x1-p2.x(), dy=y1-p2.y();
    qreal x=x1-dx*proportion, y=y1-dy*proportion;
    return QPointF(x,y);
}

/**
 * @brief 添加点的数据
 * @param point
 * @param name
 */
void MyPainter::addPointData(QPointF point, QString name)
{
    myData->addPoint(point,name);
}

/**
 * @brief 2个实数是否相等
 * @param v1 实数1
 * @param v2 实数2
 * @return bool
 */
bool MyPainter::equal(qreal v1, qreal v2)
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
bool MyPainter::equal(QPointF p1,QPointF p2)
{
    if(equal(p1.x(),p2.x()) && equal(p1.y(),p2.y()))
        return true;
    else
        return false;
}

/**
 * @brief 向3点曲线中加入中间2个控制点
 * @param A
 * @param B
 * @param C
 * @param ctrlPoints
 */
void MyPainter::addCtrlPoints(QPointF A,QPointF B,QPointF C,QList<QPointF> *ctrlPoints)
{
    QPointF E((A.x()+B.x())/2.0,(A.y()+B.y())/2.0);
    QPointF F((B.x()+C.x())/2.0,(B.y()+C.y())/2.0);
    qreal d1 = distanceBetween(A,B),    d2 = distanceBetween(B,C),    scale = d1/(d1+d2);
    QPointF D = getPointOnLine(E,F,scale);
    qreal dx = B.x()-D.x(),    dy = B.y()-D.y();
    QPointF ctrl2(E.x()+dx,E.y()+dy);
    QPointF ctrl3(F.x()+dx,F.y()+dy);
    ctrlPoints->insert(1,ctrl2);
    ctrlPoints->insert(2,ctrl3);
}

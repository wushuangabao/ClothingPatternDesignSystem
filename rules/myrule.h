#ifndef MYRULE_H
#define MYRULE_H

#include <QtMath>
#include <QPointF>
#include <QList>
#include <QMap>
#include <QStringList>

class MyPainter;
class MyRule;

struct Line{
    QPointF p1;
    QPointF p2;
};

struct Path{
    MyRule *rule;
    QString str;
    int astmTag; // 0-3: 轮廓线、内部线、经线、辅助线（棕、白、绿、红）
    QString name;
};

struct Curve{
    QPointF p1;
    QPointF p2;
    QList<QPointF> points;
};

/**
 * @brief 制板规则类
 */
class MyRule
{
private:
    const QStringList types = {"参数", "点", "直线", "路径", "曲线"}; /**< 实体类型表 */
    const QStringList pFuncs = {"求偏移", "方向向量", "求垂足", "等分点", "求交点", "逆时针转"};  /**< 点的方法名称表 */

    QString file;           /**< 规则文件路径 */
    QStringList entitiesOut;/**< 输出实体名 */
    QStringList entitiesIn; /**< 输入实体值的队列 */
    MyRule *parentRule;     /**< 本规则的调用者 */

    // 转astm字符串为ID
    int astmId(QString strTag);
    // 输入某实体的值（目前有bug）
    bool setInput(QString type, QString name);
    // 找到某规则的文件路径
    QString findRulePath(QString ruleName);

public:
    MyRule(QString file);
    ~MyRule();
    void info(QString info);

    // 基本约束方法
    QPointF pFunc(QString func, int idFunc = -1, bool* ok = nullptr); //求点的方法集合
    QPointF offset(QPointF p1, qreal distance, QPointF direction); //求偏移点
    QPointF direction(QPointF p1, QPointF p2, bool* ok = nullptr); //求方向向量
    QPointF foot(QPointF p1, Line l1, bool* ok = nullptr); //求垂足
    QPointF foot(QPointF p1, Curve c, bool* ok = nullptr); //求垂足 todo
    QPointF divide(QPointF p1, QPointF p2, qreal proprtion); //求等分点
    QPointF divide(QPointF p, Curve c, qreal proprtion, bool* ok = nullptr); //求等分点
    QPointF cross(Line l1, Line l2, bool* ok = nullptr); //求交点
    QPointF cross(Line l, Curve c, bool* ok = nullptr); //求交点
    QPointF cross(Curve c1, Curve c2, bool* ok = nullptr); //求交点
    QPointF rotate(QPointF o, qreal cos, QPointF p, bool* ok = nullptr); //求逆时针转后的某点

    // 平移路径(Path)
    Path movePath(Path &path, QString value, QList<QString> &nameList, bool* ok = nullptr);
    // 求直线
    Line line(QPointF p1, QPointF p2);
    // 求曲线
    Curve curve(QList<QPointF> points, bool* ok = nullptr);

    // 所有实体的表：
    QMap<QString,QString> params; /**< 参数型实体 */
    QMap<QString,QPointF> points; /**< 点类型实体 */
    QMap<QString,Line> lines;     /**< 直线型实体 */
    QMap<QString,Path> paths;     /**< 路径型实体 */
    QMap<QString,Curve> curves;   /**< 曲线型实体 */

    // 解析单句代码
    bool parseCode(QString code);
    // 在实体表中查询某个实体的类型
    QString getTypeOf(QString name);
    // 将字符串解析为（特定类型的）实体：
    qreal param(QString value, bool* ok = nullptr);
    QPointF point(QString value, bool* ok = nullptr);
    Line line(QString value, bool* ok = nullptr);
    Path path(QString value, bool* ok = nullptr);
    Curve curve(QString value, bool* ok = nullptr);

private:
    // 找出两组线段中哪两条线段相交
    void getCrossLines(const QList<Line> &lineList1, const QList<Line> &lineList2, int &numOfLine1, int &numOfLine2, QPointF *p = nullptr);
    // 移动某点的坐标
    void movePointF(QPointF &p, qreal dx, qreal dy);
    // 求曲线长度
    qreal lengthOfCurve(Curve c);
    // 求直线长度
    qreal lengthOfLine(Line l);
    // 预处理单句代码
    QString pretreat(QString code);
    // 读取单句代码中的所有实体名称（用字母、数字、下划线组成，且以字母开头）
    QStringList getEntityNames(QString code);
    // 获取单句代码中出现的实体类型
    QString getEntityType(QString code);
    // 获取单句代码中=后面的字符串
    QString getValue(QString code);
    // 定义实体：
    void defineEntity(int typeId, QString name);
    void defineEntity(QString type, QStringList names);
    // 给实体赋值：
    int assignEntity(QString name, QString value, MyRule* r = nullptr);
    void assignEntities(QStringList names, QString values, MyRule* r = nullptr);

public:
    // 使用自定义规则（f表示规则文件的路径）：
    QString getEntityByRule(QString f, QString in = "", MyRule* parent = nullptr); //返回实体名称
    qreal getParamByRule(QString f, QString in, bool* ok = nullptr); //返回参数值
    QPointF getPointByRule(QString f, QString in, bool* ok = nullptr); //返回点
    Line getLineByRule(QString f, QString in, bool* ok = nullptr); //返回直线段
    Curve getCurveByRule(QString f, QString in, bool* ok = nullptr); //返回曲线段
    // Path pathByRule() // todo

    // 生成绘图路径：
    MyPainter drawPath();
    MyPainter drawPathWith(QString entitiesIn);
    MyPainter drawPath(QString name);
    MyPainter drawPath(QPointF p);
    MyPainter drawPath(Line l);
    MyPainter drawPath(Curve c);
    MyPainter drawPathByCode(Path path);

    // 辅助工具函数：
    static bool zero(qreal r);
    static bool equal(qreal r1, qreal r2);
    static bool left(QPointF p1, QPointF p2);
    static bool right(QPointF p1, QPointF p2);
    static bool down(QPointF p1, QPointF p2);
    static bool up(QPointF p1, QPointF p2);
    static qreal calculate(QString expression, bool* ok = nullptr);
    static QPointF endPoint(Line l, QString s, bool* ok = nullptr);
    static QPointF endPoint(QPointF p1, QPointF p2, QString s, bool* ok = nullptr);
};

#endif // MYRULE_H

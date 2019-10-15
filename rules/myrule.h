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
    QStringList types;      /**< 实体类型表 */
    QStringList pFuncs;     /**< 点的方法名称表 */
    QString file;           /**< 规则文件路径 */
    QStringList entitiesOut;      /**< 输出实体名 */
    QStringList entitiesIn; /**< 输入实体值的队列 */
    MyRule *parentRule;     /**< 本规则的调用者 */

    int astmId(QString strTag);
    bool setInput(QString type, QString name);
    QString findRulePath(QString ruleName);
    void getCrossLines(const QList<Line> &lineList1, const QList<Line> &lineList2, int &numOfLine1, int &numOfLine2, QPointF *p = nullptr);
    void movePointF(QPointF &p, qreal dx, qreal dy);
    qreal lengthOfCurve(Curve c);
    qreal lengthOfLine(Line l);

public:
    MyRule(QString file);
    ~MyRule();
    void info(QString info);

    // 基本约束方法
    QPointF pFunc(QString func, int idFunc = -1, bool* ok = nullptr); // 求点的方法集合
    QPointF offset(QPointF p1, qreal distance, QPointF direction);
    QPointF direction(QPointF p1, QPointF p2, bool* ok = nullptr);
    QPointF foot(QPointF p1, Line l1, bool* ok = nullptr);
    QPointF divide(QPointF p1, QPointF p2, qreal proprtion);
    QPointF divide(QPointF p, Curve c, qreal proprtion, bool* ok = nullptr);
    QPointF cross(Line l1, Line l2, bool* ok = nullptr);
    QPointF cross(Line l, Curve c, bool* ok = nullptr);
    QPointF cross(Curve c1, Curve c2, bool* ok = nullptr);
    QPointF rotate(QPointF o, qreal cos, QPointF p, bool* ok = nullptr);
    Path movePath(Path &path, QString value, QList<QString> &nameList, bool* ok = nullptr);
    Line line(QPointF p1, QPointF p2);
    Curve curve(QList<QPointF> points, bool* ok = nullptr);

    // 所有实体的表：
    QMap<QString,QString> params; /**< 参数型实体 */
    QMap<QString,QPointF> points; /**< 点类型实体 */
    QMap<QString,Line> lines;     /**< 直线型实体 */
    QMap<QString,Path> paths;  /**< 路径型实体 */
    QMap<QString,Curve> curves;   /**< 曲线型实体 */

    // 规则代码解析：
    bool parseCode(QString code);
    QString pretreat(QString code);
    QStringList getEntityNames(QString code);
    QString getEntityType(QString code);
    QString getTypeOf(QString name);
    QString getValue(QString code);
    void defineEntity(int typeId, QString name);
    void defineEntity(QString type, QStringList names);
    int assignEntity(QString name, QString value, MyRule* r = nullptr);
    void assignEntity(QStringList names, QString values, MyRule* r = nullptr);
    qreal param(QString value, bool* ok = nullptr);
    QPointF point(QString value, bool* ok = nullptr);
    Line line(QString value, bool* ok = nullptr);
    Path path(QString value, bool* ok = nullptr);
    Curve curve(QString value, bool* ok = nullptr);

    // 使用自定义规则：
    QString callRule(QString f, QString in = "", MyRule* parent = nullptr);
    qreal paramByRule(QString f, QString in, bool* ok = nullptr);
    QPointF pointByRule(QString f, QString in, bool* ok = nullptr);
    Line lineByRule(QString f, QString in, bool* ok = nullptr);
    Curve curveByRule(QString f, QString in, bool* ok = nullptr);
    // Path pathByRule() // todo

    // 生成绘图路径：
    MyPainter drawPath();
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

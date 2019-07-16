#include "myrule.h"

MyRule::MyRule(QString f)
{
    types << "参数" << "点" << "直线" << "曲线";
    file = f;
}

MyRule::~MyRule(){}

/**
 * @brief 解析1句规则代码
 * @param code
 */
void MyRule::parseCode(QString code)
{

}

/**
 * @brief 读取代码中的实体名称
 * @param code
 * @return
 */
QStringList MyRule::getEntityNames(QString code)
{
    QStringList strList = code.split(,QString::SkipEmptyParts);

}

/**
 * @brief 获取实体name对应的类型
 * @param name
 * @return 返回""表示没有找到该实体
 */
QString MyRule::getTypeOf(QString name)
{
    QMap<QString,QString>::iterator it1;
    it1 = params.find(name);
    if(it1 != params.end())
        return types[0];
    QMap<QString,QPointF>::iterator it2;
    it2 = points.find(name);
    if(it2 != points.end())
        return types[1];
    QMap<QString,Line>::iterator it3;
    it3 = lines.find(name);
    if(it3 != lines.end())
        return types[2];
    QMap<QString,QList<QPointF>>::iterator it4;
    it4 = curves.find(name);
    if(it4 != curves.end())
        return types[3];
    return "";
}

/**
 * @brief 调用自定义规则
 * @param file 规则文件的路径
 * @param input 输入实体的代码
 * @return 输出实体的代码
 */
QString MyRule::callRule(QString file,QString input)
{
    QPainterPath path;
    QString code;

    // 返回一句制板语言的赋值代码
    return code;
}

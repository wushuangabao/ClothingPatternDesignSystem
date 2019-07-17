#include "myrule.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QInputDialog>
#include "../mypath.h"

/**
 * @brief 弹出提示
 * @param info 提示信息
 */
void MyRule::info(QString info)
{
    QMessageBox::information(nullptr,"information",info);
}

/**
 * @brief 弹出实体输入框
 * @param type 实体类型
 * @param name 实体名
 * @return 赋值是否成功
 */
bool MyRule::setInput(QString type, QString name)
{
    bool ok = true;
    QString v = QInputDialog::getText(nullptr,"初始化-输入实体",type+name+" =",QLineEdit::Normal,"单位：cm（不要保留单位）",&ok);
    if(ok){
        QPointF point;
        Line line;
        QList<QPointF> curve;
        switch(types.indexOf(type)){
        case 0:
            v = QString::number(param(v, &ok));
            if(ok)
                params.insert(name,v);
            break;
        case 1:
            point = this->point(v, &ok);
            if(ok)
                points.insert(name,point);
            break;
        case 2:
            line = this->line(v, &ok);
            if(ok)
                lines.insert(name,line);
            break;
        case 3:
            curve = this->curve(v, &ok);
            if(ok)
                curves.insert(name,curve);
            break;
        case -1:
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief p1是否在p2的左侧（x轴向右）
 * @param p1
 * @param p2
 * @return
 */
bool MyRule::left(QPointF p1, QPointF p2)
{
    qreal dx = p1.x()-p2.x();
    if(dx <= -0.001) return true;
    else return false;
}

/**
 * @brief p1是否在p2的上方（y轴向下）
 * @param p1
 * @param p2
 * @return
 */
bool MyRule::up(QPointF p1, QPointF p2)
{
    qreal dy = p1.y()-p2.y();
    if(dy <= -0.001) return true;
    else return false;
}

MyRule::MyRule(QString f)
{
    types << "参数" << "点" << "直线" << "曲线";
    pFuncs << "求偏移" << "定长延长" << "求垂足" << "等分点" << "求交点";
    file = f;
    entityOut = "";
}

MyRule::~MyRule(){}

/**
 * @brief 基本约束方法的前5种（求点）
 * @param func 形如“方法名(实体参数表)”的字符串
 * @param idFunc 方法的索引，默认为-1
 * @return
 */
QPointF MyRule::pFunc(QString func, int idFunc)
{
    QPointF p = QPointF(0,0);
    if(idFunc == -1){
        for(idFunc=0;idFunc<pFuncs.length();idFunc++)
            if(func.contains(pFuncs[idFunc]))
                break;
        if(idFunc == pFuncs.length()){
            // todo: 在参数中加入bool标志，赋值为false
            return p;
        }
    }
    QStringList en = func.split(QRegExp("(|)|（|）|,|，"),QString::SkipEmptyParts);
    for(int i=1;i<en.length();i++)
        en[i] = en[i].simplified();
    // todo: switch中的方法调用失败时，加入bool判别标识
    switch (idFunc) {
    case 0:
        p = offset(points[en[1]],param(en[2]),points[en[3]]);
        break;
    case 1:
        p = extend(lines[en[1]],param(en[2]),lines[en[3]]);
        break;
    case 2:
        p = foot(points[en[1]],lines[en[2]]);
        break;
    case 3:
        p = divide(points[en[1]],points[en[2]],param(en[3]));
        break;
    case 4:
        p = cross(lines[en[1]],lines[en[2]]);
        break;
    }
    return p;
}

/**
 * @brief 解析1句规则代码
 * @param code
 */
bool MyRule::parseCode(QString code)
{
    code = code.simplified();
    if(code == "")
        return true;
    QStringList names = getEntityNames(code);
    QString value = getValue(code);
    if(value == ""){
        QString type = getEntityType(code);
        if(type == ""){
            // case: 指示输出实体
            if(code.contains("输出")){
                if(names.length() == 1){
                    if(entityOut != "") info("输出实体被重复定义，\n以最后一次定义为准");
                    entityOut = names[0];
                    return true;
                }else info(code + "语法错误：\n输出实体名数目>1");
            }
            // case: 指示规则名
            else if(code.contains("规则")){
                // todo
                return true;
            }
            // case: 其他语句
            else info(code + "无法解析");
        }
        // case: 指示输入实体
        else if(code.contains("输入")){
            if(names.length() != 1) info(code + "语法错误：\n输入实体名数目!=1");
            else{
                if(getTypeOf(names[0]) == "")
                    while(!setInput(type,names[0]))
                        info("你输入的值不合法，请重新输入……");
                return true;
            }
        }
        // case: 单纯的定义语句
        else{
            if(names.length() != 1) info(code + "语法错误：\n定义中的实体名数目!=1");
            else{
                defineEntity(type,names[0]);
                return true;
            }
        }
    }else{
        QStringList items = code.split("=",QString::SkipEmptyParts);
        if(items.length() == 2){
            QStringList nameLeft = getEntityNames(items[0]);
            if(nameLeft.length() != 1) info(code + "语法错误：\n等号左边的实体名数目!=1");
            else if(nameLeft[0] != names[0]) info(code + "实体名出错");
            else{
                QString name = names[0];
                QString type = getEntityType(items[0]);
                // case: 单纯的赋值语句
                if(type == ""){
                    assignEntity(name,value);
                    return true;
                }
                // case: 初始化赋值语句
                else{
                    defineEntity(type,name);
                    assignEntity(name,value);
                    return true;
                }
            }
        }else info(code + "语法错误：\n等号的数目>1");
    }
    return false;
}

/**
 * @brief 读取代码中的实体名称
 * @param code
 * @return
 */
QStringList MyRule::getEntityNames(QString code)
{
    // 根据code中的“非字母、数字、下划线”部分，拆分成strList
    QStringList strList = code.split(QRegExp("\\W+"), QString::SkipEmptyParts);
    // 过滤出strList中以字母开头的字符串
    return strList.filter(QRegExp("^[a-z]|[A-Z]"));
}

/**
 * @brief 在code中获取实体类型
 * @param code
 * @return ""表示code中没有实体类型
 */
QString MyRule::getEntityType(QString code)
{
    for(int i=0;i<types.length();i++){
        int id = code.indexOf(types[i]);
        if(id != -1)
            return code.mid(id,types.length());
    }
    return "";
}

/**
 * @brief 在code中获取“值”字符串
 * @param code
 * @return ""表示code中没有赋值
 */
QString MyRule::getValue(QString code)
{
    int id = code.indexOf("=");
    if(id == -1)
        return "";
    else
        return code.mid(id+1).simplified();
}

/**
 * @brief 定义实体，将name作为键写入QMap，值为默认值
 * @param type
 * @param name
 */
void MyRule::defineEntity(QString type, QString name)
{
    // 重名检查
    if(getTypeOf(name) != ""){
        info("语法错误：" + name + "被多次定义。\n以最后一次为准。");
    }
    switch(types.indexOf(type)){
    case -1:
        info("无法定义实体：类型错误");
        break;
    case 0:
        params.insert(name,"0");
        break;
    case 1:
        points.insert(name,QPointF(0,0));
        break;
    case 2:
        lines.insert(name,Line{QPointF(0,0),QPointF(0,0)});
        break;
    case 3:
        QList<QPointF> pList;
        pList<<QPointF(0,0)<<QPointF(0,0)<<QPointF(0,0);
        curves.insert(name,pList);
    }
}

/**
 * @brief 实体赋值
 * @param name
 * @param value
 */
void MyRule::assignEntity(QString name, QString value)
{
    QMap<QString,QString>::iterator it1;
    it1 = params.find(name);
    if(it1 != params.end()){
        *it1 = value;
        return;
    }
    QMap<QString,QPointF>::iterator it2;
    it2 = points.find(name);
    if(it2 != points.end()){
        *it2 = point(value);
        return;
    }
    QMap<QString,Line>::iterator it3;
    it3 = lines.find(name);
    if(it3 != lines.end()){
        *it3 = line(value);
        return;
    }
    QMap<QString,QList<QPointF>>::iterator it4;
    it4 = curves.find(name);
    if(it4 != curves.end()){
        *it4 = curve(value);
        return;
    }
}

/**
 * @brief 参数求值，字符串转换为qreal类型
 * @param value
 * @param ok bool指针 必要时赋值为false
 * @return
 */
qreal MyRule::param(QString value, bool *ok)
{
    QString s = value.remove(' ');
    QMap<QString,QString>::iterator it = params.find(s);
    // case: 值为其他参数名
    if(it != params.end())
        return param(*it, ok);
    // case: 值为单个数值
    else if(!s.contains(QRegExp("[+-*/]"))){
        return s.toDouble(ok);
    }
    if(ok != nullptr) *ok = false;
}

/**
 * @brief 点值解析，字符串转换为QPointF类型
 * @param value
 * @param ok bool指针 必要时赋值为false
 * @return
 */
QPointF MyRule::point(QString value, bool *ok)
{
    value = value.simplified();
    QStringList numbers = value.split(QRegExp("\\(|\\)|（|）|,|，"),QString::SkipEmptyParts);
    if(numbers.length() != 2){
        // case: 赋值为 另一个点
        if(getTypeOf(value) == types[1])
            return points[value];
        else{
            int idFunc = 0;
            for(;idFunc < pFuncs.length(); idFunc++){
                if(value.contains(pFuncs[idFunc]))
                    break;
            }
            if(idFunc == pFuncs.length())
            {
                info("点值解析出错");
                if(ok != nullptr)
                    *ok = false;
                return QPointF(0,0);
            }
            // case: 调用基本约束方法 赋值
            else
                return pFunc(value, idFunc);
        }
    }
    // case: 采用 参数名或数值公式 赋值
    else if(!value.contains("端点")){
        qreal x = param(numbers[0],ok),y = param(numbers[1],ok);
        return QPointF(x,y);
    }
    // case: 赋值为 直线的端点
    else{

    }
}

/**
 * @brief 直线值解析，字符串转换为Line结构体
 * @param value
 * @param ok bool指针 必要时赋值为false
 * @return
 */
Line MyRule::line(QString value, bool *ok)
{
    value = value.simplified();
    QStringList p = value.split(QRegExp("\\(|\\)|（|）|,|，"),QString::SkipEmptyParts);
    if(p.length() != 2){
        info("直线值解析出错");
        if(ok != nullptr)
            *ok = false;
        return line(QPointF(0,0),QPointF(0,0));
    }
    QPointF p1 = point(p[0], ok), p2 = point(p[1], ok);
    return line(p1, p2);
}

/**
 * @brief 查QMap，获取实体name对应的类型
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
 * @brief 调用自定义规则，会弹框要求用户赋值输入实体
 * @param f 规则文件的路径
 * @return 输出实体的代码
 */
QString MyRule::callRule(QString f)
{
    QFile rule(f);
    if(!rule.open(QIODevice::ReadOnly|QIODevice::Text)){
        info("打开文件失败");
        return "";
    }
    // 逐句解析规则代码
    while (!rule.atEnd()) {
        QTextCodec *codec= QTextCodec::codecForName("utf8");
        parseCode(codec->toUnicode(rule.readLine()));
    }
    rule.close();
    // 返回输出实体名
    if(getTypeOf(entityOut) != "")
        return entityOut;
    else
        return "";
}

/**
 * @brief 调用自定义规则
 * @param f 规则文件的路径
 * @param in 输入实体的代码块
 * @return 输出实体的代码
 */
QString MyRule::callRule(QString f,QString in)
{

}

/**
 * @brief 调用自定义规则
 * @param file 规则文件的路径
 * @param in 输入实体的代码列表
 * @return 输出实体的代码
 */
QString MyRule::callRule(QString f, QStringList in)
{

}

/**
 * @brief 根据自身规则文件生成输出实体的绘图路径
 * @return
 */
QPainterPath MyRule::drawPath()
{
    return drawPath(callRule(file));
}

/**
 * @brief 根据实体名name生成实体的绘图路径
 * @param name
 * @return
 */
QPainterPath MyRule::drawPath(QString name)
{
    QPainterPath path;
    if(name == "")
        return  path;
    QString type = getTypeOf(name);
    if(type == ""){
        info("无法绘制：找不到"+name);
        return path;
    }
    switch(types.indexOf(type)){
    case 1:
        path = drawPath(points[name]);
        break;
    case 2:
        path = drawPath(lines[name]);
        break;
    case 3:
        path = drawPath(curves[name]);
        break;
    default:
        info("无法绘制");
    }
    return path;
}

QPainterPath MyRule::drawPath(QPointF point)
{
    QPainterPath p;
    // 画一个×
    qreal x = point.x(), y = point.y();
    p.moveTo(QPointF(x-5,y-5));
    p.lineTo(QPointF(x+5,y+5));
    p.moveTo(QPointF(x-5,y+5));
    p.lineTo(QPointF(x+5,y-5));
    return p;
}

QPainterPath MyRule::drawPath(Line line)
{
    QPainterPath p;
    p.moveTo(line.p1);
    p.lineTo(line.p2);
    return p;
}

QPainterPath MyRule::drawPath(QList<QPointF> curve)
{
    QPainterPath p;
    // todo: 画插值曲线
    return p;
}

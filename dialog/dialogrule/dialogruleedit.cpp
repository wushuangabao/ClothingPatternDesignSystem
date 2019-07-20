#include <QStringListModel>
#include <QInputEvent>
#include <QTextCodec>
#include <QDir>
#include "dialogruleedit.h"
#include "ui_dialogruleedit.h"
#include "dialogdefine.h"

DialogRuleEdit::DialogRuleEdit(QWidget *parent, QString d) :
    QDialog(parent),
    ui(new Ui::DialogRuleEdit)
{
    ui->setupUi(this);

    // 参数类型
    strEntityTypes<<"请选择类型"<<"参数"<<"点"<<"直线"<<"曲线";
    ui->comboBoxDefine->insertItems(0,strEntityTypes);
    ui->comboBoxInput->insertItems(0,strEntityTypes);
    ui->comboBoxInput->removeItem(4);

    // 实体名称
    insertEntity("请选择实体");

    if(d != ""){
        dir = d;
        if(dir.contains(".txt", Qt::CaseInsensitive))
            readRuleFile();  // 读取dir文件到codeList
    }else{
        dir = QDir::currentPath() + "/rules";
    }


    // 代码的列表视图
    model = new QStringListModel(codeList);
    ui->listView->setModel(model);
    //ui->listView->setMovement(QListView::Free);  // 设置为可拖动
    currentIndex = codeList.length() - 1;

    connect(model,SIGNAL(dataChanged(const QModelIndex,const QModelIndex)),this,SLOT(changeCode(const QModelIndex,const QModelIndex)));
}

DialogRuleEdit::~DialogRuleEdit()
{
    delete ui;
    delete model;
}

/**
 * @brief 弹出定义实体编辑器对话框
 * @param type 实体类型
 * @param isInput 是否为规则的输入实体
 */
void DialogRuleEdit::newEntity(const QString type, bool isInput)
{
    if(type == strEntityTypes[0])
        return;
    QString title = type + "定义";
    DialogDefine* dlg = new DialogDefine(type,this,isInput);
    dlg->setWindowTitle(title);
    dlg->exec();
    delete dlg;
}

/**
 * @brief 根据codeList生成规则文件
 */
void DialogRuleEdit::writeRuleFile()
{
    if(dir.contains(".txt", Qt::CaseInsensitive)){
        QString fileNameOld = getFileName(dir), fileNameNew = ui->lineEdit->text();
        // case: 与原文件同名，则删除原文件
        if(fileNameOld == fileNameNew)
            QFile::remove(dir);
        // case: 文件名已被修改，则修改dir
        else {
            int position = dir.lastIndexOf(fileNameOld);
            dir.replace(position,fileNameOld.length(),fileNameNew);
        }
    }
    else{
        dir = dir + "/" + ui->lineEdit->text() + ".txt";
        // todo: 检查文件名 ui->lineEdit->text() 的合法性，以及是否重复
    }
    QString s = "";
    for(QStringList::iterator it = codeList.begin(); it != codeList.end(); ++it){
        s = s + *it + "\n";
    }
    QFile file(dir);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.write(s.toUtf8());
    file.close();
}

/**
 * @brief 读取dir路径的规则文件
 */
void DialogRuleEdit::readRuleFile()
{
    QString line;
    QFile file(dir);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        // todo: 提示打开文件失败
        return;
    }
    // 设置codeList
    while (!file.atEnd()) {
        QTextCodec *codec= QTextCodec::codecForName("utf8");
        line = codec->toUnicode(file.readLine());
        codeList << line.simplified();
        // 设置strEntities
        if(isDefinition(line))
            insertEntity(getEntityName(line));
    }
    // 设置lineEdit规则名
    QString fileName = getFileName(dir);
    ui->lineEdit->setText(fileName);
    // 设置comboBoxOutput输出实体
    if(line.contains("输出")){
        QString entityOutput = line.remove("输出").simplified();
        ui->comboBoxOutput->setCurrentText(entityOutput);
    }
    file.close();
}

/**
 * @brief 获取文件名
 * @param d 文件路径
 * @return
 */
QString DialogRuleEdit::getFileName(QString d)
{
    int i = d.lastIndexOf("/");
    d = d.mid(i+1);
    i = d.lastIndexOf(".");
    return d.left(i);
}

/**
 * @brief 在当前位置之后（默认是最后）加一句代码
 * @param code
 */
void DialogRuleEdit::insertCode(const QString &code)
{
    codeList.insert(currentIndex+1,code);
    model->setStringList(codeList);
    currentIndex++;
    ui->listView->setCurrentIndex(model->index(currentIndex));  // 选中刚加入的语句

    // case: 定义了新的实体
    if(code.contains("输入") || isDefinition(code)){
        QString name = getEntityName(code);
        insertEntity(name);
    }
}

/**
 * @brief 删除一句代码
 * @param id
 */
void DialogRuleEdit::deleteCode(int id)
{
    if(id>0 && id<codeList.size()){
        QString code = codeList[id];
        codeList.removeAt(id);
        model->setStringList(codeList);
        currentIndex--;
        ui->listView->setCurrentIndex(model->index(currentIndex));  // 选中前一句

        // case: 删除了实体定义语句
        if(code.contains("输入") || isDefinition(code)){
            QString name = getEntityName(code);
            deleteEntity(name);
        }
    }
}

/**
 * @brief 修改了代码
 * @param topLeft
 * @param bottomRight
 */
void DialogRuleEdit::changeCode(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    int id = topLeft.row();  // 修改代码的行数（理应等于currentIndex）
    QString codeNew = model->data(topLeft).toString();  // 将QModelIndex转换为QString数据类型
    QString codeOld = codeList.at(id);

    if(isDefinition(codeOld)){
        deleteEntity(getEntityName(codeOld));
    }
    if(isDefinition(codeNew)){
        insertEntity(getEntityName(codeNew));
    }
    codeList.removeAt(id);
    codeList.insert(id,codeNew);
}

/**
 * @brief 将新实体name加入strEntities
 * @param name
 */
void DialogRuleEdit::insertEntity(const QString &name)
{
    // 检查是否有重名
    if(strEntities.contains(name)){
        // todo: 重新命名
        return;  // 权宜之计
    }

    int len = strEntities.length();
    strEntities.push_back(name);

    // 更新comboBox
    QStringList sl;
    sl << name;
    ui->comboBoxAssign->insertItems(len,sl);
    ui->comboBoxOutput->insertItems(len,sl);
}

/**
 * @brief 从strEntities中删除实体
 * @param name
 */
void DialogRuleEdit::deleteEntity(const QString &name)
{
    int id = strEntities.indexOf(name);
    strEntities.removeAt(id);
    ui->comboBoxAssign->removeItem(id);
    ui->comboBoxOutput->removeItem(id);
}

/**
 * @brief 定义规则的输入实体
 * @param arg1 实体类型
 */
void DialogRuleEdit::on_comboBoxInput_currentIndexChanged(const QString &arg1)
{
    newEntity(arg1,true);
    ui->comboBoxInput->setCurrentIndex(0);
}

/**
 * @brief 定义新的实体
 * @param arg1 实体类型
 */
void DialogRuleEdit::on_comboBoxDefine_currentIndexChanged(const QString &arg1)
{
    newEntity(arg1,false);
    ui->comboBoxDefine->setCurrentIndex(0);
}

/**
 * @brief 对实体赋值
 * @param arg1 实体类型
 */
void DialogRuleEdit::on_comboBoxAssign_currentIndexChanged(const QString &arg1)
{

}

/**
 * @brief 点击代码列表模型中的某一项
 * @param index
 */
void DialogRuleEdit::on_listView_clicked(const QModelIndex &index)
{
    currentIndex = index.row();
}

/**
 * @brief 点击Confirm按钮
 */
void DialogRuleEdit::on_pushButton_clicked()
{
    // todo: 检查规则名称合法性

    // todo: 检查输入实体合法性

    // todo: 检查规则体的语法正确性

    // 检查是否有输出实体
    int idOutputEntity = ui->comboBoxOutput->currentIndex();
    if(idOutputEntity == 0){
        // todo: 提示未选择输出实体
        return;
    }

    writeRuleFile();
    this->close();
}

/**
 * @brief 键盘输入事件
 * @param key event
 */
void DialogRuleEdit::keyPressEvent(QKeyEvent *ev)
{
    // 回车：
    if(ev->key() == Qt::Key_Return){
        insertCode("");
        ui->listView->edit(model->index(currentIndex));
    }
    // 删除：
    else if(ev->key() == Qt::Key_Delete){
        deleteCode(currentIndex);
    }
}

/**
 * @brief 是否为实体定义语句
 * @param code
 * @return bool
 */
bool DialogRuleEdit::isDefinition(const QString &code)
{
    for(QStringList::iterator it = strEntityTypes.begin()+1; it != strEntityTypes.end(); ++it){
        QString type = *it;
        if(code.contains(type)){
            return true;
        }
    }
    return false;
}

/**
 * @brief 从code中提取实体名称
 * @param code
 * @return ""表示没有找到
 */
QString DialogRuleEdit::getEntityName(const QString &code)
{
    QString name = code;

    // case: 是定义（亦即包含实体类型）
    for(QStringList::iterator it = strEntityTypes.begin()+1; it != strEntityTypes.end(); ++it){
        QString type = *it;
        int idStart = code.indexOf(type);
        if(idStart != -1){
            idStart += type.length();
            int idEnd = code.indexOf("=");
            name = code.mid(idStart);
            if(idEnd != -1)
                name.remove(code.mid(idEnd));
            return name.simplified();
        }
    }

    // case: 是赋值
    int idEnd = code.indexOf("=");
    if(idEnd != -1){
        name.remove(code.mid(idEnd));
        return name.simplified();
    }

    if(code.contains("输出")){
        return name.remove("输出").simplified();
    }

    return "";
}

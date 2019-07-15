#include <QStringListModel>
#include <QInputEvent>
#include "dialogruleedit.h"
#include "ui_dialogruleedit.h"
#include "dialogdefparameter.h"

DialogRuleEdit::DialogRuleEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRuleEdit)
{
    ui->setupUi(this);

    strEntityTypes<<"请选择类型"<<"参数"<<"点"<<"直线段"<<"曲线段";
    ui->comboBoxInput->insertItems(0,strEntityTypes);
    ui->comboBoxDefine->insertItems(0,strEntityTypes);

    strEntities<<"请选择实体"<<"line1"<<"distance";
    ui->comboBoxAssign->insertItems(0,strEntities);
    ui->comboBoxOutput->insertItems(0,strEntities);

    codeList<<"规则 向下平移"<<"输入 直线 line1"<<"输入 参数 distance";
    currentIndex = 2;
    model = new QStringListModel(codeList);
    ui->listView->setModel(model);
    ui->listView->setMovement(QListView::Free);
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
    DialogDefParameter* dlg = new DialogDefParameter(type,this,isInput);
    dlg->setWindowTitle(title);
    dlg->exec();
    delete dlg;
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
    // todo:选中刚加入的语句（找不到API）
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
 * @brief Confirm
 */
void DialogRuleEdit::on_pushButton_clicked()
{
    // 检查规则合法性
}

/**
 * @brief 键盘输入事件
 * @param ev
 */
void DialogRuleEdit::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::EnterKeyType
}


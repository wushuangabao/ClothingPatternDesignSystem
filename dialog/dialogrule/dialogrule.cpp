#include "dialogrule.h"
#include "ui_dialogrule.h"
#include "dialogruleedit.h"
#include "qfilesystemmodel.h"
#include "qtreeview.h"
#include "qmessagebox.h"

DialogRule::DialogRule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRule)
{
    // 创建rules文件夹
    dir = QDir::currentPath() + "/rules";
    QDir* folder = new QDir;
    if(!folder->exists(dir)){
        folder->mkdir(dir);
    }
    delete folder;

    model = new QFileSystemModel;
    model->setRootPath(dir);

    ui->setupUi(this);

    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(dir));
}

DialogRule::~DialogRule()
{
    delete ui;
    delete model;
}

/**
 * @brief 刷新视图
 */
void DialogRule::refresh()
{

}

/**
 * @brief 双击视图中某一项规则，打开规则编辑器
 * @param index 选项索引
 */
void DialogRule::on_treeView_doubleClicked(const QModelIndex &index)
{
    currentDir = model->filePath(index);
}

/**
 * @brief 点视图中某一项规则
 * @param index 选项索引
 */
void DialogRule::on_treeView_clicked(const QModelIndex &index)
{
    currentDir = model->filePath(index);
}

/**
 * @brief 创建新的规则文件
 */
void DialogRule::on_pushButtonAdd_clicked()
{
    DialogRuleEdit* dlg = new DialogRuleEdit;
    dlg->exec();
    delete dlg;
}


/**
 * @brief 删除当前选择的规则文件
 */
void DialogRule::on_pushButtonDelete_clicked()
{
    if(currentDir.contains(".txt", Qt::CaseInsensitive)){
        QString str = currentDir;
        str= "你确定要删除" + str.remove(dir+"/") + "?";
        QMessageBox msgBox(QMessageBox::Warning, tr("删除提示"),str,nullptr,this);
        msgBox.addButton("Yes",QMessageBox::AcceptRole);
        msgBox.addButton("No",QMessageBox::RejectRole);
        if(msgBox.exec()==QMessageBox::AcceptRole){
            QFile::remove(currentDir);
        }
    }
}

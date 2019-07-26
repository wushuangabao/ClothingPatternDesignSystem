#include <QFileSystemModel>
#include <QTreeView>
#include <QMessageBox>
#include <QPainterPath>
#include "dialogrule.h"
#include "ui_dialogrule.h"
#include "dialogruleedit.h"
#include "../../data/mypathdata.h"
#include "../../rules/myrule.h"
#include "../../rules/mypainter.h"
#include "../../mainwindow.h"
#include "../../painterarea.h"

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
 * @param index
 */
void DialogRule::on_treeView_doubleClicked(const QModelIndex &index)
{
    currentDir = model->filePath(index);
    if(currentDir.contains(".txt", Qt::CaseInsensitive)){
        DialogRuleEdit* dlg = new DialogRuleEdit(this,currentDir);
        dlg->exec();
        delete dlg;
    }
}

/**
 * @brief 点视图中某一项规则
 * @param index
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
    QString d = currentDir;
    if(d.contains(".txt", Qt::CaseInsensitive)){
        int i = d.lastIndexOf("/");
        d = d.left(i);
    }
    DialogRuleEdit* dlg = new DialogRuleEdit(this,d);
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

/**
 * @brief 根据当前选择的规则进行绘制
 */
void DialogRule::on_pushButton_clicked()
{
    if(currentDir.contains(".txt",Qt::CaseInsensitive)){
        MyRule rule(currentDir);
        MyPainter mp = rule.drawPath();
        MyPathData* data = new MyPathData(*mp.myData); // todo: 命名data，现在是"未命名_副本"
        MainWindow* mw = static_cast<MainWindow*>(parent());
        mw->painterArea->myPaths << data;
        mw->painterArea->update();
    }
}

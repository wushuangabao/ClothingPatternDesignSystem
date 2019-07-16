#ifndef DIALOGRULE_H
#define DIALOGRULE_H

#include <QDialog>

class QFileSystemModel;
//class QModelIndex;

namespace Ui {
class DialogRule;
}

class DialogRule : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRule(QWidget *parent = nullptr);
    ~DialogRule();

    QString dir; /**< rules文件夹的路径 */

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_treeView_clicked(const QModelIndex &index);
    void on_pushButtonAdd_clicked();
    void on_pushButtonDelete_clicked();
    void on_pushButton_clicked();

private:
    Ui::DialogRule *ui;

    QFileSystemModel* model; /**< 模型 */
    QString currentDir = ""; /**< 当前选择的路径 */

    void refresh();
};

#endif // DIALOGRULE_H

#ifndef DIALOGRULEEDIT_H
#define DIALOGRULEEDIT_H

#include <QDialog>

class QStringListModel;

namespace Ui {
class DialogRuleEdit;
}

class DialogRuleEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRuleEdit(QWidget *parent, QString d = "");
    ~DialogRuleEdit();

    void newEntity(const QString type, bool isInput);
    void writeRuleFile();
    void readRuleFile();
    QString getFileName(QString d);

public slots:
    void insertCode(const QString &code);
    void deleteCode(int id);
    void changeCode(const QModelIndex &topLeft,const QModelIndex &bottomRight);

private slots:
    void on_comboBoxInput_currentIndexChanged(const QString &arg1);
    void on_comboBoxDefine_currentIndexChanged(const QString &arg1);
    void on_comboBoxAssign_currentIndexChanged(const QString &arg1);
    void on_listView_clicked(const QModelIndex &index);
    void on_pushButton_clicked();

private:
    Ui::DialogRuleEdit *ui;

    QString dir; /**< 文件所在目录 */
    QStringListModel* model;    /**< 列表的数据模型 */
    QStringList strEntityTypes; /**< 实体类型的集合 */
    QStringList strEntities;    /**< 所有已定义实体的集合 */
    QStringList codeList;       /**< 规则代码语句的集合 */
    int currentIndex;           /**< 当前选择语句的索引 */

    void keyPressEvent(QKeyEvent *ev);

    bool isDefinition(const QString &code);
    QString getEntityName(const QString &code);

    void insertEntity(const QString &name);
    void deleteEntity(const QString &name);

signals:

};

#endif // DIALOGRULEEDIT_H

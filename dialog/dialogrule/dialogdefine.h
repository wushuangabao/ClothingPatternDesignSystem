#ifndef DIALOGDEFINE_H
#define DIALOGDEFINE_H

#include <QDialog>

namespace Ui {
class DialogDefine;
}

class DialogDefine : public QDialog
{
    Q_OBJECT

public slots:
    void assignValue(const QString &value);

public:
    explicit DialogDefine(QString type, QWidget *parent,bool input = false);
    ~DialogDefine();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_clicked();

private:
    Ui::DialogDefine *ui;

    bool isInput; /**< 是否为输入实体 */
    QString type; /**< 实体类型 */
    QString code; /**< 制板语言代码 */

signals:
    void newCode(const QString &s);
};

#endif // DIALOGDEFINE_H

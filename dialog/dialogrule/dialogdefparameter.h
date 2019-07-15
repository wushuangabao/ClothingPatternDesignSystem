#ifndef DIALOGDEFPARAMETER_H
#define DIALOGDEFPARAMETER_H

#include <QDialog>

namespace Ui {
class DialogDefParameter;
}

class DialogDefParameter : public QDialog
{
    Q_OBJECT

public slots:
    void assignValue(const QString &value);

public:
    explicit DialogDefParameter(QString type, QWidget *parent,bool input = false);
    ~DialogDefParameter();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButton_clicked();

private:
    Ui::DialogDefParameter *ui;

    bool isInput; /**< 是否为输入实体 */
    QString type; /**< 实体类型 */
    QString code; /**< 制板语言代码 */

signals:
    void newCode(const QString &s);
};

#endif // DIALOGDEFPARAMETER_H

#ifndef DIALOGAGGIGNPARAMETER_H
#define DIALOGAGGIGNPARAMETER_H

#include <QDialog>

namespace Ui {
class DialogAggignParameter;
}

class DialogAggignParameter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAggignParameter(QString arg, QWidget *parent);
    ~DialogAggignParameter();

    QString name; /**< 实体名 */

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_radioButton1_clicked();
    void on_radioButton2_clicked();
    void on_radioButton3_clicked();

private:
    Ui::DialogAggignParameter *ui;

signals:
    void initializeWith(const QString value);

};

#endif // DIALOGAGGIGNPARAMETER_H

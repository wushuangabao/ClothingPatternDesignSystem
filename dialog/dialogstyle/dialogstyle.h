#ifndef DIALOGSTYLE_H
#define DIALOGSTYLE_H

#include <QDialog>

namespace Ui {
class DialogStyle;
}

class DialogStyle : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStyle(QWidget *parent);
    ~DialogStyle();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogStyle *ui;

signals:
    void colorChanged(QString color,QString object);
};

#endif // DIALOGSTYLE_H

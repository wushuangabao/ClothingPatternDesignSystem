#ifndef DIALOGMS_H
#define DIALOGMS_H

#include <QDialog>

namespace Ui {
class DialogMS;
}

class MainWindow;

class DialogMS : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMS(QWidget *parent = nullptr);
    ~DialogMS();

private slots:
    void on_radioButton1_0_clicked(bool checked);
    void on_radioButton1_1_clicked(bool checked);
    void on_radioButton2_0_clicked(bool checked);
    void on_radioButton2_1_clicked(bool checked);

private:
    Ui::DialogMS *ui;

signals:
    void typeSangChanged(int frontOrBack,int intCase);
};

#endif // DIALOGMS_H

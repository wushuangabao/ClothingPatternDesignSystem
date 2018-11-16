#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

namespace Ui {
    class MainWindow;
}

class PainterArea;
class DialogMS;
class DialogMM;
class QLabel;


class MainWindow:public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();



public slots:
    void setStatusMouseCoordinate();
    void setStatusScalingMulti();

private slots:
    void on_action_M_S_triggered();
    void on_action_F_S_triggered();
    void on_action_M_M_triggered();

private:
    Ui::MainWindow *ui;
    PainterArea *painterArea;
    QLabel *labelScaling;
    DialogMS *dialogMS;
    DialogMM *dialogMM;

signals:
};

#endif // MAINWINDOW_H

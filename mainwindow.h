#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QDockWidget>
#include <QTableView>
#include <QStandardItemModel>

namespace Ui {
    class MainWindow;
}

class PainterArea;
class DialogMS;
class DialogMM;
class QLabel;
class CurvePoint;

class MainWindow:public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

    void removeAllDock();
    void showDock(const QList<int>& index = QList<int>());

public slots:
    void setStatusMouseCoordinate();
    void setStatusScalingMulti();
    void resetModel();
    void setPointModel(int id,CurvePoint *curvePoint);

private slots:
    void on_action_M_S_triggered();
    void on_action_F_S_triggered();
    void on_action_M_M_triggered();

private:
    Ui::MainWindow *ui;
    PainterArea *painterArea;
    QList<QDockWidget*> m_docks; //记录所有dockWidget的指针
    int numberDocks;
    QLabel *labelScaling;
    QStandardItemModel* modelPoints;
    QStandardItemModel* modelPaths;
    DialogMS *dialogMS;
    DialogMM *dialogMM;

signals:
};

#endif // MAINWINDOW_H

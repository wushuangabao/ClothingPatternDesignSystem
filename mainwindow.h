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
class DialogDesign;

class MainWindow:public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

    PainterArea *painterArea;

    void removeAllDock();
    void showDock(const QList<int>& index = QList<int>());

    void closeEvent(QCloseEvent *event);

public slots:
    void setStatusMouseCoordinate();
    void setStatusScalingMulti();
    void resetModel();
    void setPointModel(int id,CurvePoint *curvePoint);
    void showPath(int id); //在painterArea上显示tablePaths中row=id的path及其上的点
    void showPoints_L(QList<QPointF> points); //在painterArea上用label显示points（直线）
//    void showPoints_C(QList<QPointF> points); //在painterArea上用label显示points（曲线）
//    void showCtrlPoint(QPointF point); //在painterArea上用label显示锚点
    void selectTablePoint(QPointF point,int type=0); //在tablePoints表格中选中point对应的行。type:0所有位置符合的点，1直线上的点，2曲线上的点。

private slots:    
    void on_action_M_S_triggered();
    void on_action_F_S_triggered();
    void on_action_M_M_triggered();

    void on_tablePaths_clicked(const QModelIndex &index);
    void on_tablePaths_activated(const QModelIndex &index);

    void on_action_Design_triggered();

private:
    Ui::MainWindow *ui;
    QList<QDockWidget*> m_docks; //记录所有dockWidget的指针
    int numberDocks;
    QLabel *labelScaling;
    QStandardItemModel* modelPoints;
    QStandardItemModel* modelPaths;
    DialogMS *dialogMS;
    DialogMM *dialogMM;
    DialogDesign *dialogDesign;

signals:
};

#endif // MAINWINDOW_H

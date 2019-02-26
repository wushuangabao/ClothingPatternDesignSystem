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
    void showPath(int id); //在painterArea上显示tablePaths中row=id的path及其上的点
    void showCtrlPoint(CurvePoint *ctrlPoint); //在painterArea上用label显示锚点
    void selectTablePoint(QPointF point);
    void selectTablePoint(int id);

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

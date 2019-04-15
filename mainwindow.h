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

    PainterArea *painterArea; /**< TODO: describe */

    void removeAllDock();
    void showDock(const QList<int>& index = QList<int>());

    void closeEvent(QCloseEvent *event);

public slots:
    void setStatusMouseCoordinate();
    void setStatusScalingMulti();
    void resetModel();
    void showPath(int id);
    void showCtrlPoint(CurvePoint *ctrlPoint);
    void selectTablePoint(QPointF point);
    void selectTablePoint(int id);

private slots:    
    void on_action_M_S_triggered();
    void on_action_F_S_triggered();
    void on_action_M_M_triggered();

    void on_tablePaths_clicked(const QModelIndex &index);

    void on_tablePaths_activated(const QModelIndex &index);

    void on_action_Design_triggered();

    void on_actiontest_curve_triggered();

private:
    Ui::MainWindow *ui; /**< TODO: describe */
    QList<QDockWidget*> m_docks; /**< 记录所有dockWidget的指针 */
    int numberDocks; /**< TODO: describe */
    QLabel *labelScaling; /**< TODO: describe */
    QStandardItemModel* modelPoints; /**< TODO: describe */
    QStandardItemModel* modelPaths; /**< TODO: describe */
    DialogMS *dialogMS; /**< TODO: describe */
    DialogMM *dialogMM; /**< TODO: describe */
    DialogDesign *dialogDesign; /**< TODO: describe */

signals:
};

#endif // MAINWINDOW_H

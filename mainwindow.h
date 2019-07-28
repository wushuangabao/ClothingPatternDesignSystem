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

    void closeEvent(QCloseEvent *event);

public slots:
    void setStatusMouseCoordinate();
    void setStatusScalingMulti();
    void resetModel(int i);
    void showPath(int id);
    void showCtrlPoint(CurvePoint *ctrlPoint);
    void selectTablePoint(QPointF point);
    void selectTablePoint(int id);

private slots:    
    void on_action_M_S_triggered();
    void on_action_F_S_triggered();
    void on_action_M_M_triggered();
    void on_action_F_A_triggered();
    void on_action_F_D_triggered();
    void on_action_Design_triggered();
    void on_actiontest_curve_triggered();
    void on_tablePaths_clicked(const QModelIndex &index);
    void on_action_Rule_triggered();
    void on_action_Style_triggered();
    void on_action_MovePath_triggered();
    void setColor(QString color, QString object);

private:
    Ui::MainWindow *ui;

    QLabel *labelScaling;
    QStandardItemModel* modelPoints;
    QStandardItemModel* modelPaths;

    DialogMS *dialogMS; /**< 修改褶省 */
    DialogMM *dialogMM; /**< 修改尺寸 */
    DialogDesign *dialogDesign; /**< 选择廓形 */

signals:
};

#endif // MAINWINDOW_H

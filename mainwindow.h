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

/**
 * @brief
 *
 */
class MainWindow:public QMainWindow{
    Q_OBJECT

public:
    /**
     * @brief
     *
     */
    explicit MainWindow();
    /**
     * @brief
     *
     */
    ~MainWindow();

    PainterArea *painterArea; /**< TODO: describe */

    /**
     * @brief
     *
     */
    void removeAllDock();
    /**
     * @brief
     *
     * @param index
     */
    void showDock(const QList<int>& index = QList<int>());

    /**
     * @brief
     *
     * @param event
     */
    void closeEvent(QCloseEvent *event);

public slots:
    /**
     * @brief
     *
     */
    void setStatusMouseCoordinate();
    /**
     * @brief
     *
     */
    void setStatusScalingMulti();
    /**
     * @brief
     *
     */
    void resetModel();
    /**
     * @brief
     *
     * @param id
     */
    void showPath(int id); //在painterArea上显示tablePaths中row=id的path及其上的点
    /**
     * @brief
     *
     * @param ctrlPoint
     */
    void showCtrlPoint(CurvePoint *ctrlPoint); //在painterArea上用label显示锚点
    /**
     * @brief
     *
     * @param point
     */
    void selectTablePoint(QPointF point);
    /**
     * @brief
     *
     * @param id
     */
    void selectTablePoint(int id);

private slots:    
    /**
     * @brief
     *
     */
    void on_action_M_S_triggered();
    /**
     * @brief
     *
     */
    void on_action_F_S_triggered();
    /**
     * @brief
     *
     */
    void on_action_M_M_triggered();

    /**
     * @brief
     *
     * @param index
     */
    void on_tablePaths_clicked(const QModelIndex &index);
    /**
     * @brief
     *
     * @param index
     */
    void on_tablePaths_activated(const QModelIndex &index);

    /**
     * @brief
     *
     */
    void on_action_Design_triggered();

private:
    Ui::MainWindow *ui; /**< TODO: describe */
    QList<QDockWidget*> m_docks; //记录所有dockWidget的指针 /**< TODO: describe */
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

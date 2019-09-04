#ifndef DIALOGPANTSH_H
#define DIALOGPANTSH_H

#include <QDialog>
#include <QList>

namespace Ui {
class DialogPantsH;
}

class QGraphicsScene;
class QPushButton;
class QTabWidget;
class QFileInfo;
class QString;

/**
 * @brief
 *
 */
class DialogPantsH : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPantsH(QWidget *parent = nullptr);
    ~DialogPantsH();

private slots:
    void on_pantsLength_clicked();
    void on_waistPostion_clicked();
    void on_waistHead1_clicked();
    void on_waistHead2_clicked();
    void on_pantsLoop_clicked();
    void on_door_clicked();
    void on_sang1_clicked();
    void on_pocket1_clicked();
    void on_sang2_clicked();
    void on_pocket2_clicked();
    void on_pantsFoot_clicked();

    void updateString();

    void pageDown();
    void pageUp();

    void on_buttonNext_clicked();

    void selectPushButton();

private:
    Ui::DialogPantsH *ui;
    QGraphicsScene *scene;
    QList<QPushButton*> pushButtons; /**< 11个部位的按钮 */
    QList<QString> stringList; /**< 11个部位款式的文字描述 */
    QList<QList<QPushButton*>> btnInStackedPages;  /**< 所有 stackedPages 中的按钮 */

    void setLabels(QWidget* w);
    void setStackedPages();
    QString getDir(QString dir);
    QPushButton* createPushButton(QWidget* parent, const QFileInfo &fileInfo);
    void createTabPage(QTabWidget* parent, const QFileInfo &fileInfo);

    void selectPushButton(QList<QPushButton*> buttonList,QString str);
    void changePage(int id);

    void keyPressEvent(QKeyEvent *k);
};

#endif // DIALOGPANTSH_H

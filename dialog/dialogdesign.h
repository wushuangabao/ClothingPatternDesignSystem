#ifndef DIALOGDESIGN_H
#define DIALOGDESIGN_H

#include <QDialog>

namespace Ui {
class DialogDesign;
}

class WidgetKuoXing;
class DialogPantsH;

/**
 * @brief 款式设计
 */
class DialogDesign : public QDialog
{
    Q_OBJECT

public slots:
    void enterStep2(int i);

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit DialogDesign(QWidget *parent = 0);
    /**
     * @brief
     *
     */
    ~DialogDesign();

    int step; /**< TODO: describe */

private slots:
    /**
     * @brief
     *
     */
    void on_pushButton_1_clicked();

private:
    Ui::DialogDesign *ui; /**< TODO: describe */
    WidgetKuoXing *kuoxing; /**< TODO: describe */
    DialogPantsH *pantsH; /**< TODO: describe */

signals:
    /**
     * @brief
     *
     * @param i
     */
    void selectKuoXing(int i);
};

#endif // DIALOGDESIGN_H

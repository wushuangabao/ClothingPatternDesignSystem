#ifndef DIALOGMS_H
#define DIALOGMS_H

#include <QDialog>

namespace Ui {
class DialogMS;
}

class MainWindow;

/**
 * @brief
 *
 */
class DialogMS : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit DialogMS(QWidget *parent = nullptr);
    /**
     * @brief
     *
     */
    ~DialogMS();

private slots:
    /**
     * @brief
     *
     * @param checked
     */
    void on_radioButton1_0_clicked(bool checked);
    /**
     * @brief
     *
     * @param checked
     */
    void on_radioButton1_1_clicked(bool checked);
    /**
     * @brief
     *
     * @param checked
     */
    void on_radioButton2_0_clicked(bool checked);
    /**
     * @brief
     *
     * @param checked
     */
    void on_radioButton2_1_clicked(bool checked);

private:
    Ui::DialogMS *ui; /**< TODO: describe */

signals:
    /**
     * @brief
     *
     * @param frontOrBack
     * @param intCase
     */
    void typeSangChanged(int frontOrBack,int intCase);
};

#endif // DIALOGMS_H

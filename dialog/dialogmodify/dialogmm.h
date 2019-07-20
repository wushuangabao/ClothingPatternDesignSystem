#ifndef DIALOGMM_H
#define DIALOGMM_H

#include <QDialog>

namespace Ui {
class DialogMM;
}

/**
 * @brief
 *
 */
class DialogMM : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit DialogMM(QWidget *parent = nullptr);
    /**
     * @brief
     *
     */
    ~DialogMM();

    int Height=1650; /**< TODO: describe */
    int L=1020; /**< TODO: describe */
    int W=720; /**< TODO: describe */
    int H=940; /**< TODO: describe */
    int Cro=245; /**< TODO: describe */

private slots:
    /**
     * @brief
     *
     * @param arg1
     */
    void on_comboBox_W_currentTextChanged(const QString &arg1);

    /**
     * @brief
     *
     * @param arg1
     */
    void on_comboBox_H_currentTextChanged(const QString &arg1);

private:
    Ui::DialogMM *ui; /**< TODO: describe */
};

#endif // DIALOGMM_H

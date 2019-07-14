#ifndef DIALOGRULEEDIT_H
#define DIALOGRULEEDIT_H

#include <QDialog>

namespace Ui {
class DialogRuleEdit;
}

class DialogRuleEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRuleEdit(QWidget *parent = nullptr);
    ~DialogRuleEdit();

private:
    Ui::DialogRuleEdit *ui;
};

#endif // DIALOGRULEEDIT_H

#ifndef EDITCONFIGFORM_H
#define EDITCONFIGFORM_H

#include "sim/GuiController.h"

#include <QWidget>

namespace Ui {
class EditConfigForm;
}

class EditConfigForm : public QWidget
{
    Q_OBJECT

public:
    explicit EditConfigForm(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~EditConfigForm();

private:
    Ui::EditConfigForm *ui;

    stride::GuiController *guiController;
};

#endif // EDITCONFIGFORM_H

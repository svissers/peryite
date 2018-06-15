#include "editconfigform.h"
#include "ui_editconfigform.h"

using namespace stride;

EditConfigForm::EditConfigForm(GuiController *guiCtrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditConfigForm),
    guiController(guiCtrl)
{
    ui->setupUi(this);
}

EditConfigForm::~EditConfigForm()
{
    delete ui;
}

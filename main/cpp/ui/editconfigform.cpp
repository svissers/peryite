#include "editconfigform.h"
#include "ui_editconfigform.h"

using namespace stride;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

EditConfigForm::EditConfigForm(GuiController *guiCtrl, ptree ptr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditConfigForm),
    guiController(guiCtrl),
    pt(ptr)
{
    ui->setupUi(this);
    createPTreeFields(pt, "");
}

EditConfigForm::~EditConfigForm()
{
    delete ui;
}

void EditConfigForm::createField(QString name, QString value) {
    int y = 48 + lineEdits.length() * 64;

    // -----------------------------------------------------------------------------------------
    // Label
    // -----------------------------------------------------------------------------------------
    QLabel *label = new QLabel();
    label->setParent(ui->configGroupBox);
    label->setText(name);
    label->move(16, y);

    valueNames.append(name);

    // -----------------------------------------------------------------------------------------
    // LineEdit
    // -----------------------------------------------------------------------------------------
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setParent(ui->configGroupBox);
    lineEdit->setText(value);
    lineEdit->setFixedWidth(400 - 64);
    lineEdit->move(16, y + 24);
    lineEdits.append(lineEdit);
}

void EditConfigForm::createPTreeFields(ptree &ptr, QString key) {
    bool hasChildren = false;
    
    for (ptree::iterator it = ptr.begin(); it != ptr.end(); ++it) {
        QString nextKey = QString::fromStdString(it->first);

        if (!key.isEmpty()) {
            nextKey = key + "." + nextKey;
        }

        createPTreeFields(it->second, nextKey);
        hasChildren = true;
    }

    if (!key.isEmpty() && !hasChildren) {
        createField(key, QString::fromStdString(pt.get<std::string>(key.toStdString())));
    }
}
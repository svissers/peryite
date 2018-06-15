#include "editconfigform.h"
#include "ui_editconfigform.h"

#include <QVBoxLayout>
#include <QtDebug>
#include <iostream>

using namespace stride;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

EditConfigForm::EditConfigForm(GuiController *guiCtrl, StrideWindow *sw, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditConfigForm),
    guiController(guiCtrl),
    strideWindow(sw)
{
    ui->setupUi(this);

    pt = strideWindow->getConfigPTree();
    createPTreeFields(*pt, "");

    ui->verticalScrollBar->setMaximum(lineEdits.length() - 8);

    ui->saveButton->setText("Save " + strideWindow->getConfigFileName());
}

EditConfigForm::~EditConfigForm()
{
    delete ui;
}

void EditConfigForm::createField(QString name, QString value) {
    int y = 8 + lineEdits.length() * 64;

    // -----------------------------------------------------------------------------------------
    // Label
    // -----------------------------------------------------------------------------------------
    QLabel *label = new QLabel();
    label->setParent(ui->frame);
    label->setText(name);
    label->move(8, y);

    labels.append(label);

    // -----------------------------------------------------------------------------------------
    // LineEdit
    // -----------------------------------------------------------------------------------------
    QLineEdit *lineEdit = new QLineEdit();
    lineEdit->setParent(ui->frame);
    lineEdit->setText(value);
    lineEdit->setFixedWidth(400 - 64);
    lineEdit->move(8, y + 24);
    lineEdits.append(lineEdit);
}

void EditConfigForm::createPTreeFields(ptree &ptr, QString key) {
    bool hasChildren = false;
    
    for (ptree::iterator it = ptr.begin(); it != ptr.end(); ++it) {
        QString nextKey = QString::fromStdString(it->first);

        if (nextKey == "<xmlcomment>") { continue; }

        if (!key.isEmpty()) {
            nextKey = key + "." + nextKey;
        }

        createPTreeFields(it->second, nextKey);
        hasChildren = true;
    }

    if (!key.isEmpty() && !hasChildren) {
        createField(key, QString::fromStdString(pt->get<std::string>(key.toStdString())));
    }
}

void EditConfigForm::on_verticalScrollBar_valueChanged(int position)
{
    for (int i = 0; i < labels.length(); i++) {
        int y = 8 + (i - position) * 64;

        labels[i]->move(8, y);
        lineEdits[i]->move(8, y + 24);
    }
}

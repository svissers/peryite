#ifndef EDITCONFIGFORM_H
#define EDITCONFIGFORM_H

#include "sim/GuiController.h"

#include <boost/property_tree/xml_parser.hpp>
#include <QWidget>
#include <QString>
#include <QList>
#include <QLabel>
#include <QLineEdit>

namespace Ui {
class EditConfigForm;
}

class EditConfigForm : public QWidget
{
    Q_OBJECT

public:
    explicit EditConfigForm(stride::GuiController *guiCtrl, boost::property_tree::ptree ptr, QWidget *parent = 0);
    ~EditConfigForm();

private:
    Ui::EditConfigForm *ui;
    stride::GuiController *guiController;
    boost::property_tree::ptree pt;

    void createField(QString name, QString value);
    void createPTreeFields(boost::property_tree::ptree &ptr, QString key);

    QList<QLineEdit*> lineEdits;
    QList<QString> valueNames;
};

#endif // EDITCONFIGFORM_H

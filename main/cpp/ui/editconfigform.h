#ifndef EDITCONFIGFORM_H
#define EDITCONFIGFORM_H

#include "sim/GuiController.h"
#include "ui/stridewindow.h"

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
    explicit EditConfigForm(stride::GuiController *guiCtrl, StrideWindow *sw, QWidget *parent = 0);
    ~EditConfigForm();

private slots:
    void on_verticalScrollBar_valueChanged(int position);
    void on_saveButton_clicked();

private:
    Ui::EditConfigForm *ui;
    stride::GuiController *guiController;
    StrideWindow *strideWindow;
    boost::property_tree::ptree *pt;

    void createField(QString name, QString value);
    void createPTreeFields(boost::property_tree::ptree &ptr, QString key);

    QList<QLineEdit*> lineEdits;
    QList<QLabel*> labels;
};

#endif // EDITCONFIGFORM_H

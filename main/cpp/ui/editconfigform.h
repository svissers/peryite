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
    /// Constructor & Destructor
    explicit EditConfigForm(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~EditConfigForm();

private slots:
    /// Qt SLots
    void on_verticalScrollBar_valueChanged(int position);
    void on_saveButton_clicked();

private:
    /// Creates an inputfield and label for a given node
    void createField(QString name, QString value);

    /// Creates all inputfields and labels for the tre
    void createPTreeFields(boost::property_tree::ptree &ptr, QString key);

    Ui::EditConfigForm *ui;                 /// Link to the ui
    stride::GuiController *guiController;   /// Link to the guicontroller
    boost::property_tree::ptree *pt;        /// Link to Ptree that we are editing

    QList<QLineEdit*> lineEdits;            /// List of our inputfields
    QList<QLabel*> labels;                  /// List of the labels
};

#endif // EDITCONFIGFORM_H

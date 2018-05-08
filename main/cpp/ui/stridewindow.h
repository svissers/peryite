#ifndef STRIDEWINDOW_H
#define STRIDEWINDOW_H

#include "sim/GuiController.h"

#include <QWidget>

namespace Ui {
class StrideWindow;
}

class StrideWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StrideWindow(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~StrideWindow();

private slots:
    void on_startButton_clicked();

private:
    Ui::StrideWindow *ui;
    bool running;
    stride::GuiController *guiController;
};

#endif // STRIDEWINDOW_H

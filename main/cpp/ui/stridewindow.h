#ifndef STRIDEWINDOW_H
#define STRIDEWINDOW_H

#include <QWidget>

namespace Ui {
class StrideWindow;
}

class StrideWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StrideWindow(QWidget *parent = 0);
    ~StrideWindow();

private slots:
    void on_startButton_clicked();

private:
    Ui::StrideWindow *ui;
    bool running;
};

#endif // STRIDEWINDOW_H

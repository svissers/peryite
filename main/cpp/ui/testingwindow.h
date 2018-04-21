#ifndef TESTINGWINDOW_H
#define TESTINGWINDOW_H

#include <QWidget>

namespace Ui {
class TestingWindow;
}

class TestingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TestingWindow(QWidget *parent = 0);
    ~TestingWindow();

private slots:
    void on_startButton_clicked();

private:
    Ui::TestingWindow *ui;
    bool running;
};

#endif // TESTINGWINDOW_H

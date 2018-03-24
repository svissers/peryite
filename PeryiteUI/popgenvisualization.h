#ifndef POPGENVISUALIZATION_H
#define POPGENVISUALIZATION_H

#include <QWidget>

namespace Ui {
class PopGenVisualization;
}

class PopGenVisualization : public QWidget
{
    Q_OBJECT

public:
    explicit PopGenVisualization(QWidget *parent = 0);
    ~PopGenVisualization();

    void parseData(QString popgenFilePath);

private:
    static const int maxAge = 99;

    Ui::PopGenVisualization *ui;
    int ageDistribution[maxAge + 1];

    void addAge(int age);
};

#endif // POPGENVISUALIZATION_H

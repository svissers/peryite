#ifndef TESTINGSCATTERGRAPH_H
#define TESTINGSCATTERGRAPH_H

#include<QtCharts/QScatterSeries>
#include <QWidget>

namespace Ui {
class TestingScatterGraph;
}

class TestingScatterGraph : public QWidget
{
    Q_OBJECT

public:
    explicit TestingScatterGraph(QWidget *parent = 0);
    ~TestingScatterGraph();

    void createGraph(QtCharts::QScatterSeries *series);

private:
    Ui::TestingScatterGraph *ui;
};

#endif // TESTINGSCATTERGRAPH_H

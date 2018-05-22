#ifndef STRIDESCATTERGRAPH_H
#define STRIDESCATTERGRAPH_H

#include<QtCharts/QScatterSeries>
#include <QWidget>

namespace Ui {
class StrideScatterGraph;
}

class StrideScatterGraph : public QWidget
{
    Q_OBJECT

public:
    explicit StrideScatterGraph(QWidget *parent = 0);
    ~StrideScatterGraph();

    void createGraph(QtCharts::QScatterSeries *series);

private:
    Ui::StrideScatterGraph *ui;

    void loadIcon();
};

#endif // STRIDESCATTERGRAPH_H

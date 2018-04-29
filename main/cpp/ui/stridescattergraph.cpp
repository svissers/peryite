#include "stridescattergraph.h"
#include "ui_stridescattergraph.h"

#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

using namespace QtCharts;

StrideScatterGraph::StrideScatterGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StrideScatterGraph)
{
    ui->setupUi(this);
}

StrideScatterGraph::~StrideScatterGraph()
{
    delete ui;
}

void StrideScatterGraph::createGraph(QScatterSeries *series) {
    // Create the chart and add our series
    QChart *chart = new QChart();
    chart->setTitle("Stride Test Results");
    chart->addSeries(series);

    // Create the view and add it to our container
    QChartView *chartView = new QChartView(chart, ui->GraphContainer);
    chartView->resize(1280, 720);

    // Styling
    chartView->setRenderHint(QPainter::Antialiasing);
    chart->setDropShadowEnabled(false);
    chart->createDefaultAxes();
}

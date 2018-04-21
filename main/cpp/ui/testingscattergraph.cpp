#include "testingscattergraph.h"
#include "ui_testingscattergraph.h"

#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

using namespace QtCharts;

TestingScatterGraph::TestingScatterGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingScatterGraph)
{
    ui->setupUi(this);
}

TestingScatterGraph::~TestingScatterGraph()
{
    delete ui;
}

void TestingScatterGraph::createGraph(QScatterSeries *series) {
    // Create the chart and add our series
    QChart *chart = new QChart();
    chart->setTitle("Stride Test Results");
    chart->addSeries(series);

    // Create the view and add it to our container
    QChartView *chartView = new QChartView(chart, ui->GraphContainer);
    chartView->resize(900, 500);

    // Styling
    chartView->setRenderHint(QPainter::Antialiasing);
    chart->setDropShadowEnabled(false);
    chart->createDefaultAxes();
}

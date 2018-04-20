#include "visualization.h"
#include "ui_visualization.h"

Visualization::Visualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Visualization)
{
    ui->setupUi(this);
}

Visualization::~Visualization()
{
    delete ui;
}

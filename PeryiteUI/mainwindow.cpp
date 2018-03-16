#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_geoGenFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the GeoGen file."),
                ".",
                "CSV Files (*.csv)");

    if (filename == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a GeoGen file.");
        return;
    }

    ui->geoGenFileLabel->setText(filename);
}

void MainWindow::on_configFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the config file."),
                ".",
                "XML Files (*.xml)");

    if (filename == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    ui->configFileLabel->setText(filename);
}

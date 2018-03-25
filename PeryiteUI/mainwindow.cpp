#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geogenvisualization.h"
#include "popgenvisualization.h"
#include "util.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QProgressDialog>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    data(new PeryiteUIData())
{
    ui->setupUi(this);

    // Background color
    setStyleSheet("QMainWindow { background-color: #f7f7f7; }");
    ui->GeoGenGroup->setStyleSheet("QLabel { background-color: #626a77; color: #ffffff; border-radius: 2px; border: 1px solid black; }");
    ui->PopGenGroup->setStyleSheet("QLabel { background-color: #626a77; color: #ffffff; border-radius: 2px; border: 1px solid black; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Main functions (non-slots)
 */

void MainWindow::setGeoGenFolder(QString filename)
{
    // Check if folder contains required files
    QStringList requiredFiles;
    requiredFiles << "Communities.csv" << "Geogrid.csv" << "Schools.csv" << "Universities.csv" << "Workplaces.csv";
    QStringList missingFiles;

    for(int i = 0; i < requiredFiles.size(); i++) {
        if (!Util::fileExists(filename + "/" + requiredFiles[i])) {
            missingFiles.append(requiredFiles[i]);
        }
    }

    if (!missingFiles.isEmpty()) {
        QString s = missingFiles.join("\n");
        QMessageBox::warning(this, tr("File(s) not found."), "The following files are missing from the selected folder:\n\n" + s);
        return;
    }

    // Now that we're sure the files exist, we can set them.
    ui->Geo_geoGenFileLabel->setText(filename);
    ui->Pop_configFileLabel->setText(filename);
    data->geoGenFolder = filename;
    data->geogenData->setFilenames(data->geoGenFolder);
}

void MainWindow::setConfigFile(QString filename)
{
    ui->Geo_configFileLabel->setText(filename);
    data->configFile = filename;
}

void MainWindow::setPopGenFile(QString filename)
{
    ui->Pop_popGenFileLabel->setText(filename);
    data->popGenFile = filename;
}

/*
 * Slot functions
 */

void MainWindow::on_Geo_geoGenFileSelect_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(
                this,
                tr("Select the GeoGen output folder."),
                "../../cmake-build-release/installed/output",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (filename == "") {
        QMessageBox::warning(this, tr("No folder selected"), "You have not selected a GeoGen output folder.");
        return;
    }

    setGeoGenFolder(filename);
}

void MainWindow::on_Geo_configFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the config file."),
                "../../cmake-build-release/installed/config",
                "XML Files (*.xml)");

    if (filename == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    setConfigFile(filename);
}

void MainWindow::on_Geo_generateGeoGen_clicked()
{
    if (data->configFile == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    // Process name and arguments
    QStringList path = data->configFile.split(QRegularExpression("/"));
    QString configFile = path.last();
    QStringList arguments;
    arguments << "../GeoGen.sh" << configFile;

    // Starting the process and waiting for it to finish
    QProcess *process = new QProcess(this);
    process->start("/bin/bash", arguments);
    process->waitForFinished(); // Default waits max 30s, TODO: see if that's enough.

    // Construct the path of the output folder
    QString installedFolder = "";

    path.pop_front(); // First is an empty string because string starts with "/"

    while (path.length() > 2) {
        QString folder = path.first();
        installedFolder += "/" + folder;
        path.pop_front();
    }

    // Set folder data and update geogendata
    setGeoGenFolder(installedFolder + "/output/" + configFile.split(QRegularExpression("\\.")).first());

    // Message when done
    QMessageBox::information(this, tr("Done"), "GeoGen completed!");
}

void MainWindow::on_Geo_visualizeGeoGen_clicked()
{
    if (data->geoGenFolder == "") {
        QMessageBox::warning(this, tr("No folder selected"), "You have not selected a GeoGen output folder to visualize.");
        return;
    }

    GeoGenVisualization *wdg = new GeoGenVisualization;
    wdg->parseData(data->geogenData);
    wdg->show();
}

void MainWindow::on_Pop_configFileSelect_clicked()
{
    on_Geo_geoGenFileSelect_clicked();
}

void MainWindow::on_Pop_popGenFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the PopGen output file."),
                "../../cmake-build-release/installed/data",
                "CSV Files (*.csv)");

    if (filename == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a PopGen output file.");
        return;
    }

    setPopGenFile(filename);
}

void MainWindow::on_Pop_visualizePopGen_clicked()
{
    if (data->popGenFile == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a PopGen output file to visualize.");
        return;
    }

    // Set button text to show user that we're parsing
    ui->Pop_visualizePopGen->setText("Parsing...");
    // Force repaint because otherwise it would happen after this function, rendering the "parsing..." message useless.
    ui->Pop_visualizePopGen->repaint();

    // Create new window and parse pop file
    PopGenVisualization *wdg = new PopGenVisualization;
    wdg->parseData(data->popGenFile);
    wdg->show();

    // Reset the button text when we're done
    ui->Pop_visualizePopGen->setText("Visualize");
}

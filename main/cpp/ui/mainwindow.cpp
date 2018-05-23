#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geogenvisualization.h"
#include "popgenvisualization.h"
#include "stridewindow.h"
#include "util.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QProgressDialog>
#include <QThread>
#include <QRegularExpression>

using namespace stride;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    data(new PeryiteUIData()),
    guiController(new GuiController())
{
    ui->setupUi(this);

    loadLogo();
    loadIcon();
    setTooltips();

    this->setFixedSize(QSize(1013, 360));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Main functions (non-slots)
 */

void MainWindow::setOutputFolder(QString path)
{
    // Now that we're sure the files exist, we can set them.
    ui->General_outputFolderLabel->setText(path);
    data->outputFolder = path;
}

void MainWindow::setConfigFile(QString path)
{
    ui->General_configFileLabel->setText(path);
    data->configFile = path;
}

void MainWindow::loadLogo()
{
    // Load background image into image
    QString filename = QDir(QCoreApplication::applicationDirPath()).cleanPath("./ui/logo.png");
    QImage *image = new QImage(filename);

    // Setup GraphicsScene
    QGraphicsScene *gfxScene = new QGraphicsScene();
    QGraphicsPixmapItem *gfxItem = new QGraphicsPixmapItem();
    gfxScene->addItem(gfxItem);

    // Load pixmap from image
    QPixmap pixmap = QPixmap::fromImage(*image);

    // Set pixmap pixmap
    gfxItem->setPixmap(pixmap.scaledToWidth(300, Qt::SmoothTransformation));

    // Display the scene
    ui->logo->setScene(gfxScene);
}

void MainWindow::loadIcon()
{
    setWindowIcon(QIcon("./ui/logo.png"));
}

void MainWindow::setTooltips()
{
    ui->General_configFileLabel->setToolTip("The generator file that will be used by geopop.");
    ui->General_configFileSelect->setToolTip("Click to select the file that will be used by geopop.");
    ui->General_outputFolderLabel->setToolTip("The output folder that will be used by geopop.");
    ui->General_outputFolderSelect->setToolTip("Click to select the output folder that will be used by geopop.");
    ui->Geo_generateGeoGen->setToolTip("Run GeoGen.");
    ui->Geo_visualizeGeoGen->setToolTip("Visualize GeoGen (after running).");
    ui->Pop_generatePopGen->setToolTip("Run PopGen.");
    ui->Pop_visualizePopGen->setToolTip("Visualize PopGen (after running).");
    ui->openStrideWindow->setToolTip("Open the Stride Window where you can run the simulator.");
    ui->logo->setToolTip("Hey! Don't block our logo with your mouse :(");
}

/*
 * Slot functions
 */

void MainWindow::on_General_outputFolderSelect_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(
                this,
                tr("Select the output folder."),
                "./output",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (filename == "") {
        QMessageBox::warning(this, tr("No folder selected"), "You have not selected an output folder.");
        return;
    }

    setOutputFolder(filename);
}

void MainWindow::on_General_configFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the config file."),
                "./config",
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
    setOutputFolder(installedFolder + "/output/" + configFile.split(QRegularExpression("\\.")).first());

    // Message when done
    QMessageBox::information(this, tr("Done"), "GeoGen completed!");
}

void MainWindow::on_Geo_visualizeGeoGen_clicked()
{
    // Check if we selected a folder
    if (data->outputFolder == "") {
        QMessageBox::warning(this, tr("No folder selected"), "You have not selected an output folder to visualize.");
        return;
    }

    // Try setting the files in geogendata.
    // This will return false if unsuccesful (if required files are missing), and missing files will be in missingFiles.
    QStringList missingFiles;

    if (!data->setGeoGenData(data->outputFolder, missingFiles)) {
        QString s = missingFiles.join("\n");
        QMessageBox::warning(this, QObject::tr("File(s) not found."), "The following files are missing from the output folder:\n\n" + s);
        return;
    }

    GeoGenVisualization *wdg = new GeoGenVisualization;
    wdg->parseData(data->geogenData);
    wdg->show();
}


void MainWindow::on_Pop_generatePopGen_clicked()
{
    // TODO INSERT POPGEN EXECUTION HERE
    // JUST LIKE on_Geo_generateGeoGen_clicked()
}


void MainWindow::on_Pop_visualizePopGen_clicked()
{
    // Check if we selected a folder
    if (data->outputFolder == "") {
        QMessageBox::warning(this, tr("No folder selected"), "You have not selected an output folder to visualize.");
        return;
    }

    // Try setting the files in geogendata.
    // This will return false if unsuccesful (if required files are missing), and missing files will be in missingFiles.
    QStringList missingFiles;

    if (!data->setPopGenData(data->outputFolder, missingFiles)) {
        QString s = missingFiles.join("\n");
        QMessageBox::warning(this, QObject::tr("File(s) not found."), "The following files are missing from the output folder:\n\n" + s);
        return;
    }

    // Set button text to show user that we're parsing
    ui->Pop_visualizePopGen->setText("Parsing...");
    // Force repaint because otherwise it would happen after this function, rendering the "parsing..." message useless.
    ui->Pop_visualizePopGen->repaint();

    // Create new window and parse pop file
    PopGenVisualization *wdg = new PopGenVisualization;
    wdg->parseData(data->popgenData);
    wdg->show();

    // Reset the button text when we're done
    ui->Pop_visualizePopGen->setText("Visualize");
}

void MainWindow::on_openStrideWindow_clicked()
{
    StrideWindow *wdg = new StrideWindow(guiController);
    wdg->show();
}

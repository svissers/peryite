#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geogenvisualization.h"
#include "popgenvisualization.h"
#include "stridewindow.h"
#include "util.h"
#include "util/FileSys.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QProgressDialog>
#include <QThread>
#include <QRegularExpression>
#include <iostream>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::property_tree;

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
    updateButtons();

    this->setFixedSize(QSize(1013, 328));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Main functions (non-slots)
 */

void MainWindow::setConfigFile(QString path)
{
    ui->General_configFileLabel->setText(path);
    data->configFile = path;

    ptree config_pt = FileSys::ReadPtreeFile(path.toStdString());
    guiController->setupGenDirectory(config_pt);
    updateButtons();
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
    guiController->GeoGen();
    updateButtons();

    // Message when done
    QMessageBox::information(this, tr("Done"), "GeoGen completed!");
    cout << "GeoGen Completed!" << endl;
}

void MainWindow::on_Geo_visualizeGeoGen_clicked()
{
    // Check if we selected a folder    void on_General_outputFolderSelect_clicked();
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
    guiController->PopGen();
    updateButtons();

    // Message when done
    QMessageBox::information(this, tr("Done"), "PopGen completed!");
    cout << "PopGen Completed!" << endl;
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

void MainWindow::updateButtons()
{
    // Disable all
    ui->General_configFileSelect->setEnabled(false);
    ui->Geo_generateGeoGen->setEnabled(false);
    ui->Geo_visualizeGeoGen->setEnabled(false);
    ui->Pop_generatePopGen->setEnabled(false);
    ui->Pop_visualizePopGen->setEnabled(false);
    ui->openStrideWindow->setEnabled(false);

    // Enable depending on gui state
    // Empty
    if (guiController->state >= GuiState::Empty) {
        ui->General_configFileSelect->setEnabled(true);
    }

    // Config file selected
    if (guiController->state >= GuiState::ConfigFileSelected) {
        ui->Geo_generateGeoGen->setEnabled(true);
    }

    // Geo generated
    if (guiController->state >= GuiState::GeoGenerated) {
        ui->Geo_generateGeoGen->setEnabled(false);

        ui->Geo_visualizeGeoGen->setEnabled(true);
        ui->Pop_generatePopGen->setEnabled(true);
    }

    // Pop generated
    if (guiController->state >= GuiState::PopGenerated) {
        ui->Pop_generatePopGen->setEnabled(false);

        ui->Pop_visualizePopGen->setEnabled(true);
        ui->openStrideWindow->setEnabled(true);
    }
}
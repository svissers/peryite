#include "geogenvisualization.h"
#include "ui_geogenvisualization.h"
#include "geogridlocation.h"
#include "util.h"
#include "gen/GeoGrid.h"

#include <QtDebug>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QCloseEvent>
#include <QFile>
#include <QIcon>
#include <QScrollBar>

using namespace stride;
using namespace stride::gen;
using namespace stride::gen::files;

GeoGenVisualization::GeoGenVisualization(GuiController *guiCtrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeoGenVisualization),
    guiController(guiCtrl)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(1000, 600));
    loadIcon();
    
    // -----------------------------------------------------------------------------------------
    // Background color
    // -----------------------------------------------------------------------------------------
    setStyleSheet("QWidget { background-color: #eaeaea; }");

    // -----------------------------------------------------------------------------------------
    // Load background image into image
    // -----------------------------------------------------------------------------------------
    QString filename = QDir(QCoreApplication::applicationDirPath()).cleanPath("./ui/map.png");
    image = new QImage(filename);

    // -----------------------------------------------------------------------------------------
    // Setup GraphicsScene
    // -----------------------------------------------------------------------------------------
    gfxScene = new QGraphicsScene();
    gfxItem = new QGraphicsPixmapItem();
    gfxScene->addItem(gfxItem);

    // -----------------------------------------------------------------------------------------
    // Set timer interval for draw update
    // -----------------------------------------------------------------------------------------
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GeoGenVisualization::update);
    timer->start(1000 / 30); // 30 fps
}

GeoGenVisualization::~GeoGenVisualization()
{
    delete ui;
    delete timer;
    delete image;
    delete gfxScene;
    delete gfxItem;
}

void GeoGenVisualization::update() {
    // -----------------------------------------------------------------------------------------
    // Get cursor position and add scrolling offset to it
    // -----------------------------------------------------------------------------------------
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    QPoint offset = QPoint(ui->FlandersMap->horizontalScrollBar()->value(), ui->FlandersMap->verticalScrollBar()->value());

    // -----------------------------------------------------------------------------------------
    // Check hover selection for circles
    // -----------------------------------------------------------------------------------------
    updateSelection(mousePos + offset);

    // -----------------------------------------------------------------------------------------
    // Draw
    // -----------------------------------------------------------------------------------------
    draw();
}

void GeoGenVisualization::updateSelection(QPointF mousePos) {
    for (int i = 0; i < circles->size(); i++) {
        if (circles->at(i)->containsPoint(mousePos)) {
            hoverCircle(circles->at(i));
            return;
        }
    }

    // Nothing selected
    noHover();
}

void GeoGenVisualization::draw() {
    // -----------------------------------------------------------------------------------------
    // Load pixmap from image
    // -----------------------------------------------------------------------------------------
    QPixmap pixmap = QPixmap::fromImage(*image);

    // -----------------------------------------------------------------------------------------
    // Draw circles on the pixmap
    // -----------------------------------------------------------------------------------------
    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);
        drawCircle(&pixmap, c);
    }

    // -----------------------------------------------------------------------------------------
    // Set gfxitem pixmap
    // -----------------------------------------------------------------------------------------
    gfxItem->setPixmap(pixmap);

    // -----------------------------------------------------------------------------------------
    // Display the scene
    // -----------------------------------------------------------------------------------------
    ui->FlandersMap->setScene(gfxScene);
}

void GeoGenVisualization::addCircle(VisualizationCircle *c) {
    *circles << c;
}

void GeoGenVisualization::drawCircle(QPixmap *pm, VisualizationCircle *circle) {
    // -----------------------------------------------------------------------------------------
    // Get params
    // -----------------------------------------------------------------------------------------
    qreal radius = circle->getRadius();
    QPointF point = circle->position;

    // -----------------------------------------------------------------------------------------
    // QPainter
    // -----------------------------------------------------------------------------------------
    QPainter painter(pm);
    painter.scale(1, 1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(circle->getColor(circle == selected));
    painter.setPen(QColor(0, 0, 0, 0));

    // -----------------------------------------------------------------------------------------
    // Draw
    // -----------------------------------------------------------------------------------------
    painter.drawEllipse(point, radius, radius);
}

void GeoGenVisualization::closeEvent(QCloseEvent *event) {
    timer->stop();
    event->accept();
}

void GeoGenVisualization::hoverCircle(VisualizationCircle *c) {
    selected = c;

    // -----------------------------------------------------------------------------------------
    // Title
    // -----------------------------------------------------------------------------------------
    QString title = c->geoGridLocation->name + " (" + QString::number(c->geoGridLocation->latitude) + ", " + QString::number(c->geoGridLocation->longitude) + ")";
    ui->CircleInfoTitle->setText(title);

    // -----------------------------------------------------------------------------------------
    // Info text
    // -----------------------------------------------------------------------------------------
    QString text = "Population: " + Util::formatInt(c->geoGridLocation->population);
    text += "\nCommunities: " + Util::formatInt(c->geoGridLocation->communities);
    text += "\nSchools: " + Util::formatInt(c->geoGridLocation->schools);
    text += "\nUniversities: " + Util::formatInt(c->geoGridLocation->universities);
    text += "\nWorkplaces: " + Util::formatInt(c->geoGridLocation->workplaces);
    ui->CircleInfoText->setText(text);
}

void GeoGenVisualization::noHover() {
    selected = NULL;
    ui->CircleInfoTitle->setText("");
    ui->CircleInfoText->setText("");
}

void GeoGenVisualization::parseData() {
    // -----------------------------------------------------------------------------------------
    // Get the files to parse
    // -----------------------------------------------------------------------------------------
    auto geo_file = guiController->GetGeoGridFile();
    auto community_file = guiController->GetCommunityFile();
    auto school_file = guiController->GetSchoolFile();
    auto university_file = guiController->GetUniversityFile();
    auto workplace_file = guiController->GetWorkplaceFile();

    int amountOfRegions = guiController->GetAmountOfRegions();

    // -----------------------------------------------------------------------------------------
    // Create list of circles
    // -----------------------------------------------------------------------------------------
    circles = new QList<VisualizationCircle *>();
    selected = NULL;

    // -----------------------------------------------------------------------------------------
    // Parse data
    // -----------------------------------------------------------------------------------------
    parseGeoGrid(geo_file, amountOfRegions);
    parseCommunities(community_file, amountOfRegions);
    parseSchools(school_file, amountOfRegions);
    parseUniversities(university_file, amountOfRegions);
    parseWorkplaces(workplace_file, amountOfRegions);
}

VisualizationCircle* GeoGenVisualization::findCircle(float longitude, float latitude) {
    for (int i = 0; i < circles->length(); i++) {
        float lng = circles->at(i)->geoGridLocation->longitude;
        float lat = circles->at(i)->geoGridLocation->latitude;

        if (lng == longitude && lat == latitude) {
            return circles->at(i);
        }
    }

    qDebug() << "Could not findCircle() with longitude: " << longitude << ", latitude: " << latitude;
    return NULL;
}

void GeoGenVisualization::addCommunity(std::shared_ptr<Community> &community) {
    // -----------------------------------------------------------------------------------------
    // Read data and find corresponding circle
    // -----------------------------------------------------------------------------------------
    float latitude = community->coordinate.get<0>();
    float longitude = community->coordinate.get<1>();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // -----------------------------------------------------------------------------------------
    // Nullcheck
    // -----------------------------------------------------------------------------------------
    if (circle == NULL) {
        qDebug() << "addCommunity did not find circle. Did not add community.";
        return;
    }

    circle->geoGridLocation->communities++;
}

void GeoGenVisualization::addSchool(std::shared_ptr<School> &school) {
    // -----------------------------------------------------------------------------------------
    // Read data and find corresponding circle
    // -----------------------------------------------------------------------------------------
    float latitude = school->coordinate.get<0>();
    float longitude = school->coordinate.get<1>();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // -----------------------------------------------------------------------------------------
    // Nullcheck
    // -----------------------------------------------------------------------------------------
    if (circle == NULL) {
        qDebug() << "addSchool did not find circle. Did not add school.";
        return;
    }

    circle->geoGridLocation->schools++;
}

void GeoGenVisualization::addUniversity(std::shared_ptr<University> &university) {
    // -----------------------------------------------------------------------------------------
    // Read data and find corresponding circle
    // -----------------------------------------------------------------------------------------
    float latitude = university->coordinate.get<0>();
    float longitude = university->coordinate.get<1>();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // -----------------------------------------------------------------------------------------
    // Nullcheck
    // -----------------------------------------------------------------------------------------
    if (circle == NULL) {
        qDebug() << "addUniversity did not find circle. Did not add university.";
        return;
    }

    circle->geoGridLocation->universities++;
}

void GeoGenVisualization::addWorkplace(std::shared_ptr<WorkPlace> &workplace) {
    // -----------------------------------------------------------------------------------------
    // Read data and find corresponding circle
    // -----------------------------------------------------------------------------------------
    float latitude = workplace->coordinate.get<0>();
    float longitude = workplace->coordinate.get<1>();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // -----------------------------------------------------------------------------------------
    // Nullcheck
    // -----------------------------------------------------------------------------------------
    if (circle == NULL) {
        qDebug() << "addWorkplace did not find circle. Did not add workplace.";
        return;
    }

    circle->geoGridLocation->workplaces++;
}

void GeoGenVisualization::parseGeoGrid(std::map<unsigned int,GeoGridFilePtr> &geo_file, int amountOfRegions) {
    // -----------------------------------------------------------------------------------------
    // Use the geo file to create a list of visualization circles.
    // -----------------------------------------------------------------------------------------

    for (int region = 0; region < amountOfRegions; region++) {
        GeoGrid grid = geo_file[region]->ReadGrid();

        for (auto center = 0U; center < grid.size(); center++) {
            std::shared_ptr<UrbanCenter> c = grid[center];

            if (c->is_fragmented) {
                for (auto fragment = 0U; fragment < c->fragmented_populations.size(); fragment++) {
                    GeoGridLocation *loc = new GeoGridLocation(c->fragmented_coords[fragment], c->fragmented_populations[fragment], c->name);
                    addCircle(new VisualizationCircle(loc));
                }
            } else {
                GeoGridLocation *loc = new GeoGridLocation(c->coordinate, c->population, c->name);
                addCircle(new VisualizationCircle(loc));
            }
        }
    }
}

void GeoGenVisualization::parseCommunities(std::map<unsigned int,CommunityFilePtr> &community_file, int amountOfRegions) {
    for (int region = 0; region < amountOfRegions; region++) {
        auto communities = community_file[region]->Read();

        for (auto &band : communities) {
            for (auto &g_struct : band) {
                auto community = std::static_pointer_cast<Community>(g_struct);
                addCommunity(community);
            }
        }
    }
}

void GeoGenVisualization::parseSchools(std::map<unsigned int,SchoolFilePtr> &school_file, int amountOfRegions) {
    for (int region = 0; region < amountOfRegions; region++) {
        auto schools = school_file[region]->Read();

        for (auto &band : schools) {
            for (auto &g_struct : band) {
                auto school = std::static_pointer_cast<School>(g_struct);
                addSchool(school);
            }
        }
    }
}

void GeoGenVisualization::parseUniversities(std::map<unsigned int,UniversityFilePtr> &university_file, int amountOfRegions) {
    for (int region = 0; region < amountOfRegions; region++) {
        auto universities = university_file[region]->Read();

        for (auto &band : universities) {
            for (auto &g_struct : band) {
                auto university = std::static_pointer_cast<University>(g_struct);
                addUniversity(university);
            }
        }
    }
}

void GeoGenVisualization::parseWorkplaces(std::map<unsigned int,WorkplaceFilePtr> &workplace_file, int amountOfRegions) {
    for (int region = 0; region < amountOfRegions; region++) {
        auto workplaces = workplace_file[region]->Read();

        for (auto &band : workplaces) {
            for (auto &g_struct : band) {
                auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
                addWorkplace(workplace);
            }
        }
    }
}


void GeoGenVisualization::loadIcon()
{
    setWindowIcon(QIcon("./ui/logo.png"));
}

void GeoGenVisualization::focusFlanders()
{
    // -----------------------------------------------------------------------------------------
    // Set scrollbars starting value to focus on flanders
    // -----------------------------------------------------------------------------------------
    ui->FlandersMap->horizontalScrollBar()->setSliderPosition(781);
    ui->FlandersMap->verticalScrollBar()->setSliderPosition(809);
}
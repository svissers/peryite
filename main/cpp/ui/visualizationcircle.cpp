#include "visualizationcircle.h"
#include "util.h"
#include <cmath>
#include <QDebug>

using namespace stride;

int VisualizationCircle::minimumRadiusPopulation = 0;
int VisualizationCircle::maximumRadiusPopulation = 2500;
qreal VisualizationCircle::minimumRadius = 3.0f;
qreal VisualizationCircle::maximumRadius = 15.0f;

VisualizationCircle::VisualizationCircle()
{

}

VisualizationCircle::VisualizationCircle(QPointF pos, int r) :
    position(pos)
{
    geoGridLocation = new GeoGridLocation();
    geoGridLocation->population = r;
}

VisualizationCircle::VisualizationCircle(GeoGridLocation *gloc) :
    geoGridLocation(gloc)
{
    position = Util::GCSToQPointF(geoGridLocation->longitude, geoGridLocation->latitude);
}

VisualizationCircle::VisualizationCircle(util::spherical_point coord)
{
    geoGridLocation = new GeoGridLocation(coord);
    position = Util::GCSToQPointF(geoGridLocation->longitude, geoGridLocation->latitude);
}


bool VisualizationCircle::containsPoint(QPointF point, qreal margin) {
    float sqrDistance = sqrDistanceToPoint(point);
    float radius = getRadius() + margin;

    return (sqrDistance <= radius * radius);
}

void VisualizationCircle::increasePop(int amount) {
    geoGridLocation->increasePop(amount);
}

qreal VisualizationCircle::getRadius() {
    float t = ((float)(geoGridLocation->population) - minimumRadiusPopulation) / (maximumRadiusPopulation - minimumRadiusPopulation);
    return Util::lerp(minimumRadius, maximumRadius, std::sqrt(t));
}

qreal VisualizationCircle::sqrDistanceToPoint(QPointF point) {
    return (position.x() - point.x()) * (position.x() - point.x()) + (position.y() - point.y()) * (position.y() - point.y());
}

QColor VisualizationCircle::getColor(bool selected) {
    if (selected) {
        return QColor::fromHsv(206, 173, 196);
    } else {
        // Hue from green to red tints
        float t = (float)geoGridLocation->infected / geoGridLocation->population;
        
        if (geoGridLocation->population == 0) {
            t = 0;
        }
        
        float hue = Util::lerp(94, 0, t);


        return QColor::fromHsv(hue, 117, 224);
    }
}

void VisualizationCircle::resetInfected() {
    geoGridLocation->resetInfected();
}

void VisualizationCircle::increaseInfected(int amount) {
    geoGridLocation->increaseInfected(amount);
}

qreal VisualizationCircle::getInfectedPercent() {
    return geoGridLocation->getInfectedPercent();
}

int VisualizationCircle::getPopulation() {
    return geoGridLocation->population;
}
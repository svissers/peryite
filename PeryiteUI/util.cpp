#include "util.h"
#include <QLocale>
#include <QStringList>
#include <QRegularExpression>

Util::Util()
{

}

QString Util::formatInt(int n) {
    return QLocale(QLocale::English).toString(n);
}

QPointF Util::GCSToQPointF(float longitude, float latitude) {
    // These conversions are specific to our png image, calculated by hand.
    // Not sure if there is a better way to go about this.
    float x = 263 * longitude - 606.7;
    float y = -475 * latitude + 24465;

    return QPointF(x, y);
}

QStringList Util::parseCSVLine(QString csvLine) {
    csvLine.truncate(csvLine.lastIndexOf(QChar('\n')));
    return csvLine.split(QRegularExpression(", "));
}

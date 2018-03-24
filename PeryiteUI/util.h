#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QPointF>
#include <QStringList>

class Util
{
public:
    Util();

    static QString formatInt(int number);
    static QPointF GCSToQPointF(float longitude, float latitude);
    static QStringList parseCSVLine(QString csvLine);
    static bool fileExists(QString path);
};

#endif // UTIL_H

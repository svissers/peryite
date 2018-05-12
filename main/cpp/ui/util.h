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
    static QStringList getMissingFiles(QString path, QStringList requiredFiles);
    static float lerp(float a, float b, float c);
};

#endif // UTIL_H

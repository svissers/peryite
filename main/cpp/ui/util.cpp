#include "util.h"
#include <QLocale>
#include <QStringList>
#include <QRegularExpression>
#include <QFileInfo>


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
    csvLine.truncate(csvLine.lastIndexOf(QChar('\n'))); // Remove endline
    csvLine.replace(" ", ""); // Remove spaces
    return csvLine.split(QRegularExpression(","));
}

bool Util::fileExists(QString path) {
    QFileInfo check_file(path);

    // File exists AND is not a directory
    return check_file.exists() && check_file.isFile();
}

QStringList Util::getMissingFiles(QString path, QStringList requiredFiles) {
    // Check if required files exist
    QStringList missingFiles;

    for(int i = 0; i < requiredFiles.size(); i++) {
        if (!fileExists(path + "/" + requiredFiles[i])) {
            missingFiles.append(requiredFiles[i]);
        }
    }

    return missingFiles;
}

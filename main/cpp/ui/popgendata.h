#ifndef POPGENDATA_H
#define POPGENDATA_H

#include <QStringList>

class PopGenData
{
public:
    PopGenData();

    bool setFilenames(QString path, QStringList &missingFiles);
    QStringList getMissingFiles(QString path, QStringList requiredFiles);

    QString popFile;
};

#endif // POPGENDATA_H

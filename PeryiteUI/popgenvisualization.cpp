#include "popgenvisualization.h"
#include "ui_popgenvisualization.h"
#include "util.h"
#include <QStringList>
#include <QDebug>
#include <QFile>

PopGenVisualization::PopGenVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopGenVisualization)
{
    ui->setupUi(this);
}

PopGenVisualization::~PopGenVisualization()
{
    delete ui;
}


void PopGenVisualization::parseData(QString popgenFilePath) {
    // Make sure all distribution data is set to 0 first
    for (int i = 0; i <= maxAge; i++) {
        ageDistribution[i] = 0;
    }

    QFile popgenFile(popgenFilePath);

    if (!popgenFile.open(QIODevice::ReadOnly)) {
        qDebug() << popgenFile.errorString();
        return;
    }

    while (!popgenFile.atEnd()) {
        QString line = popgenFile.readLine();

        // Ignore header lines
        if (line.startsWith("\"age\"")) { continue; }

        // Get the line
        QStringList list = Util::parseCSVLine(line);

        addAge(list[0].toInt());
    }

    popgenFile.close();
}

void PopGenVisualization::addAge(int age) {
    if (age > maxAge) {
        qDebug() << "age too large: " + age;
        return;
    }

    if (age < 0) {
        qDebug() << "age too small " + age;
        return;
    }

    ageDistribution[age]++;
}

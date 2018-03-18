#include "geogendata.h"

GeoGenData::GeoGenData()
{

}

void GeoGenData::setFilenames(QString folder) {
    communitiesFile = folder + "Communities.csv";
    geogridFile = folder + "Geogrid.csv";
    schoolsFile = folder + "Schools.csv";
    universitiesFile = folder + "Universities.csv";
    workplacesFile = folder + "Workplaces.csv";
}

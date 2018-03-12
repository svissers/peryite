#include "UrbanCenter.h"
#include "geo/GeoCoordinate.h"

struct UrbanCenter
{
    unsigned int id = -1;
    string name = "";
    string province = "";
    GeoCoordinate coordinates = GeoCoordinate();
};

#include "GeoGridFile.h"


namespace stride{
namespace gen {


    std::ostream& operator<<(std::ostream& os, Community& com)
    {
        os << "||center|| ID: " << com.id << " , "
           << com.coordinate << " ,";
        if(com.is_primary)
            os << "primary: True ";
        else
            os << "primary: False ";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, School& school)
    {
        os << "||school|| ID: " << school.id << " , "
           << school.coordinate << " ";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, University& uni)
    {
        os << "||university|| ID: " << uni.id << " , "
           << uni.coordinate <<  " ";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, UrbanCenter& uc)
    {
        os << "||center|| ID: " << uc.id << " , "
           << uc.coordinate << " , Name: " << uc.name << " , Province: "
           << uc.province << " , Pop: " << uc.population << " ";
        return os;
    }


    std::ostream& operator<<(std::ostream& os, WorkPlace& wp)
    {
        os << "||workplace|| ID: " << wp.id << " , "
           << wp.coordinate <<  " ";
        return os;
    }



}
}

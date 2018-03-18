#include "GeoGridFile.h"


namespace stride{
namespace gen {

std::string getHeader(Community& com){
    std::vector<std::string> list;
    list.push_back("id");
    list.push_back("latitude");
    list.push_back("longitude");
    list.push_back("primary");

    return boost::algorithm::join(list, ", ");
}
std::string getHeader(School& school){
    std::vector<std::string> list;
    list.push_back("id");
    list.push_back("latitude");
    list.push_back("longitude");

    return boost::algorithm::join(list, ", ");
}
std::string getHeader(University& uni){
    std::vector<std::string> list;
    list.push_back("id");
    list.push_back("latitude");
    list.push_back("longitude");

    return boost::algorithm::join(list, ", ");
}
std::string getHeader(UrbanCenter& uc){
    std::vector<std::string> list;
    list.push_back("id");
    list.push_back("latitude");
    list.push_back("longitude");
    list.push_back("name");
    list.push_back("province");
    list.push_back("population");

    return boost::algorithm::join(list, ", ");
}
std::string getHeader(WorkPlace& wp){
    std::vector<std::string> list;
    list.push_back("id");
    list.push_back("latitude");
    list.push_back("longitude");

    return boost::algorithm::join(list, ", ");
}


    std::ostream& operator<<(std::ostream& os, Community& com)
    {
        // os << "||center|| ID: " << com.id << " , "
        //    << com.coordinate << " ,";
        // if(com.is_primary)
        //     os << "primary: True ";
        // else
        //     os << "primary: False ";
        // os << "\n";
        std::vector<std::string> list;
        list.push_back(std::to_string(com.id));
        list.push_back(std::to_string(com.coordinate.m_latitude));
        list.push_back(std::to_string(com.coordinate.m_longitude));
        if(com.is_primary)
            list.push_back("true");
        else
            list.push_back("false");

        os << boost::algorithm::join(list, ", ") << "\n";

        return os;
    }

    std::ostream& operator<<(std::ostream& os, School& school)
    {
        // os << "||school|| ID: " << school.id << " , "
        //    << school.coordinate << "\n";
           std::vector<std::string> list;
           list.push_back(std::to_string(school.id));
           list.push_back(std::to_string(school.coordinate.m_latitude));
           list.push_back(std::to_string(school.coordinate.m_longitude));
           os << boost::algorithm::join(list, ", ") << "\n";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, University& uni)
    {
        // os << "||university|| ID: " << uni.id << " , "
        //    << uni.coordinate <<  "\n";
           std::vector<std::string> list;
           list.push_back(std::to_string(uni.id));
           list.push_back(std::to_string(uni.coordinate.m_latitude));
           list.push_back(std::to_string(uni.coordinate.m_longitude));
           os << boost::algorithm::join(list, ", ") << "\n";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, UrbanCenter& uc)
    {
        // os << "||center|| ID: " << uc.id << " , "
        //    << uc.coordinate << " , Name: " << uc.name << " , Province: "
        //    << uc.province << " , Pop: " << uc.population << "\n";
           std::vector<std::string> list;
           list.push_back(std::to_string(uc.id));
           list.push_back(std::to_string(uc.coordinate.m_latitude));
           list.push_back(std::to_string(uc.coordinate.m_longitude));
           list.push_back(uc.name);
           list.push_back(std::to_string(uc.province));
           list.push_back(std::to_string(uc.population));
           os << boost::algorithm::join(list, ", ") << "\n";
        return os;
    }


    std::ostream& operator<<(std::ostream& os, WorkPlace& wp)
    {
        // os << "||workplace|| ID: " << wp.id << " , "
        //    << wp.coordinate <<  "\n";
           std::vector<std::string> list;
           list.push_back(std::to_string(wp.id));
           list.push_back(std::to_string(wp.coordinate.m_latitude));
           list.push_back(std::to_string(wp.coordinate.m_longitude));
           os << boost::algorithm::join(list, ", ") << "\n";
        return os;
    }



}
}

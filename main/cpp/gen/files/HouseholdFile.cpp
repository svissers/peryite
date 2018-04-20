#include "HouseholdFile.h"
#include "util/InstallDirs.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace boost::property_tree;

vector<vector<unsigned int>> getHouseholds(const GenConfiguration& config)
{
    string file_name = config.getTree().get<string>("household_profile");
    ptree tree;
    try {
        // Populate the household tree
        read_xml((util::InstallDirs::GetDataDir() /= file_name).string(), tree,
                 xml_parser::trim_whitespace | xml_parser::no_comments);
    } catch (xml_parser_error& e) {
            throw invalid_argument(string("Invalid file: ") +
                                        (util::InstallDirs::GetDataDir() /= file_name).string());
    }

    // Traverse property tree
    vector<vector<unsigned int>> result;
    BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, tree.get_child("HouseholdProfile" ) ) {
        boost::property_tree::ptree age_tree = node.second;
        vector<unsigned int> household;
        if( node.first == "hh" ) {
            BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, age_tree.get_child("") ) {
                std::string label = v.first;
                if ( label == "age" ) {
                    household.push_back(std::stoul(v.second.data()));
                }
            }
            result.push_back(household);
        }
    }

    return result;
}

} // namespace files
} // namespace gen
} // namespace stride

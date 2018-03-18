#include "GeoConfiguration.h"
#include "util/InstallDirs.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/**
 * @file
 * Implementation for the GeoConfiguration class.
 */

namespace stride {
namespace gen {

using namespace std;
using namespace boost::property_tree;

GeoConfiguration::GeoConfiguration(string config_file_name, unsigned int thread_count)
        : m_thread_count(thread_count)
{
    // Create the configuration property tree
    cout << "Checking " << endl;
    if (util::InstallDirs::GetDataDir().empty()) {
            throw runtime_error(std::string(__func__) + "> Data directory not present! Aborting.");
    }
    cout << "Reading file " << endl;
    try {
            ptree config;
            // Populate the configuration tree
            read_xml((util::InstallDirs::GetConfigDir() /= config_file_name).string(), config,
                     xml_parser::trim_whitespace | xml_parser::no_comments);
            m_config = config.get_child("Config");
    } catch (xml_parser_error& e) {
            throw invalid_argument(string("Invalid file: ") +
                                        (util::InstallDirs::GetConfigDir() /= config_file_name).string());
    }
    cout << "Done xml_parser " << endl;

    checkValidConfig();

    // Initialize the random number generator associated with the configuration
    const auto            rng_type = m_config.get<string>("rng.engine");
    const auto            rng_seed = m_config.get<unsigned long>("rng.seed");
    const util::RNManager::Info info{rng_type, rng_seed, "", m_thread_count};
    cout << "Initializing" << endl;
    m_rn_manager.Initialize(info);
}

boost::property_tree::ptree GeoConfiguration::getTree() const
{
    return m_config;
}

std::shared_ptr<util::RNManager> GeoConfiguration::getRNManager() const
{
    return make_shared<util::RNManager>(m_rn_manager);
}

void GeoConfiguration::checkValidConfig() const
{
    // TODO checking here.
}

} // namespace gen
} // namespace stride

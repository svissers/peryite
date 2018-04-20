#include "GenConfiguration.h"
#include "util/InstallDirs.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

/**
 * @file
 * Implementation for the GenConfiguration class.
 */

namespace stride {
namespace gen {

using namespace std;
using namespace boost::property_tree;

GenConfiguration::GenConfiguration(string config_file_name, unsigned int thread_count)
        : m_thread_count(thread_count), m_path(config_file_name)
{
    // Create the configuration property tree
    if (util::InstallDirs::GetDataDir().empty()) {
            throw runtime_error(std::string(__func__) + "> Data directory not present! Aborting.");
    }
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

    checkValidConfig();
    initRNG();
}

void GeoConfiguration::initRNG(){
    // Initialize the random number generator associated with the configuration
    const auto            rng_type = m_config.get<string>("rng.engine");
    const auto            rng_seed = m_config.get<unsigned long>("rng.seed");
    const util::RNManager::Info info{rng_type, rng_seed, "", m_thread_count};
    m_rn_manager->Initialize(info);
}

boost::property_tree::ptree GenConfiguration::getTree() const
{
    return m_config;
}

std::string GenConfiguration::getPath() const
{
    return m_path;
}

std::shared_ptr<util::RNManager> GenConfiguration::getRNManager() const
{
    return m_rn_manager;
}

void GenConfiguration::checkValidConfig() const
{
    // TODO checking here.
}

} // namespace gen
} // namespace stride

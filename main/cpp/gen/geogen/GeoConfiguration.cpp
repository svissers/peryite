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
        // --------------------------------------
        // Create the configuration property tree
        // --------------------------------------
        // Populate the configuration tree
        if (util::InstallDirs::GetDataDir().empty()) {
                throw runtime_error(std::string(__func__) + "> Data directory not present! Aborting.");
        }
        try {
                ptree config;
                read_xml((util::InstallDirs::GetDataDir() /= config_file_name).string(), config,
                         xml_parser::trim_whitespace | xml_parser::no_comments);
                m_config = config.get_child("Config");
        } catch (xml_parser_error& e) {
                throw invalid_argument(string("Invalid file: ") +
                                            (util::InstallDirs::GetDataDir() /= config_file_name).string());
        }

        // Check the structure of the configuration tree.
        checkValidConfig();

        // ------------------------------------------------------------------------
        // Initialize the random number generator associated with the configuration
        // ------------------------------------------------------------------------
        const auto            rng_type = getRngType();
        const auto            rng_seed = getSeed();
        const util::RNManager::Info info{rng_type, rng_seed, "", m_thread_count};
        m_rn_manager.Initialize(info);
}

string GeoConfiguration::getCitiesFileName() const
{
        return m_config.get<string>("geoprofile.cities");
}

string GeoConfiguration::getcommutingFileName() const
{
        return m_config.get<string>("geoprofile.cities");
}

string GeoConfiguration::getRngType() const
{
        return m_config.get<string>("rng.engine");
}

unsigned long GeoConfiguration::getSeed() const
{
        return m_config.get<unsigned long>("rng.seed");
}

unsigned int GeoConfiguration::getPopulationSize() const { return m_config.get<unsigned int>("population_size"); }

double GeoConfiguration::getUniversityFraction() const
{
        return m_config.get<double>("university.fraction");
}

double GeoConfiguration::getWorkFraction() const { return m_config.get<double>("work.fraction"); }


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

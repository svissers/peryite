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

GenConfiguration::GenConfiguration(boost::property_tree::ptree& config_pt, unsigned int thread_count)
        : m_config(config_pt), m_thread_count(thread_count)
{
    CheckValidConfig();

    // Initialize the random number generator associated with the configuration
    m_rn_manager = make_shared<util::RNManager>();
    const auto            rng_type = m_config.get<string>("rng.engine");
    const auto            rng_seed = m_config.get<unsigned long>("rng.seed");
    const util::RNManager::Info info{rng_type, rng_seed, "", m_thread_count};
    m_rn_manager->Initialize(info);
}


GenConfiguration::GenConfiguration(boost::property_tree::ptree& config_tree, std::shared_ptr<util::RNManager>& rn_manager)
    : m_config(config_tree), m_rn_manager(rn_manager)
{
}

boost::property_tree::ptree GenConfiguration::GetTree() const
{
    return m_config;
}

std::shared_ptr<util::RNManager> GenConfiguration::GetRNManager() const
{
    return m_rn_manager;
}

void GenConfiguration::CheckValidConfig() const
{
    // TODO checking here.
}

} // namespace gen
} // namespace stride

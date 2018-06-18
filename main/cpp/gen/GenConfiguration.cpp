#include "GenConfiguration.h"
#include "util/InstallDirs.h"

#include <boost/property_tree/xml_parser.hpp>

/**
 * @file
 * Implementation for the GenConfiguration class.
 */

namespace stride {
namespace gen {

using namespace std;
using namespace boost::property_tree;

GenConfiguration::GenConfiguration(boost::property_tree::ptree config_pt, unsigned int thread_count, string output_prefix, std::shared_ptr<util::RNManager> rn_manager)
        : m_config(config_pt), m_output_prefix(std::move(output_prefix)), m_num_threads(thread_count), m_rn_manager(std::move(rn_manager))
{
    CheckValidConfig();
}

boost::property_tree::ptree GenConfiguration::GetTree() const
{
    return m_config;
}

string GenConfiguration::GetOutputPrefix() const
{
    return m_output_prefix;
}


unsigned int GenConfiguration::GetNumThreads() const
{
    return m_num_threads;
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


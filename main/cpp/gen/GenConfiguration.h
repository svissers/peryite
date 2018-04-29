#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include "util/CSV.h"
#include "util/RNManager.h"

namespace stride {
namespace gen {

// TODO move to configuration
#define AMOUNTOFBANDS 50

/**
 * The configuration for the generator component.
 *
 **/
class GenConfiguration
{
public:
    /// Constructor. Initializes the GenConfiguration object using the config
    /// file.
    /// @param config_pt       The configuration property tree to use.
    /// @param thread_count    The amount of threads to be used for parallelization.
    /// @param output_prefix   The prefix used for output files.
    GenConfiguration(boost::property_tree::ptree config_pt, unsigned int thread_count, std::string output_prefix);

    /// Constructor. Initializes the GenConfiguration object using the config
    /// file.
    /// @param config_pt       The configuration property tree to use.
    /// @param rn_manager      The random number manager to be used by the geopop module.
    /// @param output_prefix   The prefix used for output files.
    GenConfiguration(boost::property_tree::ptree config_pt, std::shared_ptr<util::RNManager> rn_manager, std::string output_prefix);

    boost::property_tree::ptree GetTree() const;

    std::string GetOutputPrefix() const;

    std::shared_ptr<util::RNManager> GetRNManager() const;
private:
    /// The configuration in a tree structure
    boost::property_tree::ptree m_config;
    /// The prefix for the output files
    std::string m_output_prefix;
    /// Manages the parallel generation of random numbers
    std::shared_ptr<util::RNManager> m_rn_manager;
    /// The amount of threads that will be used.
    unsigned int m_thread_count;

    /// Checks if the configuration property tree is valid.
    void CheckValidConfig() const;
};

} // namespace gen
} // namespace stride

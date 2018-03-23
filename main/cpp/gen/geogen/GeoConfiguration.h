#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include "util/CSV.h"
#include "util/RNManager.h"

namespace stride {
namespace gen {

#define AMOUNTOFBANDS 50

/**
 * The configuration for a geogrid.
 */
class GeoConfiguration
{
public:
        /// Constructor. Initializes the GeoConfiguration object using the config
        /// file.
        /// @param config_file     The configuration xml file to use.
        /// @param thread_count    The amount of threads to be used for parallelization.
        GeoConfiguration(std::string config_file, unsigned int thread_count);

        /// Constructor. Initializes the GeoConfiguration object using a premade ptree (for testing purposes).
        /// @param config_pt     The configuration tree.
        /// @param thread_count    The amount of threads to be used for parallelization.
        GeoConfiguration(const boost::property_tree::ptree& config_pt, unsigned int thread_count);

        boost::property_tree::ptree getTree() const;

        std::shared_ptr<util::RNManager> getRNManager() const;
private:
        /// The configuration in a tree structure
        boost::property_tree::ptree m_config;
        /// Manages the parallel generation of random numbers
        util::RNManager m_rn_manager;
        /// The amount of threads that will be used.
        unsigned int m_thread_count;

        /// Checks if the configuration property tree is valid.
        void checkValidConfig() const;

        // Initialize the random number generator associated with the configuration
        void initRNG();
};

} // namespace gen
} // namespace stride

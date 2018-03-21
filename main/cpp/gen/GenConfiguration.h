#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include "util/CSV.h"
#include "util/RNManager.h"

namespace stride {
namespace gen {

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
        /// @param config_file     The configuration xml file to use.
        /// @param thread_count    The amount of threads to be used for parallelization.
        GenConfiguration(std::string config_file, unsigned int thread_count);

        boost::property_tree::ptree getTree() const;

        std::string getPath() const;

        std::shared_ptr<util::RNManager> getRNManager() const;
private:
        /// The configuration in a tree structure
        boost::property_tree::ptree m_config;
        /// The path leading to the configuration file
        std::string m_path;
        /// Manages the parallel generation of random numbers
        util::RNManager m_rn_manager;
        /// The amount of threads that will be used.
        unsigned int m_thread_count;

        /// Checks if the configuration property tree is valid.
        void checkValidConfig() const;
};

} // namespace gen
} // namespace stride

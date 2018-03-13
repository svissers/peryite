#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include "util/CSV.h"
#include "util/RNManager.h"
#define AMOUNTOFBANDS 50
namespace stride {
namespace gen {

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

        std::shared_ptr<util::RNManager> getRNManager() const;

        std::string getCitiesFileName() const;

        std::string getcommutingFileName() const;

        std::string getRngType() const;

        unsigned long getSeed() const;

        unsigned int getPopulationSize() const;

        unsigned int getUniversityFraction() const;

        unsigned int getWorkFraction() const;

private:
        boost::property_tree::ptree m_config; /// > The content of the xml file
        util::RNManager m_rn_manager;
        unsigned int m_thread_count;

        /// Checks if the configuration property tree is valid.
        void checkValidConfig() const;
};

} // namespace gen
} // namespace stride

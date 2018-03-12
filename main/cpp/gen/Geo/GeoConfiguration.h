#include <boost/property_tree/ptree.hpp>

/**
 * The configuration for a geogrid.
 */
class GeoConfiguration
{
public:
        /// Constructor. Initializes the GeoConfiguration object using the config file.
        /// @param config_file     The configuration xml file to use.
        GeoConfiguration(string config_file);

        boost::property_tree::ptree readCities();

        boost::property_tree::ptree readCommuting();

        string getRngType();

        unsigned long getSeed();

        unsigned int getPopulationSize();

        unsigned int getUniversityFraction();

        unsigned int getWorkFraction();
private:
        boost::property_tree::ptree m_config;   /// > The content of the xml file

        void checkValidXML();
};

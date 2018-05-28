#pragma once

#include "pool/ContactPoolType.h"
#include "pool/IdSubscriptArray.h"
#include "gen/GenConfiguration.h"
#include "util/RNManager.h"
#include <boost/property_tree/ptree.hpp>

namespace stride {
namespace util {

/**
 * @class A geographical region.
 */
struct Region
{
public:
        /// Initialize the region with a name.
        /// @param region_id        The id for the region.
        /// @param region_name      The name for the region.
        /// @param region_config    The geopop configuration for the region.
        Region(unsigned int region_id, std::string region_name, gen::GenConfiguration region_config)
        : id(region_id), name(region_name), config(region_config)
        {
            first_cps = ContactPoolType::IdSubscriptArray<unsigned int>(0);
            last_cps = ContactPoolType::IdSubscriptArray<unsigned int>(0);
        }

        unsigned int id                 = -1;
        std::string name                = "";
        gen::GenConfiguration config;
        unsigned int first_person_id    = 0;
        unsigned int last_person_id     = 0;
        ContactPoolType::IdSubscriptArray<unsigned int> first_cps;
        ContactPoolType::IdSubscriptArray<unsigned int> last_cps;
};


/**
 * @class Collection of Regions.
 */
class Regions : public std::vector<std::shared_ptr<Region>>
{
public:
        using std::vector<std::shared_ptr<Region>>::begin;
        using std::vector<std::shared_ptr<Region>>::end;
        using std::vector<std::shared_ptr<Region>>::size;
public:
        Regions() {}

        /// Initialize the regions using the configuration file.
        /// @param config_pt       The configuration to use.
        /// @param rn_manager      The random number manager to be used by the geopop module.
        Regions(
            const boost::property_tree::ptree& config_pt,
            std::shared_ptr<util::RNManager> rn_manager
        );
};


} // namespace util
} // namespace stride

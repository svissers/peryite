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
        Region(unsigned int region_id, std::string region_name)
        : id(region_id), name(region_name)
        {}

        unsigned int id                 = -1;
        std::string name                = "";
        gen::GenConfiguration config;                               ///< Only used when generating with the geopop component.
        unsigned int first_person_id    = 0;                        ///< The first person id in the population that is in this region.
        unsigned int last_person_id     = 0;                        ///< The last person id in the population that is in this region.
        ContactPoolType::IdSubscriptArray<unsigned int> first_cps{0U};;  ///< The first contactpool id in this region for every type.
        ContactPoolType::IdSubscriptArray<unsigned int> last_cps{0U};;   ///< The last contactpool id in this region for every type.
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
        /// Default constructor.
        /// Used when not utilizing the geopop component.
        Regions() {}

        /// Initialize the regions to be used by the geopop component.
        /// @param config_pt       The configuration to use.
        /// @param rn_manager      The random number manager to be used by the geopop module.
        Regions(
            const boost::property_tree::ptree& config_pt,
            std::shared_ptr<util::RNManager> rn_manager
        );
};

    /*

Config settings:
* percentage van de werkenden die business verplaatsingen doet
* percentage kans dat een werkende (die een business verplaatsing kan doen) op deze dag een business verplaatsing doet
* percentage van gezinnen/personen die toerisme doen
* percentage van toeristen die naar Frankrijk/Duitsland/Nederland/Belgie gaan
     * */

} // namespace util
} // namespace stride

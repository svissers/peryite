#include "TravelAssigner.h"
#include "../../structs/School.h"
#include "../PopGenerator.h"
#include "pool/ContactPoolType.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;
using namespace util;

void AssignTravellers(
        const std::shared_ptr<Population> population,
        std::shared_ptr<util::Regions> regions
)
{
    auto tourist_fraction = regions->at(0)->config.GetTree().get<double>("tourist_fraction");
    auto work_travel_fraction = regions->at(0)->config.GetTree().get<double>("work_travel_fraction");
    auto rn_manager = regions->at(0)->config.GetRNManager();
    auto work_travel_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(1-work_travel_fraction, work_travel_fraction));
    auto tourist_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(1-tourist_fraction, tourist_fraction));
    int previousHHid = -1;
    bool vacationGoers = false;
    unsigned int travel_prim_com_id = 0;
    unsigned int travel_sec_com_id = 0;

    for (size_t i = 0; i < population->size(); ++i) {
        Person person = population->at(i);

        if(previousHHid != person.GetPoolId(ContactPoolType::Id::Household)){
            previousHHid = person.GetPoolId(ContactPoolType::Id::Household);
            vacationGoers = tourist_dist() == 1;
            //assign travel_com_id's            // TODO
            person.setTravelComIds(travel_prim_com_id, travel_sec_com_id);
        }
        else if(vacationGoers){
            person.setTravelComIds(travel_prim_com_id, travel_sec_com_id);
        }
        if(person.GetAge() >= 18 && person.GetAge() < 65) {
            if (work_travel_dist() == 1) {
                //assign secondary workplace
                unsigned int sec_work_id = 0;   //TODO
                person.setTravelWorkId(sec_work_id);
            }
        }

    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride


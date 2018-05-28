#include "Regions.h"

namespace stride {
namespace util {

using namespace std;
using namespace gen;

Regions::Regions(const boost::property_tree::ptree& config_pt, shared_ptr<RNManager> rn_manager)
{
    auto multi_regions = config_pt.get_child_optional("run.amount_regions");
    unsigned int amount_regions = 1;
    if(multi_regions)
        amount_regions = config_pt.get<unsigned int>("run.amount_regions");
    auto prefix         = config_pt.get<string>("run.output_prefix");
    auto num_threads    = config_pt.get<unsigned int>("num_threads");

    for(unsigned int i = 0; i < amount_regions; i++) {
        string pop_config = "run.pop_config";
        pop_config.append(to_string(i+1));
        auto region = make_shared<Region>(
            i,
            "todoName",
            GenConfiguration(
                config_pt.get_child(pop_config),
                num_threads,
                std::move(prefix),
                rn_manager
            )
        );
        this->push_back(region);
    }
}

} // namespace util
} // namespace stride

#include "Regions.h"

namespace stride {
namespace util {

using namespace std;
using namespace gen;

Regions::Regions(const boost::property_tree::ptree& config_pt, shared_ptr<RNManager> rn_manager)
{
    /*
    auto multi_regions = config_pt.get_child_optional("run.amount_regions");
    unsigned int amount_regions = 1;
    if(multi_regions)
        amount_regions = config_pt.get<unsigned int>("run.amount_regions");
    */
    auto prefix         = config_pt.get<string>("run.output_prefix");
    auto num_threads    = config_pt.get<unsigned int>("run.num_threads");

    unsigned int region_id = 0;
    for (auto & region_node : config_pt.get_child("run.pop_config")) {
        if (region_node.first == "region") {
            auto region = make_shared<Region>(
                region_id++,
                region_node.second.get<string>("region_name"),
                GenConfiguration(
                    region_node.second,
                    num_threads,
                    prefix,
                    rn_manager
                )
            );
            this->push_back(region);
        }
    }
}

} // namespace util
} // namespace stride

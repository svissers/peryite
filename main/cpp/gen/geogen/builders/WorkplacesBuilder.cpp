#include "WprkplacesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"
#include "util/CSV.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

static shared_ptr<vector<WorkPlace>> build(GeoConfiguration& config, shared_ptr<GeoGrid> grid);
{
        auto workplaces = make_shared<vector<WorkPlace>>();

        // TODO: implement

        // Commuter data
        util::CSV commuter_data = util::CSV(config.getcommutingFileName());
        for (util::CSVRow& row : commuter_data) {
        }
        // Work fraction (Probably rename to commuteFraction?)
        config.getWorkFraction();


        return workplaces;
}


} // namespace gen
} // namespace stride

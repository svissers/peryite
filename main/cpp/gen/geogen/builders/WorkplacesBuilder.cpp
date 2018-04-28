#include "WorkplacesBuilder.h"

#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"
#include "util/CSV.h"
#include "../../structs/UrbanCenter.h"

#include <vector>
#include <map>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;
using namespace util;

vector<shared_ptr<WorkPlace>> BuildWorkplaces(GenConfiguration& config, GeoGrid& grid)
{
    auto workplaces = vector<shared_ptr<WorkPlace>>();
    auto total_population = config.GetTree().get<unsigned int>("population_size");
    auto work_fraction = config.GetTree().get<double>("work.work_fraction");

    // Calculate the relative active population for each center in the grid
    // Active population = population - commute_away + commute_towards
    util::CSV commuting_data = util::CSV(config.GetTree().get<string>("geoprofile.commuters"));
    size_t column_count = commuting_data.GetColumnCount();
    vector<int> relative_commute (column_count, 0);
    vector<unsigned int> total_commute (column_count, 0);
    if (commuting_data.size() > 1) {
        // Access each element in the matrix
        for (size_t row_index = 0; row_index < commuting_data.size(); row_index++) {
            for (size_t col_index = 0; col_index < column_count; col_index++) {
                util::CSVRow row = *(commuting_data.begin()+row_index);
                // When indexes are the same, commute is to the same center
                total_commute[col_index] += stoi(row.GetValue(col_index));
            }
        }
        for (size_t row_index = 0; row_index < commuting_data.size(); row_index++) {
            for (size_t col_index = 0; col_index < column_count; col_index++) {
                util::CSVRow row = *(commuting_data.begin() + row_index);
                // When indexes are the same, commute is to the same center
                if (row_index == col_index) {
                    // Commute towards
                    relative_commute[row_index] += uint(row.GetValue<double>(col_index)/(double) (total_commute[row_index]) * (grid.at(row_index)->population * (3.0/4.0) * work_fraction));
                } else {
                    // Commute away
                    relative_commute[col_index] -= uint(row.GetValue<double>(col_index)/(double) (total_commute[col_index]) * (grid.at(col_index)->population * (3.0/4.0) * work_fraction));
                    // Commute towards
                    relative_commute[row_index] += uint(row.GetValue<double>(col_index)/(double) (total_commute[row_index]) * (grid.at(row_index)->population * (3.0/4.0) * work_fraction));
                }
            }
        }
    }
    // Calculate the amount of workplaces, every workplace has 20 workers
    auto commute_fraction = config.GetTree().get<double>("work.commute_fraction");
    auto total_active_population = uint(work_fraction*total_population*(3.0/4.0)); // 3/4 being the approximate amount of people in the working age range
    auto total_commuting_actives = (unsigned int) (commute_fraction * total_active_population);
    unsigned int workplace_count =  total_commuting_actives/20;

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(size_t i = 0; i < grid.size(); i++) {
        auto local_workforce = relative_commute[i];
        fractions.push_back(double(local_workforce) / double(total_active_population));
    }
    if (fractions.empty()) {
        return workplaces;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.GetRNManager();
    auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the workplaces to their samples.
    for (unsigned int i = 0; i < workplace_count; i++) {
        auto workplace = make_shared<WorkPlace>(WorkPlace(i, grid.at(generator())->coordinate));
        workplaces.push_back(workplace);
    }

    return workplaces;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride

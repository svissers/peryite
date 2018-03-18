#include "WorkplacesBuilder.h"

#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"
#include "util/CSV.h"
#include "../structs/UrbanCenter.h"

#include <vector>
#include <map>

namespace stride {
namespace gen {

using namespace std;
using namespace util;

shared_ptr<vector<WorkPlace>> WorkplacesBuilder::build(GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
    auto workplaces = make_shared<vector<WorkPlace>>();
    unsigned int total_population = config.getTree().get<unsigned int>("population_size");

    // Calculate the relative active population for each center in the grid
    // Active population = population - commute_away + commute_towards
    util::CSV commuting_data = util::CSV(config.getTree().get<string>("geoprofile.commuters"));
    size_t column_count = commuting_data.getColumnCount();
    vector<unsigned int> relative_commute (column_count, 0);
    vector<unsigned int> total_commute (column_count, 0);
    if (commuting_data.size() > 1) {
        // Access each element in the matrix
        for (size_t row_index = 0; row_index < commuting_data.size(); row_index++) {
            for (size_t col_index = 0; col_index < column_count; col_index++) {
                util::CSVRow row = *(commuting_data.begin()+row_index);
                // When indexes are the same, commute is to the same center
                if (row_index == col_index) {
                    // Commute towards
                    relative_commute[row_index] += stoi(row.getValue(col_index));
                }
                else {
                    // Commute away
                    relative_commute[col_index] -= stoi(row.getValue(col_index));
                    // Commute towards
                    relative_commute[row_index] += stoi(row.getValue(col_index));
                }
                total_commute[col_index] += stoi(row.getValue(col_index));
            }
        }
    }
    // Calculate the amount of workplaces, every workplace has 20 workers
    // TODO: change total_population to total_active_population
    double commute_fraction = config.getTree().get<double>("work.commute_fraction");
    unsigned int workforce = (unsigned int) (commute_fraction * total_population);
    unsigned int workplace_count =  workforce/20;

    // Create the discrete distribution to sample from.
    // TODO: change total_population to total_active_population
    vector<double> fractions;
    for(size_t i = 0; i < grid->size(); i++) {
        int local_workforce = (relative_commute[i] / total_commute[i]) * (grid->at(i).population);
        fractions.push_back(double(local_workforce) / double(total_population));
    }
    if (fractions.empty()) {
        return workplaces;
    }

    // The generator allows for parallelization.
    auto rn_manager = config.getRNManager();
    auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the workplaces to their samples.
    for (unsigned int i = 0; i < workplace_count; i++) {
            workplaces->push_back(WorkPlace(i, grid->at(generator()).coordinate));
    }

    return workplaces;
}


} // namespace gen
} // namespace stride
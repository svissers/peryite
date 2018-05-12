#include "WorkplacesBuilder.h"

#include "trng/fast_discrete_dist.hpp"


namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;
using namespace util;

vector<shared_ptr<WorkPlace>> BuildWorkplaces(GenConfiguration& config, GeoGrid& grid, std::shared_ptr<Population> pop)
{
    auto workplaces         = vector<shared_ptr<WorkPlace>>();
    //auto total_population   = config.GetTree().get<unsigned int>("population_size");
    auto work_fraction      = config.GetTree().get<double>("work.work_fraction");

    // Calculate the relative active population for each center in the grid
    // Active population = population - commute_away + commute_towards
    util::CSV commuting_data    = util::CSV(config.GetTree().get<string>("geoprofile.commuters"));
    size_t column_count         = commuting_data.GetColumnCount();
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
                    // the amount of people going to the urban center is based on the relative number in the csv multiplied by the amount of working people in the urban center
                    // (grid.at(row_index)->population * (3.0/4.0) * work_fraction) is an (over)estimation of the amount of working age people in a certain urban center
                    // we could iterate over the population to find exact amounts, but this would very much not be scalable.
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
    unsigned int working_age_people = 0;
    for(size_t i = 0; i < pop->size(); i++){
        if(pop->at(i).GetAge() >=18 && pop->at(i).GetAge() <= 65)
            working_age_people++;
    }
    // Calculate the amount of workplaces, every workplace has 20 workers
    auto commute_fraction        = config.GetTree().get<double>("work.commute_fraction");
    auto total_active_population = uint(work_fraction*working_age_people);
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
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

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

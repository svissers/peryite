#include "WorkplacesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"
#include "util/CSV.h"
#include <vector>

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

shared_ptr<vector<WorkPlace>> WorkplacesBuilder::build(GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto workplaces = make_shared<vector<WorkPlace>>();
    std::vector<unsigned int> relativeCommute;
    std::vector<unsigned int> totalCommute;
        // Commuter data
    int counter = -1;
    util::CSV Commuting_data = util::CSV(config.getcommutingFileName());
    unsigned int column_count = Commuting_data.getColumnCount();
    for (util::CSVRow& row : Commuting_data) {
        if(counter != -1){
            for(unsigned int i = 0; i < column_count; i++) {
                if(i==counter){
                    relativeCommute[i]+= stoi(row.getValue(i));
                }
                else{
                    relativeCommute[i] -= stoi(row.getValue(i));
                    relativeCommute[counter] += stoi(row.getValue(i));
                }
                totalCommute[i]+= stoi(row.getValue(i));

            }
        }else{
            for(unsigned int i; i < column_count; i++){
                relativeCommute.push_back(0);
                totalCommute.push_back(0);
            }
        }
        counter++;
    }


        // Work fraction (Probably rename to commuteFraction?)
        unsigned int workforce = (unsigned int) config.getWorkFraction()*grid->getTotalPopulation();

        //every workplace has 20 workers
        unsigned int work_count =  workforce/20;
        // Create the discrete distribution to sample from.
        vector<double> fractions;

        counter = 0;

        for(UrbanCenter center : *grid) {
            int local_workforce = (relativeCommute[counter]/totalCommute[counter])*center.population;
                fractions.push_back(double(local_workforce) / double(grid->getTotalPopulation()));
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the workplaces to their samples.
        for (unsigned int i = 0; i < work_count; i++) {
                workplaces->push_back(WorkPlace(i, grid->at(generator()).coordinate));
        }

        return workplaces;
}


} // namespace gen
} // namespace stride

#include "CommunitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

shared_ptr<vector<Community>> CommunitiesBuilder::build(const GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        communities = make_shared<vector<Community>>();
        // Every community has an average of 2000 members.
        unsigned int community_count = grid->getTotalPopulation() / 2000;

        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : *grid) {
                fractions.push_back(double(center.population) / double(grid->getTotalPopulation()));
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the communities to their samples.
        for (unsigned int i = 0; i < community_count; i++) {
                // TODO: Currently only primary communities.
                communities->push_back(Community(i, true, grid->at(generator()).coordinate));
        }

        return communities;
}

    void CommunitiesBuilder::write(std::string CommunitiesFile) {
            std::vector<std::vector<Community> > sortedCommunities;
            for(auto it = communities->begin(); it < communities->end();it++){
                    for(unsigned int i=0; i < AMOUNTOFBANDS; i++){
                            if(it->coordinate.m_longitude<minLong+((i+1)*LongitudeBandWidth)){
                                    for(unsigned int j=0; j<sortedCommunities[i].size(); j++){
                                            if(sortedCommunities[i][j].coordinate.m_latitude> it->coordinate.m_latitude){
                                                    j--;
                                                    sortedCommunities[i].insert(sortedCommunities[i].begin()+j, *it);
                                                    break;
                                            }
                                    }
                                    break;
                            }
                    }
            }
            ofstream my_file{CommunitiesFile};
            if(my_file.is_open()){
                    for(unsigned int i = 0; i < sortedCommunities.size(); i++){
                            for (unsigned int j = 0; j < sortedCommunities[i].size(); j++){
                                    my_file<< "||community|| ID: " << sortedCommunities[i][j].id << " , "
                                           << sortedCommunities[i][j].coordinate << " , Primary: ";
                                    if( sortedCommunities[i][j].is_primary)
                                        my_file<< "true ";
                                    else
                                            my_file<<"false ";
                            }
                            my_file<< std::endl;
                    }
            }

    }


} // namespace gen
} // namespace stride

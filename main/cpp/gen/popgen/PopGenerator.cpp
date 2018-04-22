#include "PopGenerator.h"
#include "../files/HouseholdFile.h"
#include "assigners/householdAssigner.h"
#include "assigners/schoolAssigner.h"
#include "assigners/universityAssigner.h"
#include "assigners/workplaceAssigner.h"
#include "assigners/communityAssigner.h"
#include "util/GeoCoordCalculator.h"
#include "trng/fast_discrete_dist.hpp"
#include <map>


namespace stride {
namespace gen {
namespace popgen {

using namespace std;
using namespace gen;

void Generate(files::GenDirectory& dir, unsigned int thread_count)
{
    std::cout << "Popgen" << std::endl;
    auto config = dir.getConfig();
    // Build households
    std::cout << "buildHouseholds" << std::endl;
    auto households = assigner::BuildHouseholds(config);
    // Assign persons
    std::cout << "Assign persons to households" << std::endl;
    auto geogrid_file   = dir.getGeoGridFile();
    auto grid           = geogrid_file->readGrid();
    assigner::AssignHouseholds(households, grid, config);

    auto school_file    = dir.getSchoolFile();
    auto schools        = school_file->read();
    std::cout << "Assigning schools" << std::endl;
    assigner::AssignSchools(schools, households, config, grid);

    auto university_file = dir.getUniversityFile();
    auto universities    = university_file->read();
    std::cout << "Assigning universities" << std::endl;
    unsigned int total_commuting_students = assigner::AssignUniversities(universities, households, config, grid);

    auto workplace_file = dir.getWorkplaceFile();
    auto workplaces     = workplace_file->read();
    std::cout << "Assigning workplaces" << std::endl;
    assigner::AssignWorkplaces(workplaces, households, config, grid, total_commuting_students);

    auto community_file = dir.getCommunityFile();
    auto communities    = community_file->read();
    std::cout << "Assigning communities" << std::endl;
    assigner::AssignCommunities(communities, households, config, grid);
    // Write persons
    writePopulation(households, config);
}

void writePopulation(vector<shared_ptr<Household>> households, const GenConfiguration& config)
{
    string config_path = config.getPath();
    boost::filesystem::path out_dir = "output/"+config_path.substr(0, config_path.find_last_of("."));
    string file_path = out_dir.string()+"/pop.csv";
    std::ofstream my_file{file_path};
    if(my_file.is_open()) {
        vector<string> labels = {"age", "household_id", "school_id", "work_id", "primary_community", "secondary_community", "risk_averseness"};
        my_file << boost::algorithm::join(labels,",") << "\n";
        for (const auto& household : households) {
            for (const auto& person : household->persons) {
                vector<string> values = {
                    to_string(person->GetAge()),
                    to_string(person->GetPoolId(ContactPoolType::Id::Household)),
                    to_string(person->GetPoolId(ContactPoolType::Id::School)),
                    to_string(person->GetPoolId(ContactPoolType::Id::Work)),
                    to_string(person->GetPoolId(ContactPoolType::Id::PrimaryCommunity)),
                    to_string(person->GetPoolId(ContactPoolType::Id::SecondaryCommunity)),
                    to_string(0),
                };
                my_file << boost::algorithm::join(values,",") << "\n";
            }
        }
        my_file.close();
    }
}

vector<shared_ptr<GenStruct>> getClosestStructs(const util::GeoCoordinate& home_coord, const vector<vector<shared_ptr<GenStruct>>>& structs, const GeoGrid& grid)
{
    vector<shared_ptr<GenStruct>> closest_structs;
    const util::GeoCoordCalculator& calculator = util::GeoCoordCalculator::getInstance();
    // The amount of bands doubles every iteration
    unsigned int band_range = 2;
    // The default search range is 10 km
    unsigned int search_range = 10;
    auto band_of_hh = uint( (home_coord.m_longitude - grid.m_min_long) / grid.m_longitude_band_width );
    // Keep doubling search space until a struct is found
    while(closest_structs.empty()){
        // The first and last band define the search space
        unsigned int firstband = 0;
        unsigned int lastband = band_of_hh + band_range;
        if (band_of_hh > band_range)
            firstband = band_of_hh - band_range;
        if (lastband >= structs.size())
            lastband = structs.size() - 1;
        // Go over the search space
        for (unsigned int index = firstband; index <= lastband; index++) {
            for (const auto& gstruct : structs[index]) {
                if (calculator.getDistance(gstruct->coordinate, home_coord) <= search_range)
                    closest_structs.push_back(gstruct);
            }
        }
        search_range = search_range*2;
        band_range = band_range*2;
    }
    return closest_structs;
}

} // namespace popgen
} // namespace gen
} // namespace stride

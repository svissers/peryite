#include "PopGenerator.h"
#include "../files/HouseholdFile.h"
#include "util/GeoCoordCalculator.h"
#include "trng/fast_discrete_dist.hpp"
#include <map>


namespace stride {
namespace gen {
namespace popgen {

using namespace std;
using namespace gen;

void generate(files::GenDirectory& dir, unsigned int thread_count)
{
    std::cout << "Popgen" << std::endl;
    auto config = dir.getConfig();
    // Build households
    std::cout << "buildHouseholds" << std::endl;
    auto households = buildHouseholds(config);
    // Assign persons
    std::cout << "Assign persons to households" << std::endl;
    auto geogrid_file   = dir.getGeoGridFile();
    auto grid           = geogrid_file->readGrid();
    assignHouseholds(households, grid, config);

    auto school_file    = dir.getSchoolFile();
    auto schools        = school_file->read();
    std::cout << "Assigning schools" << std::endl;
    assignSchools(schools, households, config, grid);

    auto university_file = dir.getUniversityFile();
    auto universities    = university_file->read();
    std::cout << "Assigning universities" << std::endl;
    unsigned int total_commuting_students = assignUniversities(universities, households, config, grid);

    auto workplace_file = dir.getWorkplaceFile();
    auto workplaces     = workplace_file->read();
    std::cout << "Assigning workplaces" << std::endl;
    assignWorkplaces(workplaces, households, config, grid, total_commuting_students);

    auto community_file = dir.getCommunityFile();
    auto communities    = community_file->read();
    std::cout << "Assigning communities" << std::endl;
    assignCommunities(communities, households, config, grid);
    // Write persons
    writePopulation(households, config);
}

vector<shared_ptr<Household>> buildHouseholds(const GenConfiguration& config)
{
    auto hh_reference           = files::getHouseholds(config);
    unsigned int current_hh_id  = 0;
    unsigned int current_p_id   = 0;
    auto pop_size       = config.getTree().get<unsigned int>("population_size");

    // Create a uniform distribution for the household reference set.
    auto rn_manager = config.getRNManager();
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(hh_reference.size()));

    // Build the households
    vector<shared_ptr<Household>> result;
    while (current_p_id < pop_size) {
            // Select a household from the reference set
            int index           = generator();
            auto household_ref  = hh_reference.at(index);

            // Build the selected household
            auto household = make_shared<Household>(current_hh_id);
            vector<shared_ptr<Person>> hh_persons;
            for (unsigned int age : household_ref) {
                auto person = make_shared<Person>(
                    current_p_id, age, current_hh_id,
                    0, 0, 0, 0
                );
                hh_persons.push_back(person);
                current_p_id++;
            }
            household->persons = hh_persons;
            result.push_back(household);
            current_hh_id++;
    }
    return result;
}

void assignHouseholds (
    vector<shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config)
{
    auto total_population = config.getTree().get<unsigned int>("population_size");

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(const auto& center : grid) {
        fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty()) {
        return;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.getRNManager();
    std::function<int()> generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Map the households to their samples.
    for (const auto& household : households) {
        auto center = grid.at(generator());
        auto coords = center->coordinate;
        if (center->is_fragmented) {
            // Select one of the fragments
            vector<double> f_fractions;
            for(const auto& population : center->fragmented_populations)
                f_fractions.push_back(double(population) / double(center->population));
            auto frag_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(f_fractions.begin(), f_fractions.end()));
            coords = center->fragmented_coords.at(frag_gen());
        }
        household->coordinate = coords;
    }
}


void assignSchools(
    vector<vector<shared_ptr<GenStruct>>>& schools, const vector<shared_ptr<Household>>& households,
    const GenConfiguration& config, const GeoGrid& grid)
{
    const unsigned int school_size      = 500;
    const unsigned int school_cp_size   = 20;
    // Create the contactpools for every school
    unsigned int cp_id = 0;
    for (const auto& band : schools) {
        for (const auto& g_struct : band) {
            auto school = std::static_pointer_cast<School>(g_struct);
            for(unsigned int size = 0; size < school_size; size += school_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::Household);
                school->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    // Assign young students to schools
    for (const auto& household : households) {
        for (const auto& person : household->persons) {
            auto age = person->GetAge();
            if (age >= 3 && age < 18) {
                auto home_coord = household->coordinate;
                // Find the closest schools
                std::vector<shared_ptr<GenStruct>> closest_schools = getClosestStructs(home_coord, schools, grid);
                if (closest_schools.empty()) {
                    std::cout << "closest_schools is empty: " << age << std::endl;
                    continue;
                }
                // Create a uniform distribution to select a school
                auto rn_manager         = config.getRNManager();
                auto school_generator   = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_schools.size()));
                auto school = static_pointer_cast<School>(closest_schools.at(school_generator()));
                // Create a uniform distribution to select a contactpool in the selected school
                auto cp_generator   = rn_manager->GetGenerator(trng::fast_discrete_dist(school->pools.size()));
                auto pool           = school->pools.at(cp_generator());
                person->setPoolId(ContactPoolType::Id::School, pool->GetId());
                pool->AddMember(person.get());
            }
        }
    }
}

unsigned int assignUniversities(
    vector<vector<shared_ptr<GenStruct>>>& universities, const vector<shared_ptr<Household>>& households,
    const GenConfiguration& config, const GeoGrid& grid)
{
    // -------------
    // Contactpools
    // -------------
    const unsigned int university_size      = 3000;
    const unsigned int university_cp_size   = 20;
    unsigned int cp_id                      = 0;
    map<unsigned int, vector<shared_ptr<University>>> cities;
    for (auto& band : universities) {
        for (auto& g_struct : band) {
            auto university = std::static_pointer_cast<University>(g_struct);
            // Create the contactpools for every university
            for(unsigned int size = 0; size < university_size; size += university_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::School);
                university->pools.push_back(pool);
                cp_id++;
            }
            // Get the different university locations (urban_id = row_index in commuting data).
            if (cities.find(university->urban_id) == cities.end() ) {
                cities[university->urban_id] = vector<shared_ptr<University>>({university});
            } else {
                cities[university->urban_id].push_back(university);
            }
        }
    }

    // -------------
    // Distributions
    // -------------
    auto rn_manager = config.getRNManager();

    // Create two distributions, one to select if the person is a student and one to select if the student commutes.
    auto student_fraction       = config.getTree().get<double>("university.student_fraction");
    auto commute_fraction       = config.getTree().get<double>("university.commute_fraction");
    auto student_fractions      = vector<double>{student_fraction, 1.0 - student_fraction};
    auto commute_fractions      = vector<double>{commute_fraction, 1.0 - commute_fraction};
    auto student_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(student_fractions.begin(), student_fractions.end()));
    auto commute_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(commute_fractions.begin(), commute_fractions.end()));

    // create a uniform distribution to select a contactpool from a university
    auto cp_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(floor(university_size / university_cp_size)));

    // Commuting distributions
    util::CSV commuting_data(config.getTree().get<string>("geoprofile.commuters"));
    map<unsigned int, std::function<int()>> city_generators;
    vector<unsigned int>                    commute_towards;
    unsigned int                            commute_towards_total = 0;
    if (commuting_data.size() > 1) {
        for (auto const& city : cities) {
            // For every university city, calculate the fraction commuting towards it.
            auto row = *(commuting_data.begin() + city.first);
            unsigned int city_total = 0;
            for (unsigned int col_index = 0; col_index < commuting_data.GetColumnCount(); col_index++) {
                // Ignore commuting towards itself
                if (city.first == col_index)
                    continue;
                auto commuting_towards      = row.GetValue<unsigned int>(col_index);
                // Count the total number of people commuting towards this university city
                city_total                  += commuting_towards;
                // Count the total number of people commuting towards all university cities
                commute_towards_total       += commuting_towards;
            }
            commute_towards.push_back(city_total);
            // For every university city, create a uniform distribution to select a university
            auto uni_gen                    = rn_manager->GetGenerator(trng::fast_discrete_dist(city.second.size()));
            city_generators[city.first]     = uni_gen;
        }
    }

    // Create a distribution to select a university city when the student commutes.
    vector<double> city_fractions;
    for (auto const& commute_towards_city : commute_towards)
        city_fractions.push_back(commute_towards_city / commute_towards_total);
    auto city_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(city_fractions.begin(), city_fractions.end()));

    // --------------------------------
    // Assign students to universities.
    // --------------------------------
    unsigned int total_commuting_students = 0;
    for (const auto& household : households) {
        for (const auto& person : household->persons) {
            auto age = person->GetAge();
            if (age >= 18 && age < 26 && student_gen() == 0) {
                shared_ptr<ContactPool> pool;
                if (commute_gen() == 0) {
                    /// Commuting student
                    total_commuting_students++;
                    unsigned int city_index = city_gen();
                    auto it = cities.begin();
                    std::advance(it, city_index);
                    auto city       = *it;
                    auto university = city.second[city_generators[city.first]()];
                    pool            = university->pools[cp_gen()];
                } else {
                    /// Non-commuting student
                    auto home_coord = household->coordinate;
                    // Find the closest schools
                    std::vector<shared_ptr<GenStruct>> closest_universities = getClosestStructs(home_coord, universities, grid);
                    if (closest_universities.empty()) {
                        std::cout << "closest_universities is empty: " << age << std::endl;
                        continue;
                    }
                    // Create a uniform distribution to select a university
                    auto rn_manager = config.getRNManager();
                    auto uni_gen    = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_universities.size()));
                    auto university = static_pointer_cast<University>(closest_universities[uni_gen()]);
                    // Create a uniform distribution to select a contactpool in the selected university
                    auto cp_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(university->pools.size()));
                    pool = university->pools.at(cp_generator());
                }
                person->setPoolId(ContactPoolType::Id::School, pool->GetId());
                pool->AddMember(person.get());
            }
        }
    }
    return total_commuting_students;
}

void assignWorkplaces(
    vector<vector<shared_ptr<GenStruct>>>& workplaces, const vector<shared_ptr<Household>>& households,
    const GenConfiguration& config, const GeoGrid& grid, unsigned int total_commuting_students)
{
    // -------------
    // Contactpools
    // -------------
    unsigned int cp_id = 0;
    for (auto& band : workplaces) {
        for (auto& g_struct : band) {
            auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
            auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::Work);
            workplace->pool = pool;
            cp_id++;
        }
    }
    // -------------
    // Distributions
    // -------------
    auto rn_manager = config.getRNManager();

    auto total_population = config.getTree().get<unsigned int>("population_size");
    auto student_fraction = config.getTree().get<double>("university.student_fraction");
    auto work_fraction    = config.getTree().get<double>("work.work_fraction");
    auto commute_fraction = config.getTree().get<double>("work.commute_fraction");
    auto student_fractions  = vector<double>{student_fraction, 1.0 - student_fraction};
    auto work_fractions     = vector<double>{work_fraction, 1.0 - work_fraction};
    auto commute_fractions  = vector<double>{commute_fraction, 1.0 - commute_fraction};
    auto student_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(student_fractions.begin(), student_fractions.end()));
    auto work_gen    = rn_manager->GetGenerator(trng::fast_discrete_dist(work_fractions.begin(), work_fractions.end()));
    auto commute_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(commute_fractions.begin(), commute_fractions.end()));

    // Commuting distributions
    unsigned int total_commuting_actives    = 100000; // TODO
    double commuting_student_active_ratio   = total_commuting_students / total_commuting_actives;

    util::CSV commuting_data = util::CSV(config.getTree().get<string>("geoprofile.commuters"));
    size_t column_count = commuting_data.GetColumnCount();
    vector<int> relative_commute(column_count, 0);
    vector<unsigned int> total_commute(column_count, 0);
    if (commuting_data.size() > 1) {
        // Access each element in the matrix
        for (size_t row_index = 0; row_index < commuting_data.size(); row_index++) {
            for (size_t col_index = 0; col_index < column_count; col_index++) {
                // Ignore commuting towards itself
                if (row_index == col_index)
                    continue;
                util::CSVRow row = *(commuting_data.begin()+row_index);
                auto commute_count = row.GetValue<unsigned int>(col_index);
                // Remove commuting students
                commute_count -= (commuting_student_active_ratio * commute_count);
                // TODO: ask
                relative_commute[col_index] -= commute_count;
                relative_commute[row_index] += commute_count;
                total_commute[col_index] += commute_count;
            }
        }
    }

    // Create a distribution to select a workplace city.
    vector<double> wpc_fractions;
    for(size_t i = 0; i < relative_commute.size(); i++) {
        wpc_fractions.push_back(double(relative_commute[i]) / double(total_commute[i]));
    }
    auto city_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(wpc_fractions.begin(), wpc_fractions.end()));

    //if (fractions.empty()) {}

    // --------------------------------
    // Assign employables to workplaces.
    // --------------------------------
    for (const auto& household : households) {
        for (const auto& person : household->persons) {
            auto age = person->GetAge();
            if (age >= 18 && age < 26 && person->GetPoolId(ContactPoolType::Id::School) != 0) {
                // Students are not employable
                continue;
            }
            if (age >= 18 && age < 65) {
                if (work_gen() == 1) {
                    // The person is non active
                    person->setPoolId(ContactPoolType::Id::Work, 0);
                    continue;
                }
                // The person is active
                shared_ptr<ContactPool> pool;
                if (commute_gen() == 0) {
                    // Commuting
                    auto destination = grid[city_gen()];
                    auto dest_coord = destination->coordinate;
                    vector<shared_ptr<WorkPlace>> dest_workplaces;
                    for (auto& band : workplaces) {
                        for (auto& g_struct : band) {
                            auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
                            if (workplace->coordinate == dest_coord) {
                                dest_workplaces.push_back(workplace);
                            }
                        }
                    }
                    // TODO: ask
                    if (dest_workplaces.empty())
                        continue;
                    // Create a uniform distribution to select a workplace
                    auto wp_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(dest_workplaces.size()));
                    auto workplace = dest_workplaces[wp_generator()];
                    pool = workplace->pool;
                    std::cout << "commutor" <<std::endl;
                } else {
                    // Non-commuting
                    auto home_coord = household->coordinate;
                    std::vector<shared_ptr<GenStruct>> closest_workplaces = getClosestStructs(home_coord, workplaces, grid);
                    if (closest_workplaces.empty()) {
                        std::cout << "closest_workplaces is empty: " << age << std::endl;
                        continue;
                    }
                    // Create a uniform distribution to select a workplace
                    std::function<int()> wp_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_workplaces.size()));
                    auto workplace = static_pointer_cast<WorkPlace>(closest_workplaces.at(wp_generator()));
                    pool = workplace->pool;
                    std::cout << "non commutor" <<std::endl;

                }
                std::cout << "assigned workplace pool id: " <<pool->GetId() << std::endl;
                person->setPoolId(ContactPoolType::Id::Work, pool->GetId());
                pool->AddMember(person.get());
            }
        }
    }
}

void assignCommunities(
    vector<vector<shared_ptr<GenStruct>>> communities, const vector<shared_ptr<Household>>& households,
    const GenConfiguration& config, const GeoGrid& grid)
{
    // -------------
    // Contactpools
    // -------------
    const unsigned int community_size      = 2000;
    const unsigned int community_cp_size   = 20;
    // Create the contactpools for every community
    unsigned int cp_id = 0;
    unsigned int community_amount = 0;
    for (auto& band : communities) {
        community_amount+=band.size();
    }
    for (auto& band : communities) {
        for (auto& g_struct : band) {
            auto community  = std::static_pointer_cast<Community>(g_struct);
            if(community->id+1 >= community_amount/2){
                community->is_primary = false;
            }
            else{
                community->is_primary = true;
            }
            for(unsigned int size = 0; size < community_size; size += community_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::PrimaryCommunity);
                community->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    // ------------------------------
    // Assign persons to communities
    // ------------------------------
    for (const auto& household : households) {
        auto home_coord = household->coordinate;
        std::vector<shared_ptr<GenStruct>> closest_communities = getClosestStructs(home_coord, communities, grid);
        if (closest_communities.empty()) {
            std::cout << "closest_communities is empty: " << std::endl;
            continue;
        }
        // Create a uniform distribution to select a primary community
        auto rn_manager             = config.getRNManager();
        auto community_generator    = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_communities.size()));

        auto community_index        = community_generator();
        auto community              = static_pointer_cast<Community>(closest_communities.at(community_index));
        while(true){
            if(community->is_primary){
                break;
            }
            //std::cout << community_index << std::endl;
            //std::cout << community->is_primary << std::endl;
            community_index        = community_generator();
            community              = static_pointer_cast<Community>(closest_communities.at(community_index));
        }

        // Create a uniform distribution to select a contactpool in the selected community
        auto cp_generator           = rn_manager->GetGenerator(trng::fast_discrete_dist(community->pools.size()));
        auto cp_index               = cp_generator();
        auto pool                   = community->pools[cp_index];
        for (const auto& person : household->persons) {
            // Add person to the selected primary community
            person->setPoolId(ContactPoolType::Id::PrimaryCommunity, pool->GetId());
            pool->AddMember(person.get());
        }
        // Remove the pool from the list once it's full
        if (pool->GetSize() >= community_cp_size) {
            auto pools = community->pools;
            pools.erase(pools.begin() + cp_index);
            community->full_pools.push_back(pool);
            if (community->pools.empty()) {
                communities.erase(communities.begin() + community_index);
            }
        }
        for (const auto& person : household->persons) {
            if (closest_communities.empty()) {
                std::cout << "closest_communities is empty: " << std::endl;
                continue;
            }
            // Create a uniform distribution to select a secondary community
            auto secondary_rn_manager             = config.getRNManager();
            auto secondary_community_generator    = secondary_rn_manager->GetGenerator(trng::fast_discrete_dist(closest_communities.size()));

            auto secondary_community_index        = secondary_community_generator();
            auto secondary_community              = static_pointer_cast<Community>(closest_communities.at(secondary_community_index));
            unsigned int counter = 0;
            while(true){
                counter++;
                if(!secondary_community->is_primary){
                    break;
                }
                // ugly bandaid solution to infinite loop
                if(counter>=20){
                    break;
                }
                secondary_community_index        = secondary_community_generator();
                secondary_community              = static_pointer_cast<Community>(closest_communities.at(secondary_community_index));
            }
            // Create a uniform distribution to select a contactpool in the selected community
            auto secondary_cp_generator           = secondary_rn_manager->GetGenerator(trng::fast_discrete_dist(secondary_community->pools.size()));
            auto secondary_cp_index               = secondary_cp_generator();
            auto secondary_pool                   = secondary_community->pools[secondary_cp_index];

            // Add person to the selected secondary community
            person->setPoolId(ContactPoolType::Id::SecondaryCommunity, secondary_pool->GetId());
            secondary_pool->AddMember(person.get());

            // Remove the pool from the list once it's full
            if (secondary_pool->GetSize() >= community_cp_size) {
                auto pools = secondary_community->pools;
                pools.erase(pools.begin() + secondary_cp_index);
                secondary_community->full_pools.push_back(secondary_pool);
                if (secondary_community->pools.empty()) {
                    std::cout << "deleting secondary com" << std::endl;
                    communities.erase(communities.begin() + secondary_community_index);
                }
            }
        }

    }
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

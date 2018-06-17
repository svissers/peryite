#include "PopulationFile.h"
#include "util/InstallDirs.h"
#include "util/FileSys.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace util;
using namespace boost::filesystem;
using namespace boost::property_tree;

PopulationFile::PopulationFile(GenConfiguration& config)
{
    m_file_name     = "Pop.csv";
    m_labels        = {"age", "household_id", "school_id", "work_id", "primary_community", "secondary_community", "latitude", "longitude", "tourism_primary_community", "tourism_secondary_community", "travel_work_id", "travel_start", "travel_end"};
    // Get the output directory for this configuration.
    m_output_prefix = config.GetOutputPrefix();
    m_file_path     = FileSys::BuildPath(m_output_prefix, m_file_name);
}

PopulationFile::PopulationFile(GenConfiguration& config, std::shared_ptr<Population> population)
    : PopulationFile(config)
{
    m_population = std::move(population);
}


void PopulationFile::Write()
{
    if (m_population->empty())
        return;
    std::ofstream my_file{m_file_path.string()};
    if(my_file.is_open()) {
        my_file << boost::algorithm::join(m_labels,",") << "\n";
        for (const auto& person : *m_population)
            my_file << boost::algorithm::join(GetValues(person),",") << "\n";
        my_file.close();
    }
}

void PopulationFile::Read(shared_ptr<Population>& population)
{
    // Read the population from memory
    if (!m_population->empty()) {
        population = m_population;
        return;
    }
    // Read the population from file
    m_population = population;
    CSV pop_data(m_file_path.string());
    unsigned int person_id = 0U;
    for (CSVRow const & row : pop_data) {
        m_population->CreatePerson(
            person_id,
            row.GetValue<unsigned int>(0),
            row.GetValue<unsigned int>(1),
            row.GetValue<unsigned int>(2),
            row.GetValue<unsigned int>(3),
            row.GetValue<unsigned int>(4),
            row.GetValue<unsigned int>(5),
            row.GetValue<double>(6),
            row.GetValue<double>(7),
            row.GetValue<unsigned int>(8),
            row.GetValue<unsigned int>(9),
            row.GetValue<unsigned int>(10),
            row.GetValue<int>(11),
            row.GetValue<int>(12)
        );
       ++person_id;
    }
}

void PopulationFile::WriteRegions(string output_prefix, Regions& regions)
{
    if (regions.empty())
        return;
    std::vector<std::string> labels =
    {
        "id","name","first_person_id", "last_person_id",
        "first_household_cp", "last_household_cp",
        "first_school_id", "last_school_id",
        "first_workplace_id", "last_workplace_id",
        "first_prim_comm_id", "last_prim_comm_id",
        "first_sec_comm_id", "last_sec_comm_id"
    };
    auto file_path = FileSys::BuildPath(output_prefix, "Regions.csv");
    std::ofstream my_file{file_path.string()};
    if(my_file.is_open()) {
        my_file << boost::algorithm::join(labels,",") << "\n";
        for (const auto& region : regions)
            my_file << boost::algorithm::join(GetValues(region),",") << "\n";
        my_file.close();
    }
}

Regions PopulationFile::ReadRegions(const boost::property_tree::ptree& config_pt)
{
    // Locate the file
    const auto file_name        = config_pt.get<string>("run.region_file");
    const auto use_install_dirs = config_pt.get<bool>("run.use_install_dirs");
    const auto file_path        = (use_install_dirs) ? FileSys::GetDataDir() /= file_name : file_name;
    std::cout << "REGIONSFILE FILE PATH: " << file_path.string() << std::endl;
    if (!is_regular_file(file_path)) {
            throw runtime_error(string(__func__) + "> Region file " + file_path.string() + " not present.");
    }
    // Read from the file
    Regions regions;
    CSV reg_data(file_path.string());
    for (CSVRow const & row : reg_data) {
        std::shared_ptr<Region> region = make_shared<Region>(
            row.GetValue<unsigned int>(0),
            to_string(row.GetValue<unsigned int>(1))
        );
        region->first_person_id = row.GetValue<unsigned int>(2);
        region->last_person_id  = row.GetValue<unsigned int>(3);
        unsigned int row_value = 4;
        for (auto typ : ContactPoolType::IdList) {
            region->first_cps[typ] = row.GetValue<unsigned int>(row_value++);
            region->last_cps[typ]  = row.GetValue<unsigned int>(row_value++);
        }
        regions.push_back(region);
    }
    return regions;
}


void PopulationFile::WritePoolSys(std::string output_prefix, const ContactPoolSys& pool_sys)
{

    std::vector<std::string> labels = {"type","id","lat", "lon"};
    auto file_path = FileSys::BuildPath(output_prefix, "PoolSys.csv");
    std::ofstream my_file{file_path.string()};
    if(my_file.is_open()) {
        my_file << boost::algorithm::join(labels,",") << "\n";
        for (const auto& typ : ContactPoolType::IdList) {
            auto type_str = ContactPoolType::ToString(typ);
            for (const auto& pool : pool_sys[typ]) {
                my_file << type_str << "," << boost::algorithm::join(GetValues(pool),",") << "\n";
            }
        }
        my_file.close();
    }
}

void PopulationFile::ReadPoolSys(const boost::property_tree::ptree& config_pt, ContactPoolSys& pool_sys)
{
    // Locate the file
    const auto file_name        = config_pt.get<string>("run.poolsys_file");
    const auto use_install_dirs = config_pt.get<bool>("run.use_install_dirs");
    const auto file_path        = (use_install_dirs) ? FileSys::GetDataDir() /= file_name : file_name;
    if (!is_regular_file(file_path)) {
            throw runtime_error(string(__func__) + "> Poolsys file " + file_path.string() + " not present.");
    }
    // Read from the file
    CSV reg_data(file_path.string());
    for (CSVRow const & row : reg_data) {
        auto type   = ContactPoolType::ToType(row.GetValue<string>(0));
        auto id     = row.GetValue<unsigned int>(1);
        auto lat    = row.GetValue<double>(2);
        auto lon    = row.GetValue<double>(3);
        auto coord  = util::spherical_point(lat, lon);
        ContactPool pool(id, type, coord);
        //std::cout << row.GetValue<string>(0) << "," << to_string(id) << "," << to_string(lat) << "," << to_string(lon) << std::endl;
        pool_sys[type].emplace_back(pool);
    }
}


vector<vector<unsigned int>> GetReferenceHouseholds(const GenConfiguration& config)
{
    string file_name = config.GetTree().get<string>("household_profile");
    ptree tree;
    try {
        // Populate the household tree
        read_xml((util::InstallDirs::GetDataDir() /= file_name).string(), tree,
                 xml_parser::trim_whitespace | xml_parser::no_comments);
    } catch (xml_parser_error& e) {
            throw invalid_argument(string("Invalid file: ") +
                                        (util::InstallDirs::GetDataDir() /= file_name).string());
    }

    // Traverse property tree
    vector<vector<unsigned int>> result;
    BOOST_FOREACH( boost::property_tree::ptree::value_type const& node, tree.get_child("HouseholdProfile" ) ) {
        boost::property_tree::ptree age_tree = node.second;
        vector<unsigned int> household;
        if( node.first == "hh" ) {
            BOOST_FOREACH( boost::property_tree::ptree::value_type const& v, age_tree.get_child("") ) {
                std::string label = v.first;
                if ( label == "age" ) {
                    household.push_back(uint(std::stoul(v.second.data())));
                }
            }
            result.push_back(household);
        }
    }

    return result;
}


std::vector<std::string> PopulationFile::GetValues(const Person& person)
{
    vector<string> values = {
        to_string(person.GetAge()),
        to_string(person.GetPoolId(ContactPoolType::Id::Household)),
        to_string(person.GetPoolId(ContactPoolType::Id::School)),
        to_string(person.GetPoolId(ContactPoolType::Id::Work)),
        to_string(person.GetPoolId(ContactPoolType::Id::PrimaryCommunity)),
        to_string(person.GetPoolId(ContactPoolType::Id::SecondaryCommunity)),
        to_string(person.GetCoordinate().get<0>()),
        to_string(person.GetCoordinate().get<1>()),
        to_string(get<0>(person.getTravelComIds())),
        to_string(get<1>(person.getTravelComIds())),
        to_string(person.getTravelWorkId()),
        to_string(get<0>(person.getTravelDates())),
        to_string(get<1>(person.getTravelDates()))
    };
    return values;
}

std::vector<std::string> PopulationFile::GetValues(const shared_ptr<Region> region)
{
    vector<string> values =
    {
        to_string(region->id),
        region->name,
        to_string(region->first_person_id),
        to_string(region->last_person_id)
    };
    for (auto typ : ContactPoolType::IdList) {
        values.push_back(to_string(region->first_cps[typ]));
        values.push_back(to_string(region->last_cps[typ]));
    }
    return values;
}


std::vector<std::string> PopulationFile::GetValues(const ContactPool& pool)
{
    vector<string> values =
    {
        to_string(pool.GetId()),
        to_string(pool.GetCoordinate().get<0>()),
        to_string(pool.GetCoordinate().get<1>())
    };
    return values;
}

} // namespace files
} // namespace gen
} // namespace stride

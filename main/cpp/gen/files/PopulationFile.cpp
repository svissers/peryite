#include "PopulationFile.h"
#include "util/InstallDirs.h"
#include "util/FileSys.h"
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace util;
using namespace boost::filesystem;
using namespace boost::property_tree;

PopulationFile::PopulationFile(GenConfiguration& config)
{
    m_file_name = "Pop.csv";
    m_labels = {"age", "household_id", "school_id", "work_id", "primary_community", "secondary_community", "latitude", "longitude"};

    // Get the output directory for this configuration.
    m_output_prefix = config.GetOutputPrefix();
    m_file_path = FileSys::BuildPath(m_output_prefix, m_file_name);
    try {
        create_directories(m_output_prefix);
    } catch (exception& e) {
        cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
        throw;
    }
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
            row.GetValue<double>(7)
        );
       ++person_id;
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
                    household.push_back(std::stoul(v.second.data()));
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
        to_string(person.GetCoordinate().get<1>())
    };
    return values;
}

void PopulationFile::SetFileName(std::string newname){
    m_file_name = newname;
    m_file_path = FileSys::BuildPath(m_output_prefix, m_file_name);
}

} // namespace files
} // namespace gen
} // namespace stride

#include "PopulationFile.h"
#include "util/InstallDirs.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace boost::property_tree;

PopulationFile::PopulationFile(GenConfiguration& config)
{
    m_file_name = "UnassignedPop.csv";
    m_labels = {"age", "household_id", "school_id", "work_id", "primary_community", "secondary_community"};

    // Get the output directory for this configuration.
    string config_path = config.GetPath();
    m_out_dir = "output/"+config_path.substr(0, config_path.find_last_of("."));
    try {
        create_directories(m_out_dir);
    } catch (exception& e) {
        cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
        throw;
    }
}

PopulationFile::PopulationFile(GenConfiguration& config, std::shared_ptr<Population> population)
    : PopulationFile(config), m_population(population)
{}


void PopulationFile::Write()
{
    if (m_population.empty())
        return;
    string file_path = m_out_dir.string()+"/"+m_file_name;
    std::ofstream my_file{file_path};
    if(my_file.is_open()) {
        my_file << boost::algorithm::join(m_labels,",") << "\n";
        for (const auto& person : m_population)
            my_file << boost::algorithm::join(getValues(person),",") << "\n";
        my_file.close();
    }
}

std::shared_ptr<Population> PopulationFile::Read(const ptree& belief_pt)
{
    if (!m_population.empty())
        return m_population;
    // Populate the population and return it
    m_population = make_shared<Population>();
    string file_path = m_out_dir.string()+"/"+m_file_name;
    CSV pop_data(file_path);
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
            Health(),
            belief_pt
        );
       ++person_id;
    }
    return population;
}

vector<vector<unsigned int>> PopulationFile::GetReferenceHouseholds(const GenConfiguration& config)
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


std::vector<std::string> PopulationFile::getValues(std::shared_ptr<Person> person)
{
    vector<string> values = {
        to_string(person->GetAge()),
        to_string(person->GetPoolId(ContactPoolType::Id::Household)),
        to_string(person->GetPoolId(ContactPoolType::Id::School)),
        to_string(person->GetPoolId(ContactPoolType::Id::Work)),
        to_string(person->GetPoolId(ContactPoolType::Id::PrimaryCommunity)),
        to_string(person->GetPoolId(ContactPoolType::Id::SecondaryCommunity))
    };
    return values;
}

} // namespace files
} // namespace gen
} // namespace stride

#include "GenDirectory.h"

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace gen;

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt, unsigned int thread_count, string output_prefix)
{
    auto multi_regions = config_pt.get_child_optional("run.amount_regions");
    unsigned int amount_regions= 1;
    if(multi_regions){
        amount_regions = config_pt.get<unsigned int>("run.amount_regions");
    }
    m_amount_of_regions = amount_regions;
    for(unsigned int i; i < amount_regions; i++) {
        string pop_config = "run.pop_config";
        pop_config.append(to_string(i+1));
        m_config.push_back(GenConfiguration(config_pt.get_child(pop_config), thread_count, std::move(output_prefix)));
    }
}

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt, util::RNManager& rn_manager, string output_prefix)
{
    // Make a pointer to the rn_manager without it being automatically deleted.
    auto manager    = std::shared_ptr<util::RNManager>(&rn_manager, [](util::RNManager*){});
    auto multi_regions = config_pt.get_child_optional("run.amount_regions");
    unsigned int amount_regions= 1;
    if(multi_regions){
        amount_regions = config_pt.get<unsigned int>("run.amount_regions");
    }
    m_amount_of_regions = amount_regions;

    for(unsigned int i; i < amount_regions; i++) {
        string pop_config = "run.pop_config";
        pop_config.append(to_string(i+1));
        m_config.push_back(GenConfiguration(config_pt.get_child(pop_config), manager, std::move(output_prefix)));
    }
}

void GenDirectory::Initialize(PopulationFilePtr pop_file, GeoGridFilePtr geo_file,
                SchoolFilePtr school_file, UniversityFilePtr university_file,
                WorkplaceFilePtr workplace_file, CommunityFilePtr community_file)
{
    m_pop_file.push_back(std::move(pop_file));
    m_geo_file.push_back(std::move(geo_file));
    m_school_file.push_back(std::move(school_file));
    m_university_file.push_back(std::move(university_file));
    m_workplace_file.push_back(std::move(workplace_file));
    m_community_file.push_back(std::move(community_file));
}

std::vector<PopulationFilePtr> GenDirectory::GetPopulationFile(unsigned int region_nr)
{
    if (m_pop_file.empty()) {
        m_pop_file.push_back(make_shared<PopulationFile>(m_config[region_nr]));
    }
    return m_pop_file;
}

std::vector<GeoGridFilePtr> GenDirectory::GetGeoGridFile(unsigned int region_nr)
{
    if (m_geo_file.empty()) {
        m_geo_file.push_back(make_shared<GeoGridFile>(m_config[region_nr]));
    }
    return m_geo_file;
}

std::vector<SchoolFilePtr> GenDirectory::GetSchoolFile(unsigned int region_nr)
{
    if (m_school_file.empty()) {
        m_school_file.push_back(make_shared<SchoolFile>(m_config[region_nr]));
    }
    return m_school_file;
}

std::vector<UniversityFilePtr> GenDirectory::GetUniversityFile(unsigned int region_nr)
{
    if (m_university_file.empty()) {
        m_university_file.push_back(make_shared<UniversityFile>(m_config[region_nr]));
    }
    return m_university_file;
}

std::vector<WorkplaceFilePtr> GenDirectory::GetWorkplaceFile(unsigned int region_nr)
{
    if (m_workplace_file.empty()) {
        m_workplace_file.push_back(make_shared<WorkplaceFile>(m_config[region_nr]));
    }
    return m_workplace_file;
}

std::vector<CommunityFilePtr> GenDirectory::GetCommunityFile(unsigned int region_nr)
{
    if (m_community_file.empty()) {
        m_community_file.push_back(make_shared<CommunityFile>(m_config[region_nr]));
    }
    return m_community_file;
}

int GenDirectory::GetFirstInRegion(unsigned int region_nr) {
    if(region_nr < m_first_id_in_region.size())
        return m_first_id_in_region[region_nr];
    else
        return -1;
}

void GenDirectory::AddFirstInRegion(unsigned int person_id) {
    m_first_id_in_region.push_back(person_id);
}

unsigned int GenDirectory::GetAmountOfRegions(){
    return m_amount_of_regions;
}

} // namespace files
} // namespace gen
} // namespace stride

#include "GenDirectory.h"

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace gen;
using namespace util;

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt)
{
    // Initialize the random number generator
    auto rn_manager        = make_shared<util::RNManager>();
    const auto rng_type = config_pt.get<string>("run.rng.engine");
    const auto rng_seed = config_pt.get<unsigned long>("run.rng.seed");
    auto thread_count   = config_pt.get<unsigned int>("run.num_threads");
    const util::RNManager::Info info{rng_type, rng_seed, "", thread_count};
    rn_manager->Initialize(info);
    // Define the general geopop config.
    auto output_prefix = config_pt.get<string>("run.output_prefix");
    m_config = GenConfiguration(config_pt.get_child("run.pop_config"), thread_count, std::move(output_prefix), rn_manager);
    // Define every region using their respective region configuration.
    m_regions = Regions(config_pt, rn_manager);
}

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt, util::RNManager& rn_manager)
{
    // Make a pointer to the rn_manager without it being automatically deleted.
    auto manager = std::shared_ptr<util::RNManager>(&rn_manager, [](util::RNManager*){});
    // Define the general geopop config.
    auto thread_count   = config_pt.get<unsigned int>("run.num_threads");
    auto output_prefix = config_pt.get<string>("run.output_prefix");
    m_config = GenConfiguration(config_pt.get_child("run.pop_config"), thread_count, std::move(output_prefix), manager);
    // Define every region using their respective region configuration.
    m_regions = Regions(config_pt, manager);
}

void GenDirectory::InitializeRegion(
        unsigned int region_id,
        GeoGridFilePtr geo_file,
        SchoolFilePtr school_file, UniversityFilePtr university_file,
        WorkplaceFilePtr workplace_file, CommunityFilePtr community_file
    )
{
    m_geo_file[region_id]           = geo_file;
    m_school_file[region_id]        = school_file;
    m_university_file[region_id]    = university_file;
    m_workplace_file[region_id]     = workplace_file;
    m_community_file[region_id]     = community_file;
}

void GenDirectory::InitializePopulationFile(PopulationFilePtr pop_file)
{
    m_pop_file = pop_file;
}

PopulationFilePtr GenDirectory::GetPopulationFile()
{
    if (!m_pop_file)
        m_pop_file = make_shared<PopulationFile>(m_config);
    return m_pop_file;
}

GeoGridFilePtr GenDirectory::GetGeoGridFile(unsigned int region_id)
{
    if (m_geo_file.count(region_id) == 0)
        m_geo_file[region_id] = make_shared<GeoGridFile>(m_config, to_string(region_id));
    return m_geo_file[region_id];
}

SchoolFilePtr GenDirectory::GetSchoolFile(unsigned int region_id)
{
    if (m_school_file.count(region_id) == 0)
        m_school_file[region_id] = make_shared<SchoolFile>(m_config, to_string(region_id));
    return m_school_file[region_id];
}

UniversityFilePtr GenDirectory::GetUniversityFile(unsigned int region_id)
{
    if (m_university_file.count(region_id) == 0)
        m_university_file[region_id] = make_shared<UniversityFile>(m_config, to_string(region_id));
    return m_university_file[region_id];
}

WorkplaceFilePtr GenDirectory::GetWorkplaceFile(unsigned int region_id)
{
    if (m_workplace_file.count(region_id) == 0)
        m_workplace_file[region_id] = make_shared<WorkplaceFile>(m_config, to_string(region_id));
    return m_workplace_file[region_id];
}

CommunityFilePtr GenDirectory::GetCommunityFile(unsigned int region_id)
{
    if (m_community_file.count(region_id) == 0)
        m_community_file[region_id] = make_shared<CommunityFile>(m_config, to_string(region_id));
    return m_community_file[region_id];
}

Regions& GenDirectory::GetRegions()
{
    return m_regions;
}

} // namespace files
} // namespace gen
} // namespace stride

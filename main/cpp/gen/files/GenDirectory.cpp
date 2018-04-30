#include "GenDirectory.h"

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace gen;

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt, unsigned int thread_count, string output_prefix)
    : m_config(GenConfiguration(config_pt.get_child("pop_config"), thread_count, output_prefix))
{
    m_belief_pt = config_pt.get_child("belief_policy");
}

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt, util::RNManager& rn_manager, string output_prefix)
{
    // Make a pointer to the rn_manager without it being automatically deleted.
    auto manager = std::shared_ptr<util::RNManager>(&rn_manager, [](util::RNManager*){});
    m_config = GenConfiguration(config_pt.get_child("pop_config"), manager, output_prefix);
    m_belief_pt = config_pt.get_child("belief_policy");
}

void GenDirectory::Initialize(PopulationFilePtr pop_file, GeoGridFilePtr geo_file,
                SchoolFilePtr school_file, UniversityFilePtr university_file,
                WorkplaceFilePtr workplace_file, CommunityFilePtr community_file)
{
    m_pop_file          = pop_file;
    m_geo_file          = geo_file;
    m_school_file       = school_file;
    m_university_file   = university_file;
    m_workplace_file    = workplace_file;
    m_community_file    = community_file;
}

PopulationFilePtr GenDirectory::GetPopulationFile()
{
    if (!m_pop_file) {
        m_pop_file = make_shared<PopulationFile>(m_config);
    }
    return m_pop_file;
}

GeoGridFilePtr GenDirectory::GetGeoGridFile()
{
    if (!m_geo_file) {
        m_geo_file = make_shared<GeoGridFile>(m_config);
    }
    return m_geo_file;
}

SchoolFilePtr GenDirectory::GetSchoolFile()
{
    if (!m_school_file) {
        m_school_file = make_shared<SchoolFile>(m_config);
    }
    return m_school_file;
}

UniversityFilePtr GenDirectory::GetUniversityFile()
{
    if (!m_university_file) {
        m_university_file = make_shared<UniversityFile>(m_config);
    }
    return m_university_file;
}

WorkplaceFilePtr GenDirectory::GetWorkplaceFile()
{
    if (!m_workplace_file) {
        m_workplace_file = make_shared<WorkplaceFile>(m_config);
    }
    return m_workplace_file;
}

CommunityFilePtr GenDirectory::GetCommunityFile()
{
    if (!m_community_file) {
        m_community_file = make_shared<CommunityFile>(m_config);
    }
    return m_community_file;
}

} // namespace files
} // namespace gen
} // namespace stride

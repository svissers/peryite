#include "GenDirectory.h"

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace gen;

GenDirectory::GenDirectory(const boost::property_tree::ptree& config_pt, unsigned int thread_count)
{
    m_config_pt = GenConfiguration(config_pt.get_child("run.pop_config"), thread_count);
    m_belief_pt = config_pt.get_child("run.belief_policy");
}

void GenDirectory::initialize(PopulationFilePtr pop_file, GeoGridFilePtr geo_file,
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

GeoGridFilePtr GenDirectory::getGeoGridFile()
{
    if (!m_geo_file) {
        m_geo_file = make_shared<GeoGridFile>(m_config);
    }
    return m_geo_file;
}

SchoolFilePtr GenDirectory::getSchoolFile()
{
    if (!m_school_file) {
        m_school_file = make_shared<SchoolFile>(m_config);
    }
    return m_school_file;
}

UniversityFilePtr GenDirectory::getUniversityFile()
{
    if (!m_university_file) {
        m_university_file = make_shared<UniversityFile>(m_config);
    }
    return m_university_file;
}

WorkplaceFilePtr GenDirectory::getWorkplaceFile()
{
    if (!m_workplace_file) {
        m_workplace_file = make_shared<WorkplaceFile>(m_config);
    }
    return m_workplace_file;
}

CommunityFilePtr GenDirectory::getCommunityFile()
{
    if (!m_community_file) {
        m_community_file = make_shared<CommunityFile>(m_config);
    }
    return m_community_file;
}

} // namespace files
} // namespace gen
} // namespace stride

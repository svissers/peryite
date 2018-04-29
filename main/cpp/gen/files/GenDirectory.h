#pragma once
#include "../GenConfiguration.h"
#include "PopulationFile.h"
#include "GeoGridFile.h"
#include "SchoolFile.h"
#include "UniversityFile.h"
#include "WorkplaceFile.h"
#include "CommunityFile.h"
#include <boost/property_tree/ptree.hpp>

namespace stride {
namespace gen {
namespace files {

/**
 * Container class for GenFiles.
 */
class GenDirectory
{
public:
    GenDirectory(const boost::property_tree::ptree& config_pt, unsigned int thread_count, std::string output_prefix);
    GenDirectory(const boost::property_tree::ptree& config_pt, util::RNManager& rn_manager, std::string output_prefix);

    void initialize(PopulationFilePtr hh_file, GeoGridFilePtr geo_file,
                SchoolFilePtr school_file, UniversityFilePtr university_file,
                WorkplaceFilePtr workplace_file, CommunityFilePtr community_file);

    GenConfiguration getConfig() { return m_config; }
    boost::property_tree::ptree getBeliefConfig() { return m_belief_pt; }

    PopulationFilePtr GetPopulationFile();
    GeoGridFilePtr getGeoGridFile();
    SchoolFilePtr getSchoolFile();
    UniversityFilePtr getUniversityFile();
    WorkplaceFilePtr getWorkplaceFile();
    CommunityFilePtr getCommunityFile();

private:
    GenConfiguration m_config;
    boost::property_tree::ptree m_belief_pt;

    PopulationFilePtr m_pop_file;
    GeoGridFilePtr m_geo_file;
    SchoolFilePtr m_school_file;
    UniversityFilePtr m_university_file;
    WorkplaceFilePtr m_workplace_file;
    CommunityFilePtr m_community_file;
};

typedef std::shared_ptr<GenDirectory> GenDirectoryPtr;

} // namespace files
} // namespace gen
} // namespace stride

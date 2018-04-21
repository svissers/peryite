#pragma once
#include "../GenConfiguration.h"
#include "GeoGridFile.h"
#include "SchoolFile.h"
#include "UniversityFile.h"
#include "WorkplaceFile.h"
#include "CommunityFile.h"

namespace stride {
namespace gen {
namespace files {

/**
 * Container class for GenFiles.
 */
class GenDirectory
{
public:
    GenDirectory(std::string config_file_name, unsigned int thread_count);

    void initialize(GeoGridFilePtr geo_file,
                SchoolFilePtr school_file, UniversityFilePtr university_file,
                WorkplaceFilePtr workplace_file, CommunityFilePtr community_file);

    GenConfiguration getConfig() { return m_config; }

    GeoGridFilePtr getGeoGridFile();
    SchoolFilePtr getSchoolFile();
    UniversityFilePtr getUniversityFile();
    WorkplaceFilePtr getWorkplaceFile();
    CommunityFilePtr getCommunityFile();

private:
    GenConfiguration m_config;
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

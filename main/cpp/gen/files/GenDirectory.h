#pragma once
#include "../GenConfiguration.h"
#include "PopulationFile.h"
#include "GeoGridFile.h"
#include "SchoolFile.h"
#include "UniversityFile.h"
#include "WorkplaceFile.h"
#include "CommunityFile.h"
#include "util/Regions.h"
#include "util/RNManager.h"
#include <boost/property_tree/ptree.hpp>

namespace stride {
namespace gen {
namespace files {

/**
 * Container class for GenFiles.
 * Represents both a directory in a filesystem and a container in memory.
 * Abstracts the access to data (can be from file or from memory)
 */
class GenDirectory
{
public:
    /// Construct the directory with a new RNManager.
    /// @param config_pt            The configuration property tree to use.
    GenDirectory(const boost::property_tree::ptree& config_pt);

    /// Construct the directory using a pre-existing RNManager.
    /// @param config_pt            The configuration property tree to use.
    /// @param rn_manager           The RNManager to be used in the GeoPop module.
    GenDirectory(const boost::property_tree::ptree& config_pt, util::RNManager& rn_manager);

    /// Fill the directory with generated files for a region.
    /// @param region_id            The id of the region.
    /// @param geo_file             The file containing the geogrid.
    /// @param school_file          The file containing schools.
    /// @param university_file      The file containing universities.
    /// @param workplace_file       The file containing workplaces.
    /// @param community_file       The file containing communities.
    void InitializeRegion(unsigned int region_id,
                GeoGridFilePtr geo_file,
                SchoolFilePtr school_file, UniversityFilePtr university_file,
                WorkplaceFilePtr workplace_file, CommunityFilePtr community_file);

    /// Fill the directory with generated files for a region.
    /// @param pop_file             The file containing persons.
    void InitializePopulationFile(PopulationFilePtr pop_file);

    /// Returns the configuration for the geopop module.
    /// @return The configuration.
    GenConfiguration& GetConfig() { return m_config; }

    /// Returns the file containing persons.
    /// There is only one population file combining all regions.
    /// @return The file containing persons.
    PopulationFilePtr GetPopulationFile();

    /// Returns the file containing the geogrid.
    /// @return The file containing the geogrid.
    GeoGridFilePtr GetGeoGridFile(unsigned int region_id);

    /// Returns the file containing schools.
    /// @return The file containing schools.
    SchoolFilePtr GetSchoolFile(unsigned int region_id);

    /// Returns the file containing universities.
    /// @return The file containing universities.
    UniversityFilePtr GetUniversityFile(unsigned int region_id);

    /// Returns the file containing workplaces.
    /// @return The file containing workplaces.
    WorkplaceFilePtr GetWorkplaceFile(unsigned int region_id);

    /// Returns the file containing communities.
    /// @return The file containing communities.
    CommunityFilePtr GetCommunityFile(unsigned int region_id);

    /// Returns the regions used in this directory.
    /// @return The regions.
    util::Regions& GetRegions();

private:
    /// The geopop regions
    util::Regions m_regions;
    /// The file containing persons.
    PopulationFilePtr m_pop_file;
    /// The file containing the geogrid.
    std::map<unsigned int,GeoGridFilePtr> m_geo_file;
    /// The file containing schools.
    std::map<unsigned int,SchoolFilePtr> m_school_file;
    /// The file containing universities.
    std::map<unsigned int,UniversityFilePtr> m_university_file;
    /// The file containing workplaces.
    std::map<unsigned int,WorkplaceFilePtr> m_workplace_file;
    /// The file containing communities.
    std::map<unsigned int,CommunityFilePtr> m_community_file;
    /// The configuration for the geopop module.
    GenConfiguration m_config;
};

} // namespace files
} // namespace gen
} // namespace stride

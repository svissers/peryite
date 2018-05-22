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
 * Represents both a directory in a filesystem and a container in memory.
 * Abstracts the access to data (can be from file or from memory)
 */
class GenDirectory
{
public:
    /// Construct the directory with a new RNManager.
    /// @param config_pt            The configuration property tree to use.
    /// @param thread_count         The amount of threads to be used for parallelization.
    /// @param output_prefix        The prefix (directory name or prefix to filename) to be used for all output files.
    GenDirectory(const boost::property_tree::ptree& config_pt, unsigned int thread_count, std::string output_prefix);

    /// Construct the directory using a pre-existing RNManager.
    /// @param config_pt            The configuration property tree to use.
    /// @param rn_manager           The RNManager to be used in the GeoPop module.
    /// @param output_prefix        The prefix (directory name or prefix to filename) to be used for all output files.
    GenDirectory(const boost::property_tree::ptree& config_pt, util::RNManager& rn_manager, std::string output_prefix);

    /// Fill the directory with generated files.
    /// @param PopulationFilePtr    The file containing persons.
    /// @param GeoGridFilePtr       The file containing the geogrid.
    /// @param SchoolFilePtr        The file containing schools.
    /// @param UniversityFilePtr    The file containing universities.
    /// @param WorkplaceFilePtr     The file containing workplaces.
    /// @param CommunityFilePtr     The file containing communities.
    void Initialize(PopulationFilePtr hh_file, GeoGridFilePtr geo_file,
                SchoolFilePtr school_file, UniversityFilePtr university_file,
                WorkplaceFilePtr workplace_file, CommunityFilePtr community_file);

    /// Returns the configuration for the geopop module.
    /// @return The configuration.
    std::vector<GenConfiguration> GetConfig() { return m_config; }

    /// Returns the file containing persons.
    /// @return The file containing persons.
    std::vector<PopulationFilePtr> GetPopulationFile(unsigned int region_nr);

    /// Returns the file containing the geogrid.
    /// @return The file containing the geogrid.
    std::vector<GeoGridFilePtr> GetGeoGridFile(unsigned int region_nr);

    /// Returns the file containing schools.
    /// @return The file containing schools.
    std::vector<SchoolFilePtr> GetSchoolFile(unsigned int region_nr);

    /// Returns the file containing universities.
    /// @return The file containing universities.
    std::vector<UniversityFilePtr> GetUniversityFile(unsigned int region_nr);

    /// Returns the file containing workplaces.
    /// @return The file containing workplaces.
    std::vector<WorkplaceFilePtr> GetWorkplaceFile(unsigned int region_nr);

    /// Returns the file containing communities.
    /// @return The file containing communities.
    std::vector<CommunityFilePtr> GetCommunityFile(unsigned int region_nr);

    int GetFirstInRegion(unsigned int region_nr);

    void AddFirstInRegion(unsigned int person_id);

    unsigned int GetAmountOfRegions();

private:
    /// The configuration for the geopop module.
    std::vector<GenConfiguration> m_config;
    /// The file containing persons.
    std::vector<PopulationFilePtr> m_pop_file;
    /// The file containing the geogrid.
    std::vector<GeoGridFilePtr> m_geo_file;
    /// The file containing schools.
    std::vector<SchoolFilePtr> m_school_file;
    /// The file containing universities.
    std::vector<UniversityFilePtr> m_university_file;
    /// The file containing workplaces.
    std::vector<WorkplaceFilePtr> m_workplace_file;
    /// The file containing communities.
    std::vector<CommunityFilePtr> m_community_file;
    /// The vector containing the id of every first person in a new region.
    std::vector<unsigned int> m_first_id_in_region;
    /// The total amount of regions.
    unsigned int m_amount_of_regions;
};

} // namespace files
} // namespace gen
} // namespace stride

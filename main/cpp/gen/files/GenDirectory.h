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
    GenConfiguration GetConfig() { return m_config; }

    /// Returns the belief configuration node from the general configuration file.
    /// @return The ptree for configuration of beliefs in persons.
    boost::property_tree::ptree GetBeliefConfig() { return m_belief_pt; }

    /// Returns the file containing persons.
    /// @return The file containing persons.
    PopulationFilePtr GetPopulationFile();

    /// Returns the file containing the geogrid.
    /// @return The file containing the geogrid.
    GeoGridFilePtr GetGeoGridFile();

    /// Returns the file containing schools.
    /// @return The file containing schools.
    SchoolFilePtr GetSchoolFile();

    /// Returns the file containing universities.
    /// @return The file containing universities.
    UniversityFilePtr GetUniversityFile();

    /// Returns the file containing workplaces.
    /// @return The file containing workplaces.
    WorkplaceFilePtr GetWorkplaceFile();

    /// Returns the file containing communities.
    /// @return The file containing communities.
    CommunityFilePtr GetCommunityFile();

private:
    /// The configuration for the geopop module.
    GenConfiguration m_config;
    /// The belief configuration node from the general configuration file.
    boost::property_tree::ptree m_belief_pt;h
    /// The file containing persons.
    PopulationFilePtr m_pop_file;
    /// The file containing the geogrid.
    GeoGridFilePtr m_geo_file;
    /// The file containing schools.
    SchoolFilePtr m_school_file;
    /// The file containing universities.
    UniversityFilePtr m_university_file;
    /// The file containing workplaces.
    WorkplaceFilePtr m_workplace_file;
    /// The file containing communities.
    CommunityFilePtr m_community_file;
};

typedef std::shared_ptr<GenDirectory> GenDirectoryPtr;

} // namespace files
} // namespace gen
} // namespace stride

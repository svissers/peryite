/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Header for the command line controller.
 */

#include "calendar/Calendar.h"
#include "gen/geogen/GeoGenerator.h"
#include "gen/popgen/PopGenerator.h"
#include "pop/Population.h"
#include "sim/GuiController.h"
#include "sim/SimRunner.h"
#include "sim/Sim.h"
#include "util/ConfigInfo.h"
#include "util/FileSys.h"
#include "util/LogUtils.h"
#include "util/TimeStamp.h"
#include "util/StringUtils.h"
#include "viewers/AdoptedViewer.h"
#include "viewers/InfectedViewer.h"
#include "viewers/CliViewer.h"
#include "viewers/PersonsViewer.h"
#include "viewers/SummaryViewer.h"
#include "viewers/MapViewer.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

using namespace std;
using namespace stride::util;
using namespace stride::gen;
using namespace stride::gen::files;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace stride {

GuiController::GuiController()
    : m_config_file(""), m_p_overrides(), m_stride_log_level(), m_use_install_dirs(), m_max_num_threads(1U),
      m_output_prefix(""), m_run_clock("run_clock", true), m_config_path(),
      m_stride_logger(nullptr)
{
    state = GuiState::Empty;
};


void GuiController::CheckEnv()
{
        if (m_use_install_dirs) {
                auto log = [](const string& s) -> void { cerr << s << endl; };
                if (!FileSys::CheckInstallEnv(log)) {
                        throw runtime_error("CliController::CheckEnv> Install dirs not OK.");
                }
        }
}

void GuiController::CheckOpenMP() { m_max_num_threads = ConfigInfo::NumberAvailableThreads(); }

void GuiController::CheckOutputPrefix()
{
        if (FileSys::IsDirectoryString(m_output_prefix)) {
                try {
                        create_directories(m_output_prefix);
                } catch (exception& e) {
                        cerr << "CliController::Setup> Exception creating directory:  {}" << m_output_prefix << endl;
                        throw;
                }
        }
}

void GuiController::RunStride()
{
    // -----------------------------------------------------------------------------------------
    // Build population, instantiate SimRunner & register viewers & run.
    // -----------------------------------------------------------------------------------------
    m_runner->Run();
}

void GuiController::RunStride(int steps)
{
    // -----------------------------------------------------------------------------------------
    // Build population, instantiate SimRunner & register viewers & run.
    // -----------------------------------------------------------------------------------------
    m_runner->Run(steps);
}

void GuiController::AssignPTree(boost::property_tree::ptree pt) {
    m_config_pt = pt;

    if (m_config_pt.get<string>("run.output_prefix", "").empty()) {
            m_config_pt.put("run.output_prefix", TimeStamp().ToTag().append("/"));
    }
    m_config_pt.sort();

    m_output_prefix = m_config_pt.get<string>("run.output_prefix");

    state = GuiState::ConfigFileSelected;
}

void GuiController::MakeLogger()
{
    const auto path     = FileSys::BuildPath(m_output_prefix, "stride_log.txt");
    const auto logLevel = m_config_pt.get<string>("run.stride_log_level");
    m_stride_logger     = LogUtils::CreateCliLogger("stride_logger", path.string());
    m_stride_logger->set_level(spdlog::level::from_str(logLevel));
    m_stride_logger->flush_on(spdlog::level::err);
}

void GuiController::RegisterViewers(shared_ptr<SimRunner> runner)
{
    // Command line viewer
    m_stride_logger->info("Registering CliViewer");
    const auto cli_v = make_shared<viewers::CliViewer>(runner, m_stride_logger);
    runner->Register(cli_v, bind(&viewers::CliViewer::Update, cli_v, placeholders::_1));

    // Adopted viewer
    if (m_config_pt.get<bool>("run.output_adopted", false)) {
            m_stride_logger->info("registering AdoptedViewer,");
            const auto v = make_shared<viewers::AdoptedViewer>(runner, m_output_prefix);
            runner->Register(v, bind(&viewers::AdoptedViewer::Update, v, placeholders::_1));
    }

    // Infection counts viewer
    if (m_config_pt.get<bool>("run.output_cases", false)) {
            m_stride_logger->info("Registering InfectedViewer");
            const auto v = make_shared<viewers::InfectedViewer>(runner, m_output_prefix);
            runner->Register(v, bind(&viewers::InfectedViewer::Update, v, placeholders::_1));
    }

    // Persons viewer
    if (m_config_pt.get<bool>("run.output_persons", false)) {
            m_stride_logger->info("registering PersonsViewer.");
            const auto v = make_shared<viewers::PersonsViewer>(runner, m_output_prefix);
            runner->Register(v, bind(&viewers::PersonsViewer::Update, v, placeholders::_1));
    }

    // Summary viewer
    if (m_config_pt.get<bool>("run.output_summary", false)) {
            m_stride_logger->info("Registering SummaryViewer");
            const auto v = make_shared<viewers::SummaryViewer>(runner, m_output_prefix);
            runner->Register(v, bind(&viewers::SummaryViewer::Update, v, placeholders::_1));
    }

    // Map viewer
    if (m_config_pt.get<bool>("run.output_map", false)) {
            m_stride_logger->info("Registering MapViewer");
            const auto v = make_shared<viewers::MapViewer>(runner);
            runner->Register(v, bind(&viewers::MapViewer::Update, v, placeholders::_1));
    }
}

void GuiController::Setup()
{
        // -----------------------------------------------------------------------------------------
        // Check environment, deal with output_prefix (i.e. make the directory iff the
        // prefix contains at least one /, make a logger and register it.
        // -----------------------------------------------------------------------------------------
        CheckEnv();
        CheckOpenMP();
        CheckOutputPrefix();
        MakeLogger();
        spdlog::register_logger(m_stride_logger);

        // -----------------------------------------------------------------------------------------
        // Log the setup.
        // -----------------------------------------------------------------------------------------
        m_stride_logger->info("CliController stating up at: {}", TimeStamp().ToString());
        m_stride_logger->info("Executing revision {}", ConfigInfo::GitRevision());
        m_stride_logger->info("Using configuration file:  {}", m_config_path.string());
        m_stride_logger->debug("Creating dir:  {}", m_output_prefix);
        m_stride_logger->debug("Executing:           {}", FileSys::GetExecPath().string());
        m_stride_logger->debug("Current directory:   {}", FileSys::GetCurrentDir().string());
        if (m_use_install_dirs) {
                m_stride_logger->debug("Install directory:   {}", FileSys::GetRootDir().string());
                m_stride_logger->debug("Config  directory:   {}", FileSys::GetConfigDir().string());
                m_stride_logger->debug("Data    directory:   {}", FileSys::GetDataDir().string());
        }
        if (ConfigInfo::HaveOpenMP()) {
                m_stride_logger->info("Max number OpenMP threads in this environment: {}", m_max_num_threads);
                m_stride_logger->info("Configured number of threads: {}",
                                      m_config_pt.get<unsigned int>("run.num_threads"));
        } else {
                m_stride_logger->info("Not using OpenMP threads.");
        }

        // -----------------------------------------------------------------------------------------
        // Write ptree to file
        // -----------------------------------------------------------------------------------------
        const auto path     = FileSys::BuildPath(m_output_prefix, "config.xml");
        boost::property_tree::write_xml(path.string(), m_config_pt, std::locale(), xml_writer_make_settings<std::string>('\t', 1));

        // -----------------------------------------------------------------------------------------
        // Create the runner, population and register the viewers
        // -----------------------------------------------------------------------------------------
        m_runner = make_shared<SimRunner>(m_config_pt, m_pop);
        RegisterViewers(m_runner);
}

int GuiController::getCurrentDay()
{
    return m_runner->GetSim()->GetCalendar()->GetSimulationDay();
}

int GuiController::getTotalDays()
{
    return m_config_pt.get<unsigned int>("run.num_days");
}

bool GuiController::simulationDone()
{
    return getCurrentDay() >= getTotalDays();
}

bool GuiController::setupGenDirectory()
{
    try {
        CheckOutputPrefix();
        m_gendir = new GenDirectory(m_config_pt);
        m_pop = Population::CreateEmpty(m_config_pt);
        state = GuiState::ConfigFileSelected;
        return true;
    }
    catch (...) {
        state = GuiState::Empty;
        return false;
    }
}

bool GuiController::GeoGen()
{
    try {
        geogen::Generate(*m_gendir, m_pop);
        state = GuiState::GeoGenerated;
        return true;
    }
    catch (...) {
        state = GuiState::Empty;
        return false;
    }
}

bool GuiController::PopGen()
{
    try 
    {
        popgen::Generate(*m_gendir, m_pop);
        state = GuiState::PopGenerated;
        return true;
    }
    catch (...)
    {
        state = GuiState::Empty;
        return false;
    }
}

std::map<unsigned int, GeoGridFilePtr>& GuiController::GetGeoGridFile()
{
    return m_gendir->GetGeoGridFile();
}

std::map<unsigned int, SchoolFilePtr>& GuiController::GetSchoolFile()
{
    return m_gendir->GetSchoolFile();
}

std::map<unsigned int, UniversityFilePtr>& GuiController::GetUniversityFile()
{
    return m_gendir->GetUniversityFile();
}

std::map<unsigned int, WorkplaceFilePtr>& GuiController::GetWorkplaceFile()
{
    return m_gendir->GetWorkplaceFile();
}

std::map<unsigned int, CommunityFilePtr>& GuiController::GetCommunityFile()
{
    return m_gendir->GetCommunityFile();
}

PopulationFilePtr GuiController::GetPopulationFile()
{
    return m_gendir->GetPopulationFile();
}

int GuiController::GetAmountOfRegions()
{
    return m_gendir->GetRegions().size();
}

} // namespace stride

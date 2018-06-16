#pragma once
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

#include "util/Stopwatch.h"
#include "gen/files/GenDirectory.h"

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace stride {

class SimRunner;

// User has to follow these steps
enum GuiState {
        Empty,                          // Nothing has been done yet
        ConfigFileSelected,             // Config file has been selected
        GeoGenerated,                   // Geo has been generated
        PopGenerated                    // Pop has been generated
};

class GuiController
{
public:
        /// Straight initialization.
        explicit GuiController();

        /// Assigning a PTree
        // This happens outside of the initialization because the GuiController exists
        // way before a ptree is available, since it is created in the MainWindow.
        void AssignPTree(boost::property_tree::ptree pt);

        /// Actual run of the simulator.
        void RunStride();
        void RunStride(int steps);

        /// Setup the controller.
        void Setup();

        /// Get the current day (used by Qt windows)
        int getCurrentDay();

        /// Get the total days (used by Qt windows)
        int getTotalDays();

        /// Returns true if simulation is over.
        bool simulationDone();

        /// Setup gendirectory
        void setupGenDirectory(boost::property_tree::ptree &pt);

        std::shared_ptr<SimRunner> GetRunner() { return m_runner; }

public:
        GuiState state;

private:
        /// Check install environment.
        void CheckEnv();

        /// Check the OpenMP environment.
        void CheckOpenMP();

        // Output_prefix: if it's a string not containing any / it gets interpreted as a
        // filename prefix; otherwise we 'll create the corresponding directory.
        void CheckOutputPrefix();

        /// Make the appropriate logger for cli environment and register as stride_logger.
        void MakeLogger();

        /// Register the viewers of the SimRunner.
        void RegisterViewers(std::shared_ptr<SimRunner> runner);

private:
        std::string                                       m_config_file; /// Config parameters file name.
        std::vector<std::tuple<std::string, std::string>> m_p_overrides; /// Cli overides of config parameters.

        std::string m_stride_log_level; /// Log level (see spdlog::level in spdlog/common.h).
        bool        m_use_install_dirs; /// Working dir or install dir mode.

        unsigned int      m_max_num_threads; /// Max number  of OpenMP threads.
        std::string       m_output_prefix;   /// Prefix to output (name prefix or prefix dir)
        util::Stopwatch<> m_run_clock;       ///< Stopwatch for timing the computation.

        boost::filesystem::path         m_config_path;   ///< path to config file.
        boost::property_tree::ptree     m_config_pt;     ///< Main configuration for run and sim.
        std::shared_ptr<spdlog::logger> m_stride_logger; ///< General logger.

        std::shared_ptr<SimRunner> m_runner;
        gen::files::GenDirectory *m_gendir;
        std::shared_ptr<Population> m_pop;

};

} // namespace stride

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
 *  Copyright 2017, 2018 Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Implementation of tests to check the relation R0-attack rate.
 */

#include "gen/geogen/GeoConfiguration.h"
#include "gen/geogen/builders/GeoGridBuilder.h"
#include "gen/geogen/builders/SchoolsBuilder.h"
#include "gen/geogen/builders/UniversitiesBuilder.h"
#include "gen/geogen/builders/CommunitiesBuilder.h"
#include "gen/geogen/builders/WorkplacesBuilder.h"
#include "gen/geogen/output/GeoGridFile.h"
#include "util/CSV.h"

#include <gtest/gtest.h>
#include <omp.h>
#include <spdlog/spdlog.h>
#include <tuple>
#include <boost/filesystem.hpp>

namespace Tests {

using namespace std;
using namespace stride;
using namespace ::testing;

using boost::property_tree::ptree;

class GeoGenTests : public ::testing::TestWithParam<tuple<const char*, unsigned int>>
{
public:
        /// TestCase set up.
        static void SetUpTestCase() {}

        /// Tearing down TestCase
        static void TearDownTestCase() {}

        /// Basic config
        static ptree BasicConfig()
        {
                static ptree pt_config;
                if (pt_config.empty()) {
                        pt_config.put("Config.rng.engine", "mrg2");
                        pt_config.put("Config.rng.seed", 1);
                        pt_config.put("Config.househould_profile", "../data/households_flanders.xml");
                        pt_config.put("Config.population_size", 100000);
                        pt_config.put("Config.geoprofile.cities", "../data/flanders_cities2.csv");
                        pt_config.put("Config.geoprofile.commuters", "../data/flanders_commuting.csv");
                        pt_config.put("Config.university.fraction", 0.25);
                        pt_config.put("Config.work.fraction", 0.70);
                        pt_config.put("Config.work.commute_fraction", 0.70);
                }
                return pt_config;
        }

        /// Scenario config and test target number and margin of tolerance in percent (of target value).
        static tuple<ptree, unsigned int, double> ScenarioData(const string& tag)
        {
                ptree        pt     = BasicConfig();
                unsigned int target = 0U;
                double       margin = 0.0;

                if (tag == "geogrid") {
                        target = 327;
                }
                if (tag == "communities") {
                        target = 50;
                }
                if (tag == "schools") {
                        target = 50;
                }
                if (tag == "universities") {
                        target = 8;
                }
                if (tag == "workplaces") {
                        target = 3500;
                }

                return make_tuple(pt, target, margin);
        }

protected:
        /// Destructor has to be virtual.
        ~GeoGenTests() override {}

        /// Set up for the test fixture
        void SetUp() override {}

        /// Tearing down the test fixture
        void TearDown() override {}
};

TEST_P(GeoGenTests, Run)
{
        tuple<const char*, unsigned int> t(GetParam());

        // -------------------------------------------------------------------------------------
        // Prepare test configuration.
        // -------------------------------------------------------------------------------------
        const string test_tag{get<0>(t)};
        const auto   num_threads = get<1>(t);
        omp_set_num_threads(num_threads);
        omp_set_schedule(omp_sched_static, 1);

        // -------------------------------------------------------------------------------------
        // Scenario test configuration and target number.
        // -------------------------------------------------------------------------------------
        const auto d         = ScenarioData(test_tag);
        auto       pt_config = get<0>(d);
        const auto target    = get<1>(d);
        const auto margin    = get<2>(d);

        // -------------------------------------------------------------------------------------
        // Initialize the logger.
        // -------------------------------------------------------------------------------------
        spdlog::set_async_mode(1048576);
        auto file_logger = spdlog::rotating_logger_mt("contact_logger", "test_logfile", numeric_limits<size_t>::max(),
                                                      numeric_limits<size_t>::max());
        file_logger->set_pattern("%v"); // Remove meta data from log => time-stamp of logging

        // -----------------------------------------------------------------------------------------
        // Initialize the simulator.
        // -----------------------------------------------------------------------------------------
        cout << "Building the configuration. " << endl;
        cout << " ----> test_tag: " << test_tag << endl << " ----> threadcount:  " << num_threads << endl;
        auto config = stride::gen::GeoConfiguration(pt_config, num_threads);
        cout << "Done building config. " << endl;

        // SimulatorBuilder builder(pt_config);
        // const auto       sim = builder.Build();
        // cout << "Done building the generator" << endl;

        // -----------------------------------------------------------------------------------------
        // Config.the simulation and release logger.
        // -----------------------------------------------------------------------------------------
        // const unsigned int num_days = pt_config.get<unsigned int>("Config.num_days");
        // for (unsigned int i = 0; i < num_days; i++) {
        //         sim->TimeStep();
        // }
        std::shared_ptr<stride::gen::GeoGrid> geogrid = stride::gen::GeoGridBuilder::build(config);
        cout << "Done building geogrid. " << endl;

        // Write
        boost::filesystem::path out_dir = "output/";
        try {
                boost::filesystem::create_directories(out_dir);
        } catch (std::exception& e) {
                std::cout << "test::"+test_tag+"> Exception while creating output directory:  {}", e.what();
                throw;
        }
        std::string filename;
        if (test_tag == "geogrid") {
            filename = out_dir.string()+"/Geogrid.csv";
            writefiles(geogrid, geogrid, filename);
        }
        if (test_tag == "communities") {
            std::shared_ptr<std::vector<stride::gen::Community>> communities = stride::gen::CommunitiesBuilder::build(config, geogrid);
            filename = out_dir.string()+"/Communities.csv";
            writefiles(communities, geogrid, filename);
        }
        if (test_tag == "schools") {
            std::shared_ptr<std::vector<stride::gen::School>> schools = stride::gen::SchoolsBuilder::build(config, geogrid);
            filename = out_dir.string()+"/Schools.csv";
            writefiles(schools, geogrid, filename);
        }
        if (test_tag == "universities") {
            std::shared_ptr<std::vector<stride::gen::University>> universities = stride::gen::UniversitiesBuilder::build(config, geogrid);
            filename = out_dir.string()+"/Universities.csv";
            writefiles(universities, geogrid, filename);
        }
        if (test_tag == "workplaces") {
            std::shared_ptr<std::vector<stride::gen::WorkPlace>> workplaces = stride::gen::WorkplacesBuilder::build(config, geogrid);
            filename = out_dir.string()+"/Workplaces.csv";
            writefiles(workplaces, geogrid, filename);
        }

        util::CSV data = util::CSV(filename);

        spdlog::drop_all();

        // -----------------------------------------------------------------------------------------
        // Check resuts against target number.
        // -----------------------------------------------------------------------------------------
        const unsigned int res = data.size();
        EXPECT_NEAR(res, target, target * margin) << "!! CHANGES for " << test_tag << "with threads: " << num_threads;
}

namespace {
const char* tags[] = {"geogrid", "schools", "communities", "universities", "workplaces"};

#ifdef _OPENMP
unsigned int threads[]{1U, 4U};
#else
unsigned int threads[]{1U};
#endif

} // namespace

INSTANTIATE_TEST_CASE_P(Run, GeoGenTests, Combine(ValuesIn(tags), ValuesIn(threads)));

} // namespace Tests

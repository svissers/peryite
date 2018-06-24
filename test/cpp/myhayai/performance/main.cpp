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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

#include "myhayai/BenchmarkRunner.hpp"
#include "myhayai/CliController.hpp"
#include "myhayai/Test.hpp"

#include "pop/Population.h"
#include "sim/CliController.h"

#include "util/FileSys.h"
#include "util/RunConfigManager.h"
#include "util/StringUtils.h"
#include "util/TimeStamp.h"

#include <exception>
#include <iostream>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::property_tree;
using namespace myhayai;

int main(int argc, char** argv)
{
        int exit_status = EXIT_SUCCESS;

        auto sim_factory_builder = [](
            double student_fraction, double student_commute_fraction,
            double work_fraction, double work_commute_fraction,
            unsigned int school_size, unsigned int university_size,
            unsigned int workplace_size, unsigned int school_cp_size,
            unsigned int university_cp_size, unsigned int population_size,
            bool fragment_centers=true,
            unsigned int num_threads=1, unsigned int num_days=50, double seeding_rate=0.002, 
            double vaccine_link_probability=0, double vaccine_rate=0.8,
            double r0=11,
            double tourist_fraction = 0.10, double work_travel_fraction = 0.05)
            {
                return [
                        student_fraction, student_commute_fraction,
                        work_fraction, work_commute_fraction,
                        school_size, university_size,
                        workplace_size, school_cp_size, university_cp_size, population_size,
                        fragment_centers,
                        num_threads, num_days,
                        seeding_rate, vaccine_link_probability,
                        vaccine_rate, r0, tourist_fraction, work_travel_fraction]() 
                    {
                    
                    // Get configuration
                    auto  config = "run_performance.xml";
                    ptree configPt;

                    const boost::filesystem::path configPath =
                        FileSys::GetConfigDir() /= config;
                    configPt = FileSys::ReadPtreeFile(configPath);



                    // Turn off contact output file
                    configPt.put(
                        "run.contact_output_file",
                        false
                    );

                    // Turn off logging
                    configPt.put(
                        "run.stride_log_level",
                        "None"
                    );

                    // Student fraction
                    configPt.put(
                        "run.pop_config.region.university.student_fraction",
                        student_fraction
                    );

                    // Student commute 
                    configPt.put(
                        "run.pop_config.region.university.commute_fraction",
                        student_commute_fraction
                    );

                    // Work fraction
                    configPt.put(
                        "run.pop_config.region.work.work_fraction",
                        work_fraction
                    );

                    // Work commute
                    configPt.put(
                        "run.pop_config.region.work.commute_fraction",
                        work_commute_fraction
                    );

                    // School size
                    configPt.put(
                        "run.pop_config.region.school_size",
                        school_size
                    );

                    // University size
                    configPt.put(
                        "run.pop_config.region.university_size",
                        university_size
                    );

                    // Workplace size
                    configPt.put(
                        "run.pop_config.region.workplace_size",
                        workplace_size
                    );

                    // School cp size
                    configPt.put(
                        "run.pop_config.region.school_cp_size",
                        school_cp_size
                    );

                    // University cp size
                    configPt.put(
                        "run.pop_config.region.university_cp_size",
                        university_cp_size
                    );

                    // Population_size
                    configPt.put(
                        "run.pop_config.region.population_size",
                        population_size
                    );

                    // Fragment centers
                    configPt.put(
                        "run.pop_config.region.fragment_centers",
                        fragment_centers
                    );

                    // Num_threads
                    configPt.put(
                        "run.num_threads",
                        num_threads
                    );

                    // Num_days
                    configPt.put(
                        "run.num_days",
                        num_days
                    );

                    // seeding_rate
                    configPt.put(
                        "run.seeding_rate",
                        seeding_rate
                    );

                    // vaccine_link_probability
                    configPt.put(
                        "run.vaccine_link_probability",
                        vaccine_link_probability
                    );

                    // vaccine_rate
                    configPt.put(
                        "run.vaccine_rate",
                        vaccine_rate
                    );

                    // r0
                    configPt.put(
                        "run.r0",
                        r0
                    );

                    // tourist_fraction
                    configPt.put(
                        "run.pop_config.region.tourist_fraction",
                        tourist_fraction
                    );

                    // work_travel_fraction
                    configPt.put(
                        "run.pop_config.region.work_travel_fraction",
                        work_travel_fraction
                    );

                    // Check output
                    if (configPt.get<string>("run.output_prefix", "").empty()) {
                        configPt.put(
                            "run.output_prefix",
                            "PerfSim" + TimeStamp().ToTag().append("/"));
                    }
                    configPt.sort();                

                    return Test(
                        [configPt]() {
                            stride::CliController(configPt).Control();
                        });
                };
        };
       // Population size
       for (double i = 100000; i < 1100000; i += 100000) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "No_fragment - " + ToString(i) + " ",
                2,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, i, false));    
        } 
       // Population size
       for (double i = 100000; i < 1100000; i += 100000) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "fragment - " + ToString(i) + " ",
                2,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, i, true));    
        } 
/*
        // Student fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim", "Student_fraction - " + ToString(i) + " ",
                3,
                sim_factory_builder(i, 0.5, 0.7, 0.5, 500, 3000, 20, 20, 20, 500000));
        }

        // Student commute fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim", "Student_commute_fraction - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, i, 0.7, 0.5, 500, 3000, 20, 20, 20, 500000));
        }

        // Work fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim", "work_fraction - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, i, 0.5, 500, 3000, 20, 20, 20, 500000));
        }

        // Work commute fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim", "work_commute_fraction - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, 0.7, i, 500, 3000, 20, 20, 20, 500000));
        }

        // School size
        for (double i = 100; i < 5000; i += 500) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "school_size - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, i, 3500, 20, 20, 20, 500000));    
        } 
        // University size
        for (double i = 100; i < 10000; i += 1000) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "university_size - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, i, 20, 20, 20, 500000));    
        }  
        
        // Workplace size
        for (double i = 5; i < 1000; i += 100) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "Workplace_size - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, i, 20, 20, 500000));    
        } 

        // School cp
        for (double i = 1; i < 200; i += 20) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "School_cp_size - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, i, 20, 500000));    
        } 

       // University cp  
       for (double i = 1; i < 200; i += 20) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "University_cp_size - " + ToString(i) + " ",
                3,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, i, 500000));    
        }  
        
       // Population size
       for (double i = 10000; i < 600000; i += 50000) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "Population_size - " + ToString(i) + " ",
                5,
                sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, i));    
        } 
        // Fragment centers
        BenchmarkRunner::RegisterTest(
            "Sim",
            "No fragment centers",
            5,
            sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, false)); 
        BenchmarkRunner::RegisterTest(
            "Sim",
            "Fragment centers",
            5,
            sim_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true)); 
   
        // Num days
        for (unsigned int i = 200; i <= 700; i += 100) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "Num_days - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, i,
                    0.002, 0, 0.8,
                    11));    
        } 

        // Seeding rate
        for (double i = 0.002; i < 0.006; i += 0.001) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "Seeding rate - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, 50,
                    i, 0, 0.8,
                    11));    
        } 

        // Vaccine_link_probability
        for (double i = 0; i < 0.005; i += 0.001) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "vaccine_link_probability - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, 50,
                    0.002, i, 0.8,
                    11));    
        } 

        // Vaccine_rate
        for (double i = 0.4; i < 0.9; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "vaccine_rate - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, 50,
                    0.002, 0, i,
                    11));    
        } 

        // r0
        for (unsigned int i = 5; i < 15; i += 1) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "r0 - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, 50,
                    0.002, 0, 0.8,
                    i));    
        } 

        // tourist_fraction
        for (double i = 0.1; i < 0.9; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "tourist_fraction - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, 50,
                    0.002, 0, 0.8,
                    11, i));    
        } 

        // work_travel_fraction
        for (double i = 0.1; i < 0.7; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "work_travel_fraction - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true,
                    1, 50,
                    0.002, 0, 0.8,
                    11, 0.10, i));    
        } 
*/
        // Run tests
        try {
                myhayai::CliController controller;
                controller.ParseArgs(argc, argv);
                controller.Control();

        } catch (exception& e) {
                exit_status = EXIT_FAILURE;
                cerr << "\nEXCEPION THROWN: " << e.what() << endl;
        } catch (...) {
                exit_status = EXIT_FAILURE;
                cerr << "\nEXCEPION THROWN: Unknown exception." << endl;
        }

        return exit_status;
}

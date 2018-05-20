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
 *
 *  This software has been (completely) rewritten starting from
 *  the hayai code by Nick Bruun. The original copyright, to be
 *  found in this directory still aplies.
 */
/**
 * @file
 * Impleme.
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

        auto geopop_factory_builder = [](
            double student_fraction, double student_commute_fraction,
            double work_fraction, double work_commute_fraction,
            unsigned int school_size, unsigned int university_size,
            unsigned int workplace_size, unsigned int school_cp_size,
            unsigned int university_cp_size, unsigned int population_size,
            bool fragment_centers, string postfix)
            {
                return [
                        student_fraction, student_commute_fraction,
                        work_fraction, work_commute_fraction,
                        school_size, university_size,
                        workplace_size, school_cp_size,
                        university_cp_size, population_size,
                        fragment_centers, postfix]() 
                    {
                    // Get configuration
                    auto  config = "run_geopop.xml";
                    ptree configPt;

                    const boost::filesystem::path configPath =
                        FileSys::GetConfigDir() /= config;
                    configPt = FileSys::ReadPtreeFile(configPath);

                    // Turn off contact output file
                    configPt.put(
                        "run.contact_output_file",
                        false
                    );

                    // Student fraction
                    configPt.put(
                        "run.pop_config1.university.student_fraction",
                        student_fraction
                    );

                    // Student commute 
                    configPt.put(
                        "run.pop_config1.university.commute_fraction",
                        student_commute_fraction
                    );

                    // Work fraction
                    configPt.put(
                        "run.pop_config1.work.work_fraction",
                        work_fraction
                    );

                    // Work commute
                    configPt.put(
                        "run.pop_config1.work.commute_fraction",
                        work_commute_fraction
                    );

                    // School size
                    configPt.put(
                        "run.pop_config1.school_size",
                        school_size
                    );

                    // University size
                    configPt.put(
                        "run.pop_config1.university_size",
                        university_size
                    );

                    // Workplace size
                    configPt.put(
                        "run.pop_config1.workplace_size",
                        workplace_size
                    );

                    // School cp size
                    configPt.put(
                        "run.pop_config1.school_cp_size",
                        school_cp_size
                    );

                    // University cp size
                    configPt.put(
                        "run.pop_config1.university_cp_size",
                        university_cp_size
                    );

                    // Population_size
                    configPt.put(
                        "run.pop_config1.population_size",
                        population_size
                    );

                    // Fragment centers
                    configPt.put(
                        "run.pop_config1.fragment_centers",
                        fragment_centers
                    );

                    configPt.put(
                        "run.pop_config1.geoprofile.cities",
                        "data/flanders_cities_TEST_" + postfix + ".csv"
                    );
                    configPt.put(
                        "run.pop_config1.geoprofile.commuters",
                        "data/flanders_commuting_TEST_" + postfix + ".csv"
                    );

                    // Check output
                    if (configPt.get<string>("run.output_prefix", "").empty()) {
                        configPt.put(
                            "run.output_prefix",
                            "Perf" + TimeStamp().ToTag().append("/"));
                    }
                    configPt.sort();                

                    return Test(
                        [configPt]() {
                            Population::Create(configPt);
                        });
                };
        };

        // FRACTION
        /*
        for (double i = 0.1; i < 0.5; i += 0.1) {
            for (double j = 0.1; j < 0.5; j += 0.1) {
                for (double k = 0.1; k < 0.7; k += 0.1) {
                    for (double l = 0.1; l < 0.5; l += 0.1) {
                        BenchmarkRunner::RegisterTest(
                            "Geopop", "Student_fraction - " + ToString(i) + " "
                            "Student_commute_fraction - " + ToString(j) + " "
                            "Work_fraction - " + ToString(k) + " "
                            "Work_commute_fraction - " + ToString(l),
                             10,
                            geopop_factory_builder(i, j, k, l, 500, 3000, 20));
                    }                
                }            
            }
        }
        */
        /*
        // Student fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Geopop", "Student_fraction - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(i, 0.5, 0.7, 0.5, 500, 3000, 20, 20, 20, 500000));
        }
        // Student commute fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Geopop", "Student_commute_fraction - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, i, 0.7, 0.5, 500, 3000, 20, 20, 20, 500000));
        }
        // Work fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Geopop", "work_fraction - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, i, 0.5, 500, 3000, 20, 20, 20, 500000));
        }
        // Work commute fraction
        for (double i = 0.05; i < 1; i += 0.1) {
            BenchmarkRunner::RegisterTest(
                "Geopop", "work_commute_fraction - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, 0.7, i, 500, 3000, 20, 20, 20, 500000));
        }
        */
        // SIZES
        /*
        for (double i = 200; i < 800; i += 100) {
            for (double j = 2500; j < 3500; j += 250) {
                for (double k = 1; k < 100; k += 10) {
                    BenchmarkRunner::RegisterTest(
                        "Geopop", "school_size - " + ToString(i) + " "
                        "university_size - " + ToString(j) + " "
                        "Workplace_size - " + ToString(k) + " ",
                        5,
                        geopop_factory_builder(0.5, 0.5, 0.7, 0.5, i, j, k));          
                }            
            }
        }
        */
        /*
        // School size
        for (double i = 100; i < 5000; i += 500) {
            BenchmarkRunner::RegisterTest(
                "Geopop",
                "school_size - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, 0.7, 0.5, i, 3500, 20, 20, 20, 500000));    
        } 
        // University size
        for (double i = 100; i < 10000; i += 1000) {
            BenchmarkRunner::RegisterTest(
                "Geopop",
                "university_size - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, i, 20, 20, 20, 500000));    
        }  
        // Workplace size
        for (double i = 5; i < 1000; i += 100) {
            BenchmarkRunner::RegisterTest(
                "Geopop",
                "Workplace_size - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, i, 20, 20, 500000));    
        } 

        // School cp
        for (double i = 1; i < 200; i += 20) {
            BenchmarkRunner::RegisterTest(
                "Geopop",
                "School_cp_size - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, i, 20, 20, 20, 500000));    
        } 
        // University cp  
       for (double i = 1; i < 200; i += 20) {
            BenchmarkRunner::RegisterTest(
                "Geopop",
                "University_cp_size - 500000 - " + ToString(i) + " ",
                3,
                geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, i, 20, 20, 500000));    
        }  
        */
        /*
        // Population size
       for (double i = 10000; i < 1000000; i += 50000) {
            BenchmarkRunner::RegisterTest(
                "Geopop",
                "Population_size - " + ToString(i) + " ",
                5,
                geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, i));    
        } 
        */
        /*
        // Fragment centers
        BenchmarkRunner::RegisterTest(
            "Geopop",
            "No fragment centers",
            5,
            geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, false)); 
        BenchmarkRunner::RegisterTest(
            "Geopop",
            "Fragment centers",
            5,
            geopop_factory_builder(0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 500000, true));    
        */

        for (double i = 10000; i <= 310000; i += 100000) {
            for (double j = 0.1; j < 0.8; j += 0.1) {
                for (auto postfix : {"40", "75", "150"}) {
                    BenchmarkRunner::RegisterTest(
                        "Geopop",
                        ToString(i) + ", " + ToString(j) + ", " + postfix,
                        1,
                        geopop_factory_builder(
                            0.5, 0.5, 0.7, j, 500, 3500, 20, 20, 20, i, true, postfix)); 
                }
            }
        } 
        
        // GEOGRID
        auto geopop_geogrid_builder = [](string postfix)
            {
                return [postfix]() 
                    {
                    // Get configuration
                    auto  config = "run_geopop.xml";
                    ptree configPt;

                    const boost::filesystem::path configPath =
                        FileSys::GetConfigDir() /= config;
                    configPt = FileSys::ReadPtreeFile(configPath);

                    // Turn off contact output file
                    configPt.put(
                        "run.contact_output_file",
                        false
                    );

                    // Population_size
                    configPt.put(
                        "run.pop_config1.population_size",
                        500000
                    );

                    configPt.put(
                        "run.pop_config1.geoprofile.cities",
                        "data/flanders_cities_TEST_" + postfix + ".csv"
                    );
                    configPt.put(
                        "run.pop_config1.geoprofile.commuters",
                        "data/flanders_commuting_TEST_" + postfix + ".csv"
                    );

                    // Check output
                    if (configPt.get<string>("run.output_prefix", "").empty()) {
                        configPt.put(
                            "run.output_prefix",
                            "Perf" + TimeStamp().ToTag().append("/"));
                    }
                    configPt.sort();                

                    return Test(
                        [configPt]() {
                            Population::Create(configPt);
                        });
                };
        };  
        // 150
        BenchmarkRunner::RegisterTest(
        "Geopop",
        "Geogrid_file - 500000 pop - flanders_cities_TEST_150",
        5,
        geopop_geogrid_builder("150"));   

        /*
        // 75
        BenchmarkRunner::RegisterTest(
        "Geopop",
        "Geogrid_file - 500000 pop -  flanders_cities_TEST_75",
        5,
        geopop_geogrid_builder("75"));   

        // 40
        BenchmarkRunner::RegisterTest(
        "Geopop",
        "Geogrid_file - 500000 pop -  flanders_cities_TEST_40",
        5,
        geopop_geogrid_builder("40"));   
        */
        auto sim_factory_builder = [](
            double student_fraction, double student_commute_fraction,
            double work_fraction, double work_commute_fraction,
            unsigned int school_size, unsigned int university_size,
            unsigned int workplace_size, unsigned int school_cp_size,
            unsigned int university_cp_size, unsigned int population_size,
            unsigned int num_threads, unsigned int num_days, double seeding_rate, 
            double vaccine_link_probability, double vaccine_rate,
            double r0)
            {
                return [
                        student_fraction, student_commute_fraction,
                        work_fraction, work_commute_fraction,
                        school_size, university_size,
                        workplace_size, school_cp_size, university_cp_size, population_size,
                        num_threads, num_days,
                        seeding_rate, vaccine_link_probability,
                        vaccine_rate, r0]() 
                    {
                    
                    // Get configuration
                    auto  config = "run_geopop.xml";
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
                        "run.pop_config1.university.student_fraction",
                        student_fraction
                    );

                    // Student commute 
                    configPt.put(
                        "run.pop_config1.university.commute_fraction",
                        student_commute_fraction
                    );

                    // Work fraction
                    configPt.put(
                        "run.pop_config1.work.work_fraction",
                        work_fraction
                    );

                    // Work commute
                    configPt.put(
                        "run.pop_config1.work.commute_fraction",
                        work_commute_fraction
                    );

                    // School size
                    configPt.put(
                        "run.pop_config1.school_size",
                        school_size
                    );

                    // University size
                    configPt.put(
                        "run.pop_config1.university_size",
                        university_size
                    );

                    // Workplace size
                    configPt.put(
                        "run.pop_config1.workplace_size",
                        workplace_size
                    );

                    // School cp size
                    configPt.put(
                        "run.pop_config1.school_cp_size",
                        school_cp_size
                    );

                    // University cp size
                    configPt.put(
                        "run.pop_config1.university_cp_size",
                        university_cp_size
                    );

                    // Population_size
                    configPt.put(
                        "run.pop_config1.population_size",
                        population_size
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
        /*
        // Num threads
        for (unsigned int i = 1; i < 8; i += 1) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "Num_threads - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 100000,
                    i, 50,
                    0.002, 0, 0.8,
                    11));    
        } 

        // Num days
        for (unsigned int i = 200; i <= 700; i += 100) {
            BenchmarkRunner::RegisterTest(
                "Sim",
                "Num_days - " + ToString(i) + " ",
                3,
                sim_factory_builder(
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 100000,
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
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 100000,
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
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 100000,
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
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 100000,
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
                    0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, 100000,
                    1, 50,
                    0.002, 0, 0.8,
                    i));    
        } 
        */
        
        for (double i = 200; i <= 700; i += 100) {
            for (double j = 1; j < 8; j += 1) {
                for (double k = 100000; k <= 400000; k += 100000) {
                    BenchmarkRunner::RegisterTest(
                        "Sim",
                        ToString(i) + ", " + ToString(j) + ", " + ToString(k),
                        1,
                        sim_factory_builder(
                            0.5, 0.5, 0.7, 0.5, 500, 3500, 20, 20, 20, k,
                            j, i,
                            0.002, 0, 0.8,
                            11));   
                }
            }
        } 

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

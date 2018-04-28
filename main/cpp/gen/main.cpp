/*
 * @file
 * Main program for the generation of geogrids and a population.
 */
#include "util/ConfigInfo.h"
#include "util/InstallDirs.h"
#include "util/TimeStamp.h"
#include "util/RunConfigManager.h"
#include "pop/Population.h"
#include "pool/ContactPoolSys.h"
#include "geogen/GeoGenerator.h"
#include "popgen/PopGenerator.h"
#include "GenConfiguration.h"
#include "files/GenDirectory.h"


#include <tclap/CmdLine.h>
#include <omp.h>
#include <memory>

using namespace std;
using namespace stride::gen;
using namespace stride::gen::files;
using namespace stride::util;
using namespace TCLAP;
using boost::filesystem::path;

int main(int argc, char* argv[])
{
        int exit_status = EXIT_SUCCESS;
        try {
                // -----------------------------------------------------------------------------------------
                // Starting up ....
                // -----------------------------------------------------------------------------------------
                cout << "\n*************************************************************" << endl;
                cout << "Starting up at:      " << TimeStamp().ToString() << endl << endl;

                // -----------------------------------------------------------------------------------------
                // Parse command line.
                // -----------------------------------------------------------------------------------------
                CmdLine cmd("generator", ' ', "1.0", false);
                ValueArg<string> config_file_Arg("c", "config", "Config File", false, "peryite_generator.xml",
                                                 "CONFIGURATION FILE", cmd);
                SwitchArg geo_switch("g","geo","Generate the geogrid", cmd, false);
                SwitchArg pop_switch("p","pop","Generate the population", cmd, false);
                cmd.parse(argc, static_cast<const char* const*>(argv));
                string config_file_name = config_file_Arg.getValue();
                bool generate_geo = geo_switch.getValue();
                bool generate_pop = pop_switch.getValue();
                // -----------------------------------------------------------------------------------------
                // Check exec environment and configuration file
                // -----------------------------------------------------------------------------------------
                InstallDirs::Print(cout);
                InstallDirs::Check();
                const path file_path = InstallDirs::GetConfigDir() / path(config_file_name);
                if (!is_regular_file(file_path)) {
                        throw runtime_error(string(__func__) + ">Config file " + file_path.string() +
                                            " not present. Aborting.");
                }
                // -----------------------------------------------------------------------------------------
                // Parallellization
                // -----------------------------------------------------------------------------------------
                unsigned int num_threads = 1;
                if (ConfigInfo::HaveOpenMP()) {
                    #pragma omp parallel
                    {
                            num_threads = static_cast<unsigned int>(omp_get_num_threads());
                    }
                    cout << "\nUsing OpenMP threads:  " << num_threads << endl;
                } else {
                    cout << "\nNot using parallellization" << endl;
                }

                // -----------------------------------------------------------------------------------------
                // Initialize the directory for the given configuration
                // -----------------------------------------------------------------------------------------
                auto configPt = RunConfigManager::Create(config_file_name);
                GenDirectory dir = GenDirectory(configPt, num_threads, "output");

                // -----------------------------------------------------------------------------------------
                // Run Generator-Geo
                // -----------------------------------------------------------------------------------------
                if (generate_geo) {
                    geogen::Generate(dir);
                }
                // -----------------------------------------------------------------------------------------
                // Run Generator-Pop
                // -----------------------------------------------------------------------------------------
                if (generate_pop) {
                    auto population = make_shared<stride::Population>();
                    stride::ContactPoolSys pool_sys;
                    popgen::Generate(dir, population, pool_sys, true);
                }
                // -----------------------------------------------------------------------------------------
                // Finished
                // -----------------------------------------------------------------------------------------
                cout << "\nFinished at:      " << TimeStamp().ToString() << endl;
                cout << "*************************************************************" << endl;
        } catch (std::exception& e) {
                exit_status = EXIT_FAILURE;
                cerr << "\nEXCEPTION THROWN: " << e.what() << endl;
        } catch (...) {
                exit_status = EXIT_FAILURE;
                cerr << "\nEXCEPTION THROWN: "
                     << "Unknown exception." << endl;
        }
        return exit_status;
}

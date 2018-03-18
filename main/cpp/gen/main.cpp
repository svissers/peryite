/*
 * @file
 * Main program for the generation of geogrids and a population.
 */
#include "../util/ConfigInfo.h"
#include "../util/InstallDirs.h"
#include "../util/TimeStamp.h"
#include "gen/geogen/GeoGenerator.h"

#include <tclap/CmdLine.h>
#include <omp.h>

//namespace stride {
//namespace gen {

using namespace std;
using namespace stride::util;
using namespace TCLAP;
using boost::filesystem::path;
//} // namespace gen
//} // namespace stride
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
                ValueArg<string> generator_type_Arg("g", "generator", "Generator type (geo or pop)", false, "geo",
                                                    "GENERATOR TYPE", cmd);
                cmd.parse(argc, static_cast<const char* const*>(argv));
                string config_file_name = config_file_Arg.getValue();
                string generator_type = generator_type_Arg.getValue();

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
                // Run Generator-Geo
                // -----------------------------------------------------------------------------------------
                if (generator_type == "geo") {
                        stride::gen::GeoGenerator::generate(config_file_name, num_threads);
                }

                // -----------------------------------------------------------------------------------------
                // Run Generator-Pop
                // -----------------------------------------------------------------------------------------
                if (generator_type == "pop") {
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

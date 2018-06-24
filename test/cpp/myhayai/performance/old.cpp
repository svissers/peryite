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
                            geopop_factory_builder(i, j, k, l, 500, 3000));
                    }                
                }            
            }
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
        // Reference household file
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
        */

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
        */

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
        /*
        // 150
        BenchmarkRunner::RegisterTest(
        "Geopop",
        "Geogrid_file - 500000 pop - flanders_cities_TEST_150",
        5,
        geopop_geogrid_builder("150"));   
        */
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

        /*
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
        */


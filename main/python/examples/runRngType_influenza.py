import itertools
import os
import random
import sys
import csv
import pystride
from pystride.Simulation import Simulation

# RNG types to test
# Note: Default (mrg2) -- See results from previous test
RNG_TYPES = ["lcg64", "yarn2", "lcg64_shift", "mrg3", "yarn3"]

amount_of_runs = 1

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("../config/run_batch_default.xml")
simulation.runConfig.setParameter("output_prefix", "BatchRngTypeTesting")

# Initialize the random values
rng_values = []
for i in range(amount_of_runs):
    rng_values.append(random.randint(0, 4294967294))

## Create Forks
for rng_type in RNG_TYPES:
    for rng_value in rng_values:
        # Influenza_a
        forka = simulation.fork("influenza_a_" + rng_type + "_" + str(rng_value))
        forka.runConfig.setParameter("rng_type", rng_type)
        forka.runConfig.setParameter("rng_seed", rng_value)
        # influenza_b
        forkb = simulation.fork("influenza_b_" + rng_type + "_" + str(rng_value))
        forkb.runConfig.setParameter("seeding_rate", 0)
        forkb.runConfig.setParameter("rng_type", rng_type)
        forkb.runConfig.setParameter("rng_seed", rng_value)
        # influenza_c
        forkc = simulation.fork("influenza_c_" + rng_type + "_" + str(rng_value))
        forkc.runConfig.setParameter("seeding_rate", (1 - 0.9991) / 100)
        forkc.runConfig.setParameter("immunity_rate", 0.9991)
        forkc.runConfig.setParameter("rng_type", rng_type)
        forkc.runConfig.setParameter("rng_seed", rng_value)

# Run all forks
simulation.runForks()

# Print related data
def print_test(name, test_type):
    results_file = os.path.join(pystride.workspace, "totalResults.txt")
    with open(results_file, "a+") as results:
        for rng_type in RNG_TYPES:
            print(name + ": " + rng_type)
            results.write(name + ": " + rng_type + "\n")
            for rng_value in rng_values:
                summary_file = os.path.join(pystride.workspace, "BatchRngTypeTesting", name + "_" + rng_type + "_" + str(rng_value), "summary.csv")
                with open(summary_file) as csvfile:
                    reader = csv.DictReader(csvfile)
                    line = "RNG = " + str(rng_value) + ": " + test_type + " = " + str(next(reader)[test_type])
                    print(line)
                    results.write(line + "\n")

# num cases
print_test("influenza_a", "num_cases")
print_test("influenza_b", "num_cases")
print_test("influenza_c", "num_cases")

import itertools
import os
import random
import csv
import pystride
from pystride.Simulation import Simulation

# RNG types to test
# Note: Default (mrg2) -- See results from previous test
RNG_TYPES = ["lcg64", "yarn2", "lcg64_shift", "mrg3", "yarn3"]
# Amount of random values to test
AMOUNT_OF_VALUES = 1

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("../config/run_batch_measles.xml")
simulation.runConfig.setParameter("output_prefix", "BatchRngTypeTesting")

# Initialize the random values
rng_values = []
for i in range(AMOUNT_OF_VALUES):
    rng_values.append(random.randint(0, 4294967294))
## Create Forks
for rng_type in RNG_TYPES:
    for rng_value in rng_values: 
        # measles_16
        fork16 = simulation.fork("measles_16_" + rng_type + "_" + str(rng_value))
        fork16.runConfig.setParameter("r0", 16)
        fork16.runConfig.setParameter("rng_type", rng_type)
        fork16.runConfig.setParameter("rng_seed", rng_value)
        # measles_60
        fork60 = simulation.fork("measles_60_" + rng_type + "_" + str(rng_value))
        fork60.runConfig.setParameter("r0", 60)
        fork60.runConfig.setParameter("rng_type", rng_type)
        fork60.runConfig.setParameter("rng_seed", rng_value)
# Run all forks
simulation.runForks()

# Print related data
def print_test(name, test_type):
  for rng_type in RNG_TYPES:
    print(name + ": " + rng_type)
    for rng_value in rng_values: 
        summary_file = os.path.join(pystride.workspace, "BatchRngTypeTesting", name + "_" + rng_type + "_" + str(rng_value), "summary.csv")
        with open(summary_file) as csvfile:
            reader = csv.DictReader(csvfile)
            print("RNG = " + str(rng_value) + ": " + test_type + " = " + str(next(reader)[test_type]))

print_test("measles_16", "num_cases")
print_test("measles_60", "num_cases")

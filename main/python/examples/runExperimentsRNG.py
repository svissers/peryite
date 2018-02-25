##########################################################
# Script to obtain number of cases for various RNG values #
##########################################################

import csv
import os
import time

import pystride
from pystride.Simulation import Simulation

t_start = time.perf_counter()

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("../config/run_default.xml")
simulation.runConfig.setParameter("output_prefix", "RNGRuns")

# Create forks with different values for RNG
RNG_values = [7432523, 4755593, 4830240, 5399722, 9682632, 988142, 3270739, 6738734, 9510033]

for rng in RNG_values:
    # Create fork
    fork = simulation.fork("rng_" + str(rng))
    fork.runConfig.setParameter("rng_seed", rng)

# Run all forks
simulation.runForks()

# Get attack rates
for rng in RNG_values:
    summary_file = os.path.join(pystride.workspace, "RNGRuns", "rng_" + str(rng), "summary.csv")
    with open(summary_file) as csvfile:
        reader = csv.DictReader(csvfile)
        print("RNG = " + str(rng) + ": Num cases = " + str(next(reader)['num_cases']))

t_end = time.perf_counter()
print("Total time elapsed: " + str(round(t_end - t_start)) + " seconds")

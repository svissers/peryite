##########################################################
# Script to obtain number of cases for various RNG values #
##########################################################

import csv
import os
import time
import random
import sys

import pystride
from pystride.Simulation import Simulation

if not len(sys.argv) == 2:
    sys.exit("Please add the amount of runs as an argument: \npython3 runExperimentsBatchRuns.py 100")

t_start = time.perf_counter()

try:
    amount_of_runs = int(sys.argv[1])
except ValueError:
    sys.exit("amount of runs is not a number.")

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("../config/run_batchruntests4.xml")
simulation.runConfig.setParameter("output_prefix", "BatchRunsTesting")

# Create forks with different values for RNG
RNG_values = []
for i in range(amount_of_runs):
    RNG_values.append(random.randint(0, 4294967294))

for rng in RNG_values:
    # Create fork
    fork = simulation.fork("rng_" + str(rng))
    fork.runConfig.setParameter("rng_seed", rng)

# Run all forks
simulation.runForks()

# Get attack rates
for rng in RNG_values:
    summary_file = os.path.join(pystride.workspace, "BatchRunsTesting", "rng_" + str(rng), "summary.csv")
    with open(summary_file) as csvfile:
        reader = csv.DictReader(csvfile)
        print("RNG = " + str(rng) + ": Num cases = " + str(next(reader)['num_cases']))

t_end = time.perf_counter()
print("Total time elapsed: " + str(round(t_end - t_start)) + " seconds")

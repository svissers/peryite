import itertools

import sys
import os
import csv
os.putenv('OMP_NUM_THREADS', "1")
os.putenv('OMP_SCHEDULE' , "STATIC,1")
import pystride
from pystride.Simulation import Simulation

if not len(sys.argv) == 2:
    sys.exit("Please add the amount of runs as an argument: \npython3 runThreads_influenza.py 100")

try:
    amount_of_runs = int(sys.argv[1])
except ValueError:
    sys.exit("amount of runs is not a number.")

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("../config/run_batch_default.xml")
simulation.runConfig.setParameter("output_prefix", "BatchThreadTesting")

## Create Forks
# Influenza_a
for i in range(0, amount_of_runs):
    fork = simulation.fork("influenza_a_" + str(i))
# influenza_b
for i in range(0, amount_of_runs):
    fork = simulation.fork("influenza_b_" + str(i))
    fork.runConfig.setParameter("seeding_rate", 0)
# influenza_c
for i in range(0, amount_of_runs):
    fork = simulation.fork("influenza_c_" + str(i))
    fork.runConfig.setParameter("seeding_rate", (1 - 0.9991) / 100)
    fork.runConfig.setParameter("immunity_rate", 0.9991)

# Run all forks
simulation.runForks()

# Get attack rates
def print_test(name, test_type):
  print(name + ": " + test_type)
  for i in range(0, amount_of_runs):
      summary_file = os.path.join(pystride.workspace, "BatchThreadTesting", name + "_" + str(i), "summary.csv")
      with open(summary_file) as csvfile:
          reader = csv.DictReader(csvfile)
          print(str(next(reader)[test_type]))

# num cases
print_test("influenza_a", "num_cases")
print_test("influenza_b", "num_cases")
print_test("influenza_c", "num_cases")
# time
print_test("influenza_a", "run_time")
print_test("influenza_a", "total_time")
print_test("influenza_b", "run_time")
print_test("influenza_b", "total_time")
print_test("influenza_c", "run_time")
print_test("influenza_c", "total_time")

import itertools
import sys
import random
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
simulation.loadRunConfig("../config/run_batch_measles.xml")
simulation.runConfig.setParameter("output_prefix", "BatchThreadTesting")

## Create Forks
# measles_16
for i in range(0, amount_of_runs):
    fork = simulation.fork("measles_16_" + str(i))
    fork.runConfig.setParameter("rng_seed", random.randint(0, 4294967294))
    fork.runConfig.setParameter("r0", 16)
# measles_60
for i in range(0, amount_of_runs):
    fork = simulation.fork("measles_60_" + str(i))
    fork.runConfig.setParameter("rng_seed", random.randint(0, 4294967294))
    fork.runConfig.setParameter("r0", 60)

# Run all forks
simulation.runForks()

# Get attack rates
def print_test(name, test_type):
  results_file = os.path.join(pystride.workspace, "totalResults.txt")
  with open(results_file, "a+") as results:
      print(name + ": " + test_type)
      results.write(name + ": " + test_type + "\n")

      for i in range(0, amount_of_runs):
          summary_file = os.path.join(pystride.workspace, "BatchThreadTesting", name + "_" + str(i), "summary.csv")
          with open(summary_file) as csvfile:
              reader = csv.DictReader(csvfile)
              line = str(next(reader)[test_type])
              print(line)
              results.write(line + "\n")

# measles_16
print_test("measles_16", "num_cases")
print_test("measles_16", "run_time")
print_test("measles_16", "total_time")
# measles_60
print_test("measles_60", "num_cases")
print_test("measles_60", "run_time")
print_test("measles_60", "total_time")

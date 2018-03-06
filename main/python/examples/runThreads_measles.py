import itertools

import os
import csv
os.putenv('OMP_NUM_THREADS', "1")
os.putenv('OMP_SCHEDULE' , "STATIC,1")
import pystride
from pystride.Simulation import Simulation

# Set the workspace (default = .)
pystride.workspace = "simulations"

# Configure simulation
simulation = Simulation()
simulation.loadRunConfig("../config/run_batch_measles.xml")
simulation.runConfig.setParameter("output_prefix", "BatchThreadTesting")

## Create Forks
# measles_16
for i in range(0,1):
    fork = simulation.fork("measles_16_" + str(i))
    fork.runConfig.setParameter("r0", 16)
# measles_60
for i in range(0,1):
    fork = simulation.fork("measles_60_" + str(i))
    fork.runConfig.setParameter("r0", 60)

# Run all forks
simulation.runForks()

# Get attack rates
def print_test(name, test_type):
  print(name + ": " + test_type)
  for i in range(0,1):
      summary_file = os.path.join(pystride.workspace, "BatchThreadTesting", name + "_" + str(i), "summary.csv")
      with open(summary_file) as csvfile:
          reader = csv.DictReader(csvfile)
          print(str(next(reader)[test_type]))

# measles_16
print_test("measles_16", "num_cases")
print_test("measles_16", "run_time")
print_test("measles_16", "total_time")
# measles_60
print_test("measles_60", "num_cases")
print_test("measles_60", "run_time")
print_test("measles_60", "total_time")

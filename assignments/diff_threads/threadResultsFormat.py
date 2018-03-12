amount_of_threads = 8
filenames = ["totalResults_influenza_" + str(i) + ".txt" for i in range(1, 9)] + ["totalResults_measles_" + str(i) + ".txt" for i in range(1, 9)]
configs = ["influenza_a", "influenza_b", "influenza_c", "measles_16", "measles_60"]
variables = ["num_cases", "run_time", "total_time"]

data = [[[[] for i in range(amount_of_threads)] for i in configs] for i in variables]

# read
for filename in filenames:
    with open(filename, "r") as f:
        configIndex = -1
        variableIndex = -1
        threadIndex = int(filename[-5]) - 1

        for line in f:
            isNumber = True

            # Check for config index
            for i in range(len(configs)):
                if configs[i] in line:
                    configIndex = i
                    isNumber = False

            # Check for variable index
            for i in range(len(variables)):
                if variables[i] in line:
                    variableIndex = i
                    isNumber = False

            # Only do this if we have a number
            if isNumber:
                data[variableIndex][configIndex][threadIndex].append(line)

# write
with open("threadResults_formatted.txt", "w+") as f:
    for variable in range(len(data)):
        for config in range(len(data[variable])):
            for thread in range(len(data[variable][config])):
                f.write(", ".join([variables[variable], configs[config], str(thread + 1) + " threads"]) + "\n")

                data[variable][config][thread].sort()

                for num in data[variable][config][thread]:
                    f.write(num)

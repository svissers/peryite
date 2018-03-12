filenames = ["totalResults_influenza.txt", "totalResults_measles.txt"]
configs = ["influenza_a", "influenza_b", "influenza_c", "measles_16", "measles_60"]
rngTypes = ["lcg64", "yarn2", "lcg64_shift", "mrg3", "yarn3"]

data = [[[] for i in rngTypes] for i in configs]

# read
for filename in filenames:
    with open(filename, "r") as f:
        configIndex = -1
        rngIndex = -1

        for line in f:
            isNumber = True

            # Check for config index
            for i in range(len(configs)):
                if configs[i] in line:
                    isNumber = False
                    configIndex = i

                    for j in range(len(rngTypes)):
                        if rngTypes[j] == line[(len(configs[i]) + 2):-1]:
                            rngIndex = j

            # Only do this if we have a number
            if isNumber:
                data[configIndex][rngIndex].append(line)

# write
with open("rngTypeResults_formatted.txt", "w+") as f:
    for config in range(len(configs)):
        for rngType in range(len(data[config])):
            f.write(", ".join([configs[config], rngTypes[rngType]]) + "\n")

            data[config][rngType].sort()

            for num in data[config][rngType]:
                f.write(num)

import sys
from math import sin, cos, sqrt, atan2, radians


def calculateDistance(lat1, lon1, lat2, lon2): 
    lat1 = radians(lat1)
    lat2 = radians(lat2)
    lon1 = radians(lon1)
    lon2 = radians(lon2)
    R = 6373.0
    dlon = lon2 - lon1
    dlat = lat2 - lat1

    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2
    c = 2 * atan2(sqrt(a), sqrt(1 - a))

    distance = R * c
    return distance


if len(sys.argv) != 3:
    print("Usage: python parseCities.py filenamein filenameout")
    exit()

citiesData = []

try:
    with open(sys.argv[1], 'r') as file:
        for line in file:
            # Ignore header
            if line.startswith("id"):
                continue

            lineData = line.split(",");

            # Remove newline from city name
            lineData[4] = lineData[4][:-1]

            citiesData.append(lineData)

except  FileNotFoundError:
    print("File not found.")

###########################
# citiesData is now a 2D array representing the csv file.
###########################
commutingScores = []
totalcommutingScores = []
for city1 in citiesData:
    commutingScoresCity = []
    totalscore = 0
    for city2 in citiesData:
        weight = 0.5;
        distance = calculateDistance(float(city1[2]), float(city1[3]), float(city2[2]), float(city2[3]))
        if(distance == 0):
            weight = 3
        elif(distance <= 25):
            weight = 9
        elif(distance <= 50):
            weight = 7
        elif(distance <= 75):
            weight = 6
        elif(distance <= 100):
            weight = 5
        elif(distance <= 125):
            weight = 4
        elif(distance <= 150):
            weight = 2

        score = weight* (float(city2[1])/(float(city1[1])))/100
        commutingScoresCity.append(score)
        totalscore += score


    commutingScores.append(commutingScoresCity)
    totalcommutingScores.append(totalscore)

commutingnumbers = []
for i in range (0, len(citiesData)):
    citycommutingNumbers = []
    for j in range (0, len(citiesData)):
        citycommutingNumbers.append(int(round(((commutingScores[i][j]/totalcommutingScores[i]) * float(citiesData[i][1]))/10)))
    commutingnumbers.append(citycommutingNumbers)

###########################
# commutingnumbers is now a 2D array representing the commuting matrix.
###########################
commutingfile = open(sys.argv[2], 'w')

commutingfile.write("\"id_"+ citiesData[0][0] + "\"")

for i in range (1,len(citiesData)):
    commutingfile.write(",\"id_"+ citiesData[i][0] + "\"")

commutingfile.write("\n")

for j in range (0, len(commutingnumbers[0])):
    commutingfile.write("" + str(commutingnumbers[0][j]))
    for i in range (1,len(commutingnumbers)):
        commutingfile.write(","+ str(commutingnumbers[i][j]))
    commutingfile.write("\n")
commutingfile.close()
        

import re
import matplotlib.pyplot as plt

# Store the links we need in a list
titles = {}
times = []
with open("fragment_results.txt", "r") as f:
    lines = f.readlines()
    for i in range(0, len(lines)):
        line = lines[i]
        if 'DONE' in line:
            title_line = ''
            title_line = re.sub('\[     DONE \] Sim\.','',line)
            if '-' in line:
                title = title_line.split(' - ',1)[0]
                subtitle = title_line.split(' - ',1)[1]
                subtitle = re.sub(' ', '', subtitle.split(' (',1)[0])
            else:
                title = title_line[:-21]
                subtitle = 'default'
            line = lines[i+1]
            time_line = re.sub('\[   RUNS   \]        Median time: ','',line)
            time_line = re.sub('\n','',time_line)
            times = time_line.split(':',2)
            time = 0
            for t in times:
                time = time * 60 + int(t)
            if title not in titles:
                titles[title] = {}
            titles[title][subtitle] = time   

i = 0
xs_data = []
ys_data = []
for title in titles:
    plt.figure(i)
    i = i + 1
    x_data = []
    y_data = []
    for subtitle in titles[title]:
        if subtitle == 'default':
            print(title, ': ', titles[title][subtitle])
        else:
            x_data.append(subtitle)
            y_data.append(titles[title][subtitle])
    if len(x_data) == 0:
        continue
    xs_data.append(x_data)
    ys_data.append(y_data)

# Create plots with pre-defined labels.
ax = plt.subplot()
ax.plot(xs_data[0], ys_data[0], 'ro', label='Not fragmented')
ax.plot(xs_data[1], ys_data[1], 'bo', label='Fragmented')

legend = ax.legend(loc='upper center', shadow=True, fontsize='x-large')

# Rescale the axis
current_axis = plt.axis()
new_axis = [0,0,0,0]
new_axis[0] = current_axis[0] * 0.95
new_axis[1] = current_axis[1] * 1.05
new_axis[2] = current_axis[2] * 0.95
new_axis[3] = current_axis[3] * 1.05
plt.axis(new_axis)
# Set the labels and save
plt.xlabel("Population size")
plt.ylabel('Execution time')
plt.title(" plot")
plt.savefig("Population_size_fragments.png", bbox_inches='tight')

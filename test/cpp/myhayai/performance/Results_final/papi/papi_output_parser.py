import re
import matplotlib.pyplot as plt
import numpy as np

# Store the links we need in a list
labels = []
y_data = []
sim_steps = []
with open("papi_output_1m_no_fragment.txt", "r") as f:
    lines = f.readlines()
    for i in range(0, len(lines)):
        line = lines[i]
        if 'Counters' in line:
            title_line = ''
            title = re.sub(' Counters: ','',line)
            line = lines[i+1]
            total_instructions = int(re.sub('Total instructions: ','',line))
            if 'Timestep' in title:
                sim_steps.append(total_instructions)
            else:
                labels.append(title)
                y_data.append(total_instructions)


fig = plt.figure(1, [15, 7])
ax = plt.subplot()
x_pos = np.arange(len(labels))

#ax.bar(x_pos, y_data, width,align='center', color='green')
#ax.set_xticks(x_pos)
#ax.set_xticklabels(labels)
#ax.set_ylabel('Instruction count')
#ax.set_title('Instruction Count plot')
#plt.setp(ax.get_xticklabels(), fontsize=10, rotation='vertical')

ax.barh(x_pos, y_data, align='center',
        color='green')
ax.set_yticks(x_pos)
ax.set_yticklabels(labels)
ax.invert_yaxis()  # labels read top-to-bottom
ax.set_xlabel('Instruction count')
ax.set_title('No fragmented centers (Pop_size = 1 million)')

plt.savefig("instructions_1m_no_fragments.png")

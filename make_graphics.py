import matplotlib.pyplot as plt
import numpy as np

dots = open("all_points.txt", 'r')
lines = dots.readlines()
x_coords, y_coords = [], []
colors = []
group_colors = dict()
groups = dict()
for line in lines:
    group, coords = line.rstrip().split(':')
    x, y = map(int, coords.split(','))
    
    if group not in group_colors:
        group_colors[group] = list(map(lambda x: x / 256, list(np.random.choice(range(256), 3))))
        groups[group] = [[], []]
    groups[group][0].append(x)
    groups[group][1].append(y)
    




fig, ax = plt.subplots()
for group in groups.keys():
    ax.scatter(x=groups[group][0], y=groups[group][1], color=group_colors[group], label=group)

ax.set_title("dots")
ax.set_xlabel("x")
ax.set_ylabel("y")
ax.legend()
ax.grid(True)

plt.savefig("dots.png")
plt.show()
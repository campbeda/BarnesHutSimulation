#!/usr/bin/python

import matplotlib
matplotlib.use('Agg') # Set non GUI based backend
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import sys

if len(sys.argv) < 2:
    print("Usage: ./visualizer.py <file>")
    sys.exit(1)

data = open(sys.argv[1], "r")
# Skip first two lines and last line of file
lines = data.readlines()[2:-1]

particles = {}
for line in lines:
    # Split on space and grab time step and position
    lineSplit = line.split()
    timeStep = int(lineSplit[0])
    posx, posy= lineSplit[3:5]
    # Insert position into dictionary
    if timeStep not in particles:
        particles[timeStep] = []
    particles[timeStep].append((float(posx), float(posy)))

# Plot parameters
plt.style.use('dark_background')
fig, ax = plt.subplots()
plt.title('Barnes Hut Simulation')
plt.xlabel('X Position')
plt.ylabel('Y Position')
plt.grid(color='w', linestyle='--', linewidth=1)
ax.set_xlim(0, 24)
ax.set_ylim(0, 24)
ln, = ax.plot([], [], 'ro', lw=3)

def init():
    ln.set_data([], [])
    return ln,

def update(frame):
    xdata = [ x for x, _ in particles[frame] ]
    ydata = [ y for _, y in particles[frame] ]
    ln.set_data(xdata, ydata)
    return ln,

ani = FuncAnimation(fig, update, frames=len(particles), interval=1000, init_func=init, blit=True, repeat=False)
ani.save("bhs.gif", writer="imagemagick", fps=30)

#plt.show() # plot when GUI based backend is used

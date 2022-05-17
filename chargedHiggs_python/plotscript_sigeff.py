# Do imports just to make sure they work
import pandas as pd
import numpy as np
import uproot
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import BoundaryNorm
from matplotlib.ticker import MaxNLocator
import scipy.stats as st
from sklearn.datasets.samples_generator import make_blobs
#plt.ion()
#plt.show()
#cax = plt.axes([0.85, 0.1, 0.075, 0.8])
#plt.colorbar(5)

#np.random.seed(19680801)
#Z = np.random.rand(6, 10)
#x = np.arange(-0.5, 10, 1)  # len = 11
#y = np.arange(4.5, 11, 1)  # len = 7
#fig, ax = plt.subplots() 
#ax.pcolormesh(x, y, Z)
#fig.colorbar(ax)

harvest2 = np.array([[3.03, 3.05287,3.02175,2.9404,2.8117,2.63853,2.42376,2.17026,1.88092,1.55,1.20,0.82,0.42],
                    [3.10,3.08,3.025,2.94,2.814,2.6,2.45,2.22,1.96,0,0,0,0],
                    [3.20,3.17,3.107,3.01,2.88,2.71,2.52,2.301,2.041,0,0,0,0],
                    [3.33,3.30,3.24,3.14,3.01,2.84,2.64,2.40,2.12,0,0,0,0],
                    [3.47,3.4,3.33,3.26,3.209,0,0,0,0,0,0,0,0],
                    [3.62,3.58,3.54,3.51,3.47,0,0,0,0,0,0,0,0],
                    [3.75,3.76,3.77,3.787,3.79,0,0,0,0,0,0,0,0],
                    [3.86,0,0,0,0,0,0,0,0,0,0,0,0],        
                    [3.97,0,0,0,0,0,0,0,0,0,0,0,0],
                    [4.07,0,0,0,0,0,0,0,0,0,0,0,0],
                    [4.18,0,0,0,0,0,0,0,0,0,0,0,0],
                    [4.31,0,0,0,0,0,0,0,0,0,0,0,0],
                    [4.47,0,0,0,0,0,0,0,0,0,0,0,0]])


vegetables = ["cucumber", "tomato", "lettuce", "asparagus",
              "potato", "wheat", "barley"]

vegetables2 = [160,155,150,145,140,135,130,125,120,115,110,105,100]

#vegetables2 = [160,140,120,100]
farmers = ["Farmer Joe", "Upland Bros.", "Smith Gardening",
           "Agrifun", "Organiculture", "BioGoods Ltd.", "Cornylee Corp."]

farmers2 = [15,20,25,30,35,40,45,50,55,60,65,70,75]

harvest = np.array([[0.8, 2.4, 2.5, 3.9, 0.0, 4.0, 0.0],
                    [2.4, 0.0, 4.0, 1.0, 2.7, 0.0, 0.0],
                    [1.1, 2.4, 0.8, 4.3, 1.9, 4.4, 0.0],
                    [0.6, 0.0, 0.3, 0.0, 3.1, 0.0, 0.0],
                    [0.7, 1.7, 0.6, 2.6, 2.2, 6.2, 0.0],
                    [1.3, 1.2, 0.0, 0.0, 0.0, 3.2, 5.1],
                    [0.1, 2.0, 0.0, 1.4, 0.0, 1.9, 6.3]])




fig, ax = plt.subplots()
im = ax.imshow(harvest2)

# We want to show all ticks...
ax.set_xticks(np.arange(len(vegetables2)))
ax.set_yticks(np.arange(len(farmers2)))
# ... and label them with the respective list entries
ax.set_xticklabels(vegetables2)
ax.set_yticklabels(farmers2)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
#for i in range(len(vegetables)):
#    for j in range(len(farmers)):
#        text = ax.text(j, i, harvest[i, j],
#                       ha="center", va="center", color="w")

ax.set_title("Signal Efficiences [%]")
fig.tight_layout()
plt.xlabel('$H^{+} [GeV]$')
plt.ylabel('$m_{A} [GeV]$')
plt.show()
fig.savefig("test.png")

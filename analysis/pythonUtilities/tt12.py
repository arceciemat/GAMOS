# Import libraries
import numpy as np
import matplotlib.pyplot as plt
import random
  
# Creating dataset
n = 10
x = np.random.standard_normal(n)
y = 3.0 * x 

print("XY =",x,y)
fig = plt.subplots(figsize =(10, 7))
# Creating plt
plt.hist2d(x, y)
plt.title("Simple 2D Histogram")
  
# show plot
plt.show()
plt.savefig("tt12.jpg")

import numpy as np
from numpy.random import default_rng
import dla
import matplotlib.pylab as plt

rng = default_rng(35)
L=10
x=rng.uniform(low=-L/2,high=L/2,size=(1000,3))
y=rng.uniform(low=-L/2,high=L/2,size=(1,3))

alg=dla.dla( (x,y),sigma=[0.2,0.2,0.2],radius=0.1)

print(alg.cluster.shape)

def plotCluster(x,y):

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.scatter(x[:,0], x[:,1], x[:,2] )
    ax.scatter(y[:,0], y[:,1], y[:,2],color="red" )
    return (fig)

plotCluster(alg.free,alg.cluster)

plt.show()

alg.advance(1000)

plotCluster(alg.free,alg.cluster)

plt.show()

import numpy as np
import matplotlib.pylab as plt
import pandas as pd
import dla_cpp

class dla:
    def __init__(self,shape, state ):
        """
        shape: shaoe of the lattice
        state: a tuple of arrays ( free : array[N,D],cluster : array[N,D]) , D : dimensionality (2 or 3) , N : number of particles
        """
        self.shape=shape
        
        if len(shape)==2:
            self._dla=dla_cpp.onLattice2d(state[0],state[1] ,shape )
        else:
             if len(shape)==3:
                self._dla=dla_cpp.onLattice3d(state[0],state[1] ,shape )
    
    def advance(self,nSteps):
        self._dla.advance(nSteps)
    
    @property
    def state(self):
        return (self._dla.getFree(),self._dla.getCluster() )



def plot2d(state):
    free=state[0]
    cluster=state[1]
    
    fig = plt.figure()
    ax = fig.add_subplot()
    ax.scatter(free[:,0],free[:,1])
    ax.scatter(cluster[:,0],cluster[:,1])


def plot3d(state):
    free=state[0]
    cluster=state[1]
    
    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.scatter(free[:,0],free[:,1],free[:,2])
    ax.scatter(cluster[:,0],cluster[:,1],cluster[:,2])



def saveXYZ(cluster,filename):
    if cluster.shape[1] == 2:
        data=pd.DataFrame({"X":cluster[:,0],"Y" : cluster[:,1]   } )
        data["Z"]=0
    else:
        if cluster.shape[1] == 3:
            data=pd.DataFrame({"X":cluster[:,0],"Y" : cluster[:,1] , "Z" : cluster[:,2]  } )
    
    data["atom"]="H"
    data=data.loc[:,["atom","X","Y","Z"]]
    atoms='{}\n \n'.format(len(data)) + data.to_csv(sep=" ",header=False,index=False)
    with open(filename,"w") as f:
        f.write(atoms)
    
        

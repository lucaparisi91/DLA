import numpy as np
import py_dla
import pandas as pd

class dla:
    def __init__(self,state, radius=1,lBox=(10,10,10),sigma=None):
        self.radius=radius
        self.lBox=np.array(lBox)
        
        if sigma is None:
            self.sigma=0.1*np.sqrt(np.sum(self.lBox**2 ))
            self.sigma=np.array([self.sigma,self.sigma,self.sigma])
        else:
            self.sigma=np.array(sigma)

        free=state[0]
        cluster=state[1]

        self.cppDla=py_dla.py_dla(free,cluster,self.radius,self.lBox,self.sigma)
    
    def advance(self,nSteps=1):
        self.cppDla.advance(nSteps)
    
    @property
    def free(self):
        return self.cppDla.getFree()
    @property
    def cluster(self):
        return self.cppDla.getCluster()
    
def saveXYZ(cluster,filename):
    data=pd.DataFrame({"X":cluster[:,0],"Y" : cluster[:,1] , "Z" : cluster[:,2]  } )
    data["atom"]="H"
    data=data.loc[:,["atom","X","Y","Z"]]
    atoms='{}\n \n'.format(len(data)) + data.to_csv(sep=" ",header=False,index=False)
    with open(filename,"w") as f:
        f.write(atoms)
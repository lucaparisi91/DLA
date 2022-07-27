#include "DLA.h"

void cluster_t::addToCluster( size_t i)
{
    // !!! Warning: particle orderings is not mantained after assignment to cluster


    for(int d=0;d<DIMENSIONS;d++)
    {
        _cluster(_nCluster,d)=_free(i,d);
    }
    _nCluster++;

    // remove from list of free particles. To avoid holdes, swap the emptied position with the last valid free particles. 
    for(int d=0;d<DIMENSIONS;d++)
    {
        std::swap(_free( i,d),_free(_nFree-1,d)  );
    }
    _nFree--;

}



void DLA::initializeCluster( state_t & state )
{
    // !!! Warning: particle orderings is not mantained after assignment to cluster

    int nFreeOld=state.getNFree();

    do 
    {
        size_t i=0;
        nFreeOld=state.getNFree();

        while ( i < state.getNFree() )
        {

            if (addToClusterCondition(state,i) )
            {
                state.addToCluster(i);
            }
            else
            {
                i++;
            }

        }
    } while (  state.getNFree() != nFreeOld); 

}


void DLA::advance( state_t & state,randState_t & randG )
{
    _noise.resize(state.getN() * DIMENSIONS    );
    norm.generate(_noise.begin(),_noise.begin() + state.getNFree()*DIMENSIONS,randG);
    size_t i=0,ii=0;
    auto & free = state.getFree();

    while(i < state.getNFree())
    {
        for(int d=0;d<DIMENSIONS;d++)
        {
            free(i,d)=free(i,d) + _sigma[d]*_noise[ii];
            ii++;
            free(i,d)=_geo.pbc(free(i,d),d);
        }

        if (addToClusterCondition(state,i) )
        {
            state.addToCluster(i);
        }
        else
        {
            i++;
        }

    }

}

bool DLA::addToClusterCondition(const state_t & state,size_t i)
{

    const auto & _freeParticles=state.getFree();
    const auto & _clusterParticles=state.getCluster();
    
    for(int j=0;j<state.getNCluster();j++)
    {
        real_t r2=0;
        for(int d=0;d<DIMENSIONS;d++)
        {
            real_t diff=_geo.difference( _freeParticles(i,d) - _clusterParticles(j,d)  ,d);
            r2+=diff*diff;
        }


        if (r2 <= _radius*_radius)
        {
            return true;
        }
    }

    return false;

}
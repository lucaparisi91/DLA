#include "moves.h"
#include "lattice/particles.h"

#include <iostream>

namespace mesh
{

    template<int DIM>
    void diffusionMove<DIM>::move(index_t iParticle,state_t & state, randState_t & randG )
    {
        int iNeighbour;

        _distribution.generate(&iNeighbour, (&iNeighbour)+1,randG);

        auto iCell=state.getFree().cellIndex(iParticle);
        auto iCell2 = state.getFree().getLattice().getNeighbour(iCell,iNeighbour);
           
        state.getFree().move(iParticle,iCell2);

    }



    template<int DIM>
    void diffusionMove<DIM>::move( state_t & state, randState_t & randG )
    {
        int iNeighbour;

        for(auto it=state.getFree().begin();it!=state.getFree().end();it++)
        {
            move( it->first,state,randG);
        }

    }

    
   



template class diffusionMove<2>;
template class diffusionMove<3>;



};
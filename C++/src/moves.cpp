#include "moves.h"
#include "lattice/particles.h"

#include <iostream>

namespace mesh
{
    void clusterOnLattice::moveToCluster(index_t iParticle)
        {
            auto iCell = _free->cellIndex(iParticle);
            _cluster->add(iParticle,iCell);
            _free->remove(iParticle);
        }

    void diffusionMove::move(index_t iParticle,state_t & state, index_t iCell2 )
    {

        state.getFree().move(iParticle,iCell2);

        updateState(iParticle,state);

    }

    void diffusionMove::move(index_t iParticle,state_t & state, randState_t & randG )
    {
        int iNeighbour;

        _distribution.generate(&iNeighbour, (&iNeighbour)+1,randG);

        std::cout << iNeighbour << std::endl;
        auto iCell=state.getFree().cellIndex(iParticle);
        auto iCell2 = state.getFree().getLattice().getNeighbour(iCell,iNeighbour);


        move(iParticle,state,iCell2);


    }



    void diffusionMove::updateState(index_t iParticle,state_t & state )
    {
        auto iCell = state.getFree().cellIndex(iParticle);

        auto & lattice = state.getCluster().getLattice();



        for(int i=0;i<lattice.nCellsNeighbourhood();i++)
        {
            
            auto iCellN=lattice.getNeighbour(iCell,i);
            if (state.getCluster().getCells()[iCellN].nParticles()!=0 and (iCellN!= iCell))
            {
                //std::cout << iCell<< " " <<iCellN <<std::endl;
                state.moveToCluster(iParticle);
                break;
            }

        }

    }




};
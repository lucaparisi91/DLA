#include "moves.h"
#include "lattice/particles.h"

#include <iostream>

namespace mesh
{


    void diffusionMove::move(index_t iParticle,state_t & state, randState_t & randG )
    {
        int iNeighbour;

        _distribution.generate(&iNeighbour, (&iNeighbour)+1,randG);

        auto iCell=state.getFree().cellIndex(iParticle);
        auto iCell2 = state.getFree().getLattice().getNeighbour(iCell,iNeighbour);

        state.getFree().move(iParticle,iCell2);

    }

     void diffusionMove::move( state_t & state, randState_t & randG )
    {
        int iNeighbour;

        for(auto it=state.getFree().begin();it!=state.getFree().end();it++)
        {
            move( it->first,state,randG);
        }

    }

    

    bool clusterOnLattice::isCloseToCluster(index_t iCell)
    {
        auto & lattice = getCluster().getLattice();

        for(int i=0;i<lattice.nCellsNeighbourhood();i++)
        {
            
            auto iCellN=lattice.getNeighbour(iCell,i);
            if (getCluster().getCells()[iCellN].nParticles()!=0 and (iCellN!= iCell))
            {
                return true;
                
            }

        }

        return false;

    }


    void clusterOnLattice::updateClusterAssignment()
    {

        for (auto it = _free->begin();it!= _free->end();it++)
        {
            auto iCell=it->second;
            auto iParticle=it->first;

        
            assert( iCell < getFree().getLattice().size());

            assignToCluster(iCell);

        }


        getFree().removeInvalidated();

    }

    void clusterOnLattice::updateClusterAssignment( size_t iParticle)
    {
        auto iCell = getFree().cellIndex(iParticle);
        
        if (isCloseToCluster(iCell))
        {
            getFree().remove(iParticle);
            getCluster().add(iParticle,iCell);
        }
    }



    void clusterOnLattice::assignToCluster( size_t iCell)
    {
        const auto & cell = getFree().getCells()[iCell];

        if ( (not cell.isValid() ) or (cell.nParticles()==0) )
        {
            return;
        }

        bool toMark = isCloseToCluster(iCell);
        if (toMark)
        {
            
            for(int i=0;i<cell.nParticles();i++)
            {
                getCluster().add( cell[i] , iCell );
            }

            getFree().invalidateCell(iCell);

            auto & lattice = getFree().getLattice();

            for (int t=0;t<lattice.nCellsNeighbourhood();t++)
            {
                auto iCell2= lattice.getNeighbour(iCell,t);
            
                assignToCluster(iCell2);
            }


        }

    }









};
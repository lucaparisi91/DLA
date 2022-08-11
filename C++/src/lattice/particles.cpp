#include "particles.h"
namespace mesh
{

template<int DIM>
particles<DIM>::particles(std::shared_ptr< particles<DIM>::lattice_t > _lattice) :
_lattice(_lattice),_cells(_lattice->size() ),_nParticles(0)
{

};


template<int DIM>
Eigen::Tensor<int,2> particles<DIM>::toTensor() const
{
    Eigen::Tensor<int,2> tensor;

    tensor.resize(nParticles(),DIM);
    index_t i=0;
    for(auto it = begin() ; it!= end();it++)
    {
        auto indices = _lattice->indices(it->second);
        for(int d=0;d<DIM;d++)
        {
            tensor(i,d)=indices[d];
        }
        i++;

    }

    return tensor;
}


template<int DIM>
void particles<DIM>::add( const Eigen::Tensor<int,2> & tensor,index_t offset)
{
    for(index_t iParticle=0;iParticle<tensor.dimensions()[0];iParticle++)
    {
        index_t iCell;
        if constexpr(DIM==2)
        {
            iCell=_lattice->index( (index_t)tensor(iParticle,0) , (index_t)tensor(iParticle,1) );
        }
        if constexpr(DIM==3)
        {
            iCell=_lattice->index( (index_t)tensor(iParticle,0) , (index_t)tensor(iParticle,1),(index_t)tensor(iParticle,2) );
        }
        if constexpr(DIM==1)
        {
            iCell=_lattice->index( (index_t)tensor(iParticle,0) );
        }

        add( iParticle + offset,iCell);
    }

}



 template<int DIM>
    bool clusterOnLattice<DIM>::isCloseToCluster(index_t iCell)
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

    template<int DIM>
    void clusterOnLattice<DIM>::updateClusterAssignment()
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

    template<int DIM>
    void clusterOnLattice<DIM>::updateClusterAssignment( size_t iParticle)
    {
        auto iCell = getFree().cellIndex(iParticle);
        
        if (isCloseToCluster(iCell))
        {
            getFree().remove(iParticle);
            getCluster().add(iParticle,iCell);
        }
    }


    template<int DIM>
    void clusterOnLattice<DIM>::assignToCluster( size_t iCell)
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


template <int DIM>
bool particles<DIM>::isCoherent()
{
    bool ok=true;
    size_t k=0;
    for (auto it = _cellIndexPerParticle.begin();it!= _cellIndexPerParticle.end();it++)
    {
        
        auto ii = _subCellIndexPerParticle.at(it->first);

        ok = ok and 
        (_cells[it->second][ii] == it->first );

        ok = ok and (it->second < _lattice->size() );
        k++;
    };

    ok = ok and (nParticles() == k);
    return ok;
    
} 

template class particles<3>;
template class particles<2>;

template class clusterOnLattice<2>;
template class clusterOnLattice<3>;


};
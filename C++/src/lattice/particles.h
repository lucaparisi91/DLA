#include <vector>
#include "cells.h"
#include "lattice.h"



template<int DIM>
class particles
{
    public:

    using index_t = cellData::index_t;
    using lattice_t = lattice<DIM> ;

    particles(std::shared_ptr< lattice_t > _lattice);

    const auto & getLattice() const {return *(_lattice);}

    auto cellIndex(index_t iParticle) const  {return _cellIndexPerParticle[iParticle];}


    
    const auto & getCells() const noexcept {return _cells;}



    void resize(index_t nMax)
    {
        _cellIndexPerParticle.resize(nMax,-1);
        _subCellIndexPerParticle.resize(nMax,-1);
    }

    index_t  add(index_t iParticle,index_t cellIndex)
    {

        auto ii=_cells[cellIndex].add(iParticle);
        _cellIndexPerParticle[iParticle]=cellIndex;
        _subCellIndexPerParticle[iParticle]=ii;
        _nParticles++;
        return _nParticles -1;
    }

    void remove(index_t iParticle)
    {
        auto i = _cellIndexPerParticle[iParticle];
        auto ii = _subCellIndexPerParticle[iParticle];

        
        _subCellIndexPerParticle[ _cells[i].nParticles() - 1 ]=ii;
        _cells[i].remove(ii);
        _nParticles--;
    }

    index_t move( index_t iParticle, index_t iCell2 )
    {
        remove(iParticle);
        return add(iParticle,iCell2);
    }


    auto size() const  {return _nParticles;}



    private:


    std::shared_ptr<lattice_t > _lattice;

    /* Per particle data */
    std::vector<index_t>  _cellIndexPerParticle;
    std::vector<index_t>  _subCellIndexPerParticle;


    /* Cell data  */
    cells _cells;

    index_t _nParticles;
};

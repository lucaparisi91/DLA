#include <vector>
#include "cells.h"
#include "lattice.h"
#include <map>
#include "../traits.h"

template<int DIM>
class particles
{
    public:

    using index_t = cellData::index_t;
    using lattice_t = lattice<DIM> ;

    particles(std::shared_ptr< lattice_t > _lattice);

    const auto & getLattice() const {return *(_lattice);}

    auto cellIndex(index_t iParticle) const  {return _cellIndexPerParticle.at(iParticle);}


    
    const auto & getCells() const noexcept {return _cells;}

    auto nParticles() const noexcept {return _nParticles;}

    void resize(index_t nMax)
    {
        //_cellIndexPerParticle.resize(nMax,-1);
        //_subCellIndexPerParticle.resize(nMax,-1);
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

        auto & cell = _cells[i];    
        
        std::swap( _subCellIndexPerParticle[ cell[ii]] ,_subCellIndexPerParticle[ cell[cell.nParticles()-1]]   );
        
        cell.remove(ii);

        
        _nParticles--;

        _cellIndexPerParticle.erase(iParticle);
        _subCellIndexPerParticle.erase(iParticle);

    }



    index_t move( index_t iParticle, index_t iCell2 )
    {
        remove(iParticle);
        return add(iParticle,iCell2);
    }


    void invalidateCell(index_t iCell)
    {
        _cells.invalidate(iCell);
    }

    void removeInvalidated()
    {
        int k=0,i=0;

        for ( i=0;i<_cells.nInvalidCells();i++)
        {
            auto iCell = _cells.getInvalidCellIndex(i);
            auto & cell = _cells[  iCell];

            while( cell.nParticles() > 0  )
            {
                remove(cell[0]);
            }
        }

        _cells.validate();
    }


    auto size() const  {return _nParticles;}



    auto begin() {return _cellIndexPerParticle.begin(); }

    auto end() {return _cellIndexPerParticle.end(); }

    auto begin() const {return _cellIndexPerParticle.begin(); }

    auto end() const {return _cellIndexPerParticle.end(); }



    private:


    std::shared_ptr<lattice_t > _lattice;

    /* Per particle data */
    std::map<index_t,index_t>  _cellIndexPerParticle;
    std::map<index_t,index_t>  _subCellIndexPerParticle;
        

    /* Cell data  */
    cells _cells;

    index_t _nParticles;
};







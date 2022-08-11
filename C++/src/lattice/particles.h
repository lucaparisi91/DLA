#include <vector>
#include "cells.h"
#include "lattice.h"
#include <map>
#include "../traits.h"
#include <unsupported/Eigen/CXX11/Tensor>

namespace mesh
{


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

    

    void add(const Eigen::Tensor<int,2> & tensor,index_t offset=0);

    index_t  add(index_t iParticle,index_t cellIndex)
    {
        assert(cellIndex < _lattice->size());
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

    void move( index_t iParticle, index_t iCell2 )
    {
         auto i = _cellIndexPerParticle[iParticle];
        auto ii = _subCellIndexPerParticle[iParticle];

        auto & cell = _cells[i];    
        
        std::swap( _subCellIndexPerParticle[ cell[ii]] ,_subCellIndexPerParticle[ cell[cell.nParticles()-1]]   );
        
        cell.remove(ii);

        auto ii2=_cells[iCell2].add(iParticle);

        _cellIndexPerParticle[iParticle]=iCell2;
        _subCellIndexPerParticle[iParticle]=ii2;
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


    Eigen::Tensor<int,2> toTensor() const ;// returns a tensor of particle positions

    bool isCoherent();



    private:


    std::shared_ptr<lattice_t > _lattice;

    /* Per particle data */
    std::map<index_t,index_t>  _cellIndexPerParticle;
    std::map<index_t,index_t>  _subCellIndexPerParticle;
        

    /* Cell data  */
    cells _cells;

    index_t _nParticles;
};




template<int DIM>
    class clusterOnLattice
    {
        public:
        using particles_t=particles<DIM>;

        clusterOnLattice(std::shared_ptr<particles_t> free,std::shared_ptr<particles_t> cluster) : _free(free),_cluster(cluster) {}


        auto & getFree() {return *_free;}
        const auto & getFree() const  {return *_free;}

        const auto & getCluster() const  {return *_cluster;}
        auto & getCluster()   {return *_cluster;}


        void updateClusterAssignment(); // assign in a recusive way all particles to the cluster


        void updateClusterAssignment(index_t iParticle); // decides wheater to assign to the cluster in a non recusive way



        private:

        bool isCloseToCluster(index_t iCell);

        void assignToCluster(index_t iParticle);

        std::shared_ptr<particles_t> _free;
        std::shared_ptr<particles_t> _cluster;
        std::vector<index_t> _tmp_particles;


    };




};




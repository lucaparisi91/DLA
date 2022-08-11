#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "../tools.h"

namespace mesh{

template<int DIM>
class lattice
{
    public:
    /*
    Lattice with with dimensions DIM and nGhosts[] ghost cells in each direction
    */

    using index_t = size_t;

    lattice(std::array<size_t,DIM>  shape,std::array<size_t,DIM> nGhosts=TRUNCATE<DIM>::array(0ul,0ul,0ul));

    ENABLE_IF_EQ(DIM,3)
    inline index_t index(index_t i , index_t j , index_t k) const { return _extendedShape[0]*(  _extendedShape[1]*k + j   )  + i ; } // fuse a 3d index in a 1d index ( fortran ordering )

    ENABLE_IF_EQ(DIM,3)
    inline index_t index( int  i , int  j , int k) const { return _extendedShape[0]*(  _extendedShape[1]*k + j   )  + i ; } // fuse a 3d index in a 1d index ( fortran ordering )
        
    ENABLE_IF_EQ(DIM,2)
    inline index_t index(index_t i, index_t j) const {return _extendedShape[0]*j + i;}

    ENABLE_IF_EQ(DIM,1)
    inline auto  index(index_t i) const {return i;}
   

    // map d indices to a single index
    ENABLE_IF_EQ(DIM,3)
    inline auto index(const std::array<index_t,3> & i) const {return index(i[0],i[1],i[2]);}

    ENABLE_IF_EQ(DIM,2)
    inline auto index(const std::array<index_t,2> & i) const {return index(i[0],i[1]);} 

    ENABLE_IF_EQ(DIM,1)
    inline index_t index(const std::array<index_t,1> & i) const {return index(i[0]);}

    ENABLE_IF_EQ(DIM,3)
    std::array<index_t,DIM> index3d(index_t iCell) const 
    {    
        auto  k = iCell/(_extendedShape[0]*_extendedShape[1]);
        iCell=iCell - k*_extendedShape[0]*_extendedShape[1];
        auto  j = iCell/(_extendedShape[0]);
        auto  i = iCell - j*_extendedShape[0];
        return std::array<index_t,3>{i,j,k};
    };


    ENABLE_IF_EQ(DIM,2)
    std::array<index_t,DIM> index2d(index_t iCell) const 
    {
        auto  j = iCell/(_extendedShape[0]);
        auto  i = iCell - j*_extendedShape[0];
        return std::array<index_t,DIM>{i,j};
    };

    auto  indices(index_t iCell) const 
    {
        if constexpr( DIM == 3)
        {
            return index3d(iCell); 
        };
        if constexpr( DIM == 2)
        {
            return index2d(iCell); 
        };

        if constexpr( DIM == 1)
        {
            return std::array<index_t,1>{iCell}; 
        };

    };



    int dimensions() const noexcept{return DIM;}
    const auto & shape() const noexcept{return _shape;}


    const auto & extendedShape() const noexcept {return _extendedShape;} // including ghost cells

    const auto & nGhosts() const {return _nGhosts;}



    size_t size() const noexcept {return _size;} // returns the total number of valid cells

    auto extendedSize() const noexcept {return _extendedSize;}
    
    int nCellsNeighbourhood() const noexcept {return _nNeighboursPerCell;}


    int getNeighbour(index_t iCell, index_t iNeighboorhood ) const noexcept  {return _neighboursPerCell[iCell][iNeighboorhood];  } // includes the cell i from some non zero offset


    inline auto  lowIndex(int d) const noexcept {return _nGhosts[d];}
    inline auto  highIndex(int d) const noexcept {return _shape[d] - 1 + _nGhosts[d];}

    bool checkNeighbourIndexing();



    private:

    void initialize();
    void buildIndexNeighbours();

    void buildBC();
  
    const bool periodic;
    const int _nNeighboursPerCell;
    
    // domain data

    
    index_t _size;
    index_t _extendedSize;

    // topological data
    std::array<index_t, DIM> _shape;
    std::array<index_t, DIM > _nGhosts;
    std::array<index_t, DIM> _extendedShape;


    std::vector<std::vector<size_t> > _neighboursPerCell;

};


};


#endif
#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "tools.h"



template<int DIM>
class lattice
{
    public:
    /*
    Lattice with with dimensions DIM and nGhosts[] ghost cells in each direction
    */

    using index_t = size_t;

    lattice(std::array<size_t,DIM>  shape, std::array<real_t,DIM> lowerEdge, std::array<real_t,DIM > higherEdge,std::array<size_t,DIM> nGhosts=TRUNCATE<DIM>::array(0ul,0ul,0ul));




    ENABLE_IF_DIM(DIM,3)
    inline index_t index(index_t i , index_t j , index_t k) const { return _extendedShape[0]*(  _extendedShape[1]*k + j   )  + i ; } // fuse a 3d index in a 1d index ( fortran ordering )

    ENABLE_IF_DIM(DIM,3)
    inline index_t index( int  i , int  j , int k) const { return _extendedShape[0]*(  _extendedShape[1]*k + j   )  + i ; } // fuse a 3d index in a 1d index ( fortran ordering )
    
        
    ENABLE_IF_DIM(DIM,2)
    inline index_t index(index_t i, index_t j) const {return _extendedShape[0]*j + i;}

    ENABLE_IF_DIM(DIM,1)
    inline auto  index(index_t i) const {return i;}



    const auto & lengthBox() const {return _lBox;}
    const auto & inverseLengthBox() const {return _lBoxInverse;}
    const auto & lowerEdge() const {return _lowerEdge;}



    // map d indices to a single index
    ENABLE_IF_DIM(DIM,3)
    inline auto index(const std::array<index_t,3> & i) const {return index(i[0],i[1],i[2]);}

    ENABLE_IF_DIM(DIM,2)
    inline auto index(const std::array<index_t,2> & i) const {return index(i[0],i[1]);} 

    ENABLE_IF_DIM(DIM,1)
    inline index_t index(const std::array<index_t,1> & i) const {return index(i[0]);}


    ENABLE_IF_DIM(DIM,3)
    std::array<index_t,DIM> index(index_t iCell) const 
    {
        auto  k = iCell/(_extendedShape[0]*_extendedShape[1]);
        iCell=iCell - k*_extendedShape[0]*_extendedShape[1];
        auto  j = iCell/(_extendedShape[0]);
        auto  i = iCell - j*_extendedShape[0];
        return std::array<index_t,3>{i,j,k};
    }


    int dimensions() const noexcept{return DIM;}
    const auto & shape() const noexcept{return _shape;}


    const auto & extendedShape() const noexcept {return _extendedShape;} // including ghost cells

    const auto & nGhosts() const {return _nGhosts;}



    size_t size() const noexcept {return _size;} // returns the total number of valid cells

    auto extendedSize() const noexcept {return _extendedSize;}
    
    int nCellsNeighbourhood() const noexcept {return _nNeighboursPerCell;}


    int getNeighbour(index_t iCell, index_t iNeighboorhood ) const noexcept  {return _neighbourOffsets[iNeighboorhood] + iCell; } // includes the cell i from some non zero offset


    inline auto  lowIndex(int d) const noexcept {return _nGhosts[d];}
    inline auto  highIndex(int d) const noexcept {return _shape[d] - 1 + _nGhosts[d];}



    inline real_t wrap( index_t iCell,int d) const noexcept { return _wrap[d][iCell];} // return -1/0/1 * lBox[d] depending if the cell is on the low face, bulk , high face in direction d

    bool checkNeighbourIndexing();
    

    private:

    void initialize();
    void buildIndexOffsets();

    void buildBC();
  
    const bool periodic;
    const int _nNeighboursPerCell;
    
    // domain data

    std::array<real_t,DIM> _lBoxInverse;
    std::array<real_t,DIM > _lBox;
    std::array<real_t,DIM > _lowerEdge;
    std::array<real_t,DIM > _higherEdge;
    index_t _size;
    index_t _extendedSize;

    // topological data
    std::array<index_t, DIM> _shape;
    std::array<index_t, DIM > _nGhosts;
    std::array<index_t, DIM> _extendedShape;

    std::array<index_t, 27> _neighbourOffsets;

    std::array<std::vector<real_t> ,DIM > _wrap;
};





#endif
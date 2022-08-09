#include "lattice.h"
#include <iostream>

auto nNeighbours(int dim)
{
    int _nNeighboursPerCell=0;
    if (dim==3)
    {
        _nNeighboursPerCell=26;
    }
    else if (dim==2)
    {
        _nNeighboursPerCell=8;
    }
    else if( dim == 1)
    {
        _nNeighboursPerCell=2;
    }
    else 
    {
        throw std::runtime_error("Dimensions should be 1,2 or 3");
    }

    return _nNeighboursPerCell;
}


template<int DIM>
lattice<DIM>::lattice(std::array<size_t,DIM>  shape, std::array<real_t,DIM> lowerEdge, std::array<real_t,DIM > higherEdge,std::array<size_t,DIM> nGhosts) :
_shape(shape),
_nGhosts(nGhosts),
_lowerEdge(lowerEdge),
_higherEdge(higherEdge),
 periodic(true),
 _nNeighboursPerCell(nNeighbours(DIM))
{

    initialize();
};

template<int DIM>
void lattice<DIM>::initialize()
{
    _size=1;
    _extendedSize=1;

    for (int d=0;d<DIM;d++)
        {
            _lBox[d]=_higherEdge[d]-_lowerEdge[d];
            _lBoxInverse[d]=1./_lBox[d];
            _extendedShape[d]=_shape[d] + 2*_nGhosts[d];
            _size*=_shape[d];
            _extendedSize*=_extendedShape[d];
        }
    buildIndexNeighbours();
}

auto pbc( int i, int shape)
{
    return i>=shape ? i - shape : (
        i<0 ? i + shape : i 
    );
}

template<>
void lattice<3>::buildIndexNeighbours()
{
    int t=0;
    _neighboursPerCell.resize(size());

    for (int k=lowIndex(2);k<=highIndex(2);k++)
        for (int j=lowIndex(1);j<=highIndex(1);j++)
            for ( int  i=lowIndex(0);i<=highIndex(0);i++)
            {   
                    auto iCell = index(i,j,k);

                    _neighboursPerCell[iCell].resize( nNeighbours(3));

                    int iN=0;
                    for(int ii=-1;ii<=1;ii++)
                        for(int jj=-1;jj<=1;jj++)
                            for(int kk=-1;kk<=1;kk++)
                            {    
                                if (not(ii==0 and jj==0 and kk==0))
                                {
                                    _neighboursPerCell[iCell][iN]=index( 
                                        pbc(i +ii, _shape[0]),
                                        pbc(j +jj, _shape[1]),
                                        pbc(k +kk, _shape[2])
                                    );
                                    iN++;

                                }
                                
                            }
            }

}



template<>
void lattice<2>::buildIndexNeighbours()
{
    int t=0;
    _neighboursPerCell.resize(size());

        for (int j=lowIndex(1);j<=highIndex(1);j++)
            for ( int  i=lowIndex(0);i<=highIndex(0);i++)
            {   
                    auto iCell = index(i,j);

                    _neighboursPerCell[iCell].resize( nNeighbours(2));

                    int iN=0;
                    for(int ii=-1;ii<=1;ii++)
                        for(int jj=-1;jj<=1;jj++)
                            {    
                                if (not(ii==0 and jj==0))
                                {
                                    _neighboursPerCell[iCell][iN]=index( 
                                        pbc(i +ii, _shape[0]),
                                        pbc(j +jj, _shape[1])
                                                                                                                );
                                    iN++;

                                }
                                
                            }
            }

}




template<>
bool lattice<3>::checkNeighbourIndexing()
{

    for (int  i=lowIndex(0);i<=highIndex(0);i++)
        for (int   j=lowIndex(1);j<=highIndex(1);j++)
            for (int   k=lowIndex(2);k<=highIndex(2);k++)
            {

                auto iCell = index(i,j,k);

                for (int ii=-1;ii<=1;ii++)
                    for (int jj=-1;jj<=1;jj++)
                        for (int kk=-1;kk<=1;kk++)
                        {
                            int iCell2=index( 
                                pbc(  i+ii,_shape[0]),
                                pbc(j+jj,_shape[1]),
                                pbc(k+kk,_shape[2])  );
                            bool found=false;

                            for(int t=0;t<nCellsNeighbourhood();t++)
                            {
                                int iCell2Check=getNeighbour(iCell,t);

                                if (iCell2Check==iCell2)
                                {
                                    
                                    found=true;
                                }
                            }
                            
                            if (ii==0 and jj==0 and kk==0)
                            {
                                if (found)
                                {
                                    std::cout << i << " " <<  j << " " << k<<std::endl;
                                    std::cout << i + ii  << " " <<  j  + jj << " " << k + kk<< std::endl;
                                    
                                    
                                    return false;
                                }   
                            }
                            else
                            {
                                if (!found)
                                {
                                    std::cout << i << " " <<  j << " " << k<<std::endl;
                                    std::cout << ((int)i + ii )  << " " <<  j + jj << " " << k + kk<< std::endl;
                                    return false;
                                }
                            }                            

                        }
            }
    return true;
}


template<>
bool lattice<2>::checkNeighbourIndexing()
{

    for (int  i=lowIndex(0);i<=highIndex(0);i++)
        for (int  j=lowIndex(1);j<=highIndex(1);j++)
            {

                auto  iCell = index(i,j);

                for (int ii=-1;ii<=1;ii++)
                    for (int jj=-1;jj<=1;jj++)
                        {
                            int iCell2=index( pbc(i+ii,_shape[0]),pbc(j+jj,_shape[1]));
                            bool found=false;

                            for(int t=0;t<nCellsNeighbourhood();t++)
                            {
                                int iCell2Check=getNeighbour(iCell,t);

                                if (iCell2Check==iCell2)
                                {
                                    found=true;
                                }
                            }

                            if (ii==0 and jj==0)
                            {    
                                if (found)
                                {
                                    return false;
                                }   
                            }
                            else
                            {
                                if (!found) {
                                    std::cout << i << std::endl;
                                    return false;
                                    }
                            }


                        }
            }
    return true;
}

template class lattice<3>;
template class lattice<2>;

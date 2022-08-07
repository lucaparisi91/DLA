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
    buildBC();
    buildIndexOffsets();
}

template<>
void lattice<3>::buildBC()
{
    for(int d=0;d<3;d++)
    {
        _wrap[d].resize(extendedSize() , 0 );
    }

    for (index_t i=0;i<_extendedShape[0];i++)
        for (index_t j=0;j<_extendedShape[1];j++)
            for (index_t k=0;k<_extendedShape[2];k++)
            {
                int currentIndex=index(i,j,k);
                std::array<index_t,3> index3d{i,j,k};

                for(int d=0;d<3;d++)
                {
                    if (index3d[d]<lowIndex(d))
                    {
                        _wrap[d][currentIndex]=-_lBox[d];
                    }
                    else if (index3d[d]>highIndex(d))
                    {
                        _wrap[d][currentIndex]=+_lBox[d];
                    }
                    else
                    {
                        _wrap[d][currentIndex]=0;
                    }
                    
                }
            
            }
    
}

template<>
void lattice<2>::buildBC()
{
    for(int d=0;d<2;d++)
    {
        _wrap[d].resize(extendedSize() , 0 );
    }

    for (index_t  i=0;i<_extendedShape[0];i++)
        for (index_t  j=0;j<_extendedShape[1];j++)
            {
                auto  currentIndex=index(i,j);
                std::array<index_t,2> index2d{i,j};

                for(int d=0;d<2;d++)
                {
                    if (index2d[d]<lowIndex(d))
                    {
                        _wrap[d][currentIndex]=-_lBox[d];
                    }
                    else if (index2d[d]>highIndex(d))
                    {
                        _wrap[d][currentIndex]=+_lBox[d];
                    }
                    else
                    {
                        _wrap[d][currentIndex]=0;
                    }
                    
                }
            
            }
    
}

template<>
void lattice<1>::buildBC()
{
    for(int d=0;d<1;d++)
    {
        _wrap[d].resize(extendedSize() , 0 );
    }

    for (index_t i=0;i<_extendedShape[0];i++)
            {
                int currentIndex=index(i);
                

                    if ( i <lowIndex(0))
                    {
                        _wrap[0][currentIndex]=-_lBox[0];
                    }
                    else if ( i >highIndex(0))
                    {
                        _wrap[0][currentIndex]=+_lBox[0];
                    }
                    else
                    {
                        _wrap[0][currentIndex]=0;
                    }
                    
            }   
}

template<>
void lattice<3>::buildIndexOffsets()
{
    int t=0;
    for (index_t k=0;k<=2;k++)
        for (index_t j=0;j<=2;j++)
            for (index_t i=0;i<=2;i++)
            {
                if ( not ( i==1 and j==1 and k==1 ) )
                {
                    _neighbourOffsets[t]=index(i,j,k) - index(index_t(1),index_t(1),index_t(1));
                    t++;
                }
            }

}


template<>
void lattice<2>::buildIndexOffsets()
{
    int t=0;
        for (index_t j=0;j<=2;j++)
            for (index_t i=0;i<=2;i++)
            {
                if ( not ( i==1 and j==1 ) )
                {
                _neighbourOffsets[t]=index(i,j) - index( index_t(1),index_t(1));
                t++;
                }
            }
}


template<>
bool lattice<3>::checkNeighbourIndexing()
{

    for (auto  i=lowIndex(0);i<=highIndex(0);i++)
        for (auto  j=lowIndex(1);j<=highIndex(1);j++)
            for (auto  k=lowIndex(2);k<=highIndex(2);k++)
            {

                auto iCell = index(i,j,k);

                for (int ii=-1;ii<=1;ii++)
                    for (int jj=-1;jj<=1;jj++)
                        for (int kk=-1;kk<=1;kk++)
                        {

                            int iCell2=index(i+ii,j+jj,k+kk);
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
                                    return false;
                                }   
                            }
                            else
                            {
                                if (!found)
                                {
                                    std::cout << ii << " " << jj << " " << kk << std::endl;
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

    for (auto  i=lowIndex(0);i<=highIndex(0);i++)
        for (auto  j=lowIndex(1);j<=highIndex(1);j++)
            {

                auto  iCell = index(i,j);

                for (int ii=-1;ii<=1;ii++)
                    for (int jj=-1;jj<=1;jj++)
                        {
                            int iCell2=index(i+ii,j+jj);
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

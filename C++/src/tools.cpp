#include "tools.h"
#include "randomGenerator.h"
#include "lattice/particles.h"











namespace mesh{

template<int DIM>
void addRandom(particles<DIM> & state, size_t N,randState_t & randG, index_t offset )
{
    std::vector<mesh::index_t> indices;
    indices.resize( N ,0);

    randomGenerator::uniformIntDistribution<mesh::index_t> indexDis(0,state.getLattice().size() -1    );


    indexDis.generate(indices.begin(),indices.end(),randG);
    
    for(int i=0;i<N;i++)
    {
        state.add(i + offset, indices[i] );
    }

}

template void addRandom<2>(particles<2> & , size_t ,randState_t &, index_t);
template void addRandom<3>(particles<3> & , size_t ,randState_t &,index_t );

}
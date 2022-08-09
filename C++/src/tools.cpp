#include "tools.h"
#include "randomGenerator.h"
#include "lattice/particles.h"

template<int DIM>
void initRandom(particles<DIM> & state, size_t N,randState_t & randG )
{
    std::vector<mesh::index_t> indices;
    indices.resize( N ,0);

    randomGenerator::uniformIntDistribution<mesh::index_t> indexDis(0,state.getLattice().size() -1    );


    indexDis.generate(indices.begin(),indices.end(),randG);

    for(int i=0;i<N;i++)
    {
        state.add(i, indices[i] );
    }

}


template void initRandom<2>(particles<2> & , size_t ,randState_t &);
template void initRandom<3>(particles<3> & , size_t ,randState_t &);

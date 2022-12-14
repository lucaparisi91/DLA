#ifndef TRAITS_H
#define TRAITS_H
#include <unsupported/Eigen/CXX11/Tensor>

using real_t = double;
#define DIMENSIONS 2

#if DIMENSIONS == 2
#define TRUNC(a,b,c) a,b
#endif

#if DIMENSIONS == 3
#define TRUNC(a,b,c) a,b,c
#endif



#define START_TIMER(a) 
#define STOP_TIMER(a)

namespace randomGenerator
{
    class cppRandomState;
}

using  randState_t=randomGenerator::cppRandomState;


namespace offMesh
{
    class cluster_t;

    using state_t = cluster_t;
    using tensor_t = Eigen::Tensor<real_t, 2> ;

    class geometry;

    using geometry_t = geometry;

};


template<int DIM> class particles;

namespace mesh
{
    
    using index_t = size_t;
    template<int DIM> class clusterOnLattice;


    template<int DIM>
    using state_t = class clusterOnLattice<DIM>;
};


#endif
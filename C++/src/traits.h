#ifndef TRAITS_H
#define TRAITS_H
#include <unsupported/Eigen/CXX11/Tensor>

using real_t = double;
#define DIMENSIONS 3


using tensor_t = Eigen::Tensor<real_t, 2> ;

class geometry;

using geometry_t = geometry;


#define START_TIMER(a) 
#define STOP_TIMER(a)

namespace randomGenerator
{
    class cppRandomState;
}

using  randState_t=randomGenerator::cppRandomState;

class cluster_t;

using state_t = cluster_t;

#endif
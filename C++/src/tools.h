#ifndef TOOLS_H
#define TOOLS_H


#define ENABLE_IF_EQ( DIM , target  )     template<int T = DIM,typename = typename std::enable_if< T==target >::type>

#define ENABLE_IF_GE( DIM , target  )     template<int T = DIM,typename = typename std::enable_if< T>=target >::type>

#include "traits.h"

using real_t = double;

template<int d>
struct TRUNCATE
{

};

template<>
struct TRUNCATE<3> 
{
    template<class T>
    static auto array(T a,T b,T c) {return std::array<T,3>({a,b,c});}

};


template<>
struct TRUNCATE<2> 
{
     template<class T>
    static auto array(T a,T b,T c) {return std::array<T,2>({a,b});}
   
};


template<>
struct TRUNCATE<1> 
{
    template<class T>
    static auto array(T a,T b,T c) {return std::array<T,1>({a});}
};


namespace mesh{

template<int DIM> class particles;

template<int DIM>
void addRandom( particles<DIM> & state, size_t N, randState_t & randG , index_t offset=0);

}


#endif
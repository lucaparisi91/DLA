#ifndef TOOLS_LATTICE_H
#define TOOL_LATTICE_H

using real_t = double;


#define ENABLE_IF_DIM( DIM , target  )     template<int T = DIM,typename = typename std::enable_if<T==target>::type>

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


#endif
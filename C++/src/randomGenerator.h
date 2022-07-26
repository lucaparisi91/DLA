#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <random>

namespace randomGenerator
{

class cppRandomState
{
    public:

    cppRandomState(int seed) : _generator(seed) {}

    auto & getGenerator() {return _generator;}

    private:
    std::default_random_engine _generator;
};

class normalDistribution
{
public:

    normalDistribution() : distributionR(0,1) {}

    template<class it1_t,class it2_t >
    void generate( const it1_t & it1,const it2_t & it2,cppRandomState & randState)
    {
        for( auto it = it1 ; it < it2 ; it++)
        {
            *it=distributionR(randState.getGenerator() );
        }
    }

    private:

    std::normal_distribution<double> distributionR;


};


class uniformDistribution
{
public:

    uniformDistribution( double minX, double maxX) : distributionD(minX,maxX) {}


    template<class it1_t,class it2_t >
    void generate( const it1_t & it1,const it2_t & it2,cppRandomState & randState)
    {
        for( auto it = it1 ; it < it2 ; it++)
        {
            *it=distributionD(randState.getGenerator() );
        }
    }


    private:

    std::uniform_real_distribution<double> distributionD;


};


};
#endif
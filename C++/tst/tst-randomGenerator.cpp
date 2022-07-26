#include "gtest/gtest.h"
#include "../src/randomGenerator.h"



TEST( random, normal)
{
    std::vector<double> x( 100000);

    randomGenerator::cppRandomState randG(567);

    randomGenerator::normalDistribution dist;

    dist.generate(x.begin(),x.end(),randG);

    double mean=0;
    double var=0;

    for(int i = 0 ; i< x.size();i++)
    {
        mean+=x[i];
        var+=x[i]*x[i];
    }

    mean/=x.size();
    var/=x.size();

    ASSERT_NEAR( mean, 0 ,  1e-3);
    ASSERT_NEAR( var, 1, 1e-2);

}

TEST( random, uniform)
{
    std::vector<double> x( 1000000);

    randomGenerator::cppRandomState randG(567);

    randomGenerator::uniformDistribution dist(0,1);

    dist.generate(x.begin(),x.end(),randG);

    double mean=0;
    double var=0;

    for(int i = 0 ; i< x.size();i++)
    {
        mean+=x[i];
        var+=x[i]*x[i];
    }

    mean/=x.size();
    var/=x.size();

    ASSERT_NEAR( mean, 0.5 ,  1e-3);
    ASSERT_NEAR( var, 1./3, 1e-3);


}


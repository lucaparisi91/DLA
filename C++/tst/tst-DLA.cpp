#include "gtest/gtest.h"
#include "../src/DLA.h"
#include "../src/randomGenerator.h"

#define TOL 1e-4


TEST( DLA , init)
{
    int N=100;
    real_t lBox {100};
    real_t radius {1};


    randState_t randG(567);
    offMesh::state_t freeParticles(N,DIMENSIONS);
    offMesh::state_t clusterParticles(N,DIMENSIONS);

    randomGenerator::uniformRealDistribution<double> uniform(-lBox/2,lBox/2);

    offMesh::cluster_t particles( N - 1,1);
    auto & free = particles.getFree();
    auto & cluster = particles.getCluster();

    uniform.generate(free.data() , free.data() + free.size() , randG );
    uniform.generate(cluster.data() , cluster.data() + cluster.size() , randG );
    real_t sigma=0.1;
    offMesh::geometry geo(lBox);

    offMesh::DLA alg( { TRUNC(sigma,sigma,sigma) },radius,geo);
    

    alg.initializeCluster(particles);

    for(int i=0;i< particles.getNFree();i++)
    {

        for(int j=0;j<particles.getNCluster();j++)
        {
            real_t r2=0;
            for(int d=0;d<DIMENSIONS;d++)
            {
                real_t diff=geo.difference( free(i,d) - cluster(j,d),d );
                r2+=diff*diff;
            }

            ASSERT_GT(r2,radius*radius);

        }
    
    }

    if ( particles.getNCluster() > 1 )
    {
    for(int i=0;i< particles.getNCluster();i++)
    {
        bool inCluster=false;

        for(int j=0;j<particles.getNCluster();j++)
        {
            real_t r2=0;
            for(int d=0;d<DIMENSIONS;d++)
            {
                real_t diff=geo.difference( cluster(i,d) - cluster(j,d) ,d );
                r2+=diff*diff;
            }
            

            //std::cout << i << " " << r2<<std::endl;

            if( r2<=radius*radius and (j!=i) )
            {
                inCluster=true;
            }

        }

        ASSERT_TRUE(inCluster);
    
    }


    }

    alg.advance(particles,randG);


};


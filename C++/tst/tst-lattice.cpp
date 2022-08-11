#include "gtest/gtest.h"
#include "../src/lattice/lattice.h"
#include "../src/lattice/particles.h"
#include "../src/tools.h"


TEST( lattice , init3d)
{
    mesh::lattice<3> lattice3d( {200,50,100} );

    ASSERT_TRUE(lattice3d.checkNeighbourIndexing() );


    auto iCell=lattice3d.index(0,0,0);
    auto iCell2=lattice3d.index(200-1,0,0);

    bool found=false;

    for(int t=0;t<lattice3d.nCellsNeighbourhood();t++)
    {
        auto iiCell = lattice3d.getNeighbour(iCell,t);
        if (iiCell == iCell2)
        {
            found=true;
        }
    }

    ASSERT_TRUE(found);

    auto [i,j,k] = lattice3d.indices(lattice3d.index(4,6,8) );

    ASSERT_EQ(i,4);
    ASSERT_EQ(j,6);
    ASSERT_EQ(k,8);
    


    
};


TEST( lattice , init2d)
{
    mesh::lattice<2> lattice2d( {100,100});
    ASSERT_TRUE(lattice2d.checkNeighbourIndexing() );

   
    auto iCell=lattice2d.index(0,0);
    auto iCell2=lattice2d.index(100-1,0);

    bool found=false;

    for(int t=0;t<lattice2d.nCellsNeighbourhood();t++)
    {
        auto iiCell = lattice2d.getNeighbour(iCell,t);
        if (iiCell == iCell2)
        {
            found=true;
        }
    }

    auto [i,j] = lattice2d.indices(lattice2d.index(4,6) );

    ASSERT_EQ(i,4);
    ASSERT_EQ(j,6);

    ASSERT_TRUE(found);

};



TEST( particles , D3)
{
    
    size_t N = 100;
    auto lattice3d=std::make_shared<mesh::lattice<3> >( std::array<size_t,3>{100,100,100} );


    auto free = mesh::particles<3>(lattice3d);

    ASSERT_EQ(free.size(),0);

    free.resize(N);

    const auto & cells = free.getCells();

    auto indexCell = lattice3d->index(3,4,5);

    free.add( 0 , indexCell );

    ASSERT_EQ(free.size(),1 );

    ASSERT_EQ( cells[ indexCell ].nParticles() ,1 );
    ASSERT_EQ( cells[ indexCell ][0] ,0 );

    free.remove(0);

    ASSERT_EQ(free.size(),0 );
    ASSERT_EQ( cells[ indexCell ].nParticles() ,0 );



    free.add( 0,  lattice3d->index(3,4,5)  );
    free.add( 1, lattice3d->index(3,4,5) );
    free.add( 5, lattice3d->index(3,4,5) );
    
    free.add( 2, lattice3d->index(4,4,5) );
    free.add( 3, lattice3d->index(2,4,5) );
    free.add( 4, lattice3d->index(3,4,6) );


    ASSERT_EQ(free.size(),6 );
    ASSERT_EQ( cells[ lattice3d->index(3,4,5) ].nParticles() ,3 );
    free.remove(1);
    ASSERT_EQ( cells[ lattice3d->index(3,4,5) ].nParticles() ,2 );


    ASSERT_EQ( free.cellIndex(0) , lattice3d->index(3,4,5) );
    ASSERT_EQ( free.cellIndex(5) , lattice3d->index(3,4,5) );
    ASSERT_EQ( free.cellIndex(4) , lattice3d->index(3,4,6) );

    ASSERT_EQ( cells[free.cellIndex(4) ].nParticles() , 1 );


};

#include "../src/moves.h"

TEST( particles , cluster2D )
{
    
    size_t N = 100;
    auto mesh=std::make_shared<mesh::lattice<2> >( std::array<size_t,2> {100,100});

    auto free = std::make_shared< mesh::particles<DIMENSIONS> >( mesh);
    auto cluster = std::make_shared< mesh::particles<DIMENSIONS> >(mesh);
    free->resize(N);
    cluster->resize(N);

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),0);

   
    for ( size_t i=0;i<mesh->size();i++)
    {
        auto n2= cluster->getCells()[i].nParticles();    
        ASSERT_EQ(n2,0);
    }

    free->add(0, mesh->index(5,8));
    cluster->add(1, mesh->index(6,8));
    mesh::state_t<2> state(free,cluster);

    auto iCell = free->cellIndex(0);
    free->remove(0);
/*
    cluster->add(0,iCell);
    

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),2);

    ASSERT_EQ(cluster->cellIndex(0) ,  mesh->index(5,8) );
    ASSERT_EQ(cluster->cellIndex(1) ,  mesh->index(6,8) );
 */
}

TEST( particles , cluster3D )
{
    
    size_t N = 100;
    auto mesh=std::make_shared<mesh::lattice<3> >( std::array<size_t,3> {100,100,100});

    auto free = std::make_shared< mesh::particles<3> >( mesh);
    auto cluster = std::make_shared< mesh::particles<3> >(mesh);
    free->resize(N);
    cluster->resize(N);

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),0);

   
    for ( size_t i=0;i<mesh->size();i++)
    {
        auto n2= cluster->getCells()[i].nParticles();    
        ASSERT_EQ(n2,0);
    }

    free->add(0, mesh->index(5,8,3));
    cluster->add(1, mesh->index(6,8,3));
    mesh::state_t<3> state(free,cluster);

    auto iCell = free->cellIndex(0);
    free->remove(0);
/*
    cluster->add(0,iCell);
    

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),2);

    ASSERT_EQ(cluster->cellIndex(0) ,  mesh->index(5,8) );
    ASSERT_EQ(cluster->cellIndex(1) ,  mesh->index(6,8) );
 */
}


TEST( particles, move )
{
     size_t N = 1000,NX=30,NY=10;
    auto lattice=std::make_shared<mesh::lattice<2> >( std::array<size_t,2> { NX,NY} );

    mesh::particles<2> free( lattice);
    randState_t randG(567);

    mesh::addRandom( free , N, randG,0);

    int nMoves=100;

    std::vector<mesh::index_t> iX(nMoves*N);
    std::vector<mesh::index_t> iY(nMoves*N);

    randomGenerator::uniformIntDistribution<size_t> disX(0,NX-1);
    randomGenerator::uniformIntDistribution<size_t> disY(0,NY-1);

    disX.generate(iX.begin(),iX.end(),randG);
    disY.generate(iY.begin(),iY.end(),randG);

    ASSERT_TRUE(free.isCoherent() );


    size_t k=0;
    for(int t=0;t<nMoves;t++)
    {
        for(int iParticle=0;iParticle<N;iParticle++)
        {
            auto k = t*N + iParticle;
            free.move(iParticle,lattice->index(iX[ k],iY[k]));
        }

        for(int iParticle=0;iParticle<N;iParticle++)
        {
            auto k = t*N + iParticle;
            
            auto iCell=free.cellIndex(iParticle);
            ASSERT_EQ(iCell,lattice->index(iX[ k],iY[k]));



        }

        ASSERT_TRUE(free.isCoherent() );
        
    }


}


TEST( moves , diffusion )
{

    size_t N = 100;
    auto mesh=std::make_shared<mesh::lattice<2> >( std::array<size_t,2> {100,100} );

    
    auto free = std::make_shared< mesh::particles<2> >( mesh);
    auto cluster = std::make_shared< mesh::particles<2> >(mesh);
    free->resize(N);
    cluster->resize(N);


    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),0);


    free->add(0, mesh->index(5,8));
    cluster->add(1, mesh->index(6,8));
    mesh::state_t<2> state(free,cluster);
    
    mesh::diffusionMove<2> move;

    randState_t randG(567);


    state.updateClusterAssignment(0);


    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),2);

    free->add(2, mesh->index(3,8));

    ASSERT_EQ( free->cellIndex(2)  ,mesh->index(3,8));


    state.updateClusterAssignment(2);


    ASSERT_EQ(free->size(),1);
    ASSERT_EQ(cluster->size(),2);


    state.getFree().move(2,mesh->index(3,7));


    ASSERT_EQ(free->size(),1);
    ASSERT_EQ(cluster->size(),2);

    ASSERT_EQ(state.getFree().cellIndex(2),mesh->index(3,7));

    state.getFree().move(2,mesh->index(4,7));

    state.updateClusterAssignment(2);


    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),3);

    ASSERT_EQ(state.getCluster().cellIndex(2),mesh->index(4,7) );

    // test random diffusion

    for(int iT=0;iT<=10;iT++)
    {
        free->add(3, mesh->index(93,2));

        move.move(3,state,randG);

        // check if the particle as moved to a neighbour

        bool found=false;
        for(int ii=-1;ii<=1;ii++)
            for(int jj=-1;jj<=1;jj++)
        {
            if ( state.getFree().cellIndex(3) == mesh->index(93+ii,2+jj)  and not(ii==0 and jj==0))
            {
                found=true;    
            }
        }


        ASSERT_TRUE(found);

        free->remove(3);

    }



    // test diffusion to random position of many particles


}

TEST(particles, tensor)
{
    size_t N = 100;
    mesh::index_t NX=100, NY=100;

    auto lattice=std::make_shared<mesh::lattice<2> >( std::array<mesh::index_t,2>{NX,NY} );

    auto free = std::make_shared< mesh::particles<2> >( lattice);
    
    randState_t randG(567);

    Eigen::Tensor<int,2> freeData(N,2);

    randomGenerator::uniformIntDistribution<int> distributionX(0,NX-1);
    randomGenerator::uniformIntDistribution<int> distributionY(0,NY-1);


    distributionX.generate(freeData.data() , freeData.data() + N  ,randG);
    distributionY.generate(freeData.data() + N, freeData.data() + 2*N  ,randG);

    free->add(freeData);

    ASSERT_EQ(  free->nParticles(),N);
    int k=0;
    for(auto it=free->begin();it!=free->end();it++)
    {
        auto [i,j] = lattice->indices( it->second);
        ASSERT_EQ( freeData(it->first,0 ) , i    );
        ASSERT_EQ( freeData(it->first,1 ) , j    );
        k++;
    }
    ASSERT_EQ(k,N);

    auto freeData2=free->toTensor();


    ASSERT_EQ( freeData2.dimensions()[0] , N );
    ASSERT_EQ( freeData2.dimensions()[1] , 2 );
    

    for(int i=0;i<N;i++)
    {
        ASSERT_EQ( freeData(i,0) , freeData2(i,0) );
    }


}

template<int DIM>
void  testClusterOwnership( const mesh::state_t<DIM> & state )
{
    const auto & free = state.getFree();
    const auto & cluster = state.getCluster();
    const auto & lattice = free.getLattice();


      // check all free particles do not have a neighbour in the cluster
    for (auto it = free.begin();it != free.end();it++)
    {
        for(int ii=0;ii<lattice.nCellsNeighbourhood() ;ii++ )
        {
            auto iCell2 = lattice.getNeighbour(  it->second , ii );
            ASSERT_LT(iCell2,lattice.size());
            ASSERT_GE(iCell2, 0 );
            
            auto n2= cluster.getCells()[iCell2].nParticles();

            ASSERT_EQ(n2,0);
        }
    }

    if (cluster.nParticles() > 1 )
    {
        for (auto it = cluster.begin();it != cluster.end();it++)
        {
            bool found=false;
            for(int ii=0;ii<lattice.nCellsNeighbourhood() ;ii++ )
            {
                auto iCell2 = lattice.getNeighbour(  it->second , ii );
                ASSERT_LT(iCell2,lattice.size());
                ASSERT_GE(iCell2, 0 );
                
                auto n2= cluster.getCells()[iCell2].nParticles();

                if (n2 != 0)
                {
                    found =true;
                }
            }

            ASSERT_TRUE(found);
        }
    }


}


TEST( cluster,ownership)
{
    size_t N = 4000;
    auto mesh=std::make_shared<mesh::lattice<2> >( std::array<size_t,2> {100,100} );   


    randState_t randG(567);

    for(int t=0;t<40;t++)
        {
            auto free = std::make_shared< mesh::particles<2> >( mesh);
            auto cluster = std::make_shared< mesh::particles<2> >(mesh);

            mesh::state_t<2> state(free,cluster);
            mesh::addRandom( state.getFree() , N ,randG , 1 );
            mesh::addRandom( state.getCluster() , 1 ,randG );
            state.updateClusterAssignment();

            testClusterOwnership( state );

            ASSERT_TRUE(state.getFree().isCoherent() );
            ASSERT_TRUE(state.getCluster().isCoherent() );
        
            
        }
}



TEST( moves, diffusion_collective)
{
    size_t N = 4000;
    auto lattice=std::make_shared<mesh::lattice<2> >( std::array<size_t,2> {100,100} );

    randState_t randG(567);

    
    auto free = std::make_shared< mesh::particles<2> >( lattice);
    auto cluster = std::make_shared< mesh::particles<2> >(lattice);

    mesh::state_t<2> state(free,cluster);

    mesh::diffusionMove<2> move;

    mesh::addRandom( state.getFree() , N ,randG, 1 );
    mesh::addRandom( state.getCluster() , 1 ,randG );


    for(int t=0;t<100;t++)
    {
        move.move(state,randG);
        state.updateClusterAssignment();

        testClusterOwnership(state);

        ASSERT_TRUE(state.getFree().isCoherent() );
        ASSERT_TRUE(state.getCluster().isCoherent() );
        

        
    }    

    

}




TEST( moves, diffusion_collective_3d)
{
    size_t N = 4000;
    auto lattice=std::make_shared<mesh::lattice<3> >( std::array<size_t,3> {100,100,100} );

    randState_t randG(567);

    
    auto free = std::make_shared< mesh::particles<3> >( lattice);
    auto cluster = std::make_shared< mesh::particles<3> >(lattice);

    mesh::state_t<3> state(free,cluster);

    mesh::diffusionMove<3> move;

    mesh::addRandom( state.getFree() , N ,randG, 1 );
    mesh::addRandom( state.getCluster() , 1 ,randG );


    for(int t=0;t<100;t++)
    {
        move.move(state,randG);
        state.updateClusterAssignment();

        testClusterOwnership(state);

        ASSERT_TRUE(state.getFree().isCoherent() );
        ASSERT_TRUE(state.getCluster().isCoherent() );
        
    }    

    

}


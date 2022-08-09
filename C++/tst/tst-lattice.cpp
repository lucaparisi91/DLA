#include "gtest/gtest.h"
#include "../src/lattice/lattice.h"
#include "../src/lattice/particles.h"
#include "../src/tools.h"


TEST( lattice , init3d)
{
    lattice<3> lattice3d( {200,50,100},{-1,-1,-1},{1,1,1});

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

    
};


TEST( lattice , init2d)
{
    lattice<2> lattice2d( {100,100},{-1,-1},{1,1});
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

    ASSERT_TRUE(found);

};



TEST( particles , D3)
{
    
    size_t N = 100;
    auto lattice3d=std::make_shared<lattice<3> >( std::array<size_t,3>{100,100,100},std::array<real_t,3>{-1,-1,-1},std::array<real_t,3>{1,1,1});
    
    auto free = particles<3>(lattice3d);

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


TEST( particles , cluster )
{
    
    size_t N = 100;
    auto mesh=std::make_shared<lattice<DIMENSIONS> >( std::array<size_t,DIMENSIONS> {TRUNC(100,100,100)},
    std::array<real_t,DIMENSIONS>{TRUNC(-1,-1,-1)},
    std::array<real_t,DIMENSIONS>{ TRUNC(1,1,1)});
    
    auto free = std::make_shared< particles<DIMENSIONS> >( mesh);
    auto cluster = std::make_shared< particles<DIMENSIONS> >(mesh);
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
     mesh::state_t state(free,cluster);
    
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

TEST( moves , diffusion )
{

    size_t N = 100;
    auto mesh=std::make_shared<lattice<DIMENSIONS> >( std::array<size_t,DIMENSIONS> {TRUNC(100,100,100)},
    std::array<real_t,DIMENSIONS>{TRUNC(-1,-1,-1)},
    std::array<real_t,DIMENSIONS>{ TRUNC(1,1,1)});
    
    auto free = std::make_shared< particles<DIMENSIONS> >( mesh);
    auto cluster = std::make_shared< particles<DIMENSIONS> >(mesh);
    free->resize(N);
    cluster->resize(N);

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),0);


    free->add(0, mesh->index(5,8));
    cluster->add(1, mesh->index(6,8));
    mesh::state_t state(free,cluster);
    
    mesh::diffusionMove move;

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

}

TEST(moves,initCluster)
{
    size_t N = 4000;
    auto mesh=std::make_shared<lattice<DIMENSIONS> >( std::array<size_t,DIMENSIONS> {TRUNC(100,100,100)},
    std::array<real_t,DIMENSIONS>{TRUNC(-1,-1,-1)},
    std::array<real_t,DIMENSIONS>{ TRUNC(1,1,1)});

    auto free = std::make_shared< particles<DIMENSIONS> >( mesh);
    auto cluster = std::make_shared< particles<DIMENSIONS> >(mesh);

    mesh::state_t state(free,cluster);

    randState_t randG(567);

    initRandom( state.getFree() , N ,randG );
    initRandom( state.getCluster() , 1 ,randG );


    mesh::diffusionMove move;

    state.updateClusterAssignment();

    // check all free particles do not have a neighbour in the cluster
    for (auto it = free->begin();it != free->end();it++)
    {
        for(int ii=0;ii<mesh->nCellsNeighbourhood() ;ii++ )
        {
            auto iCell2 = mesh->getNeighbour(  it->second , ii );
            ASSERT_LT(iCell2,mesh->size());
            ASSERT_GE(iCell2, 0 );
            
            auto n2= cluster->getCells()[iCell2].nParticles();

            ASSERT_EQ(n2,0);
        }
    }


    for (auto it = cluster->begin();it != cluster->end();it++)
    {
        bool found=false;
        for(int ii=0;ii<mesh->nCellsNeighbourhood() ;ii++ )
        {
            auto iCell2 = mesh->getNeighbour(  it->second , ii );
            ASSERT_LT(iCell2,mesh->size());
            ASSERT_GE(iCell2, 0 );
            
            auto n2= cluster->getCells()[iCell2].nParticles();

            if (n2 != 0)
            {
                found =true;
            }
        }

        ASSERT_TRUE(found);
    }





}


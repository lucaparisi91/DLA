#include "gtest/gtest.h"
#include "../src/lattice/lattice.h"
#include "../src/lattice/particles.h"

TEST( lattice , init3d)
{
    lattice<3> lattice3d( {200,50,100},{-1,-1,-1},{1,1,1});

    ASSERT_TRUE(lattice3d.checkNeighbourIndexing() );
};


TEST( lattice , init2d)
{
    lattice<2> lattice2d( {100,100},{-1,-1},{1,1});
    ASSERT_TRUE(lattice2d.checkNeighbourIndexing() );
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


    free->add(0, mesh->index(5,8));
    cluster->add(1, mesh->index(6,8));
    mesh::state_t state(free,cluster);
    state.moveToCluster(0);
    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),2);

    ASSERT_EQ(cluster->cellIndex(0) ,  mesh->index(5,8) );
    ASSERT_EQ(cluster->cellIndex(1) ,  mesh->index(6,8) );

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


    move.updateState(0,state);

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),2);

    free->add(2, mesh->index(3,8));

    ASSERT_EQ( free->cellIndex(2)  ,mesh->index(3,8));


    move.updateState(2,state);

    ASSERT_EQ(free->size(),1);
    ASSERT_EQ(cluster->size(),2);

    move.move(2,state,mesh->index(3,7));

    ASSERT_EQ(free->size(),1);
    ASSERT_EQ(cluster->size(),2);

    ASSERT_EQ(state.getFree().cellIndex(2),mesh->index(3,7));

    move.move(2,state,mesh->index(4,7));

    ASSERT_EQ(free->size(),0);
    ASSERT_EQ(cluster->size(),3);

    ASSERT_EQ(state.getCluster().cellIndex(2),mesh->index(4,7) );

    // test random diffusion

    for(int iT=0;iT<=10;iT++)
    {
        free->add(3, mesh->index(93,2));

        move.move(3,state,randG);

        bool found=false;
        for(int ii=-1;ii<=1;ii++)
            for(int jj=-1;jj<=1;jj++)
        {
            if ( state.getFree().cellIndex(3) == mesh->index(93+ii,2+jj) )
            {
                found=true;    
            }
        }


        ASSERT_TRUE(found);

        free->remove(3);

    }





}

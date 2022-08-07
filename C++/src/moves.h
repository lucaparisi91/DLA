
#include "traits.h"
#include "randomGenerator.h"

namespace mesh
{
    using index_t = size_t ;

    class clusterOnLattice
    {
        public:

        clusterOnLattice(std::shared_ptr<particles_t> free,std::shared_ptr<particles_t> cluster) : _free(free),_cluster(cluster) {}


        auto & getFree() {return *_free;}
        const auto & getFree() const  {return *_free;}

        const auto & getCluster() const  {return *_cluster;}
        auto & getCluster()   {return *_cluster;}


        void moveToCluster(index_t iParticle);


        private:

        std::shared_ptr<particles_t> _free;
        std::shared_ptr<particles_t> _cluster;
    };


    class diffusionMove
    {
        public:

        diffusionMove() : _distribution(0,std::pow(3,DIMENSIONS) - 2   ) {};



        void move(index_t iParticle,state_t & state, index_t iCell2 ); // moves the particle iParticle to the iCell2 cell. If one of the neighbour is occupied the particle is added to the cluster

        void move(index_t iParticle,state_t & state, randState_t & randG ); // moves the particle in a random direction.

        void updateState(size_t iParticle,state_t & state);
        
        private:

        randomGenerator::uniformIntDistribution<int> _distribution;

    };



}
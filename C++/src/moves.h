
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


        void updateClusterAssignment(); // assign in a recusive way all particles to the cluster


        void updateClusterAssignment(index_t iParticle); // decides wheater to assign to the cluster in a non recusive way



        private:

        bool isCloseToCluster(index_t iCell);

        void assignToCluster(index_t iParticle);


        std::shared_ptr<particles_t> _free;
        std::shared_ptr<particles_t> _cluster;
        std::vector<index_t> _tmp_particles;


    };

    class diffusionMove
    {
        public:

        diffusionMove() : _distribution(0,std::pow(3,DIMENSIONS) - 2   ) {};


        void move(index_t iParticle,state_t & state, randState_t & randG ); // moves the particle in a random direction.

        
        void move(state_t & state, randState_t & randG ); // moves the particle in a random direction.



        private:

        randomGenerator::uniformIntDistribution<int> _distribution;
        std::vector<index_t> _particlesToRemove;
    };




}
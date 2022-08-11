
#include "traits.h"
#include "randomGenerator.h"

namespace mesh
{
    
    
    template<int DIM>
    class diffusionMove
    {
        public:

        using state_t = clusterOnLattice<DIM>;
        

        diffusionMove() : _distribution(0,std::pow(3,DIMENSIONS) - 2   ) {};


        void move(index_t iParticle,state_t & state, randState_t & randG ); // moves the particle in a random direction.

        
        void move(state_t & state, randState_t & randG ); // moves the particle in a random direction.



        private:

        randomGenerator::uniformIntDistribution<int> _distribution;
        std::vector<index_t> _particlesToRemove;
    };




}
#include "particles.h"

template<int DIM>
particles<DIM>::particles(std::shared_ptr< lattice_t > _lattice) :
_lattice(_lattice),_cells(_lattice->size() ),_nParticles(0)
{

};



template class particles<3>;
template class particles<2>;

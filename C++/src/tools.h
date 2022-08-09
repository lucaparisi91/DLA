#include "traits.h"

template<int> class particles;

template<int DIM>
void initRandom(particles<DIM> & state, size_t N,randState_t & randG );

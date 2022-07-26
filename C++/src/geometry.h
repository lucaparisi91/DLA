
#include "traits.h"


class geometry
{
public:


  geometry(){}
  geometry(real_t lBoxx_,real_t lBoxy_,real_t lBoxz_) : lBox{ lBoxx_,lBoxy_,lBoxz_},lBoxInverse{1./lBoxx_,1./lBoxy_,1./lBoxz_} {} ;

  geometry(real_t lBox ) : geometry(lBox,lBox,lBox) {};
  
  real_t difference(real_t t, int i ) const {return ( t - std::round(t*lBoxInverse[i] )*lBox[i]);}

  real_t getLBox(int i) const {return lBox[i];}

  real_t pbc(real_t t, int i) const {return difference(t,i);}



private:
	real_t lBox [3];
	real_t lBoxInverse [3];

};
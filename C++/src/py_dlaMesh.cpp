#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "traits.h"
#include <iostream>
#include "DLA.h"
#include <memory>
#include "moves.h"
#include "py_tools.h"
#include "lattice/particles.h"

namespace py = pybind11;

template<int DIM>
class py_dlaMesh
{
    public:

    py_dlaMesh( 
        py::array_t<int> free,
        py::array_t<int> cluster, 
        py::array_t<real_t> shape
     ) : randG(567),_constantFreeParticles(true)
    {
        auto _shape = toVector(shape);
        auto _freeTensor = toTensor<int,2>(free);
        auto _clusterTensor = toTensor<int,2>(cluster);

        std::array<mesh::index_t,DIM> arrShape;
        for(int d=0;d<DIM;d++)
        {
            arrShape[d]=_shape[d];
        }

        auto _lattice=std::make_shared<mesh::lattice<DIM> >(arrShape);

        auto _free = std::make_shared< mesh::particles<DIM> >( _lattice);
        auto _cluster = std::make_shared< mesh::particles<DIM> >(_lattice);

        _free->add(_freeTensor);
        _cluster->add(_clusterTensor,_freeTensor.dimensions()[0]);


        _state = std::make_unique<mesh::state_t<DIM> >(_free,_cluster);
        _N0Free=_free->nParticles();
        _offset=_N0Free + _cluster->nParticles() ;
        

    }

    py::array_t<int> getFree()
    {
        _freeData=_state->getFree().toTensor();
        return toArray(_freeData);
    };

    py::array_t<int> getCluster()
    {
        _clusterData=_state->getCluster().toTensor();
        return toArray(_clusterData);
    };


    void advance( int nSteps) {

        for ( mesh::index_t i =0;i<nSteps;i++)
        {   

            _move.move(*_state,randG);
            _state->updateClusterAssignment();

            if (_constantFreeParticles)
            {
                auto nParticlesNew=_N0Free - (_state->getFree().nParticles() );
                mesh::addRandom( _state->getFree() , nParticlesNew, randG, _offset );
                _offset+=nParticlesNew;
            }
        }


    }

    private:

    randState_t randG;
    mesh::diffusionMove<DIM> _move;
    std::unique_ptr< mesh::clusterOnLattice<DIM> > _state;

    Eigen::Tensor<int,2> _freeData;
    Eigen::Tensor<int,2> _clusterData;

    const bool _constantFreeParticles;
    mesh::index_t _N0Free,_offset;
};


PYBIND11_MODULE(dla_cpp, m) {
     py::class_<py_dlaMesh<2> >(m, "onLattice2d")
     .def(py::init< py::array_t<int > , py::array_t<int > , py::array_t<real_t> >() )
     .def("getFree",&py_dlaMesh<2>::getFree)
     .def("getCluster",&py_dlaMesh<2>::getCluster)
     .def("advance",&py_dlaMesh<2>::advance)
     ;
     py::class_<py_dlaMesh<3> >(m, "onLattice3d")
     .def(py::init< py::array_t<int > , py::array_t<int > , py::array_t<real_t> >() )
     .def("getFree",&py_dlaMesh<3>::getFree)
     .def("getCluster",&py_dlaMesh<3>::getCluster)
     .def("advance",&py_dlaMesh<3>::advance)
     ;
  ;

}
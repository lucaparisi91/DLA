#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include "traits.h"
#include <iostream>
#include "DLA.h"
#include <memory>

namespace py = pybind11;
using namespace offMesh;


auto toTensor(const py::array_t<double> & x)
{
    
    py::buffer_info info = x.request();
    if (info.shape.size() != 2)
    {
        throw std::runtime_error("Numpy array should have rank 2");
    }
    auto xRaw = x.unchecked<2>();

    Eigen::Tensor<real_t,2> tensor(info.shape[0],info.shape[1]);

    size_t k=0;
    for(int j=0;j<info.shape[1];j++)
        for(int i=0;i<info.shape[0];i++)
            {
                tensor(i,j)=xRaw(i,j);
                k++;
            }
    return tensor;
}


auto toVector(const py::array_t<double> & x)
{
    
    py::buffer_info info = x.request();
    if (info.shape.size() != 1)
    {
        throw std::runtime_error("Numpy array should have rank 1");
    };
    auto xRaw = x.unchecked<1>();
    
    std::vector<real_t> xCpp(info.shape[0]);

    for(int i=0;i<info.shape[0];i++)
        {
            xCpp[i]=xRaw(i);
        }
    return xCpp;
}

auto toParticles(const py::array_t<double> & freePy,const py::array_t<double> & clusterPy)
{
    py::buffer_info infoFree = freePy.request();
    py::buffer_info infoCluster = clusterPy.request();


    size_t N1= infoFree.shape[0];
    size_t N2= infoCluster.shape[0];

    size_t N = N1 + N2;

    cluster_t particles( N1,N2);

    auto & freeCpp = particles.getFree();
    auto & clusterCpp = particles.getCluster();

    auto freeRaw = freePy.unchecked<2>();
    auto clusterRaw = clusterPy.unchecked<2>();

    for(int i=0;i<N1;i++)
    {
        for(int d=0;d<DIMENSIONS;d++)
        {
            freeCpp(i,d)=freeRaw(i,d);
        }
    }

    for(int i=0;i<N2;i++)
    {
        for(int d=0;d<DIMENSIONS;d++)
        {
            clusterCpp(i,d)=clusterRaw(i,d);
        }
    }



    return particles;

}








auto toArray(const Eigen::Tensor<real_t,2> & tensor  )
{
    auto NX=tensor.dimensions()[0];
    auto NY=tensor.dimensions()[1];

    return py::array_t<double>(
            {NX,NY}, // shape
            {sizeof(double) , sizeof(double)*NX}, // F-style contiguous
            tensor.data() );
}


auto freeParticlestoArray(const cluster_t & particles  )
{
   auto & free = particles.getFree();
   size_t nFree = particles.getNFree();
   size_t NY= DIMENSIONS;    
   py::array_t<double> freePy(
            {nFree, NY} );

   auto rawFree = freePy.mutable_unchecked<2>();

   for(int i=0;i<nFree;i++)
    {
        for(int d=0;d<DIMENSIONS;d++)
        {
            rawFree(i,d)=free(i,d);
        }
    }
    return freePy;
}

auto clusterParticlestoArray(const cluster_t & particles  )
{
   auto & cluster = particles.getCluster();
   size_t nCluster = particles.getNCluster();
   size_t NY= DIMENSIONS;    
   py::array_t<double> clusterPy(
            {nCluster, NY} );

   auto rawCluster = clusterPy.mutable_unchecked<2>();

   for(int i=0;i<nCluster;i++)
    {
        for(int d=0;d<DIMENSIONS;d++)
        {
            rawCluster(i,d)=cluster(i,d);
        }
    }
    return clusterPy;
}

class py_dla
{
    public:

    py_dla( 
        py::array_t<double> x,
        py::array_t<double> y,
        real_t radius,
        py::array_t<real_t> box,
        py::array_t<real_t> sigma
     ) : randG(567)
    {
        _free=toTensor(x);
        _cluster=toTensor(y);
        _radius=radius;
        _box=toVector(box);
        _sigma=toVector(sigma);


        geometry geo( {_box[0],_box[1],_box[2]});

        std::array<real_t,DIMENSIONS> _sigmaArr { TRUNC(_sigma[0],_sigma[1],_sigma[2] )  };
        alg=std::make_unique<DLA>( _sigmaArr ,_radius,geo);
        _particles = std::make_unique<cluster_t>(toParticles(x,y) );

        alg->initializeCluster(*_particles);

    };


    auto getFree() {return freeParticlestoArray(*_particles); }
    auto getCluster() {return clusterParticlestoArray(*_particles); }

    void advance( size_t nSteps)
    {
        for(int i=0;i<nSteps;i++)
        {
            alg->advance(*_particles,randG);
        }
        
    } 

    private:
    Eigen::Tensor<real_t,2> _free;
    Eigen::Tensor<real_t,2> _cluster;

    real_t _radius;
    std::vector<real_t> _box;
    std::vector<real_t> _sigma;

    
    std::unique_ptr<DLA> alg;
    std::unique_ptr<cluster_t> _particles;
    randState_t randG;

};

PYBIND11_MODULE(py_dla, m) {
     py::class_<py_dla>(m, "py_dla")
     .def(py::init< py::array_t<real_t > , py::array_t<real_t> , real_t,py::array_t<real_t>, py::array_t<real_t> >() )
     .def("getFree",&py_dla::getFree)
     .def("getCluster",&py_dla::getCluster)
     .def("advance",&py_dla::advance)
     

     ;
  ;           
}
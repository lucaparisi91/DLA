#ifndef PY_TOOLS_H
#define PY_TOOLS_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;
#include <unsupported/Eigen/CXX11/Tensor>

using real_t = double;

template<class T, int DIM>
Eigen::Tensor< T , DIM > toTensor(const py::array_t<T> & x)
{

    py::buffer_info info = x.request();
    if (info.shape.size() != DIM)
    {
        throw std::runtime_error("Numpy array should have rank 2");
    };

    auto xRaw = x.template unchecked<2 >( );

    Eigen::Tensor<T,DIM> tensor;
    
    if constexpr( DIM == 2)
    {
        tensor.resize(info.shape[0],info.shape[1]);

        for(int j=0;j<info.shape[1];j++)
            for(int i=0;i<info.shape[0];i++)
                {
                    tensor(i,j)=xRaw(i,j);
                }
    }


    if constexpr( DIM == 3)
    {
        tensor.resize(info.shape[0],info.shape[1],info.shape[2]);
        
        for(int k=0;k<info.shape[2];k++)        
            for(int j=0;j<info.shape[1];j++)
                for(int i=0;i<info.shape[0];i++)
                {
                    tensor(i,j,k)=xRaw(i,j,k);
                }
    }


    return tensor;

    }

std::vector<real_t> toVector(const py::array_t<double> & x);

template<class T,int DIM>
py::array_t<T> toArray(const Eigen::Tensor<T,DIM> & tensor  )
{
    if constexpr (DIM==2)
    {
        auto NX=tensor.dimensions()[0];
        auto NY=tensor.dimensions()[1];

        return py::array_t<T>(
            {NX,NY}, // shape
            {sizeof(T) , sizeof(T)*NX}, // F-style contiguous
            tensor.data() );
    }

    if constexpr (DIM==3)
    {
        auto NX=tensor.dimensions()[0];
        auto NY=tensor.dimensions()[1];
        auto NZ=tensor.dimensions()[2];


        return py::array_t<T>(
            {NX,NY,NZ}, // shape
            {sizeof(T) , sizeof(T)*NX,sizeof(T)*NX*NY}, // F-style contiguous
            tensor.data() );
    }

}

#endif
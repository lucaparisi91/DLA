
#include "traits.h"
#include "geometry.h"
#include "randomGenerator.h"

class cluster_t 
{
    public:


    cluster_t( size_t nFree,size_t nCluster) : _nFree(nFree),_nCluster(nCluster),_N(_nFree + _nCluster) {
        _free.resize(_N,DIMENSIONS);
        _cluster.resize(_N,DIMENSIONS);
    }


    auto & getFree() { return _free;}
    auto & getCluster() { return _cluster;}

    const auto & getFree() const  { return _free;}
    const auto & getCluster() const { return _cluster;}

    size_t getNFree() const {return _nFree; }
    size_t getNCluster() const {return _nCluster; }
    size_t getN() const {return _N; }

    void initCluster();

    void addToCluster( size_t i);



    private:

    tensor_t _free;
    tensor_t _cluster;
    size_t _nFree;
    size_t _nCluster;
    size_t _N;
};



class DLA
{
    public:

    DLA(real_t sigma,real_t radius,geometry_t geo) : _sigma(sigma),_radius(radius),_geo(geo) {};
    
    void setSigma(real_t sigma) {_sigma=sigma;}
    void setParticleRadius(real_t radius){_radius=radius;};
    void setGeometry( const geometry_t & geo) { _geo=geo;};


    void initializeCluster(state_t & state);

    void advance(state_t & state,randState_t & randG);
    

    private:

    bool addToClusterCondition(const state_t & state,size_t i);


    real_t _sigma;
    real_t _radius;

    geometry_t _geo;
    std::vector<real_t> _noise;
    randomGenerator::normalDistribution norm;

};
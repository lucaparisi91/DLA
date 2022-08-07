#include <vector>


class cellData
{   
    public:

    using index_t = size_t;


    cellData():
    _nParticles(0)
    {}

    void resize(index_t newSize)  {_particleIndices.resize(newSize) ; }

    size_t size() const noexcept {return _nParticles;}


    index_t add(index_t iParticle){
        if (_nParticles >=_particleIndices.size())
        {
            _particleIndices.resize(_nParticles + buffer);
        }
        _particleIndices[_nParticles]=iParticle;
        _nParticles++;
        return _nParticles-1;
    }

    auto nParticles() const  {return _nParticles;}

    void remove(index_t subCellIndex)
    {
        std::swap(_particleIndices[_nParticles-1] , _particleIndices[subCellIndex]);
        _nParticles--;
    }


    auto move(index_t subCellIndex, cellData & cellDestination)
    {
        int iNewSubCell=cellDestination.add( (*this)[subCellIndex ]);
        remove(subCellIndex);
        return iNewSubCell;
    }  


    index_t  operator[](index_t i) const {
        return _particleIndices[i]; 
        } // get the i_th particle index

    private:  

    const int buffer = 10;
    std::vector<index_t> _particleIndices;
    index_t _nParticles; // number of particles containted in the box
};


class cells
{

    public:

    using cell_t=cellData;
    using index_t = cell_t::index_t;

    cells(index_t N) : _cells(N) {}
    const auto & operator[](size_t i) const  {return _cells[i];}
    auto & operator[](size_t i)  {return _cells[i];}
    
    private:

    std::vector<cell_t> _cells;
};
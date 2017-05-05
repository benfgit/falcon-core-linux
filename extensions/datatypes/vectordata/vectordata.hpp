// ---------------------------------------------------------------------
// This file is part of falcon-core.
// 
// Copyright (C) 2015, 2016, 2017 Neuro-Electronics Research Flanders
// 
// Falcon-server is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Falcon-server is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with falcon-core. If not, see <http://www.gnu.org/licenses/>.
// ---------------------------------------------------------------------

#ifndef VECTORDATA_H
#define VECTORDATA_H

#include "idata.hpp"
#include <vector>

template <class TYPE>
class VectorData : public IData
{
public:    
    void setData( const std::vector<TYPE>& data ) {
        
        _data = data; //copy
    }
    
    void setData( const TYPE* data, int len ) {
        
        // assert( len == _data.size() );
        std::copy( data, data + len, _data.begin() );
    }
    
    void setSample( int index, const TYPE& data ) {
        
        _data[index] = data;
    }
    
    const std::vector<TYPE> data() { return _data; }
    
    void reserve( int n ) { _data.reserve(n); }
    
protected:
    std::vector<TYPE> _data;
};

template <class TYPE>
class VectorDataType : public AnyDataType
{

ASSOCIATED_DATACLASS(VectorData<TYPE>)
    
public:
    VectorDataType( int n = 1 ) : _size(n) { }
        
    void InitializeData( VectorData<TYPE>& item ) const {
        
        item.reserve( _size );
    }
    
    virtual std::string name() const { return "vector"; }
    
protected:
    int _size;
};


#endif

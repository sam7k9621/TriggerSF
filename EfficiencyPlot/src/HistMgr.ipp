#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

using namespace std;

template<typename T>
void
HistMgr<T>::AddObj( T* obj ) {
    _objmap.erase( obj->GetName() ); // deleting existing instance if already exist
    _objmap[obj->GetName()] = obj ;
}

template<typename T>
T*
HistMgr<T>::GetObj( const string& name ) {
    if( _objmap.count( name ) ) {
        return _objmap.at( name );
    }

    else {
        return NULL;
    }
}

template<typename T>
const T*
HistMgr<T>::GetObj( const string& name ) const {
    if( _objmap.count( name ) ) {
        return _objmap.at( name );
    }

    else {
        return NULL;
    }
}

template<typename T>
void
HistMgr<T>::RemoveObj( const std::string& name ) {
    if( _objmap.count( name ) ) {
        _objmap.erase( name );
    }

    else {
    }
}

template<typename T>
void
HistMgr<T>::CleanAll(){
    for( auto& ptr : _objmap){
        delete ptr.second;
    }
}

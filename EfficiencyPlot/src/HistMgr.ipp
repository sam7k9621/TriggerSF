#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

using namespace std;

template<typename T>
HistMgr<T>::HistMgr(const string& tag):
    _tag(tag)
{}

template<typename T>
HistMgr<T>::HistMgr():
    _tag("")
{}

template<typename T>
void
HistMgr<T>::AddObj( T* obj ) {
    
    string aliasname = MakeAliasName( obj->GetName() );
    string storename = obj->GetName();
    
    obj->SetName(aliasname.c_str());
    obj->SetTitle(_tag.c_str());

    if( _objmap.count( storename )){
        delete _objmap.at( storename);
        _objmap.erase( storename ); // deleting existing instance if already exist
    }
    _objmap[storename] = obj ;
}

template<typename T>
string
HistMgr<T>::MakeAliasName(const string& name){
    
    if(_tag != "")
        return name + "_" + _tag;
    else
        return name;
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

template<typename T>
void 
HistMgr<T>::SetLineColor( const Color_t x){
    for(auto& h : _objmap){
        h.second->SetLineColor(x);
    }
}


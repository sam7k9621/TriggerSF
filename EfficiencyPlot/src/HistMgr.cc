#include "TriggerEfficiency/EfficiencyPlot/interface/HistMgr.h"

using namespace std;

void
HistMgr::AddObj( TH1* obj ) {
    _objmap.erase( obj->GetName() ); // deleting existing instance if already exist
    _objmap[obj->GetName()] = obj ;
}


TH1*
HistMgr::GetObj( const string& name ) {
    if( _objmap.count( name ) ) {
        return _objmap.at( name );
    }

    else {
        return NULL;
    }
}

const TH1*
HistMgr::GetObj( const string& name ) const {
    if( _objmap.count( name ) ) {
        return _objmap.at( name );
    }

    else {
        return NULL;
    }
}

void
HistMgr::RemoveObj( const std::string& name ) {
    if( _objmap.count( name ) ) {
        _objmap.erase( name );
    }

    else {
    }
}

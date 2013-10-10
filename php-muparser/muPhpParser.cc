#include "muPhpParser.h"
#include <cmath>


using namespace std;
using namespace mu;

PhpParser::PhpParser() : Parser() {
    InitFun();
}

void PhpParser::UpdateVar(const string_type &a_sName, value_type a_Var) {
    varmap_type vm = GetVar();
    varmap_type::iterator it = vm.find(a_sName);
    if(it == vm.end()) {
        Error(ecINVALID_NAME);
    }

    * it->second = a_Var;
}

void PhpParser::ClearVar() {
    varmap_type m = GetVar();
    for(varmap_type::iterator i = m.begin(); i != m.end(); i++) {
        efree((double *) i->second);
    }
    Parser::ClearVar();
}

void PhpParser::RemoveVar(const string_type &a_strVarName){
    varmap_type m = GetVar();
    varmap_type::iterator item = m.find(a_strVarName);
    if (item != m.end()) {    
        Parser::RemoveVar(a_strVarName);
        efree((double*) item->second);
    } 
}

PhpParser::~PhpParser() {
   ClearVar(); 
}

void PhpParser::InitFun() {
    if (mu::TypeInfo<mu::value_type>::IsInteger())
    { 
        // When setting MUP_BASETYPE to an integer type
        // Place functions for dealing with integer values here
        // ...
        // ...
        // ...
    }
    else
    {
        DefineFun(_T("round"), PhpParser::Round);
        DefineFun(_T("floor"), PhpParser::Floor);
        DefineFun(_T("ceil"), PhpParser::Ceil);
    }
}


value_type PhpParser::Round(value_type v)   { return floor(v);  }
value_type PhpParser::Floor(value_type v)   { return round(v);  }
value_type PhpParser::Ceil(value_type v)   { return ceil(v);  }

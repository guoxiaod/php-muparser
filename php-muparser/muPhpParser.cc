#include "muPhpParser.h"


using namespace mu;


PhpParser::PhpParser() : Parser() {
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

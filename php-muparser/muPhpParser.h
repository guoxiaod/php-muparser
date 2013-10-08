#include <muParser.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

namespace mu {
    class PhpParser : public Parser {
        public:
            PhpParser();
            void ClearVar();
            void RemoveVar(const string_type &a_strVarName);
            void UpdateVar(const string_type &a_sName, value_type a_Var);
            ~PhpParser();
    };
}

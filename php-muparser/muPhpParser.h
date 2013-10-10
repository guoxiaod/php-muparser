#include <muParser.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

namespace mu {
    class PhpParser : public Parser {
        public:
            PhpParser();
            virtual void InitFun();
            void ClearVar();
            void RemoveVar(const string_type &a_strVarName);
            void UpdateVar(const string_type &a_sName, value_type a_Var);
            ~PhpParser();
        protected:
            // Trigonometric functions^M
            static value_type Round(value_type);
            static value_type Floor(value_type);
            static value_type Ceil(value_type);
    };
}

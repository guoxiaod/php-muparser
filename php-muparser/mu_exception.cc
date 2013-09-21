#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "SAPI.h"
#include "php_muparser.h"
#include "util.h"
#include "muParser.h"

using namespace mu;

zend_class_entry * mu_exception_ce;

static zend_object_handlers _mu_exception_handlers;

ZEND_BEGIN_ARG_INFO_EX(arginfo_exception___construct, 0, 0, 0)
    ZEND_ARG_INFO(0, msg)
    ZEND_ARG_INFO(0, code)
    ZEND_ARG_INFO(0, previous)
    ZEND_ARG_INFO(0, parserError)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exception___destruct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exception_GetExpr, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exception_GetMsg, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exception_GetPos, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exception_GetToken, 0, 0, 0)
ZEND_END_ARG_INFO()

PHP_METHOD(exception, __construct) {
    int argc = ZEND_NUM_ARGS();

    int msg_len = 0;
    char * msg;

    long code = 0;
    zval * previous = NULL;
    zval * parserError = NULL;

    if(zend_parse_parameters(argc TSRMLS_CC, "|slzz", &msg, &msg_len, &code, &previous, &parserError) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    if(msg) {
        zend_update_property_string(mu_exception_ce, getThis(), ZEND_STRL("message"), msg TSRMLS_CC);
    }
    if(code) {
        zend_update_property_long(mu_exception_ce, getThis(), ZEND_STRL("code"), code TSRMLS_CC);
    }
    zend_update_property_string(mu_exception_ce, getThis(), ZEND_STRL("_expr"), "" TSRMLS_CC);
    zend_update_property_string(mu_exception_ce, getThis(), ZEND_STRL("_token"), "" TSRMLS_CC);
    zend_update_property_long(mu_exception_ce, getThis(), ZEND_STRL("_pos"), 0 TSRMLS_CC);
}
PHP_METHOD(exception, __destruct) { }
PHP_METHOD(exception, GetExpr) {
    MUP_CHECK_PARAM(0, 0);

    zval * expr =  zend_read_property(mu_exception_ce, getThis(), ZEND_STRL("_expr"), 0 TSRMLS_CC);

    RETURN_ZVAL(expr, 1, 0);
}
PHP_METHOD(exception, GetMsg) {
    MUP_CHECK_PARAM(0, 0);

    zval * msg =  zend_read_property(mu_exception_ce, getThis(), ZEND_STRL("message"), 0 TSRMLS_CC);

    RETURN_ZVAL(msg, 1, 0);
}
PHP_METHOD(exception, GetPos) {
    MUP_CHECK_PARAM(0, 0);

    zval * pos =  zend_read_property(mu_exception_ce, getThis(), ZEND_STRL("_pos"), 0 TSRMLS_CC);

    RETURN_ZVAL(pos, 1, 0);
}
PHP_METHOD(exception, GetToken) {
    MUP_CHECK_PARAM(0, 0);

    zval * token =  zend_read_property(mu_exception_ce, getThis(), ZEND_STRL("_token"), 0 TSRMLS_CC);

    RETURN_ZVAL(token, 1, 0);
}

zend_function_entry mu_exception_methods [] = {
    MUP_ME(exception, __construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    MUP_ME(exception, GetMsg, ZEND_ACC_PUBLIC)
    MUP_ME(exception, GetExpr, ZEND_ACC_PUBLIC)
    MUP_ME(exception, GetToken, ZEND_ACC_PUBLIC)
    MUP_ME(exception, GetPos, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

MUPARSER_STARTUP_FUNCTION(exception) {
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_OPERATOR);    ///< Unexpected binary operator found
    MU_REGISTER_LONG_CONSTANT(ecUNASSIGNABLE_TOKEN);     ///< Token cant be identified.
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_EOF);         ///< Unexpected end of formula. (Example: "2+sin(")
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_ARG_SEP);     ///< An unexpected comma has been found. (Example: "1,23")
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_ARG);         ///< An unexpected argument has been found
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_VAL);         ///< An unexpected value token has been found
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_VAR);         ///< An unexpected variable token has been found
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_PARENS);      ///< Unexpected Parenthesis, opening or closing
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_STR);         ///< A string has been found at an inapropriate position
    MU_REGISTER_LONG_CONSTANT(ecSTRING_EXPECTED);        ///< A string function has been called with a different type of argument
    MU_REGISTER_LONG_CONSTANT(ecVAL_EXPECTED);           ///< A numerical function has been called with a non value type of argument
    MU_REGISTER_LONG_CONSTANT(ecMISSING_PARENS);         ///< Missing parens. (Example: "3*sin(3")
    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_FUN);         ///< Unexpected function found. (Example: "sin(8)cos(9)")
    MU_REGISTER_LONG_CONSTANT(ecUNTERMINATED_STRING);    ///< unterminated string constant. (Example: "3*valueof("hello)")
    MU_REGISTER_LONG_CONSTANT(ecTOO_MANY_PARAMS);        ///< Too many function parameters
    MU_REGISTER_LONG_CONSTANT(ecTOO_FEW_PARAMS);         ///< Too few function parameters. (Example: "ite(1<2,2)")
    MU_REGISTER_LONG_CONSTANT(ecOPRT_TYPE_CONFLICT);     ///< binary operators may only be applied to value items of the same type
    MU_REGISTER_LONG_CONSTANT(ecSTR_RESULT);             ///< result is a string

    // Invalid Parser input Parameters
    MU_REGISTER_LONG_CONSTANT(ecINVALID_NAME);            ///< Invalid function, variable or constant name.
    MU_REGISTER_LONG_CONSTANT(ecINVALID_BINOP_IDENT);     ///< Invalid binary operator identifier
    MU_REGISTER_LONG_CONSTANT(ecINVALID_INFIX_IDENT);     ///< Invalid function, variable or constant name.
    MU_REGISTER_LONG_CONSTANT(ecINVALID_POSTFIX_IDENT);   ///< Invalid function, variable or constant name.

    MU_REGISTER_LONG_CONSTANT(ecBUILTIN_OVERLOAD);        ///< Trying to overload builtin operator
    MU_REGISTER_LONG_CONSTANT(ecINVALID_FUN_PTR);         ///< Invalid callback function pointer 
    MU_REGISTER_LONG_CONSTANT(ecINVALID_VAR_PTR);         ///< Invalid variable pointer 
    MU_REGISTER_LONG_CONSTANT(ecEMPTY_EXPRESSION);        ///< The Expression is empty
    MU_REGISTER_LONG_CONSTANT(ecNAME_CONFLICT);           ///< Name conflict
    MU_REGISTER_LONG_CONSTANT(ecOPT_PRI);                 ///< Invalid operator priority
    // 
    MU_REGISTER_LONG_CONSTANT(ecDOMAIN_ERROR);            ///< catch division by zero, sqrt(-1), log(0) (currently unused)
    MU_REGISTER_LONG_CONSTANT(ecDIV_BY_ZERO);            ///< Division by zero (currently unused)
    MU_REGISTER_LONG_CONSTANT(ecGENERIC);            ///< Generic error
    MU_REGISTER_LONG_CONSTANT(ecLOCALE);             ///< Conflict with current locale

    MU_REGISTER_LONG_CONSTANT(ecUNEXPECTED_CONDITIONAL);
    MU_REGISTER_LONG_CONSTANT(ecMISSING_ELSE_CLAUSE);
    MU_REGISTER_LONG_CONSTANT(ecMISPLACED_COLON);

    MU_REGISTER_LONG_CONSTANT(ecUNREASONABLE_NUMBER_OF_COMPUTATIONS);

    // internal errors
    MU_REGISTER_LONG_CONSTANT(ecINTERNAL_ERROR);   ///< Internal error of any kind.

    // The last two are special entries 
    MU_REGISTER_LONG_CONSTANT(ecCOUNT);            ///< This is no error code, It just stores just the total number of error codes
    MU_REGISTER_LONG_CONSTANT(ecUNDEFINED);        ///< Undefined message, placeholder to detect unassigned error messages

    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, "mu", "ParserException", mu_exception_methods);
    mu_exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), 0 TSRMLS_CC);
    /*
    memcpy(&_mu_exception_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    _mu_exception_handlers.clone_obj = NULL;
    */
    zend_declare_property_string(mu_exception_ce, "_expr", sizeof("_expr")-1, "", ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_string(mu_exception_ce, "_token", sizeof("_token")-1, "", ZEND_ACC_PROTECTED TSRMLS_CC);
    zend_declare_property_long(mu_exception_ce, "_pos", sizeof("_pos")-1, 0, ZEND_ACC_PROTECTED TSRMLS_CC);
}

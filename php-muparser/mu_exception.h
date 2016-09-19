#ifndef __MU_EXCEPTION__
#define __MU_EXCEPTION__

#define MUP_TRY  try {
#define MUP_CATCH } catch (Parser::exception_type &e) {
#define MUP_END_TRY }

#define MUP_THROW_EXCEPTION \
        { \
            zval _ex; \
            zend_object * obj = zend_throw_exception(mu_exception_ce, (char*)e.GetMsg().c_str(), e.GetCode() TSRMLS_DC); \
            ZVAL_OBJ(&_ex, obj); \
            zend_update_property_string(mu_exception_ce, &_ex, ZEND_STRL("_expr"), e.GetExpr().c_str() TSRMLS_CC); \
            zend_update_property_string(mu_exception_ce, &_ex, ZEND_STRL("_token"), e.GetToken().c_str() TSRMLS_CC); \
            zend_update_property_long(mu_exception_ce, &_ex, ZEND_STRL("_pos"), e.GetPos() TSRMLS_CC); \
        }

#define MUP_CATCH_AND_END_TRY \
    MUP_CATCH \
    MUP_THROW_EXCEPTION \
    MUP_END_TRY

MUPARSER_STARTUP_FUNCTION(exception);

extern zend_class_entry * mu_exception_ce;

#endif

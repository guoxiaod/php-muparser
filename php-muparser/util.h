#ifndef __MUPARSER_UTIL_H__
#define __MUPARSER_UTIL_H__

#include <Zend/zend_exceptions.h>

#define MUP_GET_OBJ(T, o)  { \
        mu_wrapper * _r = static_cast<mu_wrapper*>(mup_extract_wrapper(getThis() TSRMLS_CC)); \
        if( _r == NULL) { \
            php_error(E_WARNING, "Cann't call %s() method as static method", get_active_function_name(TSRMLS_C)); \
            RETURN_NULL(); \
        } \
        o = static_cast<T*> (_r->ptr); \
    }
#define MUP_CHECK_PARAM(a, b) { \
        int _c = ZEND_NUM_ARGS(); \
        if(_c < a || _c > b) { \
            WRONG_PARAM_COUNT; \
        } \
    }

#define MU_REGISTER_LONG_CONSTANT(n) \
    REGISTER_NS_LONG_CONSTANT("mu", #n, n, CONST_CS | CONST_PERSISTENT)

#define MUP_CONCAT(a, b) a##b

#define MUP_ME(name, method, flag) \
    PHP_ME(name, method, MUP_CONCAT(arginfo_##name, _##method), flag)


typedef struct _mu_wrapper {
    void * ptr;
    zend_object obj;
} mu_wrapper;

typedef struct _mu_callback {
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
} mu_callback;

mu_wrapper * mup_create_wrapper(zend_class_entry * ce TSRMLS_DC);

void * mup_extract_wrapper(zval * z TSRMLS_DC);

static inline mu_wrapper *php_mu_wrapper_from_obj(zend_object *obj) {
    return (mu_wrapper*)((char*)(obj) - XtOffsetOf(mu_wrapper, obj));
}

#define Z_USEROBJ_P(zv)  php_mu_wrapper_from_obj(Z_OBJ_P((zv)))


#endif

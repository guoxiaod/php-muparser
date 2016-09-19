#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "util.h"

mu_wrapper * mup_create_wrapper(zend_class_entry * ce TSRMLS_DC) {
    mu_wrapper * wrapper;

    wrapper = (mu_wrapper*) ecalloc(1, sizeof(mu_wrapper) + zend_object_properties_size(ce));

    zend_object_std_init(&wrapper->obj, ce TSRMLS_CC);

    object_properties_init(&wrapper->obj, ce);

    return  wrapper;
}

void * mup_extract_wrapper(zval * z TSRMLS_DC) {
    if(!z) {
        char buffer[1024] = {0};
        snprintf(buffer, sizeof(buffer), "method %s() must be invoked on an object", get_active_function_name(TSRMLS_C));
        zend_throw_error_exception(zend_get_error_exception(TSRMLS_C), buffer, 0, 0 TSRMLS_CC);
        return NULL;
    }

    mu_wrapper* obj = (mu_wrapper*) Z_USEROBJ_P(z);
    if(!obj) {
        char buffer[1024] = {0};
        snprintf(buffer, sizeof(buffer), "no object found in %s()", get_active_function_name(TSRMLS_C));
        zend_throw_error_exception(zend_get_error_exception(TSRMLS_C), buffer, 0, 0 TSRMLS_CC);
        return NULL;
    }

    return obj;
}


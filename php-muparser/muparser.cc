
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_muparser.h"
#include "mu_parser.h"
#include "mu_def.h"
#include "mu_exception.h"

/* If you declare any globals in php_muparser.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(muparser)

/* True global resources - no need for thread safety here */
static int le_muparser;

/* {{{ muparser_functions[]
 *
 * Every user visible function must have an entry in muparser_functions[].
 */
const zend_function_entry muparser_functions[] = {
	PHP_FE(confirm_muparser_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in muparser_functions[] */
};
/* }}} */

/* {{{ muparser_module_entry
 */
zend_module_entry muparser_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"muparser",
	muparser_functions,
	PHP_MINIT(muparser),
	PHP_MSHUTDOWN(muparser),
	PHP_RINIT(muparser),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(muparser),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(muparser),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MUPARSER
ZEND_GET_MODULE(muparser)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("muparser.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_muparser_globals, muparser_globals)
    STD_PHP_INI_ENTRY("muparser.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_muparser_globals, muparser_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_muparser_init_globals
 */
/* Uncomment this function if you have INI entries */
static void php_muparser_init_globals(zend_muparser_globals *muparser_globals)
{
	muparser_globals->saved_args = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(muparser)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

    MUPARSER_STARTUP(def);
    MUPARSER_STARTUP(exception);
    MUPARSER_STARTUP(parser);
    //MUPARSER_STARTUP(parser_base);
    //MUPARSER_STARTUP(stoken);
    /*
    MUPARSER_STARTUP(parser_bytecode);
    MUPARSER_STARTUP(parser_callback);
    MUPARSER_STARTUP(parser_error_msg);
    MUPARSER_STARTUP(parser_error);
    MUPARSER_STARTUP(parser_int);
    MUPARSER_STARTUP(parser_stack);
    MUPARSER_STARTUP(type_info);
    MUPARSER_STARTUP(math_impl);
    MUPARSER_STARTUP(parser_token);
    MUPARSER_STARTUP(parser_token_reader);
    */

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(muparser)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
    MUPARSER_SHUTDOWN(parser);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(muparser)
{
    MUPARSER_ACTIVATE_FUNCTION(parser);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(muparser)
{
    MUPARSER_DEACTIVATE_FUNCTION(parser);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(muparser)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "muparser support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_muparser_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_muparser_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "muparser", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

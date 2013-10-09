
#ifndef PHP_MUPARSER_H
#define PHP_MUPARSER_H

extern zend_module_entry muparser_module_entry;
#define phpext_muparser_ptr &muparser_module_entry

#ifdef PHP_WIN32
#	define PHP_MUPARSER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_MUPARSER_API __attribute__ ((visibility("default")))
#else
#	define PHP_MUPARSER_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

using namespace std;

PHP_MINIT_FUNCTION(muparser);
PHP_MSHUTDOWN_FUNCTION(muparser);
PHP_RINIT_FUNCTION(muparser);
PHP_RSHUTDOWN_FUNCTION(muparser);
PHP_MINFO_FUNCTION(muparser);

PHP_FUNCTION(confirm_muparser_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     
*/
ZEND_BEGIN_MODULE_GLOBALS(muparser)
ZEND_END_MODULE_GLOBALS(muparser)

/* In every utility function you add that needs to use variables 
   in php_muparser_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as MUPARSER_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define MUPARSER_G(v) TSRMG(muparser_globals_id, zend_muparser_globals *, v)
#else
#define MUPARSER_G(v) (muparser_globals.v)
#endif


extern ZEND_DECLARE_MODULE_GLOBALS(muparser);

#define MUPARSER_STARTUP_FUNCTION(module)    ZEND_MINIT_FUNCTION(muparser_##module)
#define MUPARSER_STARTUP(module)             ZEND_MODULE_STARTUP_N(muparser_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define MUPARSER_SHUTDOWN_FUNCTION(module)       ZEND_MSHUTDOWN_FUNCTION(muparser_##module)
#define MUPARSER_SHUTDOWN(module)                ZEND_MODULE_SHUTDOWN_N(muparser_##module)(SHUTDOWN_FUNC_ARGS_PASSTHRU)
#define MUPARSER_ACTIVATE_FUNCTION(module)       ZEND_MODULE_ACTIVATE_D(muparser_##module)
#define MUPARSER_ACTIVATE(module)                ZEND_MODULE_ACTIVATE_N(muparser_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define MUPARSER_DEACTIVATE_FUNCTION(module)  ZEND_MODULE_DEACTIVATE_D(muparser_##module)
#define MUPARSER_DEACTIVATE(module)           ZEND_MODULE_DEACTIVATE_N(muparser_##module)(SHUTDOWN_FUNC_ARGS_PASSTHRU)


#ifndef PHP_FE_END
#define PHP_FE_END { NULL, NULL, NULL }
#endif


#endif	/* PHP_MUPARSER_H */


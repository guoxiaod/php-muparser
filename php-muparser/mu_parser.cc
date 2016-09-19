#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "SAPI.h"
#include "php_muparser.h"
#include "util.h"
// #include "muParser.h"
#include "mu_exception.h"
#include "muPhpParser.h"

using namespace std;
using namespace mu;

static zend_class_entry * mu_parser_ce = NULL;
static zend_class_entry * mu_parser_base_ce = NULL;

static zend_object_handlers _mu_parser_handlers;

static void mup_destroy_mu_parser_object(zend_object * object) {
    mu_wrapper * wrapper = php_mu_wrapper_from_obj(object);
    PhpParser * ptr = static_cast<PhpParser*> (wrapper->ptr);
    if(ptr != NULL) {
        delete ptr;
        ptr = NULL;
    }
    zend_object_std_dtor(&wrapper->obj);
    efree(wrapper);
    wrapper = NULL;
}

static zend_object * mup_create_mu_parser_object(zend_class_entry * ce TSRMLS_DC) {
    mu_wrapper * wrapper = mup_create_wrapper(ce);

    MUP_TRY
        wrapper->ptr = new PhpParser();
    MUP_CATCH_AND_END_TRY

    wrapper->obj.handlers = &_mu_parser_handlers;
    
    return &wrapper->obj;
}

/** mu\Parser arguments info {{{ **/

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser___construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser___destruct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_InitCharSets, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_InitFun, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_InitConst, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_InitOprt, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_OnDetectVar, 0, 0, 3)
    ZEND_ARG_INFO(0, pExpr)
    ZEND_ARG_INFO(0, nStart)
    ZEND_ARG_INFO(0, nEnd)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_Diff, 0, 0, 3)
    ZEND_ARG_INFO(1, a_Var) // DONE
    ZEND_ARG_INFO(0, a_fPos)
    ZEND_ARG_INFO(0, a_fEpsilon)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_ClearVar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_RemoveVar, 0, 0, 1)
    ZEND_ARG_INFO(0, a_strVarName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_UpdateVar, 0, 0, 2)
    ZEND_ARG_INFO(0, a_sName)
    ZEND_ARG_INFO(0, a_Var)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_DefineVar, 0, 0, 1)
    ZEND_ARG_INFO(0, a_sName)
    ZEND_ARG_INFO(0, a_fVal)
ZEND_END_ARG_INFO()
/** }}} **/


/** mu\ParserBase arguments info {{{ **/

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_EnableDebugDump, 0, 0, 2)
    ZEND_ARG_INFO(0, bDumpCmd)
    ZEND_ARG_INFO(0, bDumpStack)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base___construct, 0, 0, 0)
    ZEND_ARG_INFO(0, a_Parser)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base___destruct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_Eval, 0, 0, 0)
    ZEND_ARG_INFO(1, nStackSize) // DONE
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_EvalByBulk, 0, 0, 1)
    ZEND_ARG_INFO(0, nBulkSize)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetNumResults, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_SetExpr, 0, 0, 1)
    ZEND_ARG_INFO(0, a_sExpr)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_SetVarFactory, 0, 0, 1)
    ZEND_ARG_INFO(0, a_pFactory)
    ZEND_ARG_INFO(0, pUserData)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_SetDecSep, 0, 0, 1)
    ZEND_ARG_INFO(0, cDecSep)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_SetThousandsSep, 0, 0, 0)
    ZEND_ARG_INFO(0, cThousandsSep)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ResetLocale, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_EnableOptimizer, 0, 0, 0)
    ZEND_ARG_INFO(0, a_bIsOn)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_EnableBuiltInOprt, 0, 0, 0)
    ZEND_ARG_INFO(0, a_bIsOn)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_HasBuiltInOprt, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_AddValIdent, 0, 0, 1)
    ZEND_ARG_INFO(0, a_pCallback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineFun, 0, 0, 2)
    ZEND_ARG_INFO(0, a_strName)
    ZEND_ARG_INFO(0, a_pFun)
    ZEND_ARG_INFO(0, a_bAllowOpt)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineOprt, 0, 0, 2)
    ZEND_ARG_INFO(0, a_strName)
    ZEND_ARG_INFO(0, a_pFun)
    ZEND_ARG_INFO(0, a_iPri)
    ZEND_ARG_INFO(0, a_eAssociativity)
    ZEND_ARG_INFO(0, a_bAllowOpt)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineConst, 0, 0, 2)
    ZEND_ARG_INFO(0, a_sName)
    ZEND_ARG_INFO(0, a_fVal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineStrConst, 0, 0, 2)
    ZEND_ARG_INFO(0, a_sName)
    ZEND_ARG_INFO(0, a_strVal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineVar, 0, 0, 1)
    ZEND_ARG_INFO(0, a_sName)
    ZEND_ARG_INFO(0, a_fVal)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefinePostfixOprt, 0, 0, 2)
    ZEND_ARG_INFO(0, a_strFun)
    ZEND_ARG_INFO(0, a_pOprt)
    ZEND_ARG_INFO(0, a_bAllowOpt)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineInfixOprt, 0, 0, 2)
    ZEND_ARG_INFO(0, a_strName)
    ZEND_ARG_INFO(0, a_pOprt)
    ZEND_ARG_INFO(0, a_iPrec)
    ZEND_ARG_INFO(0, a_bAllowOpt)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ClearVar, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ClearFun, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ClearConst, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ClearInfixOprt, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ClearPostfixOprt, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ClearOprt, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_RemoveVar, 0, 0, 1)
    ZEND_ARG_INFO(0, a_strVarName)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetUsedVar, 0, 0, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetVar , 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetConst , 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetExpr, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetFunDef, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetVersion, 0, 0, 0)
    ZEND_ARG_INFO(0, eInfo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetOprtDef, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineNameChars, 0, 0, 1)
    ZEND_ARG_INFO(0, a_szCharset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineOprtChars, 0, 0, 1)
    ZEND_ARG_INFO(0, a_szCharset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_DefineInfixOprtChars, 0, 0, 1)
    ZEND_ARG_INFO(0, a_szCharset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ValidNameChars, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ValidOprtChars, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_ValidInfixOprtChars, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_SetArgSep, 0, 0, 1)
    ZEND_ARG_INFO(0, cArgSep)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_GetArgSep, 0, 0, 0)
ZEND_END_ARG_INFO()

/*
ZEND_BEGIN_ARG_INFO_EX(arginfo_parser_base_Error, 0, 0, 1)
    ZEND_ARG_INFO(0, a_iErrc)
    ZEND_ARG_INFO(0, a_iPos)
    ZEND_ARG_INFO(0, a_strTok)
ZEND_END_ARG_INFO()
*/

/** }}} **/


/** mu\Parser method {{{ **/
PHP_METHOD(parser, __construct) { }
PHP_METHOD(parser, __destruct) { }

PHP_METHOD(parser, InitCharSets) { 
    MUP_CHECK_PARAM(0, 0);

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    _self->InitCharSets();
}
PHP_METHOD(parser, InitFun) {
    MUP_CHECK_PARAM(0, 0);

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    MUP_TRY
    _self->InitFun();
    MUP_CATCH_AND_END_TRY
}
PHP_METHOD(parser, InitConst) {
    MUP_CHECK_PARAM(0, 0);

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    MUP_TRY
    _self->InitConst();
    MUP_CATCH_AND_END_TRY
}
PHP_METHOD(parser, InitOprt) {
    MUP_CHECK_PARAM(0, 0);

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    _self->InitOprt();
}
PHP_METHOD(parser, OnDetectVar) {
    // TODO
}
PHP_METHOD(parser, Diff) {
    int argc = ZEND_NUM_ARGS();

    zval * a_Var;
    value_type a_fPos = 0.0, a_fEpsilon = 0.0;
    if(zend_parse_parameters(argc TSRMLS_CC, "zdd", &a_Var, &a_fPos, &a_fEpsilon) == FAILURE) {
        WRONG_PARAM_COUNT;
    }
    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    if(Z_TYPE_P(a_Var) != IS_DOUBLE) {
        convert_to_double_ex(a_Var);
    }

    value_type ret = _self->Diff(&Z_DVAL_P(a_Var), a_fPos, a_fEpsilon);

    RETURN_DOUBLE(ret);
}

PHP_METHOD(parser, RemoveVar) {
    int argc = ZEND_NUM_ARGS();
    
    zend_string * sName;

    if(zend_parse_parameters(argc TSRMLS_CC, "S", &sName) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    _self->RemoveVar(ZSTR_VAL(sName));
}

PHP_METHOD(parser, ClearVar) {
    MUP_CHECK_PARAM(0, 0);

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    _self->ClearVar();
}

PHP_METHOD(parser, UpdateVar) {
    int argc = ZEND_NUM_ARGS();

    int key_len = 0;
    zend_string * key;
    value_type val = 0.0;

    if(zend_parse_parameters(argc TSRMLS_CC, "Sd", &key, &val) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    MUP_TRY
    _self->UpdateVar(ZSTR_VAL(key), val);
    MUP_CATCH_AND_END_TRY
}

/** {{{ **/
PHP_METHOD(parser, DefineVar) {
    int argc = ZEND_NUM_ARGS();
    
    zval * key = NULL;
    value_type val = 0;

    if(zend_parse_parameters(argc TSRMLS_CC, "z/|d", &key, &val) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    PhpParser * _self = NULL;
    MUP_GET_OBJ(PhpParser, _self);

    char * _key = NULL;
    zval ** _val = NULL;
    // the first parameter is array
    if(Z_TYPE_P(key) == IS_ARRAY && argc == 1) {
        HashPosition pos;
        HashTable * kv = Z_ARRVAL_P(key);
        MUP_TRY
            //double * arr = (double *) emalloc(sizeof(double) * zend_hash_num_elements(kv));
            for(zend_hash_internal_pointer_reset_ex(kv, &pos);
                zend_hash_get_current_key_ex(kv, &_key, NULL, NULL, 0, &pos) != FAILURE &&
                    zend_hash_get_current_data_ex(kv, (void **) &_val, &pos) != FAILURE; 
                zend_hash_move_forward_ex(kv, &pos)) {

                convert_to_double_ex(_val);
                double * n = (double *) emalloc(sizeof(double));
                *n = Z_DVAL_PP(_val);

                _self->DefineVar(_key, n);
            }
        MUP_CATCH_AND_END_TRY
    // if first parameter is string
    } else if (Z_TYPE_P(key) == IS_STRING && argc == 2) {
        MUP_TRY
            double *n = (double *) emalloc(sizeof(double));
            *n = val;
            _self->DefineVar(Z_STRVAL_P(key), n);
        MUP_CATCH_AND_END_TRY
    } else {
        WRONG_PARAM_COUNT;
    }
}
/** }}} **/
/** }}} **/

/** mu\ParserBase **/
PHP_METHOD(parser_base, __construct) { } 
PHP_METHOD(parser_base, __destruct) { } 

PHP_METHOD(parser_base, EnableDebugDump) {
    int argc = ZEND_NUM_ARGS();

    zend_bool bDumpCmd, bDumpStack;
    if(zend_parse_parameters(argc TSRMLS_CC, "bb", &bDumpCmd, &bDumpStack) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->EnableDebugDump(bDumpCmd, bDumpStack);
}
PHP_METHOD(parser_base, Eval) {
    int argc = ZEND_NUM_ARGS();

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    if(argc == 0) {
        value_type ret;
    
        MUP_TRY
            ret =  _self->Eval(); 
        MUP_CATCH_AND_END_TRY

        RETURN_DOUBLE(ret);
    } else if(argc == 1) {
        zval * nStackSize = NULL;
        if(zend_parse_parameters(argc TSRMLS_CC, "z", &nStackSize) == FAILURE) {
            WRONG_PARAM_COUNT;
        }

        // force to long 
        if(Z_TYPE_P(nStackSize) != IS_LONG) {
            convert_to_long_ex(nStackSize);
        }

        int n = Z_LVAL_P(nStackSize);
        value_type * ret;

        MUP_TRY
            ret = _self->Eval(n);
        MUP_CATCH
            MUP_THROW_EXCEPTION
            RETURN_NULL();
        MUP_END_TRY

        Z_LVAL_P(nStackSize) = n;

        if(ret) {
            array_init(return_value);

            for(int i = 0; i < n; i ++) {
                add_next_index_double(return_value, ret[i]);
            }
            return;
        }
        RETURN_NULL();
    }
    WRONG_PARAM_COUNT;
    RETURN_NULL();
}

PHP_METHOD(parser_base, EvalByBulk) {
    int argc = ZEND_NUM_ARGS();

    long nBulkSize;

    if(zend_parse_parameters(argc TSRMLS_CC, "l", &nBulkSize) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    value_type * results = (value_type*) emalloc(sizeof(value_type) * nBulkSize);

    MUP_TRY
        _self->Eval(results, nBulkSize);
    MUP_CATCH
        MUP_THROW_EXCEPTION
        efree(results);
        RETURN_NULL();
    MUP_END_TRY

    array_init(return_value);

    for(int i = 0; i < nBulkSize; i ++) {
        add_next_index_double(return_value, results[i]);
    }
    efree(results);
}

PHP_METHOD(parser_base, GetNumResults) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    long ret = _self->GetNumResults();

    RETURN_LONG(ret);
}

PHP_METHOD(parser_base, SetExpr) { 
    int argc = ZEND_NUM_ARGS();

    zend_string * sExpr = NULL;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &sExpr) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    MUP_TRY
    _self->SetExpr(ZSTR_VAL(sExpr));
    MUP_CATCH_AND_END_TRY
} 
PHP_METHOD(parser_base, SetVarFactory) {
    int argc = ZEND_NUM_ARGS();

    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
    zval * z = NULL;

    if(zend_parse_parameters(argc TSRMLS_CC, "fz", &fci, &fci_cache, &z) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    // TODO

    facfun_type func = NULL;

    _self->SetVarFactory(func, NULL);
}
PHP_METHOD(parser_base, SetDecSep) { 
    int argc = ZEND_NUM_ARGS();

    zend_string * cDecSep;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &cDecSep) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->SetDecSep(* ZSTR_VAL(cDecSep));
} 
PHP_METHOD(parser_base, SetThousandsSep) {
    int argc = ZEND_NUM_ARGS();

    zend_string * cThousandsSep;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &cThousandsSep) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->SetThousandsSep(*ZSTR_VAL(cThousandsSep));
}
PHP_METHOD(parser_base, ResetLocale) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ResetLocale();
}


PHP_METHOD(parser_base, EnableOptimizer) {
    int argc = ZEND_NUM_ARGS();

    int len = 0;
    zend_bool bIsOn;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "b", &bIsOn) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->EnableOptimizer(bIsOn);
}
PHP_METHOD(parser_base, EnableBuiltInOprt) {
    int argc = ZEND_NUM_ARGS();

    int len = 0;
    zend_bool bIsOn;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "b", &bIsOn) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->EnableBuiltInOprt(bIsOn);
}

PHP_METHOD(parser_base, HasBuiltInOprt) { 
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    bool ret = _self->HasBuiltInOprt();

    RETURN_BOOL(ret);
}
PHP_METHOD(parser_base, AddValIdent) {
    int argc = ZEND_NUM_ARGS();

    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    if(zend_parse_parameters(argc TSRMLS_CC, "f", &fci, &fci_cache) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    // TODO

    identfun_type func = NULL;

    _self->AddValIdent(func);
}

PHP_METHOD(parser_base, DefineFun) {
    int argc = ZEND_NUM_ARGS();

    zend_string * a_strName;
    zend_bool a_bAllowOpt;

    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    if(zend_parse_parameters(argc TSRMLS_CC, "sfb", &a_strName, &fci, &fci_cache, &a_bAllowOpt) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * self = NULL;
    MUP_GET_OBJ(ParserBase, self);

    // TODO
    MUP_TRY
    switch(fci.param_count) {
        case 0:
            //self->DefineFun(a_strName, p, (bool)a_bAllowOpt);
            break;
    
    }
    MUP_CATCH_AND_END_TRY
}
PHP_METHOD(parser_base, DefineOprt) {
    int argc = ZEND_NUM_ARGS();
    
    zend_string * a_sName;
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;
    long a_iPrec;
    long a_eAssoc;
    zend_bool a_bAllowOpt;


    if(zend_parse_parameters(argc TSRMLS_CC, "Sfllb", &a_sName,
            &fci, &fci_cache, &a_iPrec, &a_eAssoc, &a_bAllowOpt) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    // TODO

    fun_type2 func = NULL;

    MUP_TRY
    _self->DefineOprt(ZSTR_VAL(a_sName), func, a_iPrec, (EOprtAssociativity) a_eAssoc, a_bAllowOpt);
    MUP_CATCH_AND_END_TRY
}

/** {{{ **/
PHP_METHOD(parser_base, DefineConst) {
    int argc = ZEND_NUM_ARGS();
    
    zval * key = NULL;
    value_type val = 0.0;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "z/|d", &key, &val) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    HashPosition pos;
    HashTable * kv;

    zval * v = NULL;
    zend_string * k = NULL;
            

    MUP_TRY
    switch(Z_TYPE_P(key)) {
        case IS_ARRAY:
            kv = Z_ARRVAL_P(key);

            ZEND_HASH_FOREACH_STR_KEY_VAL(kv, k, v) {
                convert_to_double_ex(v);
                _self->DefineConst(ZSTR_VAL(k), Z_DVAL_P(v));
            } ZEND_HASH_FOREACH_END();
            break;
        case IS_STRING:
            if(argc == 2) {
                _self->DefineConst(Z_STRVAL_P(key), val);
            } else {
                WRONG_PARAM_COUNT;
            }
            break;
        default:
            WRONG_PARAM_COUNT;
            break;
    }
    MUP_CATCH_AND_END_TRY
}
/** }}} **/

/** {{{ **/
PHP_METHOD(parser_base, DefineStrConst) {
    int argc = ZEND_NUM_ARGS();
    
    zval * key = NULL, * val = NULL;

    if(zend_parse_parameters(argc TSRMLS_CC, "z/|z/", &key, &val) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    HashPosition pos;
    HashTable * kv;

    MUP_TRY
    switch(Z_TYPE_P(key)) {
        case IS_ARRAY:
            kv = Z_ARRVAL_P(key);

            zval * v;
            zend_string * k;
            ZEND_HASH_FOREACH_STR_KEY_VAL(kv, k, v) {
                convert_to_string_ex(v);

                _self->DefineStrConst(ZSTR_VAL(k), Z_STRVAL_P(v));
            } ZEND_HASH_FOREACH_END();
            break;
        case IS_STRING:
            if(argc == 2) {
                convert_to_string_ex(val);
                _self->DefineStrConst(Z_STRVAL_P(key), Z_STRVAL_P(val));
            } else {
                WRONG_PARAM_COUNT;
            }
            break;
        default:
            WRONG_PARAM_COUNT;
            break;
    }
    MUP_CATCH_AND_END_TRY
}
/** }}} **/

/** {{{ **/
PHP_METHOD(parser_base, DefineVar) {
    php_error(E_ERROR, "not implement");
    /*
    int argc = ZEND_NUM_ARGS();
    
    zval * key = NULL;
    value_type val = 0;

    if(zend_parse_parameters(argc TSRMLS_CC, "z/|d", &key, &val) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    // the first parameter is array
    if(Z_TYPE_P(key) == IS_ARRAY && argc == 1) {
        HashPosition pos;
        HashTable * kv = Z_ARRVAL_P(key);


        zval * v = NULL;
        zend_string * k = NULL;
        MUP_TRY
            //double * arr = (double *) emalloc(sizeof(double) * zend_hash_num_elements(kv));
            
            ZEND_HASH_FOREACH_STR_KEY_VAL(kv, k, v) {
                convert_to_double_ex(v);

                double * n = (double *) emalloc(sizeof(double));
                *n = Z_DVAL_P(v);

                _self->DefineVar(ZSTR_VAL(k), n);
            } ZEND_HASH_FOREACH_END();
        MUP_CATCH_AND_END_TRY
    // if first parameter is string
    } else if (Z_TYPE_P(key) == IS_STRING && argc == 2) {
        MUP_TRY
            double *n = (double *) emalloc(sizeof(double));
            *n = val;
            _self->DefineVar(Z_STRVAL_P(key), n);
        MUP_CATCH_AND_END_TRY
    } else {
        WRONG_PARAM_COUNT;
    }
    */
}
/** }}} **/

/** {{{ **/
PHP_METHOD(parser_base, DefinePostfixOprt) {
    int argc = ZEND_NUM_ARGS();
    
    zend_string * a_sName;

    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    zend_bool a_bAllowOpt;


    if(zend_parse_parameters(argc TSRMLS_CC, "Sfb", &a_sName,
            &fci, &fci_cache, &a_bAllowOpt) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    // TODO

    fun_type1 func = NULL;

    MUP_TRY
    _self->DefinePostfixOprt(ZSTR_VAL(a_sName), func, a_bAllowOpt);
    MUP_CATCH_AND_END_TRY
}
/** }}} **/
PHP_METHOD(parser_base, DefineInfixOprt) {
    int argc = ZEND_NUM_ARGS();
    
    zend_string * a_sName;

    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    long a_iPrec;
    zend_bool a_bAllowOpt;


    if(zend_parse_parameters(argc TSRMLS_CC, "Sflb", &a_sName,
            &fci, &fci_cache, &a_iPrec, &a_bAllowOpt) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    // TODO

    fun_type1 func = NULL;

    MUP_TRY
    _self->DefineInfixOprt(ZSTR_VAL(a_sName), func, a_iPrec, a_bAllowOpt);
    MUP_CATCH_AND_END_TRY
}

PHP_METHOD(parser_base, ClearVar) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ClearVar();
}
PHP_METHOD(parser_base, ClearFun) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ClearFun();
}
PHP_METHOD(parser_base, ClearConst) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ClearConst();
}
PHP_METHOD(parser_base, ClearInfixOprt) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ClearInfixOprt();
}
PHP_METHOD(parser_base, ClearPostfixOprt) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ClearPostfixOprt();
}
PHP_METHOD(parser_base, ClearOprt) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->ClearOprt();
}

PHP_METHOD(parser_base, RemoveVar) {
    int argc = ZEND_NUM_ARGS();
    
    zend_string * a_sName;

    if(zend_parse_parameters(argc TSRMLS_CC, "S", &a_sName) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->RemoveVar(ZSTR_VAL(a_sName));
}
PHP_METHOD(parser_base, GetUsedVar) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    varmap_type ret;

    MUP_TRY
        ret = _self->GetUsedVar();
    MUP_CATCH
        MUP_THROW_EXCEPTION
        RETURN_NULL();
    MUP_END_TRY

    array_init(return_value);

    for(varmap_type::iterator i = ret.begin(); i != ret.end(); i ++) {
        add_assoc_double(return_value, i->first.c_str(), *i->second);
    }
}
PHP_METHOD(parser_base, GetVar) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    varmap_type ret = _self->GetVar();

    array_init(return_value);

    for(varmap_type::iterator i = ret.begin(); i != ret.end(); i ++) {
        add_assoc_double(return_value, i->first.c_str(), *i->second);
    }
}

PHP_METHOD(parser_base, GetConst) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    valmap_type ret = _self->GetConst();

    array_init(return_value);

    for(valmap_type::iterator i = ret.begin(); i != ret.end(); i ++) {
        add_assoc_double(return_value, i->first.c_str(), i->second);
    }
}
PHP_METHOD(parser_base, GetExpr) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    string_type ret = _self->GetExpr();

    RETURN_STRINGL(ret.c_str(), ret.size());
}
PHP_METHOD(parser_base, GetFunDef) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    funmap_type ret = _self->GetFunDef();

    array_init(return_value);

    for(funmap_type::iterator i = ret.begin(); i != ret.end(); i ++) {
        add_next_index_string(return_value, i->first.c_str());
    }
}
PHP_METHOD(parser_base, GetVersion) {
    int argc = ZEND_NUM_ARGS();

    int len = 0;
    long eInfo;
    
    if(zend_parse_parameters(argc TSRMLS_CC, const_cast<char*>("l"), &eInfo) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    string_type ret = _self->GetVersion((EParserVersionInfo)eInfo);

    RETURN_STRINGL(ret.c_str(), ret.size());
}

PHP_METHOD(parser_base, GetOprtDef) {
    // TODO
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    const char_type **ret = _self->GetOprtDef();

    //TODO 
    //RETURN_STRINGL(ret, strlen(ret));
}

PHP_METHOD(parser_base, DefineNameChars) {
    int argc = ZEND_NUM_ARGS();

    zend_string * szCharset = NULL;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &szCharset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->DefineNameChars(ZSTR_VAL(szCharset));
}

PHP_METHOD(parser_base, DefineOprtChars) {
    int argc = ZEND_NUM_ARGS();

    zend_string * szCharset = NULL;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &szCharset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->DefineOprtChars(ZSTR_VAL(szCharset));
}
PHP_METHOD(parser_base, DefineInfixOprtChars) {
    int argc = ZEND_NUM_ARGS();

    zend_string * szCharset = NULL;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &szCharset) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->DefineInfixOprtChars(ZSTR_VAL(szCharset));
}

PHP_METHOD(parser_base, ValidNameChars) {
    int argc = ZEND_NUM_ARGS();

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    const char_type * ret = _self->ValidNameChars();
    RETURN_STRINGL(ret, strlen(ret));
}

PHP_METHOD(parser_base, ValidOprtChars) {
    int argc = ZEND_NUM_ARGS();

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    const char_type * ret = _self->ValidOprtChars();
    RETURN_STRINGL(ret, strlen(ret));
}
PHP_METHOD(parser_base, ValidInfixOprtChars) {
    int argc = ZEND_NUM_ARGS();

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    const char_type * ret = _self->ValidInfixOprtChars();
    RETURN_STRINGL(ret, strlen(ret));
}

PHP_METHOD(parser_base, SetArgSep) {
    int argc = ZEND_NUM_ARGS();

    zend_string * cArgSep = NULL;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "S", &cArgSep) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->SetArgSep(*ZSTR_VAL(cArgSep));
}
PHP_METHOD(parser_base, GetArgSep) {
    MUP_CHECK_PARAM(0, 0);

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    char_type ret = _self->GetArgSep();

    RETURN_STRINGL(&ret, 1);
}
/*
PHP_METHOD(parser_base, Error) {
    int argc = ZEND_NUM_ARGS();

    long a_iErrc = 0;
    long a_iPos = 0;
    int a_len = 0;
    char * a_sTok = NULL;
    
    if(zend_parse_parameters(argc TSRMLS_CC, "lls", &a_iErrc, &a_iPos, &a_sTok, &a_len) == FAILURE) {
        WRONG_PARAM_COUNT;
    }

    ParserBase * _self = NULL;
    MUP_GET_OBJ(ParserBase, _self);

    _self->Error((EErrorCodes) a_iErrc, a_iPos, a_sTok);
}
*/


zend_function_entry mu_parser_methods [] = {
    MUP_ME(parser, __construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    MUP_ME(parser, __destruct,  ZEND_ACC_PUBLIC)
    MUP_ME(parser, InitCharSets,  ZEND_ACC_PUBLIC)
    MUP_ME(parser, InitFun, ZEND_ACC_PUBLIC)
    MUP_ME(parser, InitConst, ZEND_ACC_PUBLIC)
    MUP_ME(parser, InitOprt, ZEND_ACC_PUBLIC)
    MUP_ME(parser, OnDetectVar, ZEND_ACC_PUBLIC)
//    MUP_ME(parser, Diff,  ZEND_ACC_PUBLIC)
    MUP_ME(parser, RemoveVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser, ClearVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser, DefineVar,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

zend_function_entry mu_parser_base_methods [] = {
    MUP_ME(parser_base, __construct,  ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    MUP_ME(parser_base, __destruct,  ZEND_ACC_PUBLIC)

    MUP_ME(parser_base, EnableDebugDump, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

    MUP_ME(parser_base, Eval,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, EvalByBulk,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetNumResults,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, SetExpr,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, SetVarFactory,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, SetDecSep,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, SetThousandsSep,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, ResetLocale,  ZEND_ACC_PUBLIC)

    MUP_ME(parser_base, EnableOptimizer,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, EnableBuiltInOprt,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, HasBuiltInOprt,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, AddValIdent,  ZEND_ACC_PUBLIC) // TODO

    MUP_ME(parser_base, DefineFun,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, DefineOprt,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, DefineConst,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, DefineStrConst,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, DefineVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, DefinePostfixOprt,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, DefineInfixOprt,  ZEND_ACC_PUBLIC) // TODO

    MUP_ME(parser_base, ClearVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, ClearFun,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, ClearConst,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, ClearInfixOprt,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, ClearPostfixOprt,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, ClearOprt,  ZEND_ACC_PUBLIC) // TODO

    MUP_ME(parser_base, RemoveVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetUsedVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetVar,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetConst,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetExpr,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetFunDef,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, GetVersion,  ZEND_ACC_PUBLIC)

    MUP_ME(parser_base, GetOprtDef,  ZEND_ACC_PUBLIC) // TODO
    MUP_ME(parser_base, DefineNameChars,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, DefineOprtChars,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, DefineInfixOprtChars,  ZEND_ACC_PUBLIC)

    MUP_ME(parser_base, ValidNameChars,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, ValidOprtChars,  ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, ValidInfixOprtChars,  ZEND_ACC_PUBLIC)

    MUP_ME(parser_base, SetArgSep, ZEND_ACC_PUBLIC)
    MUP_ME(parser_base, GetArgSep, ZEND_ACC_PUBLIC)
    //MUP_ME(parser_base, Error, ZEND_ACC_PUBLIC)
    PHP_FE_END
};


MUPARSER_STARTUP_FUNCTION(parser) {
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "mu\\ParserBase", mu_parser_base_methods);
    mu_parser_base_ce = zend_register_internal_class(&ce TSRMLS_CC);
    //ce.create_object = mup_create_mu_parser_base_object<ParserBase>;

    INIT_CLASS_ENTRY(ce, "mu\\Parser", mu_parser_methods);
    mu_parser_ce = zend_register_internal_class_ex(&ce, mu_parser_base_ce);
    mu_parser_ce->create_object = mup_create_mu_parser_object;
    memcpy(&_mu_parser_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    _mu_parser_handlers.offset = XtOffsetOf(mu_wrapper, obj);
    _mu_parser_handlers.clone_obj = NULL;
    _mu_parser_handlers.free_obj = mup_destroy_mu_parser_object;

    return SUCCESS;
}


MUPARSER_SHUTDOWN_FUNCTION(parser) {
    return SUCCESS;
}

MUPARSER_ACTIVATE_FUNCTION(parser) {
    return SUCCESS;
}

MUPARSER_DEACTIVATE_FUNCTION(parser) {
    // TODO
    return SUCCESS;
}

dnl $Id$
dnl config.m4 for extension muparser

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(muparser, for muparser support,
dnl Make sure that the comment is aligned:
[  --with-muparser             Include muparser support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(muparser, whether to enable muparser support,
dnl Make sure that the comment is aligned:
dnl [  --enable-muparser           Enable muparser support])

if test "$PHP_MUPARSER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-muparser -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/muparser.h"  # you most likely want to change this
  dnl if test -r $PHP_MUPARSER/$SEARCH_FOR; then # path given as parameter
  dnl   MUPARSER_DIR=$PHP_MUPARSER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for muparser files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MUPARSER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MUPARSER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the muparser distribution])
  dnl fi

  dnl # --with-muparser -> add include path
  dnl PHP_ADD_INCLUDE($MUPARSER_DIR/include)

  dnl # --with-muparser -> check for lib and symbol presence
  dnl LIBNAME=muparser # you may want to change this
  dnl LIBSYMBOL=muparser # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MUPARSER_DIR/lib, MUPARSER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MUPARSERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong muparser lib version or lib not found])
  dnl ],[
  dnl   -L$MUPARSER_DIR/lib -lm
  dnl ])
  dnl
  PHP_SUBST(MUPARSER_SHARED_LIBADD)
  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, 1, MUPARSER_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(muparser, /usr/lib/, MUPARSER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(muparser, muparser.cc util.cc mu_def.cc mu_parser.cc mu_exception.cc, $ext_shared)
fi

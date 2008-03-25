dnl AM_PATH_OPENAL
dnl
dnl    Option:      "yes" if the use of the OpenAL library is mandatory.
dnl    Description: check the instalation of the OpenGL library and set the
dnl                 OPENAL_CFLAGS and OPENAL_LIBS variables to use it.
# =========================================================================
# AM_PATH_OPENAL : OpenAL checking macros

AC_DEFUN(AM_PATH_OPENAL,
[ is_mandatory="$1"

AC_REQUIRE_CPP()

dnl Get from the user option the path to the OpenAL files location
AC_ARG_WITH( openal,
    [  --with-openal=<path>   path to the OpenAL install files directory.
                          e.g. /usr/local])

AC_ARG_WITH( openal-include,
    [  --with-openal-include=<path>
                          path to the OpenAL header files directory.
                          e.g. /usr/local/include])

AC_ARG_WITH( openal-lib,
    [  --with-openal-lib=<path>
                          path to the OpenAL library files directory.
                          e.g. /usr/local/lib])

openal_lib="openal"

if test $with_openal
then
    openal_includes="$with_openal/include"
    openal_libraries="$with_openal/lib"
fi

if test "$with_openal_include"
then
    openal_includes="$with_openal_include"
fi

if test "$with_openal_lib"
then
    openal_libraries="$with_openal_lib"
fi


dnl Set OPENAL_CFLAGS
if test "$openal_includes"
then
    OPENAL_CFLAGS="-I$openal_includes"
fi

dnl Set OPENAL_LIBS
if test "$openal_libraries"
then
    OPENAL_LIBS="-L$openal_libraries"
fi
OPENAL_LIBS="$OPENAL_LIBS -l$openal_lib"

_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CXXFLAGS $OPENAL_CFLAGS"

AC_MSG_CHECKING(for OpenAL headers)
AC_EGREP_CPP( yo_openal,
[#include <AL/altypes.h>
#ifdef AL_VERSION
   yo_openal
#endif],
  have_openal_headers="yes",
  have_openal_headers="no" )

if test "$have_openal_headers" = "yes"
then
    if test "$openal_includes"
    then
        AC_MSG_RESULT([$openal_includes])
    else
        AC_MSG_RESULT(yes)
    fi
else
    AC_MSG_RESULT(no)
fi

dnl Test the libraries
AC_MSG_CHECKING(for OpenAL libraries)

CPPFLAGS="$CXXFLAGS $OPENAL_LIBS"

AC_TRY_LINK( , , have_openal_libraries="yes", have_openal_libraries="no")

CPPFLAGS="$_CPPFLAGS"

if test "$have_openal_libraries" = "yes"
then
    if test "$openal_libraries"
    then
        AC_MSG_RESULT([$openal_libraries])
    else
        AC_MSG_RESULT(yes)
    fi
else
    AC_MSG_RESULT(no)
fi

openal_libraries="$openal_libraries"

if test "$have_openal_headers" = "yes" \
   && test "$have_openal_libraries" = "yes"
then
    have_openal="yes"
else
    have_openal="no"
fi

if test "$have_openal" = "no" -a "$is_mandatory" = "yes"
then
    AC_MSG_ERROR([OpenAL is needed to compile NeL (http://www.openal.org).])
fi

AC_SUBST(OPENAL_CFLAGS)
AC_SUBST(OPENAL_LIBS)


])

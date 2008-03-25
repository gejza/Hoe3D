dnl AC_CHECK_LIBPNG([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for PNG library, and define LIBPNG_CFLAGS and LIBPNG_LIBS
dnl
AC_DEFUN(AC_CHECK_LIBPNG,
[
dnl Search for libpng-config script
AC_PATH_PROG(LIBPNG_CONFIG, libpng-config, no)

if test "$LIBPNG_CONFIG" = no ; then
  dnl Search for alternative libpng12-config script (Mandrake)
  AC_PATH_PROG(LIBPNG12_CONFIG, libpng12-config, no)
  LIBPNG_CONFIG="$LIBPNG12_CONFIG"
fi

if test "$LIBPNG_CONFIG" = no ; then
  dnl Try to find PNG headers and extract version info
  dnl FIXME
  AC_MSG_ERROR("FIXME: see config/libpng.m4 for what's missing to properly detect libpng")
  version=none.none.none
else
  version=`$LIBPNG_CONFIG --version`
fi

min_version=ifelse([$1], , 1.0.0., $1)
AC_MSG_CHECKING(for libpng - version >= $min_version)

version_major=`echo $version | cut -d. -f1`
version_minor=`echo $version | cut -d. -f2`
version_micro=`echo $version | cut -d. -f3`
min_version_major=`echo $min_version | cut -d. -f1`
min_version_minor=`echo $min_version | cut -d. -f2`
min_version_micro=`echo $min_version | cut -d. -f3`
if test $version_major -lt $min_version_major ; then
  version_too_old=yes
else
  if test $version_minor -lt $min_version_minor ; then
    version_too_old=yes
  else
    if test $version_micro -lt $min_version_micro ; then
      version_too_old=yes
    else
      version_too_old=no
    fi
  fi
fi
if test $version_too_old = no ; then
  AC_MSG_RESULT(yes)
  ifelse([$2], , :, [$2])
  LIBPNG_CFLAGS=`$LIBPNG_CONFIG --cflags`
  LIBPNG_LIBS=`$LIBPNG_CONFIG --libs`
else
  AC_MSG_RESULT(no)
  ifelse([$3], , :, [$3])
  echo
  echo "*** Your installed version of the PNG library is too old."
  echo "*** You have $version while at least $min_version is required."
  echo
fi

AC_SUBST(LIBPNG_CFLAGS)
AC_SUBST(LIBPNG_LIBS)
])


# AC_NGL_LIBPNG([minimun_version])
#
# Checks for optional PNG library presence and minimal version.
#
# Output :
#   $with_libpng=no|yes

AC_DEFUN(AC_NGL_LIBPNG,
[
min_version=$1
AC_ARG_WITH(libpng,
            [[  --with-libpng           use PNG library (yes, no, [auto])]],,
            [with_libpng="auto"])
if test "$with_libpng" != "no" ; then
  AC_CHECK_LIBPNG($min_version, have_libpng=yes, have_libpng=no)
  case "$with_libpng" in
    yes)
      if test "$have_libpng" == "no"; then
        AC_MSG_ERROR("unmet dependency")
      fi
      ;;
    auto)
      with_libpng="$have_libpng"
      ;;
  esac
fi
if test "$with_libpng" != no ; then
  AC_DEFINE(HAVE_LIBPNG, 1, [Define to 1 if you have the libpng image library])
  NGL_CFLAGS="$NGL_CFLAGS $LIBPNG_CFLAGS"
  NGL_LIBS="$NGL_LIBS $LIBPNG_LIBS"
fi
AM_CONDITIONAL(HAVE_LIBPNG, test "$with_libpng" != no)
])

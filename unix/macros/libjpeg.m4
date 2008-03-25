dnl AC_CHECK_LIBJPEG([[ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for IJG JPEG library, and define LIBJPEG_CFLAGS and LIBJPEG_LIBS
dnl
AC_DEFUN(AC_CHECK_LIBJPEG,
[
AC_CHECK_LIB(jpeg, jpeg_start_decompress, LIBJPEG_LIBS="-ljpeg", LIBJPEG_LIBS="", )
if test "$LIBJPEG_LIBS" = "-ljpeg" ; then
  ifelse([$1], , :, [$1])
  LIBJPEG_CFLAGS=""
else
  ifelse([$2], , :, [$2])
  echo
  echo "*** Could not build and link a test sample using the JPEG library."
  echo
fi

AC_SUBST(LIBJPEG_CFLAGS)
AC_SUBST(LIBJPEG_LIBS)
])


# AC_NGL_LIBJPEG()
#
# Checks for optional JPEG library presence and minimal version.
#
# Output :
#   $with_libjpeg=no|yes

AC_DEFUN(AC_NGL_LIBJPEG,
[
AC_ARG_WITH(libjpeg,
            [[  --with-libjpeg          use IJG JPEG library (yes, no, [auto])]],,
            [with_libjpeg="auto"])
if test "$with_libjpeg" != "no" ; then
  AC_CHECK_LIBJPEG(have_libjpeg=yes, have_libjpeg=no)
  case "$with_libjpeg" in
    yes)
      if test "$have_libjpeg" == "no"; then
        AC_MSG_ERROR("unmet dependency")
      fi
      ;;
    auto)
      with_libjpeg="$have_libpng"
      ;;
  esac
fi
if test "$with_libjpeg" != no ; then
  AC_DEFINE(HAVE_LIBJPEG, 1, [Define to 1 if you have the JPEG image library])
  NGL_CFLAGS="$NGL_CFLAGS $LIBJPEG_CFLAGS"
  NGL_LIBS="$NGL_LIBS $LIBJPEG_LIBS"
fi
AM_CONDITIONAL(HAVE_LIBJPEG, test "$with_libjpeg" != no)
])

dnl X11 detection and build flags setup
dnl

AC_DEFUN(AC_NGL_X11,
[
AC_PATH_X
if test "$no_x" != yes ; then
  CPPFLAGS="$CPPFLAGS -I$x_includes"
#  CXXFLAGS="$CXXFLAGS -I$x_includes"
  LIBS="$LIBS -L$x_libraries -lX11 -lXext"
fi
])


# AC_NGL_XF86VIDMODE
#
# Checks for optional XF86VidMode extension support

AC_DEFUN(AC_NGL_XF86VIDMODE,
[
AC_CHECK_HEADER(X11/extensions/xf86vmode.h, have_xf86vidmode_include=yes,, [#include <X11/Xlib.h>])
if test "$have_xf86vidmode_include" = "yes" ; then
  AC_CHECK_LIB(Xxf86vm, XF86VidModeSwitchToMode, have_xf86vidmode_lib=yes)
  if test "$have_xf86vidmode_lib" = "yes" ; then
    AC_DEFINE(HAVE_XF86VIDMODE, 1, [Define to 1 if you want support for the XFree86 VidMode extension])
    NGL_LIBS="$NGL_LIBS -lXxf86vm"
    x_ext="$x_ext XF86VidMode"
  fi
fi
])


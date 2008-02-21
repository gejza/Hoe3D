dnl Check for Hoe Libraries
dnl CHECK_HOE(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  HOE_CFLAGS
dnl  HOE_LIBS
AC_DEFUN([CHECK_HOE],
[dnl

AC_ARG_WITH(
    hoe,
    [AC_HELP_STRING([--with-hoe=PATH],[Path to the Hoe Engine prefix])],
    hoe_path="$withval",
    :)

dnl # Determine hoe lib directory
if test -z $hoe_path; then
    test_paths="$HOME/Hoe $HOME /usr /usr/local"
else
    test_paths="${hoe_path}"
fi

for x in $test_paths ; do
    AC_MSG_CHECKING([for hoe3d.h in ${x}/Hoe3D/include/Hoe3D or ${x}/include/Hoe3D])
    if test -f ${x}/Hoe3D/include/Hoe3D/hoe3d.h; then
        AC_MSG_RESULT([yes])
        save_CFLAGS=$CFLAGS
        save_LDFLAGS=$LDFLAGS
        CFLAGS="$CFLAGS"
        LDFLAGS="-L$x/lib $LDFLAGS"
        HOE_LIBS="-L$x/bin -lHoeGame -lHoeCore"
        HOE_CFLAGS="-I$x/HoeGame/include/HoeGame -I$x/Hoe3D/include/Hoe3D -I$x/HoeCore/include/HoeCore/"
        CFLAGS=$save_CFLAGS
        LDFLAGS=$save_LDFLAGS
        break
	elif test -f ${x}/include/Hoe3D/hoe3d.h; then
        AC_MSG_RESULT([yes])
        save_CFLAGS=$CFLAGS
        save_LDFLAGS=$LDFLAGS
        CFLAGS="$CFLAGS"
        LDFLAGS="-L$x/lib $LDFLAGS"
        HOE_CFLAGS="-I$x/include/HoeGame -I$x/include/Hoe3D -I$x/include/HoeCore"
        CFLAGS=$save_CFLAGS
        LDFLAGS=$save_LDFLAGS
        break
		
    else
        AC_MSG_RESULT([no])
    fi
done

AC_SUBST(HOE_LIBS)
AC_SUBST(HOE_CFLAGS)

if test -z "${HOE_LIBS}"; then
  AC_MSG_NOTICE([*** HoeEngine not found.])
  ifelse([$2], , AC_MSG_ERROR([HoeEngine is required]), $2)
else
  AC_MSG_NOTICE([using '${HOE_CFLAGS}' for HoeEngine])
  ifelse([$1], , , $1) 
fi 
])

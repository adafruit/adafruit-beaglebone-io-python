AC_DEFUN([CHECK_KERNEL_RELEASE],
[
  AC_MSG_CHECKING([if kernel release >= 4.1])

  kernel_ge_41=$(python -c 'import platform; print("yes" if platform.release() >= "4.1.0" else "no")')

  if test "x$kernel_ge_41" = "xyes"; then
    CPPFLAGS="$CPPFLAGS -DBBBVERSION41"
    AC_SUBST(CPPFLAGS)
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
  fi
])

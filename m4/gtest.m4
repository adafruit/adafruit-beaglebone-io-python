AC_DEFUN([CHECK_GTEST],
[
  AC_MSG_CHECKING([for google test])

  AC_LANG_PUSH([C++])
  AC_CHECK_HEADER([gtest/gtest.h],[gtest_found=yes])
  AC_LANG_POP

  #if test "x$gtest_found" = "xyes"; then
  #  GTEST_CPPFLAGS=
  #  AC_SUBST(GTEST_CPPFLAGS)
  #fi

  AC_SUBST(HAVE_GTEST)
  AM_CONDITIONAL([HAVE_GTEST], [test "x$gtest_found" = "xyes"])
])

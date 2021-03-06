AC_INIT([utk], [0.1], [homer242@gmail.com])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

sinclude(m4/ax_cflags_gcc_option.m4)

# workaround autoconf setting default flags which we don't want
test ".$CFLAGS" = "." && CFLAGS=" "

# programs needed to build/install
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# checks for header files.
AC_HEADER_STDC

# config options
AC_ARG_ENABLE(debug,
        [  --enable-debug  compile utk with debug flag (-g, ...)])

# pimp CFLAGS
if test "x$GCC" = "xyes"; then
   # gcc specific options
   CFLAGS="$CFLAGS -Wall -Wextra"
   AX_CFLAGS_GCC_OPTION(-Wfloat-equal)
   AX_CFLAGS_GCC_OPTION(-Wundef)
   AX_CFLAGS_GCC_OPTION(-Wshadow)
   AX_CFLAGS_GCC_OPTION(-Wpointer-arith)
#   AX_CFLAGS_GCC_OPTION(-Wcast-align)
   AX_CFLAGS_GCC_OPTION(-Wstrict-prototypes)
   AX_CFLAGS_GCC_OPTION(-Wstrict-overflow=5)
   AX_CFLAGS_GCC_OPTION(-Wwrite-strings)
   AX_CFLAGS_GCC_OPTION(-Waggregate-return)
#   AX_CFLAGS_GCC_OPTION(-Wcast-qual)
   AX_CFLAGS_GCC_OPTION(-Wswitch-default)
#   AX_CFLAGS_GCC_OPTION(-Wswitch-enum)
   AX_CFLAGS_GCC_OPTION(-Wconversion)
   AX_CFLAGS_GCC_OPTION(-Wunreachable-code)
   AX_CFLAGS_GCC_OPTION(-Wformat=2)
fi

if test "x$enable_debug" = "xyes"; then
   CFLAGS="$CFLAGS -g -DDEBUG"
   AC_MSG_RESULT( => enable debug (-g, -DDEBUG))
fi

# the generated files
AC_CONFIG_FILES([
utk.pc
Makefile
src/Makefile
tests/Makefile
])

AC_OUTPUT

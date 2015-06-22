AC_PREREQ([2.68])
AC_INIT([CppFasta], [0.0.1], [okamura@informationsea.info])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_SRCDIR([src/cppfasta.cpp])
AC_CONFIG_HEADERS([src/config.h])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

# Checks for programs.
AM_PROG_AR
AC_CONFIG_MACRO_DIR([m4])
ACLOCAL_AMFLAGS="-I m4"
AM_SILENT_RULES([yes])
AM_PROG_LIBTOOL([static])

AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lbz2':
AC_CHECK_LIB([bz2], [BZ2_bzread])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lz':
AC_CHECK_LIB([z], [gzgets])


# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h limits.h netdb.h stddef.h stdlib.h string.h strings.h sys/time.h sys/timeb.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([bzero dup2 fchdir getcwd getpagesize gettimeofday memset mkdir munmap regcomp rmdir socket strcasecmp strchr strdup strerror strrchr strstr strtol strtoull])

AC_CONFIG_FILES([src/Makefile Makefile test/Makefile test/data/Makefile test/gtest/Makefile])
AC_OUTPUT

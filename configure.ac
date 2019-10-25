##
## Package name, version, bug-report, package tarname, URL
##
AC_INIT([lunar-calendar], [0.1.0], [kychou2@illinois.edu], [lunar-calendar],
        [https://github.com/kyechou/lunar-calendar])


##
## Check distribution
##
AS_IF([test -f /etc/os-release], [. /etc/os-release; OS=$(echo $NAME | cut -f 1 -d ' ')],
      [type lsb_release >/dev/null 2>&1], [OS=$(lsb_release -si)],
      [test -f /etc/lsb-release], [. /etc/lsb-release; OS=$DISTRIB_ID],
      [test -f /etc/arch-release], [OS="Arch"],
      [test -f /etc/debian_version], [OS="Debian"],
      [test -f /etc/SuSe-release], [OS="openSUSE"],
      [test -f /etc/fedora-release], [OS="Fedora"],
      [test -f /etc/redhat-release], [OS="CentOS"],
      [type uname >/dev/null 2>&1], [OS="$(uname -s)"],
      [AC_ERROR([unable to determine the distribution])])


##
## Determine license install path
##
AS_IF([test "$OS" = "Debian" -o "$OS" = "Ubuntu"],
      [AC_SUBST([licensedir], [\${docdir}])],
      [AC_SUBST([licensedir], [\${datarootdir}/licenses/\${PACKAGE_TARNAME}])])


##
## Config files and directories
##
AC_CONFIG_FILES([Makefile])
#AC_CONFIG_FILES([codecov.sh:tests/codecov.sh.in], [chmod +x codecov.sh])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])


##
## Preset default flags
##
: ${CFLAGS="-O3"}
: ${LDFLAGS=""}
: ${LIBS=""}
: ${TESTCFLAGS="-O3"}
: ${TESTLDFLAGS=""}
: ${TESTLIBS=""}


##
## Language environments
##
AC_LANG(C)
AC_C_CONST
AC_C_INLINE


##
## Programs
##
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_LN_S


###
### Configure arguments
###
AC_ARG_ENABLE([coverage],
        AS_HELP_STRING([--enable-coverage], [Enable coverage testing using gcov]))
AC_ARG_ENABLE([asan],
        AS_HELP_STRING([--enable-asan], [Enable AddressSanitizer, LeakSanitizer,
                        and UndefinedBehaviorSanitizer]))
AC_ARG_ENABLE([tsan],
        AS_HELP_STRING([--enable-tsan], [Enable ThreadSanitizer]))
AS_IF([test "$enable_asan" = "yes" -a "$enable_tsan" = "yes"], [
       AC_ERROR([ASan and TSan cannot be used at the same time])
])


###
### Predefined functions
###
define([ADD_CFLAG], [
        AS_IF([test -z "$CFLAGS"], [CFLAGS="$1"],
              [AS_IF([test -n "${CFLAGS##*$1*}"], [CFLAGS="$CFLAGS $1"])])
        CPPFLAGS="$CFLAGS"])
define([ADD_LDFLAG], [
        AS_IF([test -z "$LDFLAGS"], [LDFLAGS="$1"],
              [AS_IF([test -n "${LDFLAGS##*$1*}"], [LDFLAGS="$LDFLAGS $1"])])])
define([ADD_LIB], [
        AS_IF([test -z "$LIBS"], [LIBS="$1"],
              [AS_IF([test -n "${LIBS##*$1*}"], [LIBS="$LIBS $1"])])])
define([ADD_TESTCFLAG], [
        AS_IF([test -z "$TESTCFLAGS"], [TESTCFLAGS="$1"],
              [AS_IF([test -n "${TESTCFLAGS##*$1*}"], [TESTCFLAGS="$TESTCFLAGS $1"])])])
define([ADD_TESTLDFLAG], [
        AS_IF([test -z "$TESTLDFLAGS"], [TESTLDFLAGS="$1"],
              [AS_IF([test -n "${TESTLDFLAGS##*$1*}"], [TESTLDFLAGS="$TESTLDFLAGS $1"])])])
define([ADD_TESTLIB], [
        AS_IF([test -z "$TESTLIBS"], [TESTLIBS="$1"],
              [AS_IF([test -n "${TESTLIBS##*$1*}"], [TESTLIBS="$TESTLIBS $1"])])])
define([SAFE_ADD_CFLAG_AND_TESTCFLAG],[
        AC_MSG_CHECKING([if compiler flag $1 works])
        save_CFLAGS="$CFLAGS"
        ADD_CFLAG($1)
        pushdef(prog, [int main(void) { return 0; }])
        flag_test=0
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [flag_test=1],[flag_test=0])
        popdef([prog])
        if test $flag_test = 1
        then
                AC_MSG_RESULT(yes)
                ADD_TESTCFLAG($1)
        else
                AC_MSG_RESULT(no)
                CFLAGS="$save_CFLAGS"
        fi
])
define([SAFE_ADD_LDFLAG_AND_TESTLDFLAG],[
        AC_MSG_CHECKING([if linker flag $1 works])
        save_LDFLAGS="$LDFLAGS"
        ADD_LDFLAG($1)
        pushdef(prog, [int main(void) { return 0; }])
        flag_test=0
        AC_LINK_IFELSE([AC_LANG_SOURCE([prog])], [flag_test=1],[flag_test=0])
        popdef([prog])
        if test $flag_test = 1
        then
                AC_MSG_RESULT(yes)
                ADD_TESTLDFLAG($1)
        else
                AC_MSG_RESULT(no)
                LDFLAGS="$save_LDFLAGS"
        fi
])
define([CHECK_COMPILER_FLAGS], [
        AC_MSG_CHECKING([if compiler flags "$1" work])
        save_CFLAGS="$CFLAGS"
        CFLAGS="$1"
        pushdef(prog, [int main(void) { return 0; }])
        flag_test=0
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [flag_test=1],[flag_test=0])
        popdef([prog])
        CFLAGS="$save_CFLAGS"
        if test $flag_test = 1
        then
                AC_MSG_RESULT(yes)
        else
                AC_MSG_RESULT(no)
                AC_ERROR([failed to compile with flags "$1"])
        fi
])
define([CHECK_LINKER_FLAGS], [
        AC_MSG_CHECKING([if linker flags "$1" work])
        save_LDFLAGS="$LDFLAGS"
        LDFLAGS="$1"
        pushdef(prog, [int main(void) { return 0; }])
        flag_test=0
        AC_LINK_IFELSE([AC_LANG_SOURCE([prog])], [flag_test=1],[flag_test=0])
        popdef([prog])
        LDFLAGS="$save_LDFLAGS"
        if test $flag_test = 1
        then
                AC_MSG_RESULT(yes)
        else
                AC_MSG_RESULT(no)
                AC_ERROR([failed to link with flags "$1"])
        fi
])
define([CHECK_LIB], [
        save_LIBS="$LIBS"
        AC_CHECK_LIB($1, $2, $3, $4)
        LIBS="$save_LIBS"
])


##
## Compiler and linker flags
##
AC_LANG_WERROR
SAFE_ADD_CFLAG_AND_TESTCFLAG([-Wall])
SAFE_ADD_CFLAG_AND_TESTCFLAG([-Wextra])
SAFE_ADD_CFLAG_AND_TESTCFLAG([-Werror])
SAFE_ADD_CFLAG_AND_TESTCFLAG([-std=c11])
ADD_CFLAG([-I${srcdir}/src -D_POSIX_C_SOURCE=200809L -fPIC])
ADD_LDFLAG([-Wl,-s])
#ADD_TESTCFLAG([-I${srcdir}/src])
#ADD_TESTLDFLAG([-L.])
AS_IF([test "$enable_coverage" = "yes"], [
        CFLAGS="$(echo $CFLAGS | sed 's/-O[[0-9]]/-O0/g')"
        TESTCFLAGS="$(echo $TESTCFLAGS | sed 's/-O[[0-9]]/-O0/g')"
        ADD_CFLAG(-g)
        ADD_CFLAG(--coverage)
        ADD_TESTCFLAG(-g)
        ADD_TESTCFLAG(--coverage)
        ADD_LIB(--coverage)
        ADD_TESTLIB(--coverage)
        AS_IF([test -z "$GCOV"], [      # if GCOV is not defined or null
               AS_IF([test -z "${CC##*gcc*}"], [GCOV="$(which gcov)"],
                     [test -z "${CC##*clang*}"], [GCOV="$(which llvm-cov) gcov"],
                     [AC_ERROR([unsupported compiler: $CC])])],
              [test -n "${GCOV##/*}"], [        # else if GCOV is not an abs path
               GCOV_PATH="$(which $(echo $GCOV | cut -f 1 -d ' ') 2>/dev/null)"
               AS_IF([test -z "$GCOV_PATH"],
                     [AC_ERROR(["$(echo $GCOV | cut -f 1 -d ' ')" file not found])])
               GCOV="$(echo $GCOV | sed "s|[[^ ]]\+|$GCOV_PATH|")"])
        AC_MSG_CHECKING([for $(echo $GCOV | cut -f 1 -d ' ')])
        AS_IF([test -x "$(echo $GCOV | cut -f 1 -d ' ')"], [AC_MSG_RESULT(yes)], [
               AC_MSG_RESULT(no)
               AC_ERROR([$(echo $GCOV | cut -f 1 -d ' ') cannot be executed])])
        CC_VER="$($CC --version | grep -oE '[[0-9]]\.[[0-9]]\.[[0-9]]' | head -n1)"
        GCOV_VER="$($GCOV --version | grep -oE '[[0-9]]\.[[0-9]]\.[[0-9]]' | head -n1)"
        AS_IF([test "$CC_VER" != "$GCOV_VER"],
              [AC_ERROR([$GCOV $GCOV_VER and $CC $CC_VER version mismatch])])
        AC_SUBST([GCOV], [$GCOV])
])
AS_IF([test "$enable_asan" = "yes"], [
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fsanitize=address)
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fsanitize=pointer-compare)
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fsanitize=pointer-subtract)
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fsanitize=leak)
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fno-omit-frame-pointer)
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fno-common)
        SAFE_ADD_LDFLAG_AND_TESTLDFLAG(-fsanitize=address)
        SAFE_ADD_LDFLAG_AND_TESTLDFLAG(-fsanitize=pointer-compare)
        SAFE_ADD_LDFLAG_AND_TESTLDFLAG(-fsanitize=pointer-subtract)
        SAFE_ADD_LDFLAG_AND_TESTLDFLAG(-fsanitize=leak)
])
AS_IF([test "$enable_tsan" = "yes"], [
        SAFE_ADD_CFLAG_AND_TESTCFLAG(-fsanitize=thread)
        SAFE_ADD_LDFLAG_AND_TESTLDFLAG(-fsanitize=thread)
])


##
## Check flags usability
##
CHECK_COMPILER_FLAGS([$CFLAGS])
CHECK_LINKER_FLAGS([$LDFLAGS])
CHECK_COMPILER_FLAGS([$TESTCFLAGS])
CHECK_LINKER_FLAGS([$TESTLDFLAGS])


##
## Libraries
##
AC_CHECK_HEADERS(pthread.h, [], [AC_ERROR([pthread is not available])])
CHECK_LIB(pthread, pthread_create, [], [AC_ERROR([pthread does not work])])
ADD_CFLAG(-pthread)
ADD_LIB(-pthread)
AC_CHECK_HEADERS(math.h, [], [AC_ERROR([math is not available])])
ADD_LIB(-lm)
AC_CHECK_HEADERS(uuid/uuid.h, [], [AC_ERROR([uuid is not available])])
ADD_LIB(-luuid)


echo >& AS_MESSAGE_FD
AC_OUTPUT
# vim: set ts=8 sw=8 et:

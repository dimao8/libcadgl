#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libcadgl], [0.1.0.0], [dimao8@yandex.ru])
AC_CONFIG_SRCDIR([src/cadgl.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.10 foreign subdir-objects silent-rules])
AC_LANG([C++])
AC_CONFIG_MACRO_DIRS([m4])

LT_INIT([dlopen win32-dll])
LT_LANG([C++])

# Checks for programs.
CXXFLAGS="-std=gnu++11"
AC_PROG_CXX([clang++ llvw-g++ g++])

# Check for pdflatex
AC_ARG_ENABLE([build-doc],
              [AS_HELP_STRING([--enable-build-doc], [Set for documentation building])],
              [AS_IF([test "x$enableval" = "xyes"],
                     [BUILD_DOC=yes],
                     [BUILD_DOC=no])],
              [BUILD_DOC=yes])
AC_PATH_PROG([PDFLATEX], [pdflatex], [BUILD_DOC=no])
AM_CONDITIONAL([BUILD_DOC], [test "x$BUILD_DOC" = "xyes"])

AC_CONFIG_FILES([Makefile
                 doc/Doxyfile])
AC_OUTPUT
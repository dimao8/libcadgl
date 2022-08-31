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

CXXFLAGS=
LIBS=

# Check for glm
AX_CHECK_GLM

# Check for OpenGL
AX_CHECK_OPENGL

# Check for X11s
ax_have_x=no
AC_PATH_X([X11], [X11/Xlib.h], [XrmInitialize()])
AC_PATH_XTRA([X11], [X11/Xlib.h], [XrmInitialize()])
AS_IF([test "x$no_x" == "xyes"],
	[
		AX_CHECK_GDI
	],
	[
		CXXFLAGS+="$X_CFLAGS"
		LIBS+="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"
	])

AC_CONFIG_FILES([Makefile
	doc/Doxyfile])
AC_OUTPUT
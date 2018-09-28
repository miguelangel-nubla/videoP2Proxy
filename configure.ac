dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT([videop2proxy], [0.1], [miguelangel.nubla@gmail.com], , [https://github.com/miguelangel-nubla/videoP2Proxy])
AC_CANONICAL_TARGET

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11 foreign])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX

ROOT_DIR=`cd $srcdir && pwd`

PKG_CHECK_MODULES([JSON_C],[json-c])

AC_ARG_WITH(p2plibpath,
	AS_HELP_STRING([--with-p2plibpath=DIR],
		[compile using specified library path. Search applicable library for your target in ./lib]))
AS_IF([test "${with_p2plibpath}" != ""], [
	P2P_LIBS="${ROOT_DIR}/${with_p2plibpath}"
], [
	AS_CASE([$target],
	  [x86_64-pc-linux-gnu], [P2P_LIBS="${ROOT_DIR}/lib/Linux/x64"],
	  [armv6l-unknown-linux-gnueabihf], [P2P_LIBS="${ROOT_DIR}/lib/Linux/Arm_BCM2836_4.8.3"],
	  [armv7l-unknown-linux-gnueabihf], [P2P_LIBS="${ROOT_DIR}/lib/Linux/Arm_BCM2836_4.8.3"],
	  [AC_MSG_ERROR([unknown library path for target: $target. Define library path manually with --with-p2plibpath=./lib/<path>])])
])
P2P_LIBS="$(readlink -f ${P2P_LIBS})"

AC_ARG_ENABLE([live555],
	AS_HELP_STRING([--enable-live555],
		[enable RTSP server through live555 (default enabled)]))
AS_IF([test "${enable_live555}" != "no"], [
	PKG_CHECK_MODULES([LIVE555],[live555])
	AC_DEFINE([ENABLE_RTSP], [], [Enable rtsp server])
])

AC_SUBST([P2P_LIBS], [$P2P_LIBS])

AC_OUTPUT([
Makefile
])

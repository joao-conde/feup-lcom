# Makefile for lab6

COMPILER_TYPE= gnu

CC=gcc

PROG= lab6

SRCS= lab6.c rtc.c

CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

MAN=
BINDIR?= /usr/sbin
.include <bsd.prog.mk>
.include <bsd.gcc.mk>

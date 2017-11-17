# Makefile for lab5

COMPILER_TYPE= gnu

CC=gcc

PROG= lab5

SRCS= lab5.c vbe.c video_gr.c test5.c read_xpm.c

CFLAGS= -Wall
CPPFLAGS += -D LAB5

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys
LDFLAGS+= -L .

MAN=
BINDIR?= /usr/sbin
.include <bsd.prog.mk>
.include <bsd.gcc.mk>

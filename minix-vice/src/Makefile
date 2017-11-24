#Makefile for lcom project - Minix Vice

COMPILER_TYPE = gnu

CC = gcc

PROG = minix-vice
SRCS = main.c utils.c video_gr.c vbe.c read_xpm.c

CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -ldriver -llm -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
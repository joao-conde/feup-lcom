#Makefile for lcom project - Minix Vice

COMPILER_TYPE = gnu

CC = gcc

PROG = minix-vice
SRCS = bitmap.c entities.c kbd.c main.c mouse.c MinixVice.c read_xpm.c timer.c utils.c video_gr.c vbe.c 

CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -ldriver -llm -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
#Makefile for lcom project - Minix Vice

COMPILER_TYPE = gnu

CC = gcc

PROG = minix-vice
SRCS += bitmap.c ColliderBox.c entities.c game.c graphics.c IO.c kbd.c logic.c main.c mouse.c MinixVice.c rtc.c state_machines.c
SRCS += timer.c utils.c video_gr.c vbe.c 

CCFLAGS= -Wall -O3

DPADD += ${LIBDRIVER} ${LIBSYS}
LDADD += -ldriver -llm -lsys

LDFLAGS += -L.

MAN=
BINDIR? = /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
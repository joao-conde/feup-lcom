#include "i8042.h"

.globl _assIH

.text

_assIH:
	xor %eax,%eax //setting eax to 0

	KBD6:
	inb $STAT_REG
	test KBD_IRQ, %al
	loopz KBD6

	inb $OUT_BUF
	ret

#include "i8042.h"

.globl _assIH

.text

_assIH:
	xor %eax,%eax //setting eax to 0

	ciclo:
	inb $STAT_REG
	test $OUTBUF_FULL_ASS, %al
	loopz ciclo

	inb $OUT_BUF
	ret

#include "i8042.h"

.global _assIH
.global _scancode

.text

_assIH:
	xor %eax,%eax //setting eax to 0

	ciclo:
	inb $STAT_REG
	test $OUTBUF_FULL_ASS, %al
	loopz ciclo

	inb $OUT_BUF

	mov %eax, _scancode

	ret

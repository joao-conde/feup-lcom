#ifndef MINIXVICE_H
#define MINIXVICE_H


typedef struct{

	int irq_set_kbd;

	int done, draw;

	unsigned long scancode;


} MinixVice;


MinixVice* initMinixVice();

void updateMinixVice(MinixVice* game);

void drawMinixVice(MinixVice* game);

void endMinixVice(MinixVice* game);

#endif /* MINIXVICE_H */

#ifndef MINIXVICE_H
#define MINIXVICE_H


typedef struct{

	int irq_set_kbd;

	int done, draw;

	unsigned long scancode;


} MinixVice;


MinixVice* init();

void update(MinixVice* game);

void draw(MinixVice* game);

void end(MinixVice* game);

#endif /* MINIXVICE_H */

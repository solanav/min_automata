#ifndef SBC_H
#define SBC_H

typedef struct _sbc
{
	int **data;
	int *num_states;
	int total_classes;
	int total_states;
} sbc;

void print_sbc(AFND *afd, sbc *states);

int add_class_sbc(sbc *states);
void remove_class_sbc(sbc *states, int class_i);

int get_class_sbc(sbc *states, int state);

void add_state_sbc(sbc *states, int class_i, int state);
void remove_state_sbc(sbc *states, int class_i, int state_i);

void free_sbc(sbc *states);
sbc *create_sbc();

#endif
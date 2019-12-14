#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/minimiza.h"
#include "../include/types.h"
#include "../include/sbc.h"

void divide_final(AFND *afd, sbc *states)
{
	int total_states = AFNDNumEstados(afd);

	// Add new class for final states
	int starting_class = 0;
	int final_class = add_class_sbc(states);

	// Add states to their class
	for (int i = 0; i < total_states; i++)
	{
		if (AFNDTipoEstadoEn(afd, i) == FINAL)
			add_state_sbc(states, final_class, i);
		else
			add_state_sbc(states, starting_class, i);
	}
}

void destroy_world(AFND *afd, sbc *states)
{
	// For each state create a new class
}

AFND *AFNDMinimiza(AFND *afd)
{
	sbc *states;

	states = create_sbc(afd);

	divide_final(afd, states);
	print_sbc(afd, states);

	free_sbc(states);

	return NULL;
}
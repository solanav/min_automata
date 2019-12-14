#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/afnd_plus.h"
#include "../include/minimiza.h"
#include "../include/types.h"
#include "../include/sbc.h"

int check_class(AFND *afd, sbc *states, int s0, int s1)
{
	int num_sybols = AFNDNumSimbolos(afd);

	// For each symbol
	for (int i = 0; i < num_sybols; i++)
	{
		int *t0;
		get_transitions(afd, &t0, s0, i);

		int *t1;
		get_transitions(afd, &t1, s1, i);

#ifdef DEBUG
		printf("\t%s > (%s:%d) [%d] - %s > (%s:%d) [%d]\n",
			   AFNDNombreEstadoEn(afd, s0),
			   AFNDNombreEstadoEn(afd, t0[0]),
			   t0[0],
			   get_class_sbc(states, t0[0]),
			   AFNDNombreEstadoEn(afd, s1),
			   AFNDNombreEstadoEn(afd, t1[0]),
			   t1[0],
			   get_class_sbc(states, t1[0]));
#endif

		if (get_class_sbc(states, t0[0]) != get_class_sbc(states, t1[0]))
		{
			free(t0);
			free(t1);
			return 0;
		}

		free(t0);
		free(t1);
	}

	return 1;
}

void divide_class(AFND *afd, sbc *states, int class_i)
{
	int *done_states = calloc(states->num_states[class_i], sizeof(int));
	int dsi = 0;

	// For each state in the class
	for (int i = 0; i < states->num_states[class_i]; i++)
	{
		int state_0 = states->data[class_i][i];

		// Add state to done list
		done_states[dsi] = state_0;
		dsi++;

		// We check with every other state
		for (int j = 0; j < states->num_states[class_i]; j++)
		{
			int state_1 = states->data[class_i][j];

			printf("\n>> [%s:%s]",
				AFNDNombreEstadoEn(afd, state_0),
				AFNDNombreEstadoEn(afd, state_1)
			);
			
			// If state is in done list continue
			int not_done = 0;
			
			for (int k = 0; k < dsi; k++)
			{
				if (done_states[k] == state_1)
				{
					not_done = 1;
					break;
				}
			}
			if (not_done == 1)
				continue;

			// Check if both states are of the same class
			if (check_class(afd, states, state_0, state_1) == 1)
			{
				done_states[dsi] = state_1;
				dsi++;

				printf(" <--");
			}
		}
	}

	printf("\n");
}

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

AFND *AFNDMinimiza(AFND *afd)
{
	sbc *states;

	states = create_sbc(afd);

	divide_final(afd, states);
	print_sbc(afd, states);

	divide_class(afd, states, 0);
	print_sbc(afd, states);

	free_sbc(states);

	return NULL;
}
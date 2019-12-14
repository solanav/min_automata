#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/afnd_plus.h"
#include "../include/minimiza.h"
#include "../include/types.h"
#include "../include/sbc.h"

int get_transition_class(AFND *afd, sbc *states, int state, int symbol_i)
{
	int *transition_list;
	get_transitions(afd, &transition_list, state, symbol_i);

	int transition = get_class_sbc(states, transition_list[0]);

	free(transition_list);

	return transition;
}

int check_class(AFND *afd, sbc *states, int s0, int s1)
{
	int num_sybols = AFNDNumSimbolos(afd);

	// For each symbol
	for (int i = 0; i < num_sybols; i++)
	{
		int class_s0 = get_transition_class(afd, states, s0, i);
		int class_s1 = get_transition_class(afd, states, s1, i);

		if (class_s0 != class_s1)
			return 0;
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

		// If state is in done list continue
		int not_done = 0;
		for (int k = 0; k < dsi; k++)
		{
			if (done_states[k] == state_0)
			{
				not_done = 1;
				break;
			}
		}
		if (not_done == 1)
			continue;

		// Add state to done list
		done_states[dsi] = state_0;
		dsi++;

		// Add new class for this state
		int state_0_class = add_class_sbc(states);
		add_state_sbc(states, state_0_class, state_0);

		// We check with every other state
		for (int j = 0; j < states->num_states[class_i]; j++)
		{
			int state_1 = states->data[class_i][j];

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

				add_state_sbc(states, state_0_class, state_1);
			}
		}
	}

	free(done_states);
	remove_class_sbc(states, class_i);
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

	// Divide the final state from the rest
	divide_final(afd, states);
	print_sbc(afd, states);

	// Keep dividing until nothing changes
	int old_nc;
	do
	{
		old_nc = states->total_classes;
		for (int i = 0; i < states->total_classes; i++)
		{
			divide_class(afd, states, i);
			print_sbc(afd, states);
		}
	} while (old_nc != states->total_classes);

	// Count number of states
	int total_states = 0;
	for (int i = 0; i < states->total_classes; i++)
		total_states += states->num_states[i];

	int total_symbols = AFNDNumSimbolos(afd);

	AFND *afd_min = AFNDNuevo("afd_min", states->total_classes, total_symbols);

	// Insert the symbols
	for (int i = 0; i < total_symbols; i++)
		AFNDInsertaSimbolo(afd_min, AFNDSimboloEn(afd, i));

	// Insert one state for each class
	int initial = NO_INITIAL;
	for (int i = 0; i < states->total_classes; i++)
	{
		int type = gen_type(afd, states->data[i], states->num_states[i], &initial);

		AFNDInsertaEstado(afd_min, AFNDNombreEstadoEn(afd, states->data[i][0]), type);
	}

	for (int i = 0; i < states->total_classes; i++)
	{
		for (int j = 0; j < total_symbols; j++)
		{
			char *symbol = AFNDSimboloEn(afd_min, j);

			int s0 = states->data[i][0];
			int s1 = states->data[get_transition_class(afd, states, s0, j)][0];

			AFNDInsertaTransicion(afd_min,
				AFNDNombreEstadoEn(afd, s0),
				symbol,
				AFNDNombreEstadoEn(afd, s1)
			);
		}
	}

	// Free sbc
	free_sbc(states);

	return afd_min;
}
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
	// Keep a list of states we already checked
	int *done_states = calloc(states->num_states[class_i], sizeof(int));
	int done_index = 0;

	// For each state in the class
	for (int i = 0; i < states->num_states[class_i]; i++)
	{
		// Get state_0
		int state_0 = states->data[class_i][i];

		// If state_0 is in done list, skip it
		int not_done = 0;
		for (int k = 0; k < done_index; k++)
		{
			if (done_states[k] == state_0)
			{
				not_done = 1;
				break;
			}
		}
		if (not_done == 1)
			continue;

		// Add state_0 to done list
		done_states[done_index] = state_0;
		done_index++;

		// Add new class for state_0
		int state_0_class = add_class_sbc(states);
		add_state_sbc(states, state_0_class, state_0);

		// For every other state
		for (int j = 0; j < states->num_states[class_i]; j++)
		{
			// Get state_1
			int state_1 = states->data[class_i][j];

			// If state_1 is in done list, skip it
			int not_done = 0;
			for (int k = 0; k < done_index; k++)
			{
				if (done_states[k] == state_1)
				{
					not_done = 1;
					break;
				}
			}
			if (not_done == 1)
				continue;

			// Check if both states are equivalent
			if (check_class(afd, states, state_0, state_1) == 1)
			{
				// Add state_1 to done_states
				done_states[done_index] = state_1;
				done_index++;

				// Add state_1 to state_0's class
				add_state_sbc(states, state_0_class, state_1);
			}
		}
	}

	// We remove the class we were checking, because we copied all the states to new classes
	remove_class_sbc(states, class_i);

	free(done_states);
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

	// Divide final states from non-final states
	divide_final(afd, states);
	print_sbc(afd, states);

	// Keep dividing until nothing changes
	int num_classes_old;
	do
	{
		// Save the number of classes we have now
		num_classes_old = states->total_classes;

		// Try to divide the SBC
		for (int i = 0; i < states->total_classes; i++)
		{
			divide_class(afd, states, i);
			print_sbc(afd, states);
		}

		// Check if the number of classes changed, keep going until it doesn't change
	} while (num_classes_old != states->total_classes);

	// Get number of symbols to create the AFND
	int total_symbols = AFNDNumSimbolos(afd);

	// Create one state for each class
	AFND *afd_min = AFNDNuevo("afd_min", states->total_classes, total_symbols);

	// Insert the same symbols as the original
	for (int i = 0; i < total_symbols; i++)
		AFNDInsertaSimbolo(afd_min, AFNDSimboloEn(afd, i));

	// Insert one state for each class
	int initial = NO_INITIAL;
	for (int i = 0; i < states->total_classes; i++)
	{
		int type = gen_type(afd, states->data[i], states->num_states[i], &initial);

		AFNDInsertaEstado(afd_min, AFNDNombreEstadoEn(afd, states->data[i][0]), type);
	}

	// For each new state (or each class)
	for (int i = 0; i < states->total_classes; i++)
	{
		// For each symbol
		for (int j = 0; j < total_symbols; j++)
		{
			// Get symbol
			char *symbol = AFNDSimboloEn(afd_min, j);

			// State zero is the first of the class
			int s0 = states->data[i][0];

			// State one is the first of the next class
			int s1 = states->data[get_transition_class(afd, states, s0, j)][0];

			// Create the transition [s0 --symbol--> s1]
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
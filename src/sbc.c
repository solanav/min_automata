#include <stdio.h>
#include <stdlib.h>

#include "../include/afnd.h"
#include "../include/sbc.h"

void swap_class(sbc *states, int i, int j)
{
    // Swap pointers
    int *tmp_p = states->data[i];
    states->data[i] = states->data[j];
    states->data[j] = tmp_p;

    // Swap counters
    int tmp_i = states->num_states[i];
    states->num_states[i] = states->num_states[j];
    states->num_states[j] = tmp_i;
}

void swap_state(sbc *states, int class_i, int i, int j)
{
    int tmp = states->data[class_i][i];
    states->data[class_i][i] = states->data[class_i][j];
    states->data[class_i][j] = tmp;
}

void print_sbc(AFND *afd, sbc *states)
{
	printf("Classes > [%d]\n", states->total_classes);

	for (int i = 0; i < states->total_classes; i++)
	{
		printf("\tClass %d (%d) [", i, states->num_states[i]);
		for (int j = 0; j < states->num_states[i]; j++)
		{
			printf("(%s:%d) ", AFNDNombreEstadoEn(afd, states->data[i][j]), states->data[i][j]);
		}
		printf("]\n");
	}
}

int add_class_sbc(sbc *states)
{
	// Add one to total classes
	states->total_classes++;
	int tc = states->total_classes;

	// Get another int in num_states
	states->num_states = realloc(states->num_states, tc * sizeof(int));
	states->num_states[tc - 1] = 0;

	// Get another list inside data
	states->data = realloc(states->data, tc * sizeof(int *));

	// Get memory for the new list
	states->data[tc - 1] = calloc(states->num_states[tc - 1], sizeof(int));

    return states->total_classes - 1;
}

void remove_class_sbc(sbc *states, int class_i)
{
    // Swap with last class
    int last_class = states->total_classes - 1;
    swap_class(states, class_i, last_class);

    // Remove states inside the class
    for (int i = 0; i < states->num_states[last_class]; i++)
        remove_state_sbc(states, last_class, i);

    // Free the last class
    free(states->data[last_class]);
    
    // Update total classes
    states->total_classes--;
}

int get_class_sbc(sbc *states, int state)
{
    int found = 0;

    // Find the given state
    for (int i = 0; i < states->total_classes && found == 0; i++)
    {
        for (int j = 0; j < states->num_states[i] && found == 0; j++)
        {
            if (states->data[i][j] == state)
                return i;
        }
    }

    return -1;    
}

void add_state_sbc(sbc *states, int class_i, int state)
{
	states->num_states[class_i]++;
	int ns = states->num_states[class_i];

	states->data[class_i] = realloc(states->data[class_i], ns * sizeof(int));
	states->data[class_i][ns - 1] = state;
}

void remove_state_sbc(sbc *states, int class_i, int state_i)
{
    // Swap with the last state
    int last_state = states->num_states[class_i] - 1;
    swap_state(states, class_i, state_i, last_state);

    // Update the number of states of the class
    states->num_states[class_i]--;
    states->data[class_i] = realloc(states->data[class_i], states->num_states[class_i] * sizeof(int));
}

void free_sbc(sbc *states)
{
	for (int i = 0; i < states->total_classes; i++)
		free(states->data[i]);

	free(states->data);
	free(states->num_states);
	free(states);
}

sbc *create_sbc()
{
	sbc *states = calloc(1, sizeof(sbc));

	// Total
	states->total_classes = 1;

	// One int for each class
	states->num_states = calloc(states->total_classes, sizeof(int));
	states->num_states[0] = 0;

	// Get memory for data
	states->data = calloc(states->total_classes, sizeof(int *));
	states->data[0] = calloc(states->num_states[0], sizeof(int));

	return states;
}
#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/afnd_plus.h"
#include "../include/transforma.h"
#include "../include/minimiza.h"
#include "../include/types.h"



AFND * AFNDMinimiza(AFND * afnd){
	return NULL;
}


int list_final_states(AFND *afnd, int **t_list_finals,int **t_list_nofinals, int *states){
	int num_states_f = 0; 
	int num_states_nf = 0; 
	int num_states = AFNDNumEstados(afnd);

	*t_list_finals = calloc(num_states, sizeof(int));
	*t_list_nofinals = calloc(num_states, sizeof(int));

	for (int i = 0; i < num_states; i++)
	{
		type = AFNDTipoEstadoEn(afnd, states[i]);
		if ( type == 1 || type == 2)
		{
			(*t_list_finals)[num_states_f] = i;
			num_states_f++;
		}
		else{
			(*t_list_nofinals)[num_states_nf] = i;
			num_states_nf++;
		}
	}
	*t_list_finals = realloc(*t_list_finals, num_states_f * sizeof(int));
	*t_list_nofinals = realloc(*t_list_nofinals, num_states_nf * sizeof(int));
	return num_states_f;

}
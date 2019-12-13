#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/afnd_plus.h"
#include "../include/transforma.h"
#include "../include/minimiza.h"
#include "../include/types.h"

// Estructura para estado
typedef struct _state
{
    char *name;     // Name of the state
    int type;       // Type of the state
    int *i_list;    // Index of the states contained in the original afnd
    int i_n;        // Size of i_list
} state;

// Estructura que arupa una pareja si son de la misma clase
typedef struct _couple_states
{
    state *s1;  // State 1
    state *s2;  // State 2
} couple_states;

// Estructura clase que agrupa todas las clases
typedef struct _class_states
{
	char *name;				    // Name of the class
    //class_states * class_list;  // Set of classes grouped in this class
} class_states;



AFND * AFNDMinimiza(AFND * afnd){
	return NULL;
}


int list_final_states(AFND *afnd, int **t_list_finals,int **t_list_nofinals, int *states){
	
	int num_states_f = 0; 
	int num_states_nf = 0; 
	int num_states = AFNDNumEstados(afnd);
	int type;
	
	*t_list_finals = calloc(num_states, sizeof(int));
	*t_list_nofinals = calloc(num_states, sizeof(int));

	for (int i = 0; i < num_states; i++)
	{
		type = AFNDTipoEstadoEn(afnd, states[i]);
		if ( type == FINAL)
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

// Dada una lista de estados, devuelve todas las parejas posibles.
int couples (AFND * afnd, int * states, int n_states, int **t_list_couples){
	couple_states * cp = NULL;
	int n_couples = 0;
	
	*t_list_couples = calloc(n_states, sizeof(int));
	
	for(int i=0; i<n_states; i++){
		cp->s1 = states[i];
		for(int j=i; j<n_states; j++){
			cp->s2 = states[j];
			(*t_list_couples)[n_couples] = &cp;
			cp = NULL;
			n_couples++;
		}
	}

	*t_list_couples = realloc(*t_list_couples, n_couples * sizeof(int));
	return n_couples;
}
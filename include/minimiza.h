#ifndef MINIMIZA_H
#define MINIMIZA_H
#include "../include/afnd.h"

typedef struct _state
{
    char *name;     // Name of the state
    int type;       // Type of the state
    int *i_list;    // Index of the states contained in the original afnd
    int i_n;        // Size of i_list
} state;


AFND * AFNDMinimiza(AFND * afnd);

/**
 * Separate the states of automatons in two, final and not final
 * 
 * t_list_finals: list where the final type states are put
 * t_list_nofinals: list where the  no final type (initial or normal) states are put
 * states: array of states of which the automaton is composed
 * 
 * Returns the number of final states
 */

int list_final_states(AFND *afnd, int **t_list_finals,int **t_list_nofinals, int *states);

#endif
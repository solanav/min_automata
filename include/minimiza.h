#ifndef MINIMIZA_H
#define MINIMIZA_H
#include "../include/afnd.h"




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

/**
 * Separate the states of automatons in two, final and not final
 * 
 * t_list_finals: list where the final type states are put
 * t_list_nofinals: list where the  no final type (initial or normal) states are put
 * states: array of states of which the automaton is composed
 * 
 * Returns the number of final states
 */

int couples (AFND * afnd, int * states, int n_states, int **t_list_couples);

#endif
#ifndef AFND_PLUS_H
#define AFND_PLUS_H

#include <stdbool.h>
#include "../include/afnd.h"

typedef unsigned int state_i;
typedef unsigned int symbol_i;

#define NO_INITIAL 0
#define INITIAL 1


/**
 * Generate a list of transitions for a state
 * 
 * afnd: afnd to get the transitions from
 * t_list: list to the transitions (memory is allocated inside)
 * state: index of the state you want the transitions from
 * symbol_i: symbol with look at transitions
 * 
 * Returns the number of transitions in the given state
 */
int get_transitions(AFND *afnd, int **t_list, int state, int symbol_i);

/**
 * Generate a list of transitions for several states
 * 
 * afnd: afnd to get the transitions from
 * t_list: list to the transitions (memory is allocated inside)
 * num_states: total number of states of the automata
 * symbol_i: symbol with look at transitions
 * 
 * Returns the number of transitions in the given state
 */
int get_transitions_x(AFND *afnd, int **t_list, const int *states, int num_states, int symbol_i);

/**
 * Generate a name for a new state
 * 
 * states: list of states to generate the name from
 * num_states: number of states in the list
 * 
 * Returns a string containing the new name
 */
char *gen_name(AFND *afnd, int *states, int num_states);

/**
 * Create the AD by extracting the data from the matrix
 * 
 * name: automata name
 * matrix: the data is taken from there
 * 
 * Returns the new AD
 */
AFND *AFD(char *name, int **matrix);

/**
 * Given an state, returns a list of states connected by lambda
 * 
 * states: array where the states will be saved (memory allocated inside)
 * state: objective of the operation
 * 
 * Returns the number of states in the states array
 */
int get_states_connected(AFND *afnd, int **states, int state);

/**
 * Given a list of states it returns the type of the group
 * 
 * states: list of states to check
 * state_n: number of the states
 * initial: variable to check if there already is an initial state
 * 
 * Returns: the type of the state as defined in afnd.h
 */
int gen_type(AFND *afnd, int *states, int state_n, int *initial);

/**
 * Given a list of states it returns the list plus all the lambda connections of each state
 * 
 * lstates: list plus lambda connections
 * states: list without lambda connections
 * states_n: size of the list of states
 * 
 * Returns size of lstates
 */
int add_lambdas(AFND *afnd, int **lstates, const int *states, int states_n);

#endif
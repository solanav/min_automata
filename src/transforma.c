#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/afnd.h"
#include "../include/afnd_plus.h"
#include "../include/transforma.h"
#include "../include/types.h"

typedef struct _state
{
    char *name;  // Name of the state
    int type;    // Type of the state
    int *i_list; // Index of the states contained in the original afnd
    int i_n;     // Size of i_list
} state;

#define save_state(afnd, sl, i, il, iln, initial)      \
    {                                                  \
        sl[i].name = gen_name(afnd, il, iln);          \
        sl[i].type = gen_type(afnd, il, iln, initial); \
        sl[i].i_list = calloc(iln, sizeof(int));       \
        memcpy(sl[i].i_list, il, iln * sizeof(int));   \
        sl[i].i_n = iln;                               \
    }

#ifdef DEBUG
void print_state(const state *s)
{
    char type_str[16] = {0};

    if (s->type == 0)
        strcpy(type_str, "inicial");
    else if (s->type == 1)
        strcpy(type_str, "final");
    else if (s->type == 2)
        strcpy(type_str, "inicial y final");
    else if (s->type == 3)
        strcpy(type_str, "normal");
    else
        strcpy(type_str, "      ");

    if (s->name != NULL)
    {
        printf(P_INFO "\"%8s\", %7s, [", s->name, type_str);

        for (int i = 0; i < s->i_n; i++)
            printf("%d, ", s->i_list[i]);
        printf("]\n");
    }
    else
        printf(P_INFO "          , %7s, [ ]\n", type_str);
}
#else
void print_state(const state *s)
{
}
#endif

void free_state_list(state *state_list, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        if (state_list[i].type != -1)
        {
            free(state_list[i].name);
            free(state_list[i].i_list);
        }
    }
    free(state_list);
}

bool contains(state *transitions, int transitions_n, char *state_name)
{
    for (int i = 0; i < transitions_n; i++)
    {
        if (strcmp(transitions[i].name, state_name) == 0)
            return true;
    }

    return false;
}

AFND *create_afd(AFND *afnd, state *states, size_t sl, state *transitions)
{
    static int name_var = 0;

    // Create the new automata
    int n_simbolos = AFNDNumSimbolos(afnd);
    printf("Creating afnd with %ld states and %d symbols\n", sl, n_simbolos);
    char name[16] = {0};
    sprintf(name, "stadia_%d", name_var);
    AFND *afd = AFNDNuevo(name, sl, n_simbolos);

    // Insert the symbols
    for (int i = 0; i < n_simbolos; i++)
        AFNDInsertaSimbolo(afd, AFNDSimboloEn(afnd, i));

    // Insert the states
    for (int i = 0; i < sl; i++)
        AFNDInsertaEstado(afd, states[i].name, states[i].type);

    // Insert the normal transitions
    for (int i = 0; i < sl; i++)
    {
        for (int j = 0; j < n_simbolos; j++)
        {
            if (transitions[i * n_simbolos + j].type != -1)
            {
                AFNDInsertaTransicion(
                    afd,
                    states[i].name,
                    AFNDSimboloEn(afnd, j),
                    transitions[i * n_simbolos + j].name);
            }
        }
    }

    name_var++;

    return afd;
}

AFND *AFNDTransforma(AFND *afnd)
{
    // Temporal list of states of final automata
    int f_states_n = 0;
    int f_transitions_n = 0;
    state *f_states = calloc(1, sizeof(state));
    state *f_transitions = calloc(1, sizeof(state));

    // Get initial state
    int *initial_list = NULL;
    int initial_list_n = get_states_connected(afnd, &initial_list, AFNDIndiceEstadoInicial(afnd));

#ifdef DEBUG
    printf(P_INFO "Initial state list: [");
    for (int i = 0; i < initial_list_n; i++)
        printf("%d, ", initial_list[i]);
    printf("]\n");
#endif

    // Insert the initial state to have something inside the list
    int initial = NO_INITIAL;
    save_state(afnd, f_states, f_states_n, initial_list, initial_list_n, &initial);
    free(initial_list);
    f_states_n++;

#ifdef DEBUG
    print_state(&f_states[f_states_n - 1]);
    printf("\n");
#endif

    // Main loop
    for (int current_state = 0; current_state < f_states_n; current_state++)
    {
        // Get all transitions and insert if new
        for (int i = 0; i < AFNDNumSimbolos(afnd); i++)
        {
            // Get transition data
            int *t_list = NULL;
            int t_list_n = get_transitions_x(
                afnd,
                &t_list,
                f_states[current_state].i_list,
                f_states[current_state].i_n,
                i);

            // Add the lambdas to the transitions
            int *tl_list = NULL;
            int tl_list_n = add_lambdas(afnd, &tl_list, t_list, t_list_n);
            free(t_list);

            if (tl_list_n == 0)
            {
                f_transitions = realloc(f_transitions, (f_transitions_n + 1) * sizeof(state));
                f_transitions[f_transitions_n].type = -1;
                f_transitions_n++;
                free(tl_list);
                continue;
            }

            char *transition_name = gen_name(afnd, tl_list, tl_list_n);

            // Insert into the transition array
            f_transitions = realloc(f_transitions, (f_transitions_n + 1) * sizeof(state));
            save_state(afnd, f_transitions, f_transitions_n, tl_list, tl_list_n, &initial);
            f_transitions_n++;

#ifdef DEBUG
            printf(P_INFO "Inserted transition (%s):\n", AFNDSimboloEn(afnd, i));
            print_state(&f_transitions[f_transitions_n - 1]);
            printf("\n");
#endif

            // Insert into the states array
            if (contains(f_states, f_states_n, transition_name) == false)
            {
                f_states = realloc(f_states, (f_states_n + 1) * sizeof(state));
                save_state(afnd, f_states, f_states_n, tl_list, tl_list_n, &initial);
                f_states_n++;
            }

            free(tl_list);
            free(transition_name);
        }
    }

    // Print what we got
#ifdef DEBUG
    printf("\n" P_INFO "All the states:\n");
    for (int i = 0; i < f_states_n; i++)
    {
        printf("\t");
        print_state(&f_states[i]);
    }

    printf("\n" P_INFO "All the transitions:\n");
    for (int i = 0; i < f_transitions_n; i++)
    {
        if (f_transitions[i].type == -1)
        {
            printf("\t [%02d]\n", i);
            continue;
        }
        printf("\t [%02d]", i);
        print_state(&f_transitions[i]);
    }
    printf("\n");
#endif

    AFND *afd = create_afd(afnd, f_states, f_states_n, f_transitions);

    free_state_list(f_states, f_states_n);
    free_state_list(f_transitions, f_transitions_n);

    return afd;
}

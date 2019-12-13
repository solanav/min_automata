#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/transforma.h"
#include "../include/types.h"


AFND *AFND2()
{

	AFND *p_afnd;
	p_afnd = AFNDNuevo("afnd2", 4, 2);

	AFNDInsertaSimbolo(p_afnd, "0");
	AFNDInsertaSimbolo(p_afnd, "1");

	AFNDInsertaEstado(p_afnd, "q0", INICIAL);
	AFNDInsertaEstado(p_afnd, "q1", NORMAL);
	AFNDInsertaEstado(p_afnd, "q3", NORMAL);
	AFNDInsertaEstado(p_afnd, "q2", FINAL);

	
	AFNDInsertaTransicion(p_afnd, "q0", "0", "q1");
	AFNDInsertaTransicion(p_afnd, "q0", "1", "q0");
	AFNDInsertaTransicion(p_afnd, "q1", "1", "q2");
	AFNDInsertaTransicion(p_afnd, "q3", "1", "q2");

	// There are not lambda transitions in this example
	AFNDCierraLTransicion(p_afnd);

	return p_afnd;
}


int main(int argc, char **argv)
{
	AFND *afd;
	AFND *(*test_afnd[])() = {AFND2};
	

	// AFND practica
	for (int i = 0; i < 1; i++)
	{
		AFND *p_afnd = (*test_afnd[i])();

		afd = AFNDTransforma(p_afnd);
		if (afd == NULL)
		{
			printf(P_ERROR "AFNDTransforma returned NULL\n");
			AFNDElimina(p_afnd);
			return EXIT_SUCCESS;
		}
		
		AFNDADot(afd);

		AFNDElimina(afd);
		AFNDElimina(p_afnd);
	}

	return EXIT_SUCCESS;
}

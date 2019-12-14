#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/afnd.h"
#include "../include/transforma.h"
#include "../include/types.h"
#include "../include/minimiza.h"

AFND *AFND_TEST0()
{

	AFND *p_afnd;
	p_afnd = AFNDNuevo("afnd_test", 6, 2);

	AFNDInsertaSimbolo(p_afnd, "0");
	AFNDInsertaSimbolo(p_afnd, "1");

	AFNDInsertaEstado(p_afnd, "A", INICIAL);
	AFNDInsertaEstado(p_afnd, "B", NORMAL);
	AFNDInsertaEstado(p_afnd, "C", FINAL);
	AFNDInsertaEstado(p_afnd, "D", FINAL);
	AFNDInsertaEstado(p_afnd, "E", FINAL);
	AFNDInsertaEstado(p_afnd, "F", NORMAL);
	
	AFNDInsertaTransicion(p_afnd, "A", "0", "B");
	AFNDInsertaTransicion(p_afnd, "A", "1", "C");

	AFNDInsertaTransicion(p_afnd, "B", "0", "A");
	AFNDInsertaTransicion(p_afnd, "B", "1", "D");

	AFNDInsertaTransicion(p_afnd, "C", "0", "E");
	AFNDInsertaTransicion(p_afnd, "C", "1", "F");

	AFNDInsertaTransicion(p_afnd, "D", "0", "E");
	AFNDInsertaTransicion(p_afnd, "D", "1", "F");

	AFNDInsertaTransicion(p_afnd, "E", "0", "E");
	AFNDInsertaTransicion(p_afnd, "E", "1", "F");

	AFNDInsertaTransicion(p_afnd, "F", "0", "F");
	AFNDInsertaTransicion(p_afnd, "F", "1", "F");

	AFNDCierraLTransicion(p_afnd);

	return p_afnd;
}

AFND *AFND_TEST1()
{

	AFND *p_afnd;
	p_afnd = AFNDNuevo("afnd_test", 5, 2);

	AFNDInsertaSimbolo(p_afnd, "0");
	AFNDInsertaSimbolo(p_afnd, "1");

	AFNDInsertaEstado(p_afnd, "A", INICIAL);
	AFNDInsertaEstado(p_afnd, "B", NORMAL);
	AFNDInsertaEstado(p_afnd, "C", NORMAL);
	AFNDInsertaEstado(p_afnd, "D", NORMAL);
	AFNDInsertaEstado(p_afnd, "E", FINAL);
	
	AFNDInsertaTransicion(p_afnd, "A", "0", "B");
	AFNDInsertaTransicion(p_afnd, "A", "1", "C");
	
	AFNDInsertaTransicion(p_afnd, "B", "0", "B");
	AFNDInsertaTransicion(p_afnd, "B", "1", "D");
	
	AFNDInsertaTransicion(p_afnd, "C", "0", "B");
	AFNDInsertaTransicion(p_afnd, "C", "1", "C");
	
	AFNDInsertaTransicion(p_afnd, "D", "0", "B");
	AFNDInsertaTransicion(p_afnd, "D", "1", "E");
	
	AFNDInsertaTransicion(p_afnd, "E", "0", "B");
	AFNDInsertaTransicion(p_afnd, "E", "1", "C");
	
	AFNDCierraLTransicion(p_afnd);

	return p_afnd;
}

AFND *AFND_TEST2()
{

	AFND *p_afnd;
	p_afnd = AFNDNuevo("afnd_test", 8, 2);

	AFNDInsertaSimbolo(p_afnd, "0");
	AFNDInsertaSimbolo(p_afnd, "1");

	AFNDInsertaEstado(p_afnd, "A", INICIAL);
	AFNDInsertaEstado(p_afnd, "B", NORMAL);
	AFNDInsertaEstado(p_afnd, "C", FINAL);
	AFNDInsertaEstado(p_afnd, "D", NORMAL);
	AFNDInsertaEstado(p_afnd, "E", NORMAL);
	AFNDInsertaEstado(p_afnd, "F", NORMAL);
	AFNDInsertaEstado(p_afnd, "G", NORMAL);
	AFNDInsertaEstado(p_afnd, "H", NORMAL);
	
	AFNDInsertaTransicion(p_afnd, "A", "0", "B");
	AFNDInsertaTransicion(p_afnd, "A", "1", "F");
	
	AFNDInsertaTransicion(p_afnd, "B", "0", "G");
	AFNDInsertaTransicion(p_afnd, "B", "1", "C");
	
	AFNDInsertaTransicion(p_afnd, "C", "0", "A");
	AFNDInsertaTransicion(p_afnd, "C", "1", "C");
	
	AFNDInsertaTransicion(p_afnd, "D", "0", "C");
	AFNDInsertaTransicion(p_afnd, "D", "1", "G");
	
	AFNDInsertaTransicion(p_afnd, "E", "0", "H");
	AFNDInsertaTransicion(p_afnd, "E", "1", "F");
	
	AFNDInsertaTransicion(p_afnd, "F", "0", "C");
	AFNDInsertaTransicion(p_afnd, "F", "1", "G");
	
	AFNDInsertaTransicion(p_afnd, "G", "0", "G");
	AFNDInsertaTransicion(p_afnd, "G", "1", "E");
	
	AFNDInsertaTransicion(p_afnd, "H", "0", "G");
	AFNDInsertaTransicion(p_afnd, "H", "1", "C");

	AFNDCierraLTransicion(p_afnd);

	return p_afnd;
}


int main(int argc, char **argv)
{
	AFND *afd;
	AFND *afd_min;
	AFND *p_afnd = AFND_TEST2();

	// Removing inaccessible nodes
	afd = AFNDTransforma(p_afnd);
	if (afd == NULL)
	{
		printf(P_ERROR "AFNDTransforma returned NULL\n");
		AFNDElimina(p_afnd);
		return EXIT_SUCCESS;
	}

	AFNDImprime(stdout, afd);
	
	// Call minimiza
	afd_min = AFNDMinimiza(afd);
	if (afd_min == NULL)
	{
		printf(P_ERROR "Minimiza returned NULL\n");
		AFNDElimina(afd);
		AFNDElimina(p_afnd);
		return EXIT_SUCCESS;
	}

	AFNDImprime(stdout, afd_min);

	// Create dot
	AFNDADot(afd_min);

	// Cleaning
	AFNDElimina(afd);
	AFNDElimina(afd_min);
	AFNDElimina(p_afnd);

	return EXIT_SUCCESS;
}

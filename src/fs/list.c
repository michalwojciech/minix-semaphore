/* 
 * Archivo: list.c
 * ---------------
 * Implementacion recursiva del ADT lista lineal simplemente encadenadas
 * con header y sin repeticiones.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Cantidad maxima de elementos para utilizar en la suma de todas las listas */
#define MAX_ELEMS (MAX_LISTS * MAX_NODES)

#define Precondition(x) assert(x)

/* Nodo distinguido header */

struct listCDT 
{
	struct nodeCDT *nodes;
	int (*Compare)(listElementT elem1, listElementT elem2);
	size_t size;
	struct nodeCDT *current;
	listSortT order;
};



/* Formato del resto de los nodos */

typedef struct nodeCDT 
{
	listElementT element;
	struct nodeCDT *tail;
} nodeCDT;


/* Espacio de elementos, nodos y listas a utilizar */

typedef struct
{
	char element[MAX_ELEM_SIZE];
	int busy;
} elementSlot;

typedef struct
{
	nodeCDT node;
	int busy;
} nodeSlot;

typedef struct
{
	struct listCDT list;
	int busy;
} listSlot;

elementSlot elements[MAX_ELEMS];
nodeSlot nodes[MAX_NODES];
listSlot lists[MAX_LISTS];

/* Funciones de liberacion */

static void
free_element(listElementT element)
{
	int i;
	
	for (i = 0; i < MAX_ELEMS; i++)
		if ((listElementT)(elements[i].element) == element)
			elements[i].busy = 0;

	return;
}

static void
free_node(nodeCDT *node)
{
	int i;

	for (i = 0; i < MAX_NODES; i++)
		if (&(nodes[i].node) == node)
			nodes[i].busy = 0;

	return;
}

static void
free_list(struct listCDT *list)
{
	int i;
	
	for (i = 0; i < MAX_LISTS; i++)
		if (&(lists[i].list) == list)
			lists[i].busy = 0;

	return;
}


/* Prototipos de funciones locales */

static int ListFindNode(listADT, nodeCDT *, listElementT, nodeCDT **);

#define DEBUG 1

/* Implementacion de funciones publicas */

listADT
NewList(int (*fn)(listElementT elem1, listElementT elem2), size_t size, listSortT order) 
{
	int i;
	listADT auxi = NULL;
       
	Precondition( fn != NULL );
        Precondition( size != 0 );

	/* Busco un slot de lista libre */
	for (i = 0; i < MAX_LISTS; i++)
	{
		if (lists[i].busy == 0)
		{
			auxi = &(lists[i].list);
			lists[i].busy = 1;
			break;
		}
	}
	
	if (auxi == NULL)
		return NULL;
	
	auxi->nodes = NULL;
	auxi->Compare = fn;
	auxi->size = size;
	auxi->current = NULL;
	auxi->order = order;
	return auxi;
}


int
ListIsEmpty(listADT list) 
{
	return list->nodes == NULL;
}


void
ListToBegin(listADT list)
{
	list->current = list->nodes;
}



int
ListGetElement(listADT list, listElementT element)
{
	if (list->current == NULL)
		return LIST_END;

	memcpy(element, list->current->element, list->size);
	list->current = list->current->tail;

	return LIST_NO_ERR;
}


int
ListInsert(listADT list, listElementT element)
{
	int i;
	nodeCDT *aux = NULL, *new = NULL;

	if (list->order == LIST_SORTED) {

		/* Si la lista esta ordenada, debo buscar el lugar donde
		 * insertar el nuevo elemento, y de estar repetido, 
		 * retornar un valor de error.
		 */
		
		if (ListFindNode(list, list->nodes, element, &aux))
			return LIST_ELEM_EXISTS;

	}
	else {

		/* Si la lista esta desordenada, el nuevo elemento 
		 * va al principio (arbitratiamente), y por tal razon se
		 * permite que este repetido, asi que no es necesario
		 * ningun chequeo.
		 */

		aux = NULL;

	}
	
	/* En este momento, aux contiene el nodo que debe aparecer
	 * antes del nuevo elemento.  Si el nuevo elemento debe aparecer
	 * al princpio, entonces aux = NULL.
	 */
	
	/* Busco un nodo libre */
	for (i = 0; i < MAX_NODES; i++)
	{
		if (nodes[i].busy == 0)
		{
			new = &(nodes[i].node);
			nodes[i].busy = 1;
			break;
		}
	}
	
	if (new == NULL)
		return LIST_NO_MEM;
	
	new->tail = (aux == NULL) ? list->nodes : aux->tail;
	new->element = NULL;

	for (i = 0; i < MAX_ELEMS; i++)
	{
		if (elements[i].busy == 0)
		{
			new->element = (listElementT)elements[i].element;
			elements[i].busy = 1;
			break;
		}
	}

	if (new->element == NULL)
	{
		free_node(new);
		return LIST_NO_MEM;
	}
	
	memcpy(new->element, element, list->size);

	if (aux == NULL)  /* es el primero */
		list->nodes = new;
	else	
		aux->tail = new;

	return LIST_NO_ERR;
}


int
ListDelete(listADT list, listElementT element)
{
	nodeCDT *aux = NULL ;
	nodeCDT *borrar ;

	/* Aunque la lista este desordenada y admita elementos
	 * repetidos, la eliminacion se hace sobre la primer ocurrencia
	 * de element dentro de la lista.
	 */

	if (! ListFindNode(list, list->nodes, element, &aux) )
		return LIST_NO_ELEM;

	if (aux == NULL) {
		borrar = list->nodes;
		list->nodes = borrar->tail;
	}
	else {
		borrar = aux->tail;
		aux->tail = borrar->tail;
	}

	if (list->current == borrar)
		list->current = borrar->tail;

	free_element(borrar->element);
	free_node(borrar);
	return LIST_NO_ERR;
}


int	
ListElementBelongs(listADT list, listElementT element, listElementT copy)
{
	nodeCDT *aux = NULL;
	nodeCDT *source;

	if ( ! ListFindNode(list, list->nodes, element, &aux) )
		return 0;

	if (copy != NULL) {
		source = (aux == NULL) ? list->nodes : aux->tail;
		memcpy(copy, source->element, list->size);
	}

	return 1;
}	


void
FreeList(listADT list)
{
	nodeCDT *auxi;
	
	while ( !ListIsEmpty(list) ) {
		auxi = list->nodes;
		list->nodes = auxi->tail;
		free_element(auxi->element);
		free_node(auxi);
	}

	free_list(list);
}


int
ListRemoveFirst(listADT list)
{
	nodeCDT *aux;
	
	if ( ListIsEmpty(list) )
		return LIST_END;

	ListToBegin(list);
	aux = list->nodes;
	list->nodes = list->nodes->tail;
	free_element(aux->element);
	free_node(aux);

	return LIST_NO_ERR;
	
}

/* Implementacion de funciones locales */


static int
ListFindNode(listADT list, nodeCDT *chain, listElementT element, nodeCDT **result)
{
	int aux;
		
	/* Si se termino la cadena, retorno 0.  En result quedo la direccion
	 * del ultimo nodo de la lista.
	 */	
	if (chain == NULL) {
		return 0;
	}

	aux = list->Compare( (listElementT)chain->element, element);
	
	if (aux < 0 || (aux > 0 && list->order == LIST_UNSORTED) )
       	{
		/* Almaceno en result la direccion del nodo actual, para que
		 * si la siguiente iteracion tiene que cortar, ya quede el valor
		 * de retorno.
		 */
		*result = chain;
		return ListFindNode(list, chain->tail, element, result);
	}
	else {
		if (aux == 0) {

			/* Como lo encontre, retorno un puntero al nodo buscado */
			/* *result = chain;*/
			return 1;
		}
		else {
		
			/* Si el nodo no aparece, pero deberia estar al principio, no
			 * tengo manera de retornar el nodo anterior, y por lo tanto
			 * devuelvo NULL.
			 */
			return 0;
		}
	}	
}

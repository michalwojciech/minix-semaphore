/* 
 * Archivo: list.h
 * ---------------
 * Interfaz del ADT lista lineal simplemente encadenada, ordenada, 
 * sin repeticiones y con header.
 */

#ifndef _LIST_H
#define _LIST_H

#define LIST_NO_ERR      1  /* No hubo errores */
#define LIST_NO_MEM      0  /* Memoria insuficiente */
#define LIST_END         2  /* El cursor actual se encuentra al final de la lista */
#define LIST_NO_ELEM     3  /* El elemento solicitado no existe en la lista */
#define LIST_ELEM_EXISTS 4  /* El elemento ya existe */

/* Cantidad m·xima de listas */
#define MAX_LISTS 80

/* Cantidad m·xima de nodos para todas las listas de la biblioteca */
#define MAX_NODES 20

/* Tamanio maximo del elemento a guardar en cada nodo */
#define MAX_ELEM_SIZE 4

/* El siguiente tipo define el tipo de datos que manipula
 * el TAD lista con header.  Como se observa los elementos
 * son direcciones previamente alocadas por el usuario (heap o stack).
 * Esto NO permite manipular constantes.
 */
typedef void *listElementT;


/* El siguiente tipo define el TAD lista.
 * Se lo define como puntero a "cierta estructura"
 * para ocultar la real representacion de la misma
 */
typedef struct listCDT *listADT;

typedef enum {LIST_SORTED, LIST_UNSORTED} listSortT;

/* Operaciones con las que se puede acceder al TAD */

/* Funcion:       NewList
 * Uso:           lista = NewList(myFuncion, sizeof(double), LIST_SORTED);
 * ------------------------------------------------------------------
 * Descripcion:   Inicializa la lista y setea la funcion de
 *                comparacion para sus elementos y el tama√o del
 *                tipo de datos que va a manejar la lista.  Dicha
 *                funcion compara dos elementos de tipo listElementT
 *                y debe devolver un numero negativo si el primer 
 *                argumento debe aparecer antes que el segundo, un 
 *                numero positivo si debe aparecer despues, o 0
 *                si se trata del mismo elemento.
 *                Usarla una sola vez antes de empezar a usar el
 *                resto de las operaciones.
 *                Devuelve NULL si no se pudo alocar memoria.
 * ------------------------------------------------------------------
 * Precondicion:  Funcion distinta de NULL, tama√o distinto de 0
 * Postcondicion: Lista creada
 */
listADT NewList(int (*fn)(listElementT elem1, 
			listElementT elem2), size_t size, listSortT order);


/* Funcion:       ListInsert
 * Uso:           double nro = 12.3;
 *                Insert(lista, &nro);
 * ------------------------------------------------------------------
 * Descripcion:   Inserta un elemento en la lista ordenada, usando
 *                la funcion de comparacion seteada en la creacion
 *                de la lista.  El usuario debe proporcionar la 
 *                direccion del elemento.  Si el elemento ya existia
 *                lo ignora.  Si hay problemas en la alocacion de 
 *                memoria retorna 0, caso contrario 1.
 * ------------------------------------------------------------------
 * Precondicion:  Lista que sea valida, previamente creada.
 * Postcondicion: Lista con el nuevo elemento, si no estaba repetido
 */
int ListInsert(listADT list, listElementT element);


/* Funcion:       ListDelete
 * Uso:           double nro = 12.3;
 *                if ( Delete(lista, &nro) ) ...
 * ------------------------------------------------------------------
 * Descripcion:   Elimina el elemento de la lista ordenada, usando
 *                la funcion de comparacion seteada en la creacion
 *                de la lista.  Se proporciona la direccion del 
 *                elemento.  Si el elemento no existia,  devuelve 0,
 *                caso contrario 1.
 * ------------------------------------------------------------------
 * Precondicion:  Lista que sea valida, previamente creada
 * Postcondicion: Lista sin el elemento
 */
int ListDelete(listADT list, listElementT element);



/* Funcion:       ListIsEmpty
 * Uso:           if ( ListIsEmpty( lista ) ) ...
 * ------------------------------------------------------------------
 * Descripcion:   Devuelve 1 si la lista est√ vac√a o cero en
 *                caso contrario.
 * ------------------------------------------------------------------
 * Precondicion:  Lista que sea valida, previamente creada.
 * Postcondicion: - (lista invariante)
 */
int ListIsEmpty(listADT list);


/* Funcion:       ListElementBelongs
 * Uso:           double nro = 12.3;
 *                if ( ListElementBelongs(lista, &nro, NULL) ) ...
 * ------------------------------------------------------------------
 * Descripcion:   Devuelve 1 si el elemento pertenece a la lista
 *                o cero en caso contrario.  El tercer parametro
 *                puede ser NULL o una direccion en donde dejar
 *                una copia del elemento encontrado.
 * ------------------------------------------------------------------
 * Precondicion:  Lista previamente creada
 * Postcondicion: - (lista invariante)
 */
int ListElementBelongs(listADT list, listElementT element, listElementT copy);



/* Funcion:       ListToBegin
 * Uso:           ListToBegin(lista)
 * ------------------------------------------------------------------
 * Descripcion:   Permite comenzar a recorrer la lista desde el
 *                comienzo.  Se usa conjuntamente con ListGetElement
 * ------------------------------------------------------------------
 * Precondicion:  Lista previamente creada
 * Postcondicion: Proximo nodo a acceder con ListGetElement: el 1¬∫
 */
void ListToBegin(listADT list);



/* Funcion:       ListGetElement
 * Uso:           listADT lista;
 *                double nro;
 *                ...
 *                ListToBegin(lista);
 *                while (ListGetElement(lista, &nro) ) 
 *                   printf("%g\n", nro);
 * ------------------------------------------------------------------
 * Descripcion:   Es de consulta.  Sirve para iterar por la lista,
 *                la cual sigue intacta (invariante).  Devuelve 0
 *                cuando se intenta avanzar y se est√°sobre el 
 *                ultimo nodo, o bien 1 en caso contrario.
 *                Luego de la invocacion, el proximo ListGetElement
 *                sera el siguiente elemento y asi sucesivamente.
 *                El segundo parametro debe ser un lugar en stack
 *                o heap reservado por el usuario para recibir
 *                la copia del nodo de la lista.
 * ------------------------------------------------------------------
 * Precondicion:  Lista no vacia
 * Postcondicion: - (lista invariante)
 */
int ListGetElement(listADT list, listElementT element);



/* Funcion:       FreeList
 * Uso:           FreeList( lista );
 * ------------------------------------------------------------------
 * Descripcion:   Destruye la lista, liberando los recursos alocados.
 *                Usarla una sola vez al finalizar el uso de la 
 *                lista.  Despues no volver a invocar operaciones
 *                sobre la lista.
 * ------------------------------------------------------------------
 * Precondicion:  Lista que sea valida.
 * Postcondicion: -
 */
void FreeList(listADT list);


int ListRemoveFirst(listADT list);

#endif

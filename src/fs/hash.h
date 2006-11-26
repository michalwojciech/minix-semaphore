/*
 * Archivo: hash.h
 * ---------------
 * Interfaz para el tipo hashADT que permite manejar tablas
 * de hash en memoria utilizando zona de overflow.
 *  
 */

#ifndef __HASH_TABLE_
#define __HASH_TABLE_

typedef struct hashTableCDT * hashTableADT;

/* La clave es siempre un string.  El elemento es siempre 
 * un puntero.  No modificar estas lineas.
 */
typedef char * keyT;
typedef void * valueT;

#define HASH_NO_ERR 1
#define HASH_NO_MEM 3
#define HASH_ELEM_REPEATED 2
#define HASH_NO_ELEM 0

#define MAX_HASH_ELEM 40


/*
 * Funcion: NewHashTable
 * Uso: handle = NewHashTable(free);
 * ---------------------------------------
 * Reserva espacio en memoria para una nueva tabla de hash.
 * Retorna un handle a la misma, o NULL si hubo problemas.
 */
hashTableADT NewHashTable(void);



/*
 * Funcion: FreeHashTable
 * Uso: FreeHashTable(handle);
 * ----------------------
 * Libera el espacio reservado por NewHashTable.
 */
void FreeHashTable(hashTableADT handle);



/*
 * Funcion: HashTableInsert
 * Uso: HashTableInsert(handle, key, value);
 * ------------------------------------------
 * Inserta en la tabla la combinacion key-value que se le pase.
 * Si key ya existe o hubo problemas de memoria, retorna 0.
 * Caso contrario retorna 1.
 */
int HashTableInsert(hashTableADT handle, keyT key, valueT value);


/*
 * Funcion: HashTableRemove
 * Uso: HashTableRemove(handle, key, NULL);
 * ----------------------------------
 * Elimina de la tabla la entrada correspondiente a key.  Si key
 * no existe o hubo problemas de memoria retorna 0, caso contrario
 * retorna 1.  El tercer valor puede ser NULL, o un puntero en
 * donde dejar la copia del elemento eliminado de la tabla.
 */
int HashTableRemove(hashTableADT handle, keyT key, valueT *target);


int HashTableFind(hashTableADT handle, keyT key, valueT *target);

#endif


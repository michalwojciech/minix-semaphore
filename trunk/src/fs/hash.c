/*
 * Archivo: hash.c
 * ---------------
 * Implementacion del tipo hashTableADT con linear hashing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#define MAX_STR 255
/* Tipo de dato hashEntryT que van a contener las listas de cada bucket */
typedef struct {
	keyT key;
	valueT value;
} hashEntryT;


struct hashTableCDT {
	hashEntryT entry_array[MAX_HASH_ELEM];
	char str_hash[MAX_STR][MAX_HASH_ELEM];
	int used_slot;
};

struct hashTableCDT h1;



/* Implementacion de funciones publicas */

hashTableADT
NewHashTable(void)
{
	int i;
	for (i=0; i<MAX_HASH_ELEM; i++)
	{
		h1.entry_array[i].key = NULL;
	}
	h1.used_slot = 0;
	return &h1;
}

void
FreeHashTable(hashTableADT handle)
{
	return;
}



int
HashTableInsert(hashTableADT handle, keyT key, valueT value)
{
	int i;
	if ( handle->used_slot == MAX_HASH_ELEM )
		return HASH_NO_MEM;

	/* Me fijo si el elemento ya existe. */
	for (i = 0; i < MAX_HASH_ELEM ; i++)
	{
		if ( handle->entry_array[i].key != NULL )
			if (strncmp( handle->entry_array[i].key, key, MAX_STR) == 0)
				return HASH_ELEM_REPEATED;
	}

	for (i = 0; i < MAX_HASH_ELEM ; i++)
	{
		if ( handle->entry_array[i].key == NULL )
		{
			handle->entry_array[i].key = handle->str_hash[i];
			strncpy(handle->str_hash[i], key, MAX_STR );
			handle->entry_array[i].value = value;
			handle->used_slot++;
			return HASH_NO_ERR;
		}
	}
}


int
HashTableRemove(hashTableADT handle, keyT key, valueT *target)
{
	int i;
	
	for (i = 0; i < MAX_HASH_ELEM ; i++)
	{
		if ( handle->entry_array[i].key != NULL )
			if (strncmp( handle->entry_array[i].key, key, MAX_STR) == 0)
			{
				if ( target != NULL )
				{
					*target = handle->entry_array[i].value; 
				}
				handle->entry_array[i].key = NULL; 
				handle->used_slot--;
			}
	}
	return HASH_NO_ELEM;
}



int
HashTableFind(hashTableADT handle, keyT key, valueT *target)
{
	int i;
	printf("HASH FIND: looking %s\n", key);
	for (i = 0; i < MAX_HASH_ELEM ; i++)
	{
		if ( handle->entry_array[i].key != NULL )
		{	
			printf("HASH FIND: i = %d, key = %s\n", i, handle->entry_array[i].key);
			if (strncmp( handle->entry_array[i].key, key, MAX_STR) == 0)
			{
				printf("HASH FIND: FOunded\n");
				if ( target != NULL )
				{
					*target = handle->entry_array[i].value; 
					printf("HASH FIND: *target = %d\n", *target);
				}
				return 1;
			}
		}
	}
	return 0;
}
	

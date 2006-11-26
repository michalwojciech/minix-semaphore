/* Inclusiones solo para MINIX */
#ifndef _LINUX__
#include "fs.h"
#endif
#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include "hash.h"
#include "list.h"

#define DEBUG 1

#define INITIAL_SEM 10
#define SEM_INC 10

typedef struct {
	int sem_count; /* Valor de cuenta del semaforo */
	int ref_count; /* Cantidad de procesos que hicieron open */ 
	listADT proc_list; /* Lista de los procesos que pueden lockear el semaforo */
	listADT blocked_list; /* Lista de los procesos que estan bloqueados sobre este semaforo. */
	mode_t permision;
	int unlinked;
}sem_cdt;

/* Descriptor de semaforos. */
static sem_cdt  sem_array[INITIAL_SEM];
int idx_array[INITIAL_SEM];
static int sem_array_size;

/* Mapa de semaforos, por cada nombre de semaforo contiene una entrada al sem_array */
static hashTableADT sem_name_map;

static int proc_cmp(void * p1, void * p2)
{
	if (*(int*)p1 != *(int*)p2)
		return -1;
	else 
		return 0;
}

static int blocked_cmp(void * p1, void * p2)
{
	if (*(int*)p1 != *(int*)p2)
		return 1;
	else 
		return 0;
}
/* TODO Borrar estas lineas --------------*/
#define TRUE 1
#define FALSE 0

#ifdef _LINUX__
extern int who;
extern int dont_reply;
#endif
/*extern int who;
extern int dont_reply;

#include <stdio.h>
 ---------------------------------------*/
void sem_wake(int, int);

/* Entero para pasar como puntero de respuesta (POR no poder usar malloc)*/
int rta_aux;

void 
init_sem_server(void)
{
	int i;
	sem_name_map = NewHashTable(free);

/*	sem_array = calloc (INITIAL_SEM , sizeof(sem_cdt));*/
	sem_array_size = INITIAL_SEM;

	for ( i = 0; i < INITIAL_SEM ; i++)
	{
		sem_array[i].ref_count = 0;
		idx_array[i] = i;
	}

	/* Si falla no tengo a quien reportarlo. */
	
	return;
}

static 
int sem_create(sem_t * sem, mode_t mode, unsigned value)
{
	int i;
	/* Busco un slot vacio. */
	for ( i=0; i< sem_array_size; i++)
	{
		if (sem_array[i].ref_count == 0)
			break;
	}
	/* Si no habia un slot vacio indico que no se puede crear el semaforo. */
	if ( i == sem_array_size )
	{
		errno = ENOSPC;
	}
	if ( errno == 0 )
	{			
		/* Si pude obtener un slot creo el semaforo. */
		sem_array[i].permision = mode;
		sem_array[i].sem_count = value;
		if (value > SEM_VALUE_MAX)
		{
			errno = EINVAL;
			return -1;
		}
		sem_array[i].ref_count = 1;
		sem_array[i].unlinked = FALSE;
		sem_array[i].proc_list = NewList(proc_cmp, sizeof(int), LIST_UNSORTED);
		ListInsert(sem_array[i].proc_list, (void *)&who);
		sem_array[i].blocked_list = NewList(blocked_cmp, sizeof(int), LIST_UNSORTED);
		*sem = i;
		return 0;
	}
	return -1;

}

sem_t *
sem_open(const char *name, int oflag, ...)
{
	va_list ap;
	int *sem_fd = NULL;
	sem_t * rta = SEM_FAILED;
	int i;
	mode_t mod;
	unsigned val;
	
	va_start (ap, oflag);
#ifdef DEBUG
	printf("OPEN: name = (%s), who = %d \n", name, who);
#endif
	/* Marco que no hubo errores */
	errno = 0;
	
	/* Busco el semaforo segun su nombre */
	HashTableFind(sem_name_map,(char*) name, (void **)&sem_fd);

	
	if (sem_fd == NULL && !(oflag & O_CREAT))
		errno = ENOENT;
	else if (strlen(name) > PATH_MAX)
		errno = ENAMETOOLONG;
	else if ( sem_fd != NULL && oflag & O_CREAT && oflag & O_EXCL )
		errno = EEXIST;
	else if ( sem_fd != NULL && !(oflag & O_EXCL) )
	{
		/* TODO Chequear si tiene permisos para abrir el semaforo. */

		/* Incremento la cuenta de referencia y	agrego el pid a la lista de procesos.*/
		sem_array[*sem_fd].ref_count ++;
		ListInsert(sem_array[*sem_fd].proc_list, (void *)&who);
		
		/* Devuelvo el semaforo ya creado por otro proceso */
		rta_aux = *sem_fd;
		rta = &rta_aux;
#ifdef DEBUG
		printf("OPEN: Abriendo el semaforo existente sem_fd = %d\n", *sem_fd);
#endif

	}
	else if ( sem_fd == NULL && oflag & O_CREAT && !(oflag & O_EXCL) )
	{
		mod = va_arg(ap, mode_t);
		val = va_arg(ap, unsigned);
		if (sem_create(&rta_aux, mod, val) == -1)
			return SEM_FAILED;
#ifdef DEBUG
		printf("OPEN: Creando el semaforo sem_fd = %d\n", rta_aux);
#endif
		
		/* Agrego el semaforo al mapa */
		HashTableInsert(sem_name_map,(char *) name, (void *) &idx_array[rta_aux]);
		rta = &rta_aux;
	}
	
	va_end(ap);
		
	return rta;	
}


int
sem_init(sem_t *handle, int pshared, unsigned value)
{
	if (sem_create(handle, 0, value))
		return -1;
	return 0;
}

int
sem_destroy(sem_t * handle)
{
	sem_t sem_fd = *handle;
	/* Me fijo si el proceso pertenece a la lista de procesos validos. */
	if ( sem_fd < 0 ||sem_fd >= sem_array_size || !ListElementBelongs(sem_array[sem_fd].proc_list, (void *)&who,NULL)) 
	{
		errno = EINVAL;	
		return -1;
	}
	else
	{
		if (!ListIsEmpty(sem_array[sem_fd].blocked_list))
		{
			errno = EBUSY;
			return -1;
		}
		else
		{
			FreeList(sem_array[sem_fd].proc_list);
			FreeList(sem_array[sem_fd].blocked_list);

			/* Libero el slot */
			sem_array[sem_fd].ref_count = 0;
		}
	}
	return 0;
}	

int
sem_unlink(const char *name)
{
	int *sem_fd = NULL;
	
	/* Busco el semaforo segun su nombre */
	HashTableFind(sem_name_map, (char*)name, (void **)&sem_fd);

	if (strlen(name) > PATH_MAX)
		errno = ENAMETOOLONG;
	else if (sem_fd == NULL)
		/* Semaforo no encontrado */
		errno = ENOENT;
	else if (!ListElementBelongs(sem_array[*sem_fd].proc_list, (void *)&who,NULL))
		/* El proceso que me requiere unlink no esta en la lista de
		 * procesos asociados a este semaforo */
		errno = EACCES;
	else if (!ListIsEmpty(sem_array[*sem_fd].proc_list))
	{
		/* Tengo procesos asociados. Lo marco para que en el ultimo close
		 * se elimine del todo */
		sem_array[*sem_fd].unlinked = TRUE;
		
		/* Saco del mapa al semáforo para que en el próximo open sobre
		 * este nombre se cree uno nuevo, como especifica POSIX */
		HashTableRemove(sem_name_map, (char*)name, NULL);
		
		return 0;
	}

	return -1;
}

int
sem_close(sem_t *handle)
{
	int sem_fd = *handle;
	
	/* Me fijo si el proceso pertenece a la lista de procesos validos. */
	if ( sem_fd < 0 ||sem_fd >= sem_array_size || !ListElementBelongs(sem_array[sem_fd].proc_list, (void *)&who,NULL)) 
	{
		errno = EINVAL;	
	}
	else
	{
		/* Remuevo el proceso de la lista de procesos asociados */
		ListDelete(sem_array[sem_fd].proc_list, (void*)&who);

		/* Si estaba marcado como unlinked y el que removi era el ultimo proceso asociado,
		 * elimino totalmente el semaforo */
		if (sem_array[sem_fd].unlinked && ListIsEmpty(sem_array[sem_fd].proc_list))
		{
			FreeList(sem_array[sem_fd].proc_list);
			FreeList(sem_array[sem_fd].blocked_list);

			/* Libero el slot */
			sem_array[sem_fd].ref_count = 0;
		}
	
		return 0;
	}

	return -1;
}


int
sem_wait(sem_t * handle)
{
	int sem_fd = *handle;

	/* Marco que no hubo errores */
	errno = 0;
	/* Me fijo si el proceso pertenece a la lista de procesos validos. */
	if ( sem_fd < 0 || sem_fd >= sem_array_size || !ListElementBelongs(sem_array[sem_fd].proc_list, (void *)&who,NULL)) 
	{
		errno = EINVAL;	
		return -1;
	}
	else
	{
#ifdef DEBUG
	printf("WAIT: handle = %d, who = %d, count_antes = %d \n", *handle, who, sem_array[sem_fd].sem_count);
#endif

		/* Si el valor del semaforo es 0 bloqueo al proceso no enviandole una respuesta */
		if (sem_array[sem_fd].sem_count <= 0 )
		{
			dont_reply = TRUE;
		
			/* Inserto al proceso en la lista de procesos bloqueados. */
			ListInsert(sem_array[sem_fd].blocked_list, (void *)&who);
		}
		sem_array[sem_fd].sem_count--;
	}
	return 0;
	
}

int
sem_trywait(sem_t * handle)
{
	int sem_fd = *handle;

	/* Marco que no hubo errores */
	errno = 0;
	/* Me fijo si el proceso pertenece a la lista de procesos validos. */
	if ( sem_fd < 0 || sem_fd >= sem_array_size || !ListElementBelongs(sem_array[sem_fd].proc_list, (void *)&who,NULL))
	{
		errno = EINVAL;	
		return -1;
	}
	else
	{
		/* Si el valor del semaforo es menor o igual a cero el semaforo esta tomado. */
		if (sem_array[sem_fd].sem_count <= 0 )
		{
			errno = EAGAIN;
			return -1;
		}
		else
			sem_array[sem_fd].sem_count--;
	}
	return 0;
}

int
sem_post(sem_t * handle)
{
	int sem_fd = *handle;
	int proc;

	/* Marco que no hubo errores */
	errno = 0;
	
	/* Me fijo si el proceso pertenece a la lista de procesos validos. */
	if ( sem_fd < 0 || sem_fd >= sem_array_size || !ListElementBelongs(sem_array[sem_fd].proc_list, (void *)&who,NULL)) 
	{
		errno = EINVAL;	
	}
	else
	{
#ifdef DEBUG
	printf("POST: handle = %d, who = %d, count_antes = %d \n", *handle, who, sem_array[sem_fd].sem_count);
#endif

		/* Si el semaforo es negativo tengo que despertar a alguien. */
		if (sem_array[sem_fd].sem_count < 0 )
		{
			ListToBegin(sem_array[sem_fd].blocked_list);
			ListGetElement(sem_array[sem_fd].blocked_list, &proc);
			ListRemoveFirst(sem_array[sem_fd].blocked_list);
#ifdef DEBUG
	printf("POST: Despierto a %d\n", proc);
#endif

			/* Despierto al primer proceso dormido. */
			sem_wake(proc, 0);
		}
		sem_array[sem_fd].sem_count++;
	}
}

int
sem_getvalue(sem_t *sem, int *sval)
{
	int sem_fd = *sem;
	
	/* Me fijo si el proceso pertenece a la lista de procesos validos. */
	if ( sem_fd < 0 || sem_fd >= sem_array_size || !ListElementBelongs(sem_array[sem_fd].proc_list, (void *)&who,NULL)) 
	{
		errno = EINVAL;
		return -1;
	}
	
	/* Devuelvo el valor asociado al semaforo */
	*sval = sem_array[sem_fd].sem_count;
	printf("semaphore.c: SEM_GETVALUE: value = %d\n", *sval);	

	return 0;
}

/*int    sem_timedwait(sem_t *restrict, const struct timespec *restrict); */




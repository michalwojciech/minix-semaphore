/* Este archivo contiene los procedimientos del semáforo. 
 *
 * Tiene el siguiente punto de entrada que se encarga de manejar todos los pedidos
 * al semáforo
 *   semaphore_entry
 * 
 * semaphore_entry recibe el mensaje con la tarea a realizar, lo analiza y en 
 * función del mismo llama a las diferentes funciones del semáforo.
 * Las funciones son
 * 	sem_open
 * 	sem_init
 * 	sem_blablabla
 */
#include "fs.h"
#include "my_param.h"
#include <semaphore.h>
#include <errno.h>
#include <minix/syslib.h>
#include <string.h>


#define MAX_NAME 255
#define DEBUG 1


/* funciones para comunicación de procesos */
void sem_wake(int, int);
static void answer (int rta, int param1, int param2, int params);
static int no_func(void);


void sem_wake (int proc, int rta)
{
    answer (rta, 0, 0, 1);
    reply (proc, errno);
    return;
}

int semaphore_entry(void)
{
/* para evitar que la función modifique los valores del mensaje recibido, 
 * creamos variables locales de manera de evitar tener que acceder 
 * al mensaje para armar la respuesta, es más ineficiente, pero...
 */
	int return_val;

	sem_t sema;
	sem_t *semaux;
	int pshared, oflags, permission, initial, value_now, argc, phsared;
	char name[MAX_NAME];



#ifdef DEBUG
/*
int i;
char *m1;*/
printf("en el semaphore.c\n");/*
for (i = 0; i < 100; i++)
{
	if ((m1 = malloc(128)) == NULL)
	{
		printf("FALLO MALLOC en la iteracion %d\n", i);
		break;
	}
	strcpy(m1, "malloc\n");
	printf("i = %d m1 = %d m1 = %s\n", i, (int)m1, m1);
}*/
#endif
	


	sema = mensaje_semaforo;
	oflags = mensaje_oflag;
	permission = mensaje_permisos;
	initial = mensaje_valInicial;
	value_now = mensaje_valActual;
	argc = mensaje_cantArg;
	pshared = mensaje_pshared;

	sys_copy (who, D, (phys_bytes) mensaje_nombre, FS_PROC_NR, D,  (phys_bytes) name,  (phys_bytes) MAX_NAME);
	name[MAX_NAME - 1] = '\0';

	switch(mensaje_funcion)
	{
		case SEM_OPEN: if (argc == 4)
			       {
#ifdef DEBUG
printf("SEMOPEN name: %s, oflags: %d, permission: %d, initial: %d\n", name, oflags, permission, initial);
#endif 
					semaux = sem_open(name, oflags, permission,
								   initial);
			       }
			       else
			       {
#ifdef DEBUG
printf("SEMOPEN name: %s, oflags: %d\n", name, oflags);
#endif 
					semaux = sem_open(name, oflags);
			       }
			       answer(*semaux,0,0,1);
			       break;

		case SEM_CLOSE: 
				return_val = sem_close(&sema);
				answer(return_val,sema,0,2);
				break;

		case SEM_INIT:  return_val = sem_init(&sema, pshared, initial);
				answer(return_val,sema,0,2);
				break;

		case SEM_WAIT:  return_val = sem_wait(&sema);
				answer(return_val,sema,0,2);
				break;

		case SEM_POST:  return_val = sem_post(&sema);
				answer(return_val,sema,0,2);
				break;

		case SEM_ULINK: return_val = sem_unlink(name);
				answer(return_val,0,0,1);
				break;

		case SEM_GETVAL: return_val = sem_getvalue(&sema, &value_now);
				 answer(return_val,sema,value_now,3);
				 break;

		case SEM_TRYWAIT: return_val = sem_trywait(&sema);
				  answer(return_val,sema,0,2);
				  break;
 
		case SEM_DESTROY: return_val = sem_destroy(&sema);
				  answer(return_val,sema,0,2);
				  break;

		case SEM_TIMEDWAIT: return_val = no_func();
				    answer(return_val,0,0,1);
				    break;
	}
#ifdef DEBUG
printf("fin de semaphore.c\n");
#endif
	if (return_val != 0)
		return (errno);		
	return(OK);
}

static int no_func(void)
{
#ifdef DEBUG
printf("no implementado/a \n");
#endif
	return 0;
}


/* Función answer: arma el mensaje de respuesta 
 * recibe 4 parámetros: 
 * reply_rta: es el valor que retorna la función en su nombre 
 * reply_param1: es el valor que retorna la función en el parámetro 1 si era un puntero 
 * reply_param2: es el valor que retorna la función en el parámetro 2 si era un puntero
 */
static void answer (int rta, int param1, int param2, int params)
{
	switch (params)
	{
		case 3:	reply_param2 = param2;
#ifdef DEBUG
printf("respuesta3: %d\n", param2);
#endif			
			/* caemos al siguiente caso */
		case 2: reply_param1 = param1;
#ifdef DEBUG
printf("respuesta2: %d\n", param1);
#endif			
			/* caemos al siguiente caso */
		case 1: reply_rta = rta;
#ifdef DEBUG
printf("respuesta1: %d\n", reply_rta);
#endif			
	}

	return;

}


/*
int sem_close(sem_t *sem)
{
	printf("en sem_close\n");
	printf("sem: %d -> %d\n", (int)sem, (int)*sem);
	return 0;
}

int sem_getvalue(sem_t *sem, int *val)
{
	printf("en sem_getval\n");
	printf("sem: %d -> %d, val: %d -> %d\n", (int)sem, (int)*sem, (int)val, *val);
	*val = 34;
	return 0;
}


sem_t *sem_open(const char *name, int oflag, ...)
{
	sem_t *s;

	printf("en sem_open\n");

	*s = 10;
	printf("sem: %s, oflag %d\n", name, oflag);
	return s;
}


int    sem_post(sem_t *sem)
{
	printf("en sem_post\n");
	printf("sem: %d -> %d\n", (int)sem, (int)*sem);
	return 0;
}


int    sem_trywait(sem_t *sem)
{
	printf("en sem_try\n");
	printf("sem: %d -> %d\n", (int)sem, (int)*sem);
	return 0;
}


int    sem_unlink(const char *name)
{
	printf("en sem_unlink\n");
	printf("sem: %s\n", name);
	return 0;
}


int    sem_wait(sem_t *sem)
{
	printf("en sem_wait\n");
	printf("sem: %d -> %d\n", (int)sem, (int)*sem);
	errno = 3;
	return 0;
}


int    sem_init(sem_t *sem, int pshared, unsigned initval)
{
	printf("en sem_init\n");
	printf("sem: %d -> %d, pshared: %d, init: %d\n", (int)sem, (int)*sem, pshared, initval);
	errno = 5;
	return -1;
}

int    sem_destroy(sem_t *sem)
{
	printf("en sem_destroy\n");
	printf("sem: %d -> %i\n", sem, (int)*sem);
	return 0;
}
*/




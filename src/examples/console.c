#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

void productor(void);
void consumidor(void);

#define MAX_SEM 5

char *sem_name[MAX_SEM];
sem_t * sem_array[MAX_SEM];

int get_num(int *num)
{
	char buffer[255];
	char * aux;
	do
	{
		aux = fgets(buffer, 255, stdin);	
	}
	while (aux != NULL && sscanf(buffer, "%d\n", num) != 1 );

	if ( aux == NULL )
		return -1;
	else
		return 0;
}

int
get_sem_fd(void)
{
	int i;
	int num;
	do
	{		
		printf("Ingrese el numero de semaforo\n");
		for (i = 0; i< MAX_SEM ; i++)
		{
			if ( sem_array[i] != SEM_FAILED )
				printf("%d - %s\n", i, sem_name[i]);
			else
				printf("%d - EMPTY\n", i);
		}
			
		get_num(&num);
	}while(num < 0 || num > MAX_SEM);
	return num;

}

char *
get_sem_name(void)
{
	return sem_name[get_sem_fd()];
}

sem_t *
get_sem(void)
{
	return sem_array[get_sem_fd()];
}

void
free_space(void)
{
	int fd = get_sem_fd();
	sem_array[fd] = SEM_FAILED;
	free ( sem_name[fd]);
	sem_name[fd] = NULL;
}

void
insert_sem(char * name, sem_t * sem)
{
	int i;
	int len;
	for (i = 0; i< MAX_SEM ; i++)
		if ( sem_array [i] == SEM_FAILED )
			break;

	if ( i == MAX_SEM )
		printf("CONSOLE: No hay mas semaforos disponibles\n");
	else
	{
		sem_array[i] = sem;
		len = strlen(name);
		sem_name[i] = malloc(len + 1);
		strcpy (sem_name[i], name);
	}
}

void 
m_open(void)
{
	int num = -1;
	int flags;
	int value = -1;
	char name[255];
	sem_t * rta;
	
	printf("Ingrese el nombre del semaforo\n");
	if (fgets(name, 255, stdin) == NULL)
		exit(0);
	name[strlen(name) - 1] = 0;

	do
	{		
		printf("Ingrese una opcion\n");
		printf("1 - Sin flags\n");
		printf("2 - O_CREAT\n");
		printf("3 - O_CREAT | O_EXCL\n");
		get_num(&num);
	}while(num < 0 || num > 3);

	switch (num)
	{
		case 1: flags = 0; break;
		case 2: flags = O_CREAT; break;
		case 3: flags = O_CREAT | O_EXCL; break;
	}

	

	if ( flags & O_CREAT )
	{
		do
		{		
			printf("Ingrese el valor del semaforo\n");
			get_num(&value);
		}while(value < 0);

		printf("A ejecutar: sem_open(\"%s\", %d, 0, %d);\n", name, flags, value);

		rta = sem_open(name, flags, 0, value);
	}
	else
	{
		printf("A ejecutar: sem_open(\"%s\", %d);\n", name, flags);
		rta = sem_open(name, 0);
	}

	if ( rta == SEM_FAILED )
		printf("Error al abrir el semaforo, errno = %d (%s)\n", errno, strerror(errno));
	else
		insert_sem(name, rta);

	putchar('\n');
}

void
m_wait(void)
{
	int rta;
	sem_t * sem;
	sem = get_sem();
	printf("A ejecutar: sem_wait(%p);\n", sem);
	rta = sem_wait(sem);
	if ( rta == -1 )
		printf("Error al hacer wait, errno = %d (%s)\n", errno, strerror(errno));
	
	putchar('\n');
}

void
m_trywait(void)
{
	int rta;
	sem_t * sem;
	sem = get_sem();
	printf("A ejecutar: sem_trywait(%p);\n", sem);
	rta = sem_trywait(sem);
	if ( rta == -1 )
		printf("Error al hacer trywait, errno = %d (%s)\n", errno, strerror(errno));
	
	putchar('\n');
}

void
m_getvalue(void)
{
	int rta, value;
	sem_t * sem;
	sem = get_sem();
	printf("A ejecutar: get_value(%p);\n", sem);
	rta = sem_getvalue(sem, &value);
	if ( rta == -1 )
		printf("Error al hacer trywait, errno = %d (%s)\n", errno, strerror(errno));
	else
		printf("Getvalue devolvio %d\n", value);

	putchar('\n');
}

void
m_post(void)
{
	int rta;
	sem_t * sem;
	sem = get_sem();
	printf("A ejecutar: sem_post(%p);\n", sem);
	rta = sem_post(sem);
	if ( rta == -1 )
		printf("Error al hacer post, errno = %d (%s)\n", errno, strerror(errno));
	
	putchar('\n');
}

void
m_close(void)
{
	int rta;
	sem_t * sem;
	sem = get_sem();
	printf("A ejecutar: sem_close(%p);\n", sem);
	rta = sem_close(sem);
	if ( rta == -1 )
		printf("Error al hacer close, errno = %d (%s)\n", errno, strerror(errno));
	
	putchar('\n');
}

void
m_unlink(void)
{
	int rta;
	char * sem;
	sem = get_sem_name();
	printf("A ejecutar: sem_unlink(%s);\n", sem);
	rta = sem_unlink(sem);
	if ( rta == -1 )
		printf("Error al hacer close, errno = %d (%s)\n", errno, strerror(errno));
	
	putchar('\n');
}

void 
m_sem_init(void)
{
	int num = -1;
	int flags;
	int value = -1;
	int rta;
	sem_t *sem;
	sem = malloc(sizeof(sem_t));
	
	do
	{		
		printf("Ingrese el valor del semaforo\n");
		get_num(&value);
	}while(value < 0);

	printf("A ejecutar: sem_init(&sem, 0, %d);\n", value);

	rta = sem_init(sem, 0, value);

	if ( rta == -1 )
		printf("Error al abrir el semaforo, errno = %d (%s)\n", errno, strerror(errno));
	else
		insert_sem("UNNAMED", sem);

	putchar('\n');
}
void
m_destroy(void)
{
	int rta;
	sem_t * sem;
	sem = get_sem();
	printf("A ejecutar: sem_destroy(%p);\n", sem);
	rta = sem_destroy(sem);
	if ( rta == -1 )
		printf("Error al hacer destroy, errno = %d (%s)\n", errno, strerror(errno));
	
	putchar('\n');
}
int
main(int argc, char *argv[])
{
	int opt;
	int hasEnded = 0;
	int i;

	for (i=0; i < MAX_SEM; i++)
		sem_array[i] = SEM_FAILED;

	while ( !hasEnded )
	{
		printf("Escriba la opcion deseada\n1 - OPEN\t2 - WAIT\t3 - POST\t4 - CLOSE\n");
		printf(				  "5 - UNLINK\t6 - GETVALUE\t7 - TRYWAIT\t8 - INIT\n");
		printf(				  "9 - DESTROY\t10 - FREE SPACE\n");
		if ( get_num(&opt) == -1 )
		{
			hasEnded = 1;
			continue;
		}
		
		putchar('\n');
		switch (opt)
		{
			case 1: m_open(); break;
			case 2: m_wait(); break;
			case 3: m_post(); break;
			case 4: m_close(); break;
			case 5: m_unlink(); break;
			case 6: m_getvalue(); break;
			case 7: m_trywait(); break;
			case 8: m_sem_init(); break;
			case 9: m_destroy(); break;
			case 10: free_space(); break;
			default: printf("Opcion invalida\n");
		}
	}
}


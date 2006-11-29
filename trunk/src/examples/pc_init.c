#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define ITEMS 15
#define ITEM_MIN 0
#define ITEM_MAX 100
#define P_BETWEEN 2
#define C_BETWEEN 1


void producer(void);
void consumer(void);
int int_rand(int high, int low);

sem_t s_empty;
sem_t s_full;

int
main(void)
{
	int pid;
	
	/* Inicializo rand */
	srand(time(NULL));
	
	sem_init(&s_empty, 0, ITEMS);
	sem_init(&s_full, 0, 0);

	printf("s_empty = %d\n", s_empty);
	printf("s_full = %d\n", s_full);

	/* Genero los procesos consumidor y productor */
	if ((pid = fork()) == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		/* Proceso hijo - Consumidor */
		consumer();
	else
	{
		/* Proceso padre - Productor */
		producer();
		/* Espero a mi hijo */
		wait(NULL);
		sem_destroy(&s_empty);
		sem_destroy(&s_full);
	}

	return 0;
}


/* Genera números al azar en un rango */
int
int_rand(int high, int low)
{
	return (rand() % (high - low + 1)) + low;
}


/* Función del proceso productor */
void
producer(void)
{
	int item;
	int items = ITEMS;

	/* Ciclo para escritura de cada elemento */
	while (items--)
	{
		/* Genero el item */
		item = int_rand(ITEM_MAX, ITEM_MIN);
		/* Decremento la cantidad de casilleros vacios.
		 * Si lo logro es porque habia espacio. */
		sem_wait(&s_empty);

		printf("PRODUCTOR: Creo item.\n");
		sleep(P_BETWEEN);

		/* Incremento la cantidad de casilleros llenos */
		sem_post(&s_full);
	}
	
	printf("PRODUCTOR: Me muero\n");
}


/* Función de proceso consumidor */
void
consumer(void)
{
	int item;
	int items = ITEMS;
	
	/* Ciclo para lectura de cada elemento */
	while (items--)
	{
		/* Decremento la cantidad de casilleros llenos.
		 * Si lo logro es porque había algo que consumir. */
		sem_wait(&s_full);

		printf("CONSUMIDOR: Consumo item.\n");

		sleep(C_BETWEEN);

		/* Incremento la cantidad de casilleros vacíos */
		sem_post(&s_empty);
	}
	
	printf("CONSUMIDOR: Me muero\n");
}


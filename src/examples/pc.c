#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define ITEMS 10
#define ITEM_MIN 0
#define ITEM_MAX 100
#define T_BETWEEN 2

#define FNAME "pc-test"


void producer(void);
void consumer(void);
int int_rand(int high, int low);

sem_t *s_empty;
sem_t *s_full;
sem_t *mutex;
int fd; /* File descriptor del archivo usado */

int
main(void)
{
	int pid;
	
	/* Inicializo rand */
	srand(time(NULL));

	/* Abro el archivo */
	fd = open(FNAME,
		O_RDWR | O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);


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
	s_empty = sem_open("/empty",  O_CREAT, 0, ITEMS);
	s_full = sem_open("/full",  O_CREAT, 0, 0);
	mutex = sem_open("/mutex",  O_CREAT, 0, 1);

	/* Ciclo para escritura de cada elemento */
	while (items--)
	{
		/* Genero el item */
		item = int_rand(ITEM_MAX, ITEM_MIN);
		/* Decremento la cantidad de casilleros vacios.
		 * Si lo logro es porque habia espacio. */
		sem_wait(s_empty);

		/* Entro a la region critica: escribo archivo */
		sem_wait(mutex);

		/* Coloco el elemento en el archivo */
		write(fd, (void*)&item, sizeof(int));
		sync();
		//sleep(T_BETWEEN);
		printf("PRODUCTOR: Produccion de %d.\n", item);

		/* Salgo de la region critica de escritura */
		sem_post(mutex);

		/* Incremento la cantidad de casilleros llenos */
		sem_post(s_full);
	}
	
	close(fd);
	
	sem_close(s_empty);
	sem_close(s_full);
	sem_close(mutex);
	printf("PRODUCTOR: Me muero\n");
}


/* Función de proceso consumidor */
void
consumer(void)
{
	int item;
	int items = ITEMS;
	
	s_empty = sem_open("/empty",  O_CREAT, 0, ITEMS);
	s_full = sem_open("/full",  O_CREAT, 0, 0);
	mutex = sem_open("/mutex",  O_CREAT, 0, 1);

	/* Ciclo para lectura de cada elemento */
	while (items--)
	{
		/* Decremento la cantidad de casilleros llenos.
		 * Si lo logro es porque había algo que consumir. */
		sem_wait(s_full);

		/* Entro a la región crítica: escribo archivo */
		sem_wait(mutex);

		/* Consumo un elemento desde el archivo */
		int p = read(fd, (void*)&item, sizeof(int));
		printf("read devolvio %d\n", p);
		printf("CONSUMIDOR: Consumicion de %d.\n", item);

		/* Salgo de la región crítica de escritura */
		sem_post(mutex);
		sleep(T_BETWEEN);

		/* Incremento la cantidad de casilleros vacíos */
		sem_post(s_empty);
	}
	
	close(fd);
	
	sem_unlink("/empty");
	sem_unlink("/full");
	sem_unlink("/mutex");
	
	sem_close(s_empty);
	sem_close(s_full);
	sem_close(mutex);
	printf("CONSUMIDOR: Me muero\n");
}


#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_SILLAS 5
#define MAX_BARBEROS 1
#define MAX_PROCESOS 10

void th_barbero(void);
void th_cliente(void);
int int_rand(int high, int low);

sem_t *s_clientes;
sem_t *s_esperando;
sem_t *s_barberos;
sem_t *m_mutex;
int nro_cliente = 0;
int nro_barbero = MAX_BARBEROS;

int
main(void)
{
	int pid;
	int i;
	/* Inicializo rand */
	srand(time(NULL));

	for (i = 0; i<MAX_PROCESOS; i++)
	{
		nro_barbero--;
		nro_cliente++;
		if ((pid = fork()) == -1)
			exit(EXIT_FAILURE);
		else if (pid == 0)
		{
			/* Proceso hijo  */
			if ( nro_barbero >= 0 )
				th_barbero();
			else
			{
				th_cliente();
				exit(1);
			}
		}
	}
	while (1)
	{
		wait(NULL);
/*		printf("Se murio un proceso \n");*/
	}
	return 0;
}


/* Genera números al azar en un rango */
int
int_rand(int high, int low)
{
	return (rand() % (high - low + 1)) + low;
}



void th_cliente(void)
{
	int esperando;
	printf("Cliente!!!\n");
	s_clientes = sem_open("s_clientes", O_CREAT, 0, 0);
	s_esperando = sem_open("s_esperando", O_CREAT, 0, 0);
	s_barberos = sem_open("s_barberos", O_CREAT, 0, 0);
	m_mutex = sem_open("m_mutex", O_CREAT, 0, 1);


	sem_wait(m_mutex); 
	sem_getvalue(s_esperando, &esperando);
	printf("Cliente %d: Entro al mutex. Esperando = %d\n", nro_cliente, esperando);
	if ( esperando < MAX_SILLAS ) {
		sem_post(s_clientes);
		sem_post(s_esperando);
		sem_post(m_mutex);
		printf("Cliente %d: Me siento a esperar a un barbero.\n", nro_cliente);
		sem_wait(s_barberos);
		printf("Cliente %d: Me cortaron el pelo\n", nro_cliente);
	}else{
		sem_post(m_mutex);
		printf("Cliente %d: El local esta lleno, me voy.\n", nro_cliente);
	}
	
	sem_close(s_clientes);
	sem_close(s_esperando);
	sem_close(s_barberos);
	sem_close(m_mutex);
		
}

void
th_barbero(void)
{
	printf("Barbero!!!\n");
	s_clientes = sem_open("s_clientes", O_CREAT, 0, 0);
	s_esperando = sem_open("s_esperando", O_CREAT, 0, 0);
	s_barberos = sem_open("s_barberos", O_CREAT, 0, 0);
	m_mutex = sem_open("m_mutex", O_CREAT, 0, 1);
	
	while(1)
	{
		sem_wait(s_clientes);
		printf("Barbero: por entrar al mutex\n");
		sem_wait(m_mutex);
		printf("Barbero: por entrar al wait esperando\n");
		sem_wait(s_esperando);
		sem_post(s_barberos);
		sem_post(m_mutex);
		printf("Barbero %d, Le corto el pelo a alguien.\n", MAX_BARBEROS - nro_barbero);
	}
}


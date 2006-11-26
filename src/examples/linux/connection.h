/* Archivo: connection.h
 * Descripcion: Esta libreria es un wrapper para enviar y recibir mensajes PTP de manera confiable utilizando TCP o UPTP (para UDP)
 * utiliza el tipo de datos abstracto connectionADT para esconder la capa que ofrece la confiabilidad.
 */
#ifndef CONNECTION_H_
#define CONNECTION_H_
#include <unistd.h>
#include <sys/socket.h>

#define TCP_QLEN 20

#define CONN_ERR_HOST_ENTRY -15
#define CONN_ERR_SERVICE_ENTRY -16
#define CONN_ERR_PROTOCOL_ENTRY -17
#define CONN_NO_ERR 0
#define CONN_ERR_NOMEM -25
#define CONN_ERR_UPTP -18
#define CONN_ERR_TPTP -19
#define CONN_ERR_ACCEPT -20
#define CONN_ERR_SOCKET -21
#define CONN_ERR_INVOCATION -22
#define CONN_WRONG_TRANSPORT -23
#define CONN_ERR_BIND -24
#define CONN_ERR_LISTEN -26

/* Estructura de conexion que contiene la informacion necesaria para esconder la capa de confiabilidad. */
typedef struct connectionCDT * connectionADT;

/* Funcion: connection_connect_to
 * Descripcion: Establece la conexion contra un host que esta escuchando un servicio determinado
 * 				y con uno de los siguientes protocolos de transporte TCP o UDP. Crea un objeto
 * 				conexion y lo devuelve en su nombre. NULL en caso de error.
*/
connectionADT connection_connect_to(const char *host, const char *service, const char *transport );

/* Funcion: connection_pasivesock
 * Descripcion: Crea una estructura conexion y la deja escuchando el servicio "service", transport puede ser 
 * 				"tcp" o "udp". Devuelve en su nombre la estructura de conexion, NULL en caso de 
 * 				error.
*/
connectionADT connection_passivesock(const char *service, const char *transport);

/* Funcion: connection_accept
 * Descripcion: Esta funcion devuelve una nueva estructura conexion basada en otra que este escuchando
 * 				algun servicio. Devuelve en su nombre la nueva estructura, NULL en caso de error.
 * Uso: uso
*/
connectionADT connection_accept(connectionADT handle, struct sockaddr *addr, socklen_t *addrlen);


/* Funcion: connection_destroy
 * Descripcion: Esta funcion libera los recursos creados por connection_create o connection_accept_udp.
 * 				Debe ser llamada una vez que se termine la conexion o si ocurre algun error en 
 * 				connection_accept_udp
 * 
 * Uso: connection_destroy(conexion);
*/
void connection_destroy(connectionADT *handle);

/* Funcion: connection_recv
 * Descripcion: Funcion utilizada para recibir un mensaje PTP de manera confiable. Handle debe haber sido
 * 				creado con la funcion connection_create. Devuelve en su nombre la cantidad de bytes recibidos
 * 				o -1 en caso de error. En cuyo caso se debera leer la variable errno con el numero de error.
 * 				En buf debe haber un buffer de tamano count
*/
ssize_t connection_recv( connectionADT handle, void *buf, size_t count );

/* Funcion: connection_send
 * Descripcion: Funcion utilizada para enviar un mensaje PTP de manera confiable. El mensaje debe estar en buf
 * 				y tener tamano count. Handle debe haber sido creado con la funcion connection_create. Devuelve
 * 				en su nombre la cantidad de bytes enviados o -1 en caso de error. En cuyo caso se debera leer 
 * 				la variable errno con el numero de error.
*/
ssize_t connection_send( connectionADT handle, void *buf, size_t count );

/* Funcion: connection_get_socket
 * Descripcion: Funcion que devuelve el sokcet utilizado por una connexion
*/
int connection_get_socket( connectionADT handle);


/* Funcion que devuelve un string con el mensaje de error. */
char * connection_strerror(int errnmb);

/* Funcion que devuelve un string con un detalle del error. */
char * connection_strdetail(int errnmb);

/* Variable similar a errno */
extern int connection_errno;

#endif /*CONEXION_H_*/

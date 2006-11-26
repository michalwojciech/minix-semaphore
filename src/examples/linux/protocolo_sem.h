#ifndef __PROT_SEM
#define __PROT_SEM

#define OPEN 1
#define POST 2
#define WAIT 3
#define DISCONNECT 4
#define UNLINK 5
#define CLOSE 6
#define TRYWAIT 7
#define INIT 8
#define DESTROY 9
#define GETVALUE 10

#define MAX_MSG 255

void clear_buffer(void);
char * get_buffer(void);
int get_buffer_size(void);

void put_long(long n);
long get_long(void);

void put_char(int c);
int get_char(void);

void put_string(char * str, int size);
int get_string(char * str, int size);

#endif

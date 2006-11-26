/* Las siguientes macros esconden el nombre de los campos del mensaje 
 * Tratamos de seguir el estilo que fija Tanenbaum en Minix
 */

#define SEM_OPEN	1
#define SEM_CLOSE	2
#define SEM_INIT	3
#define SEM_WAIT	4
#define SEM_POST	5
#define SEM_ULINK	6
#define SEM_GETVAL	7
#define SEM_TRYWAIT	8
#define SEM_DESTROY	9
#define SEM_TIMEDWAIT	10

#define mensaje_funcion       m.m2_i1	/*campo 1*/
#define mensaje_semaforo      m.m2_i2   /*campo 2*/
#define mensaje_nombre	      m.m2_p1	/*campo 6*/
#define mensaje_oflag	      m.m2_l2	/*campo 5*/
#define mensaje_permisos      m.m2_i2	/*campo 2*/
#define mensaje_valInicial    m.m2_l1	/*campo 4*/
#define mensaje_valActual     m.m2_i3	/*campo 3*/
#define mensaje_cantArg	      m.m2_i3	/*campo 3*/
#define mensaje_pshared	      m.m2_i3	/*campo 3*/	

/* auxiliares, tal vez no se usen */
#define mensaje2_funcion      m.m2_i1
#define mensaje3_funcion      m.m3_i1
#define mensaje4_funcion      m.m4_l1
#define mensaje5_funcion      m.m5_i1
#define mensaje6_funcion      m.m6_i1


/*#define buffer	      m.m1_p1
#define child	      m.m1_i2
#define co_mode	      m.m1_i1
#define eff_grp_id    m.m1_i3
#define eff_user_id   m.m1_i3
#define erki          m.m1_p1
#define fd	      m.m1_i1
#define fd2	      m.m1_i2
#define ioflags       m.m1_i3
#define group	      m.m1_i3
#define real_grp_id   m.m1_i2
#define ls_fd	      m.m2_i1
#define mk_mode	      m.m1_i2
#define mode	      m.m3_i2
#define c_mode        m.m1_i3
#define c_name        m.m1_p1
#define name	      m.m3_p1
#define name1	      m.m1_p1
#define name2	      m.m1_p2
#define	name_length   m.m3_i1
#define name1_length  m.m1_i1
#define name2_length  m.m1_i2
#define nbytes        m.m1_i2
#define offset	      m.m2_l1
#define owner	      m.m1_i2
#define parent	      m.m1_i1
#define pathname      m.m3_ca1
#define pid	      m.m1_i3
#define pro	      m.m1_i1
#define rd_only	      m.m1_i3
#define real_user_id  m.m1_i2
#define request       m.m1_i2
#define sig	      m.m1_i2
#define slot1	      m.m1_i1
#define tp	      m.m2_l1
#define utime_actime  m.m2_l1
#define utime_modtime m.m2_l2
#define utime_file    m.m2_p1
#define utime_length  m.m2_i1
#define whence	      m.m2_i2
*/
/* The following names are synonyms for the variables in the output message. */
#define reply_type     m1.m_type	/* si es error u OK */
#define reply_rta      m1.m1_i1		/* es el valor que retorna la función en su nombre*/
#define reply_param1   m1.m1_i2		/* es el valor que retorna la función en el parámetro 1 si era un puntero */
#define reply_param2   m1.m1_i3		/* es el valor que retorna la función en el parámetro 2 si era un puntero */




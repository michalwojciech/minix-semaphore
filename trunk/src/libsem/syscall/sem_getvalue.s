.sect .text
.extern	__sem_getvalue
.define	_sem_getvalue
.align 2

_sem_getvalue:
	jmp	__sem_getvalue

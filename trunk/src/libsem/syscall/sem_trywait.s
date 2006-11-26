.sect .text
.extern	__sem_trywait
.define	_sem_trywait
.align 2

_sem_trywait:
	jmp	__sem_trywait

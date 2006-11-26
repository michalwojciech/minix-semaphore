.sect .text
.extern	__sem_wait
.define	_sem_wait
.align 2

_sem_wait:
	jmp	__sem_wait

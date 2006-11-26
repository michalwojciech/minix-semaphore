.sect .text
.extern	__sem_destroy
.define	_sem_destroy
.align 2

_sem_destroy:
	jmp	__sem_destroy

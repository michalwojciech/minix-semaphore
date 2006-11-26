.sect .text
.extern	__sem_post
.define	_sem_post
.align 2

_sem_post:
	jmp	__sem_post

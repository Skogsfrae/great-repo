#include "pcb.e"
#include "const.h"

typedef struct semd_t {
        struct semd_t *s_next; /* next element on the ASL */
        int *s_semAdd;         /* pointer to the semaphore */
        pcb_t *s_procQ;        /*tail pointer to a process queue */
} semd_t;

HIDDEN semd_t *semdFree, *semdFree_h, *semd_h;
static semd_t semdTable[MAXPROC+1];

void initASL()
{
	int i;

// Initialize the dummy semd
	semd_h = &semdTable[0];
	semd_h->s_next = NULL;

	for(i=1; i<MAXPROC+1; i++)
	{
		semdFree = &semdTable[i];

		if(i == 1)
			semdFree_h = semdFree;

		if(i == MAXPROC)
			semdFree->s_next = NULL;

		else semdFree->s_next = &semdTable[i+1];
	}
}


// This function searches for the desired semaphore in the ASL and returns the
// previous descriptor
semd_t *look4sema4(int *semAdd)
{
	semd_t *semdTmp = semd_h;

	while((semdTmp->s_next)->s_semAdd != semAdd && semdTmp->s_next != NULL)
		semdTmp = semdTmp->s_next;

	return semdTmp;
}

int insertBlocked(int *semAdd, pcb_t *p)
{
	semd_t *semdTmp, *semdList = semd_h;

// If the semaphore is found, insert the process block to its queue
	if((semdTmp = look4sema4(semAdd))->s_next != NULL)
	{
		insertProcQ(&((semdTmp->s_next)->s_procQ), p);
		p->p_semAdd = semAdd;
		return FALSE;
	}

// Otherwise, if the semdFree is not empty, create a new semaphore descriptor
// and insert the process block to its queue
	else
	{
		if(semdFree_h == NULL)
			return TRUE;

		semdTmp = semdFree_h;
		semdFree_h = semdFree_h->s_next;

		while((semdList->s_next) != NULL)
		{
			if(semAdd < (semdList->s_next)->s_semAdd)
				break;
			semdList = semdList->s_next;

		}

		semdTmp->s_semAdd = semAdd;
		semdTmp->s_next = semdList->s_next;
		semdList->s_next = semdTmp;

		semdTmp->s_procQ = mkEmptyProcQ();
		insertProcQ(&(semdTmp->s_procQ), p);
		p->p_semAdd = semAdd;

		return FALSE;
	}
}

pcb_t *removeBlocked(int *semAdd)
{
	semd_t *semdTmp, *semRet;
	pcb_t *p;

// Search for the requested semaphore, if none is found return NULL
	if((semdTmp = look4sema4(semAdd))->s_next == NULL)
		return NULL;

// Remove the head element of the semaphore process queue and return it
	p = removeProcQ(&((semdTmp->s_next)->s_procQ));

	if(emptyProcQ((semdTmp->s_next)->s_procQ))
	{
		semRet = semdTmp->s_next;
		semdTmp->s_next = (semdTmp->s_next)->s_next;

		semRet->s_next = semdFree_h;
		semdFree_h = semRet;
	}

	return p;
}

pcb_t *outBlocked(pcb_t *p)
{
	semd_t *semdTmp;

	semdTmp = (look4sema4(p->p_semAdd))->s_next;

	if((p = outProcQ(&(semdTmp->s_procQ), p)) == NULL)
		return NULL;

	return p;
}

pcb_t *headBlocked(int *semAdd)
{
	semd_t *semdTmp;

	semdTmp = (look4sema4(semAdd))->s_next;

	if(semdTmp == NULL || semdTmp->s_procQ == NULL)
		return NULL;

	return (semdTmp->s_procQ)->p_next;
}


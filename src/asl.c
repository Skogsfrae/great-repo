#include "../include/pcb.e"
#include "../include/const.h"

typedef struct semd_t {
        struct semd_t *s_next; /* next element on the ASL */
        int *s_semAdd;         /* pointer to the semaphore */
        pcb_t *s_procQ;        /*tail pointer to a process queue */
} semd_t;

HIDDEN semd_t *semdFree_h, *semd_h;

void initASL()
{
	semd_t *semdFree;
	static semd_t semdTable[MAXPROC+1];
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

// Remove semdTmp from ASL
void removeSema4(semd_t *semdTmp)
{
	semd_t *semRet;

	semRet = semdTmp->s_next;
        semdTmp->s_next = (semdTmp->s_next)->s_next;

        semRet->s_next = semdFree_h;
        semdFree_h = semRet;
}

semd_t *findPosition(int *semAdd)
{
	semd_t *semdTmp = semd_h;

	while((semdTmp->s_next)->s_semAdd != semAdd && semdTmp->s_next != NULL)
        {
                if(semAdd > (semdTmp->s_next)->s_semAdd)
                        break;

                semdTmp = semdTmp->s_next;
        }

	return semdTmp;
}

// This function searches for the desired semaphore in the ASL and returns the
// previous descriptor
semd_t *look4sema4(int *semAdd)
{
	semd_t *semdTmp = findPosition(semAdd);

	if( (semdTmp->s_next)->s_semAdd == semAdd)
		return semdTmp;

	else
		return NULL;
}

int insertBlocked(int *semAdd, pcb_t *p)
{
	semd_t *semdTmp, *semdList;

// If the semaphore is found, insert the process block to its queue
	if((semdTmp = look4sema4(semAdd))->s_next != NULL)
	{
		insertProcQ(&((semdTmp->s_next)->s_procQ), p);
		p->p_semAdd = semAdd;
		return FALSE;
	}

// Otherwise, unless semdFree is empty, create a new semaphore descriptor
// and insert the process block to its queue
	else
	{
		if(semdFree_h == NULL)
			return TRUE;

		else
		{
			semdTmp = semdFree_h;
			semdFree_h = semdFree_h->s_next;

			semdList = findPosition(semAdd);

			semdTmp->s_semAdd = semAdd;
			semdTmp->s_next = semdList->s_next;
			semdList->s_next = semdTmp;

			semdTmp->s_procQ = mkEmptyProcQ();
			insertProcQ(&(semdTmp->s_procQ), p);
			p->p_semAdd = semAdd;

			return FALSE;
		}
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

// If the procQ becomes empty, remove semdTmp from ASL
	if(emptyProcQ((semdTmp->s_next)->s_procQ))
		removeSema4(semdTmp);

	return p;
}

pcb_t *outBlocked(pcb_t *p)
{
	semd_t *semdTmp;

	semdTmp = look4sema4(p->p_semAdd);

	if((p = outProcQ(&((semdTmp->s_next)->s_procQ), p)) == NULL)
		return NULL;

	if( emptyProcQ((semdTmp->s_next)->s_procQ) )
		removeSema4(semdTmp);

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


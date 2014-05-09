#include "../include/const.h"
#include "../include/pcb.h"

HIDDEN pcb_t *pcbFree_h;

// Pcb_t struct was made bidirectional in orther to easily remove or add a new
// item to 
void initPcbs()
{
	pcb_t *pcbFree;
	static pcb_t procp[MAXPROC];
	int i;

	for(i=0; i<MAXPROC; i++)
	{
		pcbFree = &procp[i];

		if(i == 0)
		{
			pcbFree_h = pcbFree;
			pcbFree->p_prev = NULL;
		}
   
		else 
			pcbFree->p_prev = &procp[i-1];


		if(i == MAXPROC-1)
		{
			pcbFree->p_next = NULL;
		}

		else pcbFree->p_next = &procp[i+1];
	}
}

// As requested in Kaya documentation
pcb_t *allocPcb()
{
	pcb_t *pcbTmp;

	if(pcbFree_h == NULL)
		return NULL;

	pcbTmp = pcbFree_h;

	pcbFree_h = pcbFree_h->p_next;
	pcbFree_h->p_prev = NULL;

	pcbTmp->p_prnt = NULL;
	pcbTmp->p_child = NULL;
	pcbTmp->p_sib = NULL;
	pcbTmp->p_next = NULL;
	pcbFree_h->p_prev = NULL;
	pcbTmp->p_s = 0;
	pcbTmp->p_semAdd = NULL;

	return pcbTmp;
}

// As requested in Kaya documentation
void freePcb(pcb_t *p)
{
	p->p_next = pcbFree_h;
	p->p_prev = NULL;
	pcbFree_h = p;
}

pcb_t *mkEmptyProcQ()
{
	pcb_t *pcbTmp = NULL;
	return pcbTmp;
}

int emptyProcQ(pcb_t *tp)
{
	if(tp == NULL)
		return TRUE;

	else return FALSE;
}

void insertProcQ(pcb_t **tp, pcb_t *p)
{

// If the queue is empty, insert the entry and link it to itself
	if((*tp) == NULL)
	{
		(*tp) = p;
		p->p_next = p;
		p->p_prev = p;
	}

// Otherwise, insert the entry at the tail of the queue
	else
	{
		((*tp)->p_next)->p_prev = p;
		p->p_next = (*tp)->p_next;
		p->p_prev = (*tp);
		(*tp)->p_next = p;

		(*tp) = p;
	}
}

pcb_t *removeProcQ(pcb_t **tp)
{
	pcb_t *pcbTmp;

// If the queue is empty, hence the requested entry is not there, return NULL
        if(emptyProcQ(*tp))
		return NULL;

// If the requested entry is the only one of the queue, remove it and make *tp
// point to NULL, then return the removed entry
	if((*tp)->p_next == (*tp))
	{
		pcbTmp = (*tp);
		(*tp) = NULL;
	}

// Simply remove the requested entry and return it
	else
	{
		pcbTmp = (*tp)->p_next;

		(((*tp)->p_next)->p_next)->p_prev = (*tp);
		(*tp)->p_next = ((*tp)->p_next)->p_next;
	}

	return pcbTmp;
}

pcb_t *outProcQ(pcb_t **tp, pcb_t *p)
{
        pcb_t *pcbTmp = (*tp);
	_Bool found = FALSE;

// If the queue is empty, hence the requested entry is not there, return NULL
	if(emptyProcQ(*tp))
		return NULL;

// If the requested entry is the tail remove it and update *tp properly
        if (p == (*tp))
	{
		if((*tp)->p_next == (*tp))
		{
			*tp = NULL;
			p->p_prev = p->p_next = NULL;			
		}

		else
		{
			(*tp) = p->p_prev;
			(*tp)->p_next = p->p_next;
			((*tp)->p_next)->p_prev = (*tp);
			p->p_prev = p->p_next = NULL;
               	}

		found = TRUE;
	}

// If none of the above search for the requested entry and remove it
        else
        {
                pcbTmp = pcbTmp->p_next;

                while (pcbTmp != (*tp))
                {
                        if (p == pcbTmp)
			{
				pcbTmp = p->p_prev;
				pcbTmp->p_next = p->p_next;
				(pcbTmp->p_next)->p_prev = pcbTmp;
				p->p_prev = p->p_next = NULL;
				break;
			}

                        else pcbTmp = pcbTmp->p_next;
                }

		found = TRUE;
        }

	if(found)
		return p;

	else return NULL;
}

pcb_t *headProcQ(pcb_t *tp)
{
	if (tp == NULL)
		return NULL;


	else return tp->p_next;
}

int emptyChild(pcb_t *p)
{
	if (p->p_child == NULL)
		return TRUE;

	return FALSE;
}

void insertChild(pcb_t *prnt, pcb_t *p)
{
	p->p_prnt = prnt;
	p->p_sib = prnt->p_child;
	prnt->p_child = p;
}

pcb_t *removeChild(pcb_t *p)
{
	pcb_t *pcbTmp;

	if(p->p_child == NULL)
		return NULL;

	pcbTmp = p->p_child;
	p->p_child = (p->p_child)->p_sib;
	pcbTmp->p_sib = pcbTmp->p_prnt = NULL;

	return pcbTmp;
}

pcb_t *outChild(pcb_t *p)
{
	pcb_t *pcbTmp;

	if(p->p_prnt == NULL)
		return NULL;

// If p is the first child, then simply remove it
	if((p->p_prnt)->p_child == p)
		removeChild(p->p_prnt);

// Otherwise search for it and remove it
	else
	{
		pcbTmp = (p->p_prnt)->p_child;

		while (pcbTmp->p_sib != p)
			pcbTmp = pcbTmp->p_sib;

		pcbTmp->p_sib = (pcbTmp->p_sib)->p_sib;
		p->p_prnt = p->p_sib = NULL;
	}

	return p;
}

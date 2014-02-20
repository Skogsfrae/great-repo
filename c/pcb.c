#include "../h/const.h"
#include "../h/pcb.h"

pcb_t *pcbFree, *pcbFree_h, *pcbFree_t;
static pcb_t procp[MAXPROC];

void initPcbs()
{
	int i;

	for(i=0; i<MAXPROC; i++)
	{
		pcbFree = &procp[i];

		if(i == 0)
		{
			pcbFree_h = pcbFree;
			pcbFree->p_prev = NULL;
		}   
		else pcbFree->p_prev = &procp[i-1];

		if(i == MAXPROC-1)
		{
			pcbFree_t = pcbFree;
			pcbFree->p_next = NULL;
		}
		else pcbFree->p_next = &procp[i+1];

		pcbFree = pcbFree->p_next;
	}
}

pcb_t *allocPcb()
{
	pcb_t *pcbTmp;

	if(pcbFree_h == NULL)
		return NULL;

	pcbTmp = pcbFree_h;

	pcbFree_h = pcbFree_h->p_next;
	pcbFree_h->p_prev = NULL;

	pcbTmp->p_prnt = pcbTmp->p_child = pcbTmp->p_sib = pcbTmp->p_next = pcbFree->p_prev = NULL;
	pcbTmp->p_s = 0;
	pcbTmp->p_semAdd = NULL;

	return pcbTmp;
}

void freePcb(pcb_t *p)
{
	if(pcbFree_h == NULL)
	{
		pcbFree_h = pcbFree_t = p;
		pcbFree_h->p_next = pcbFree_t->p_prev = NULL;
	}

	else
	{
		p->p_next = pcbFree_h;
		p->p_prev = NULL;
		pcbFree_h = p;
	}
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
	if((*tp) == NULL)
	{
		(*tp) = p;
		p->p_next = p;
		p->p_prev = p;
	}

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

	if( (*tp) == NULL)
		return NULL;

	if((*tp)->p_next == (*tp))
	{
		pcbTmp = (*tp);
		(*tp) = NULL;
		return pcbTmp;
	}

	pcbTmp = (*tp)->p_next;

	(((*tp)->p_next)->p_next)->p_prev = (*tp);
	(*tp)->p_next = ((*tp)->p_next)->p_next;

	return pcbTmp;
}

pcb_t *outProcQ(pcb_t **tp, pcb_t *p)
{
        pcb_t *pcbTmp = (*tp);

        if (p == (*tp))
	{
		(*tp) = p->p_prev;
		(*tp)->p_next = p->p_next;
                return p;
	}

        else
        {
                pcbTmp = pcbTmp->p_next;
                while (pcbTmp != (*tp))
                {
                        if (p == pcbTmp)
			{
				pcbTmp = p->p_prev;
				pcbTmp->p_next = p->p_next;
                                return p;
			}

                        else pcbTmp = pcbTmp->p_next;
                }

        }

        return NULL;
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

	if((p->p_prnt)->p_child == p)
		return removeChild(p->p_prnt);

	else
	{
		pcbTmp = (p->p_prnt)->p_child;

		while (pcbTmp->p_sib != p)
			pcbTmp = pcbTmp->p_sib;

		pcbTmp->p_sib = (pcbTmp->p_sib)->p_sib;
		p->p_prnt = p->p_sib = NULL;
		return p;
	}
}

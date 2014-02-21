#ifndef PCB_E
#define PCB_E

#include "pcb.h"

extern pcb_t *mkEmptyProcQ(void);
extern int emptyProcQ(pcb_t *tp);
extern void insertProcQ(pcb_t **tp, pcb_t *p);
extern pcb_t *removeProcQ(pcb_t **tp);
extern pcb_t *outProcQ(pcb_t **tp, pcb_t *p);
extern pcb_t *headProcQ(pcb_t *tp);

#endif

#include <stdio.h>
#include <assert.h>

#include "rob.h"


extern int32_t NUM_ROB_ENTRIES;
int flag;
/////////////////////////////////////////////////////////////
// Init function initializes the ROB
/////////////////////////////////////////////////////////////

ROB* ROB_init(void){
  int ii;
  ROB *t = (ROB *) calloc (1, sizeof (ROB));
  for(ii=0; ii<MAX_ROB_ENTRIES; ii++){
    t->ROB_Entries[ii].valid=false;
    t->ROB_Entries[ii].ready=false;
    t->ROB_Entries[ii].exec=false;
  }
  t->head_ptr=0;
  t->tail_ptr=0;
  return t;
}

/////////////////////////////////////////////////////////////
// Print State
/////////////////////////////////////////////////////////////
void ROB_print_state(ROB *t){
 int ii = 0;
  printf("Printing ROB \n");
  printf("Entry  Inst   Valid   ready\n");
  for(ii = 0; ii < 256; ii++) {
    printf("%5d ::  %d\t", ii, (int)t->ROB_Entries[ii].inst.inst_num);
    printf(" %5d\t", t->ROB_Entries[ii].valid);
    printf(" %5d\n", t->ROB_Entries[ii].ready);
    printf(" %5d\n", t->ROB_Entries[ii].exec);
  }
  printf("\n");
}

/////////////////////////////////////////////////////////////
// If there is space in ROB return true, else false
/////////////////////////////////////////////////////////////

bool ROB_check_space(ROB *t){
    if (t->ROB_Entries[t->tail_ptr].valid == 0)
    return true;
    else
    return false;
}

/////////////////////////////////////////////////////////////
// insert entry at tail, increment tail (do check_space first)
/////////////////////////////////////////////////////////////

int ROB_insert(ROB *t, Inst_Info inst){
    int tail =t->tail_ptr;
     t->ROB_Entries[t->tail_ptr].inst= inst;
     t->ROB_Entries[t->tail_ptr].valid=1;
     t->tail_ptr++;
     if(t->tail_ptr==NUM_ROB_ENTRIES)
     t->tail_ptr =0;
     return tail;
}

/////////////////////////////////////////////////////////////
// When an inst gets scheduled for execution, mark exec
/////////////////////////////////////////////////////////////

void ROB_mark_exec(ROB *t, Inst_Info inst){
    t->ROB_Entries[inst.dr_tag].exec = 1;
}


/////////////////////////////////////////////////////////////
// Once an instruction finishes execution, mark rob entry as done
/////////////////////////////////////////////////////////////

void ROB_mark_ready(ROB *t, Inst_Info inst){
 t->ROB_Entries[inst.dr_tag].ready = 1;
}

/////////////////////////////////////////////////////////////
// Find whether the prf (rob entry) is ready
/////////////////////////////////////////////////////////////

bool ROB_check_ready(ROB *t, int tag){
if (t->ROB_Entries[tag].ready ==1)
   return 1;
else
    return 0;
}


/////////////////////////////////////////////////////////////
// Check if the oldest ROB entry is ready for commit
/////////////////////////////////////////////////////////////

bool ROB_check_head(ROB *t){
if (t->ROB_Entries[t->head_ptr].ready ==1)
   return 1;
else
   return 0;
}

/////////////////////////////////////////////////////////////
// For writeback of freshly ready tags, wakeup waiting inst
/////////////////////////////////////////////////////////////

void  ROB_wakeup(ROB *t, int tag){
   int ii;
   for(ii=0;ii<NUM_ROB_ENTRIES;ii++)
   {
     if (t->ROB_Entries[ii].inst.src1_reg !=-1)
     {
       if(tag==t->ROB_Entries[ii].inst.src1_tag)
       {
         t->ROB_Entries[ii].inst.src1_ready=1;}
       }
       if (t->ROB_Entries[ii].inst.src2_reg !=-1)
       {
         if(tag==t->ROB_Entries[ii].inst.src2_tag)
         {
           t->ROB_Entries[ii].inst.src2_ready=1;
         }
       }
     }
   }
/////////////////////////////////////////////////////////////
// Remove oldest entry from ROB (after ROB_check_head)
/////////////////////////////////////////////////////////////

Inst_Info ROB_remove_head(ROB *t){
  if (ROB_check_head(t))
  {
    int head=t->head_ptr;
    t->head_ptr++;
    if (t->head_ptr==NUM_ROB_ENTRIES)
    {t->head_ptr=0;}
    t->ROB_Entries[head].valid=0; //make use bit of the commited instruction 0
    t->ROB_Entries[head].ready=0;
    t->ROB_Entries[head].exec=0;

    return t->ROB_Entries[head].inst;
  }
  else return t->ROB_Entries[t->head_ptr].inst;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

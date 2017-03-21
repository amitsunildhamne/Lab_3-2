ROB *p->pipe_ROB = p->pipe_ROB;

 for (int ii = 0; ii < PIPE_WIDTH; ii++){
   Inst_Info curr_inst = p->ID_latch[ii].inst;

   // TODO: Find space in ROB and transfer instruction (valid = 1, exec = 0, ready = 0)
   bool is_space = ROB_check_space(p->pipe_ROB);
   if (is_space && p->ID_latch[ii].valid)
   {
      int prf    = ROB_insert(p->pipe_ROB,curr_inst);

       // TODO: If src1/src2 is not remapped, set src1ready/src2ready
       // p->pipe_ROB->ROB_Entries[prf].inst.src1_reg;
      int src1 = p->pipe_ROB->ROB_Entries[prf].inst.src1_reg;
      int src2 = p->pipe_ROB->ROB_Entries[prf].inst.src2_reg;

      int m1 = -1;
      int m2 = -1;

      if(p->pipe_ROB->ROB_Entries[prf].inst.src1_reg!= -1)
      {
        m1= RAT_get_remap(p->pipe_RAT,src1);

        if (m1 == -1)
        {
          p->pipe_ROB->ROB_Entries[prf].inst.src1_ready = true;
        }
        else
        {
          p->pipe_ROB->ROB_Entries[prf].inst.src1_tag = m1;
          p->pipe_ROB->ROB_Entries[prf].inst.src1_ready = (p->pipe_ROB->ROB_Entries[m1].ready) ? true : p->pipe_ROB->ROB_Entries[prf].inst.src1_ready;
        }
      }
      else { // The ready should be high even when the src reg is not needed
          p->pipe_ROB->ROB_Entries[prf].inst.src1_ready = true;
      }

      if(src2 != -1){
        m2 = RAT_get_remap(p->pipe_RAT,src2);

        if (m2 == -1){
          p->pipe_ROB->ROB_Entries[prf].inst.src2_ready = true;
        } else{
          p->pipe_ROB->ROB_Entries[prf].inst.src2_tag = m2;
          p->pipe_ROB->ROB_Entries[prf].inst.src2_ready = (p->pipe_ROB->ROB_Entries[m2].ready) ? true : p->pipe_ROB->ROB_Entries[prf].inst.src2_ready;
        }
      } else { //The ready should be high even if the src reg is not needed
          p->pipe_ROB->ROB_Entries[prf].inst.src2_ready = true;
      }
       // TODO: If src1/src is remapped, set src1tag/src2tag from RAT. Set src1ready/src2ready based on ready bit from ROB entries.
       // TODO: Set dr_tag

      int dest_reg = p->pipe_ROB->ROB_Entries[prf].inst.dest_reg;
      if(dest_reg != -1) RAT_set_remap(p->pipe_RAT,dest_reg,prf);
      p->pipe_ROB->ROB_Entries[prf].inst.dr_tag = prf;

      //Now since we have inserted the value in the ROB the ID latch could be
      //written. Set the valid false
      p->ID_latch[ii].valid = false;
   }
 }

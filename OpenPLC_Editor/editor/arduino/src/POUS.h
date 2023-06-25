#ifndef __POUS_H
#define __POUS_H

#include "accessor.h"
#include "iec_std_lib.h"

// PROGRAM BLINK
// Data part
typedef struct {
  // PROGRAM Interface - IN, OUT, IN_OUT variables

  // PROGRAM private variables - TEMP, private and located variables
  __DECLARE_VAR(BOOL,BLINK_LED)
  TON TON0;
  TOF TOF0;
  __DECLARE_LOCATED(INT,IN01)
  __DECLARE_LOCATED(INT,IN02)
  __DECLARE_LOCATED(INT,OUT01)
  __DECLARE_LOCATED(BOOL,TEST1)
  CTU CTU0;
  R_TRIG R_TRIG1;
  R_TRIG R_TRIG2;
  R_TRIG R_TRIG3;
  __DECLARE_VAR(BOOL,_TMP_ADD15_ENO)
  __DECLARE_VAR(INT,_TMP_ADD15_OUT)

} BLINK;

void BLINK_init__(BLINK *data__, BOOL retain);
// Code part
void BLINK_body__(BLINK *data__);
#endif //__POUS_H

#include "POUS.h"

static inline INT __BLINK_ADD__INT__INT1(BOOL EN,
  UINT __PARAM_COUNT,
  INT IN1,
  INT IN2,
  BLINK *data__)
{
  INT __res;
  BOOL __TMP_ENO = __GET_VAR(data__->_TMP_ADD15_ENO,);
  __res = ADD__INT__INT(EN,
    &__TMP_ENO,
    __PARAM_COUNT,
    IN1,
    IN2);
  __SET_VAR(,data__->_TMP_ADD15_ENO,,__TMP_ENO);
  return __res;
}

void BLINK_init__(BLINK *data__, BOOL retain) {
  __INIT_VAR(data__->BLINK_LED,__BOOL_LITERAL(FALSE),retain)
  TON_init__(&data__->TON0,retain);
  TOF_init__(&data__->TOF0,retain);
  __INIT_LOCATED(INT,__QW99,data__->IN01,1)
  __INIT_LOCATED_VALUE(data__->IN01,200)
  __INIT_LOCATED(INT,__QW98,data__->IN02,1)
  __INIT_LOCATED_VALUE(data__->IN02,200)
  __INIT_LOCATED(INT,__QW100,data__->OUT01,1)
  __INIT_LOCATED_VALUE(data__->OUT01,200)
  __INIT_LOCATED(BOOL,__QX0_0,data__->TEST1,retain)
  __INIT_LOCATED_VALUE(data__->TEST1,1)
  CTU_init__(&data__->CTU0,retain);
  R_TRIG_init__(&data__->R_TRIG1,retain);
  R_TRIG_init__(&data__->R_TRIG2,retain);
  R_TRIG_init__(&data__->R_TRIG3,retain);
  __INIT_VAR(data__->_TMP_ADD15_ENO,__BOOL_LITERAL(FALSE),retain)
  __INIT_VAR(data__->_TMP_ADD15_OUT,0,retain)
}

// Code part
void BLINK_body__(BLINK *data__) {
  // Initialise TEMP variables

  __SET_VAR(data__->TON0.,EN,,__BOOL_LITERAL(TRUE));
  __SET_VAR(data__->TON0.,IN,,!(__GET_VAR(data__->BLINK_LED,)));
  __SET_VAR(data__->TON0.,PT,,__time_to_timespec(1, 1000, 0, 0, 0, 0));
  TON_body__(&data__->TON0);
  __SET_VAR(data__->TOF0.,EN,,__GET_VAR(data__->TON0.ENO,));
  __SET_VAR(data__->TOF0.,IN,,__GET_VAR(data__->TON0.Q,));
  __SET_VAR(data__->TOF0.,PT,,__time_to_timespec(1, 500, 0, 0, 0, 0));
  TOF_body__(&data__->TOF0);
  __SET_VAR(data__->,BLINK_LED,,__GET_VAR(data__->TOF0.Q,));
  __SET_VAR(data__->R_TRIG1.,CLK,,__GET_VAR(data__->BLINK_LED,));
  R_TRIG_body__(&data__->R_TRIG1);
  __SET_VAR(data__->R_TRIG2.,CLK,,__GET_VAR(data__->R_TRIG1.Q,));
  R_TRIG_body__(&data__->R_TRIG2);
  __SET_VAR(data__->CTU0.,EN,,__BOOL_LITERAL(TRUE));
  __SET_VAR(data__->CTU0.,CU,,__GET_VAR(data__->R_TRIG2.Q,));
  __SET_VAR(data__->CTU0.,PV,,150);
  CTU_body__(&data__->CTU0);
  __SET_LOCATED(data__->,TEST1,,__GET_VAR(data__->CTU0.Q,));
  if (__GET_VAR(data__->CTU0.ENO,)) {
    __SET_LOCATED(data__->,IN01,,__GET_VAR(data__->CTU0.CV,));
  };
  __SET_VAR(data__->R_TRIG3.,CLK,,__GET_VAR(data__->BLINK_LED,));
  R_TRIG_body__(&data__->R_TRIG3);
  __SET_VAR(data__->,_TMP_ADD15_OUT,,__BLINK_ADD__INT__INT1(
    (BOOL)__GET_VAR(data__->R_TRIG3.Q,),
    (UINT)2,
    (INT)__GET_LOCATED(data__->IN02,),
    (INT)1,
    data__));
  if (__GET_VAR(data__->_TMP_ADD15_ENO,)) {
    __SET_LOCATED(data__->,IN02,,__GET_VAR(data__->_TMP_ADD15_OUT,));
  };

  goto __end;

__end:
  return;
} // BLINK_body__() 






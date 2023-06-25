PROGRAM Blink
  VAR
    blink_led : BOOL;
    TON0 : TON;
    TOF0 : TOF;
  END_VAR
  VAR RETAIN
    In01 AT %QW99 : INT := 200;
    In02 AT %QW98 : INT := 200;
    Out01 AT %QW100 : INT := 200;
  END_VAR
  VAR
    Test1 AT %QX0.0 : BOOL := 1;
  END_VAR
  VAR
    CTU0 : CTU;
    R_TRIG1 : R_TRIG;
    R_TRIG2 : R_TRIG;
    R_TRIG3 : R_TRIG;
    _TMP_ADD15_ENO : BOOL;
    _TMP_ADD15_OUT : INT;
  END_VAR

  TON0(EN := TRUE, IN := NOT(blink_led), PT := T#1000ms);
  TOF0(EN := TON0.ENO, IN := TON0.Q, PT := T#500ms);
  blink_led := TOF0.Q;
  R_TRIG1(CLK := blink_led);
  R_TRIG2(CLK := R_TRIG1.Q);
  CTU0(EN := TRUE, CU := R_TRIG2.Q, PV := 150);
  Test1 := CTU0.Q;
  IF CTU0.ENO THEN
      In01 := CTU0.CV;
  END_IF;
  R_TRIG3(CLK := blink_led);
  _TMP_ADD15_OUT := ADD(EN := R_TRIG3.Q, IN1 := In02, IN2 := 1, ENO => _TMP_ADD15_ENO);
  IF _TMP_ADD15_ENO THEN
      In02 := _TMP_ADD15_OUT;
  END_IF;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : Blink;
  END_RESOURCE
END_CONFIGURATION

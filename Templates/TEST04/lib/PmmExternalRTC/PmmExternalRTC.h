#ifdef PMMExternalRTC

extern bool PMMInitializeExternalRTC();
extern void PMMSetRTCUNIXTime(uint32_t unixTime);
extern DateTime PMMGetRTCNOW();
extern uint32_t PMMGetRTCUNIXTime();
extern uint8_t PMMReturnMonthNumber();
extern uint8_t PMMReturnDayNumber();
extern int16_t PMMReturnDayOfYear();
extern int16_t PMMReturnCurrentHour();
extern int16_t PMMReturnCurrentMinute();
extern bool PMMRTCCheck();
extern string PMMRTCStringTime();

#endif

#ifdef PMMInternalRTC


extern void PMMInternalRTCSetup();
extern void PMMInternalRTCSetUnixTime(int32_t UnixTime, float TimeZone);
extern uint32_t PMMInternalRTCGetUnixTime();
extern int16_t PMMInternalRTCGetDay();
extern int16_t PMMInternalRTCGetMonth();
extern int16_t PMMInternalRTCGetYear();
extern int16_t PMMInternalRTCGetHours();
extern int16_t PMMInternalRTCGetMinutes();
extern int16_t PMMInternalRTCGetSeconds();



#endif
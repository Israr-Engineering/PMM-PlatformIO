
#ifdef PMMFlashMemory
// Use 0-2. Larger for more debugging messages
#define FLASH_DEBUG       0
const int WRITTEN_SIGNATURE = 0xBEEFDEED;
const int START_ADDRESS     = 0;

extern void PMMFlashWriteInt (int Address, int Value);
extern void PMMFlashWriteFloat (int Address, float Value);
extern void PMMFlashWriteLong (int Address, long Value);
extern void PMMFlashWriteString (int Address, String Value);
extern int PMMFlashReadInt(int Address, int Value);
extern float PMMFlashReadFloat(int Address, float Value);
extern long PMMFlashReadFloat(int Address, long Value);
extern String PMMFlashReadFloat(int Address, String Value);

#endif
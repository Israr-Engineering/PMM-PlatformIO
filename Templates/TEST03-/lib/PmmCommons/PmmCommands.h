
#ifdef PMMCommands

extern void PMMInitializeEthernet(IPAddress ip, byte mac[]);
extern void PMMReadCommands();
extern string PMMCommnads(string readData);
extern string PMMReturnDataFromSerialUSB();
extern string PMMReturnDataFromAPIHTTPHeader();

extern void PMMSetUSBConfigurationSettings(string Settings);
extern string PMMGetUSBConfigurationSettings();
extern string PMMIsAlive();

#endif
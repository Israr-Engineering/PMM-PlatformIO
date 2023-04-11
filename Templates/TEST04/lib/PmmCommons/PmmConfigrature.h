vector<string> PMMStringToArray(const string &csvStr, char delimiter = ',');
void PMMWriteIntoFlashAllSettings(string Message);
void PMMWriteIntoFlashGeneralSettings(string Message);
void PMMWriteIntoFlashSerialSettings(string Message);
void PMMWriteIntoFlashTCPSettings(string Message);
void PMMWriteIntoFlashOptionsAndPinsSettings(string Message);
string PMMReadFromFlashAllSettings();

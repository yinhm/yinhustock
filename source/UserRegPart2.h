int IsUserRegisteredPart2(CTime &t);
bool GetDiskIDPart2(CString &s, const char* IDE, int &nFlag, CTime &t);
long ReadDiskIDPart2(const char* IDE, CString &strMachine, int &nFlag, int nDelay, CTime &t);
BOOL IsCreditUserPart2(CString &strSerial,CString &strCredit,CString &strMachine);
BOOL OpenDiskVxdOrSys();
void CloseDiskVxdOrSys();
extern char g_chMachineID[16];
long ReadDiskIDPartCwd();

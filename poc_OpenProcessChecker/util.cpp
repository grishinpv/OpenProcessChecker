
#include "util.h"

using namespace std;

bool checkOpenProcessSuccess(DWORD pid, DWORD dwDesiredAccess) {
	bool bRes = false;

	HANDLE hProcess;

	hProcess = OpenProcess(dwDesiredAccess, FALSE, pid);

	if (hProcess) {
		bRes = true;
	}

	CloseHandle(hProcess);

	return bRes;
}

void WaitEscKey() {
	int ch;

	_cputs("Press <Esc> to continue...");
	for (;;)
	{
		ch = _getch();
		if (ch == 0x1B)
			break;
	}

	cout << endl << endl;
}

ISELEVATED IsElevated() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	return fRet;
}

void about() {
	cout << endl << endl;
	cerr << "Usage: " << " --scan|-s\tKey to scan all process" << endl;
}

BOOL GetProcessFullPathFromPID(DWORD pid, wstring &pProcessFullPath) {
	WCHAR tmpProcessName[MAX_PATH] = UNKNOWN_STRING;
	HANDLE Handle = OpenProcess(
		PROCESS_QUERY_INFORMATION,
		FALSE,
		pid
	);

	bool fRes = false;

	if (Handle)
	{

		if (GetModuleFileNameEx(Handle, 0, tmpProcessName, MAX_PATH))
		{
			// At this point, buffer contains the full path to the executable
			fRes = true;
		}
		else
		{
			// You better call GetLastError() here
		}
		CloseHandle(Handle);
	}

	pProcessFullPath = tmpProcessName;

	return fRes;
}

BOOL GetProcessNameFromPID(DWORD pid, wstring &pProcessName) {
	WCHAR tmpProcessName [MAX_PATH] = UNKNOWN_STRING;
	HANDLE Handle = OpenProcess(
		PROCESS_QUERY_INFORMATION,
		FALSE,
		pid
	);

	bool fRes = false;
	
	if (Handle)
	{

		if (GetProcessImageFileName(Handle, tmpProcessName, MAX_PATH))
		{
			// At this point, buffer contains the executable name
			
			fRes = true;
		}
		else
		{
			// You better call GetLastError() here
		}
		CloseHandle(Handle);
	}

	pProcessName = tmpProcessName;

	return fRes;
}


BOOL IsProcessRunning(DWORD pid)
{
	HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, pid);
	DWORD ret = WaitForSingleObject(process, 0);
	CloseHandle(process);
	return ret == WAIT_TIMEOUT;
}

// convert wstring_to_string
void string_to_wstring(const std::string& src, std::wstring& dest)
{
	std::wstring tmp;
	tmp.resize(src.size());
	std::transform(src.begin(), src.end(), tmp.begin(), btowc);
	tmp.swap(dest);
}

wstring string_to_wstring_return(const std::string& src)
{
	std::wstring tmp;
	tmp.resize(src.size());
	std::transform(src.begin(), src.end(), tmp.begin(), btowc);
	return tmp;
}

// convert wstring_to_string
void wstring_to_string(const std::wstring& src, std::string& dest)
{
	std::string tmp;
	tmp.resize(src.size());
	std::transform(src.begin(), src.end(), tmp.begin(), wctob);
	tmp.swap(dest);
}

string wstring_to_string_return(const std::wstring& src)
{
	std::string tmp;
	tmp.resize(src.size());
	std::transform(src.begin(), src.end(), tmp.begin(), wctob);
	

	return tmp;
}

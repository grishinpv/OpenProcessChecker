#include "worker.h"
#include "util.h"


using namespace std;

vector <pair<PCWSTR, DWORD>> vDesiredAccessList{
	{L"DELETE", DELETE},
	{L"READ_CONTROL", READ_CONTROL},
	{L"WRITE_OWNER", WRITE_OWNER},
	{L"SYNCHRONIZE", SYNCHRONIZE},
	{L"STANDARD_RIGHTS_REQUIRED", STANDARD_RIGHTS_REQUIRED},
	{L"ACCESS_SYSTEM_SECURITY", ACCESS_SYSTEM_SECURITY},
	{L"WRITE_DAC", WRITE_DAC},
	{L"PROCESS_TERMINATE", PROCESS_TERMINATE},
	{L"PROCESS_CREATE_THREAD", PROCESS_CREATE_THREAD},
	{L"PROCESS_SET_SESSIONID", PROCESS_SET_SESSIONID},
	{L"PROCESS_VM_OPERATION", PROCESS_VM_OPERATION},
	{L"PROCESS_VM_READ", PROCESS_VM_READ},
	{L"PROCESS_VM_WRITE", PROCESS_VM_WRITE},
	{L"PROCESS_DUP_HANDLE", PROCESS_DUP_HANDLE},
	{L"PROCESS_CREATE_PROCESS", PROCESS_CREATE_PROCESS},
	{L"PROCESS_SET_QUOTA", PROCESS_SET_QUOTA},
	{L"PROCESS_SET_INFORMATION", PROCESS_SET_INFORMATION},
	{L"PROCESS_QUERY_INFORMATION", PROCESS_QUERY_INFORMATION},
	{L"PROCESS_SUSPEND_RESUME", PROCESS_SUSPEND_RESUME},
	{L"PROCESS_QUERY_LIMITED_INFORMATION", PROCESS_QUERY_LIMITED_INFORMATION},
	{L"PROCESS_SET_LIMITED_INFORMATION", PROCESS_SET_LIMITED_INFORMATION}
};

void ProcessSingle() {

	DWORD pid;
	int ch = 0;
	std::wstring wstrPID = L"";

	const WCHAR separator = L' ';
	const int nameWidth = 35;
	const int numWidth = 8;

	while (ch != 0x1B) {

		pid = -1;
		wstrPID = L"";

		// GET CURRENT USER NAME  ------------------------- 
		TCHAR g_UserName[256 + 1];
		DWORD username_len = 256 + 1;
		GetUserName(g_UserName, &username_len);

		//print if current process is elevated
		wcout << L"Current process is " << to_str(IsElevated()) << endl;
		wcout << L"Current user: " << g_UserName << endl;
		wcout << endl << endl;

		wcout << L"-----------------------------------------" << endl;
		wcout << L"Input PID to check available access: ";
		wcin >> wstrPID;
		wcin.clear();
		wcout << endl;

		try {
			pid = _wtoi(wstrPID.c_str());
		}
		catch (...) {
			wcout << L"Incorrect PID";
			continue;
		}

		if (pid < 0) {
			std::cout << "Failed to convert to PID";
		}

		if (!IsProcessRunning(pid)) {
			wcout << L"Process not found";
			
		}
		else {

			// GET PROCESS NAME  ------------------------- 
			//WCHAR wProcessName[MAX_PATH];
			wstring wProcessName;
			GetProcessNameFromPID(pid, wProcessName);

			//iterate over vDesiredAccessList and check;

			wcout << L"Process: " << wProcessName << L" (" << pid << L")" << endl;
			for (auto const& pairDesiredAccess : vDesiredAccessList) {
				/* std::cout << value; ... */
				wcout << left << setw(nameWidth) << setfill(separator) << pairDesiredAccess.first << setw(numWidth) << to_str(checkOpenProcessSuccess(pid, pairDesiredAccess.second)) << endl;
			}
		}

		//stop before continue
		wcout << endl;
		WaitEscKey();
		//clrscr();
		system("cls");	//clears the screen
	}

}

void ProcessMultiple(LPWSTR pFilePath) {
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	wstring wProcessName;
	unsigned int i;
	const WCHAR separator = L' ';
	const int nameWidth = 35;
	const int numWidth = 8;
	CSVFile *csv_obj = new CSVFile();
	BOOL bOutputToExternal = true;
	PCWSTR wasElevated = to_str(IsElevated());
	

	string sFilePath = "OpenProcessChecker.csv";

	try {
		wstring_to_string(pFilePath, sFilePath);
		csv_obj->init(sFilePath);
		csv_obj->header = { "WasElevated", "ProcessName", "DesiredAccess", "Result"};
		csv_obj->write_header();
	}
	catch (...) {
		bOutputToExternal = false;
	}

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return;
	}

	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.

	map<string, string> row_map; // Map formatted Column: Column value
	PCWSTR opResult;

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			// GET PROCESS NAME  ------------------------- 
			GetProcessNameFromPID(aProcesses[i], wProcessName);

			//iterate over vDesiredAccessList and check;
			wcout << L"Process: " << wProcessName << L" (" << aProcesses[i] << L")\n" << L"----------------------------" << endl << endl;
			for (auto const& pairDesiredAccess : vDesiredAccessList) {
				opResult = to_str(checkOpenProcessSuccess(aProcesses[i], pairDesiredAccess.second));

				// output to console
				wcout << left << setw(nameWidth) << setfill(separator) << pairDesiredAccess.first << setw(numWidth) << opResult << endl;

				// output to file
				if (bOutputToExternal) {
					row_map["WasElevated"] = wstring_to_string_return(wasElevated);
					row_map["ProcessName"] = wstring_to_string_return(wProcessName);
					row_map["DesiredAccess"] = wstring_to_string_return(pairDesiredAccess.first);
					row_map["Result"] = wstring_to_string_return(opResult);

					try {
						csv_obj->write_row(row_map);
					}
					catch (...) {
						// do nothing, simply skip it
					}
				}

			}
		}
	}

	// cleanup
	csv_obj->close();
	wcout << endl;

}
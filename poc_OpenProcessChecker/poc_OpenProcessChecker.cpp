

#include "util.h"
#include "worker.h"
#include <filesystem>


namespace fs = std::experimental::filesystem;
using namespace std;



enum Mode {
	ScanSingle,
	ScanAll
};

int main(int argc, WCHAR *argv[])
{
	DWORD dMode;
	fs::path wWorkingDir = fs::current_path();

	// --------------------
	//
	// MODE SELECT
	//
	// --------------------
	if (argc == 1) {
		//std::wcout << L"No PID provided" << endl;
		dMode = Mode::ScanSingle;
	}
	else if (argc == 2) {
		if ((wcscmp(argv[1], L"-s") != 0) |
			(wcscmp(argv[1], L"--scan") != 0)) {

			dMode = Mode::ScanAll;
		}
		else
			about();
	}
	else {

		about();
		return 1;
	}

	

	// --------------------
	//
	// Process
	//
	// --------------------
	switch (dMode)
	{
	case Mode::ScanSingle:
		ProcessSingle();
	case Mode::ScanAll:
		WCHAR out_name [8] ;
		wcsncpy_s(out_name, 8+1, L"TEST.csv", 8+1);
		ProcessMultiple(out_name);
	default:
		throw "Bad Mode selection";
		break;
	};
	
	return 0;


}


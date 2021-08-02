#pragma once

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <psapi.h>
#include "CSVFile.hpp"
#include <algorithm>
#include <cwchar>



typedef BOOL ISELEVATED;

#define	UNKNOWN_STRING	L"Unknown";

bool checkOpenProcessSuccess(DWORD pid, DWORD dwDesiredAccess);

void WaitEscKey();

ISELEVATED IsElevated();

inline PCWSTR to_str(bool v) {
	switch (v) {
	case true:
		return L"SUCCESS";
	case false:
		return L"FAILED";
	}
	return L"<?>";
}

inline PCWSTR to_str(ISELEVATED v) {
	switch (v) {
	case true:
		return L"ELEVATED";
	case false:
		return L"NOT ELEVATED";
	}
	return L"<?>";
}

void about();

BOOL GetProcessFullPathFromPID(DWORD pid, wstring &pProcessFullPath);

BOOL GetProcessNameFromPID(DWORD pid, wstring &pProcessName);

BOOL IsProcessRunning(DWORD pid);

void string_to_wstring(const std::string& src, std::wstring& dest);
wstring string_to_wstring_return(const std::string& src);

void wstring_to_string(const std::wstring& src, std::string& dest);
string wstring_to_string_return(const std::wstring& src);
#pragma once

#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <psapi.h>
#include "CSVFile.hpp"


void ProcessSingle();

void ProcessMultiple(LPWSTR pFilePath);

#include "Utils/Platforms.h"
#ifdef JOC_WINDOWS_BUILD

#include <Windows.h>
#include <iostream>
#include "dbghelp.h"

#include "Exception/Exception.h"

void Exception::demangle(std::stringstream& ss, char** symbollist, size_t addrlen) const {
    return;
}

static void printStackItem(std::stringstream& ss, HANDLE process, void* address, SYMBOL_INFO* symbol, IMAGEHLP_LINE* line) {
	DWORD dwDisplacement;

	bool ret1 = SymFromAddr(process, (DWORD64)address, 0, symbol);
	bool ret2 = SymGetLineFromAddr(process, (DWORD)address, &dwDisplacement, line);

	if (ret1) {
		ss << "    " << symbol->Name;
		if (ret2) {
			ss << " - " << line->FileName << ":" << line->LineNumber;
		}
		ss << std::endl;
	}
}

#define MAX_SYMBOL_LENGTH 1024
void Exception::printStackTrace(std::stringstream& ss) const {
	char* symbol_data_raw = new char[MAX_SYMBOL_LENGTH + sizeof(SYMBOL_INFO)/sizeof(char)];
	SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbol_data_raw;
	symbol->MaxNameLen = MAX_SYMBOL_LENGTH;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	IMAGEHLP_LINE *line = new IMAGEHLP_LINE;
	line->SizeOfStruct = sizeof(IMAGEHLP_LINE);

	HANDLE process = GetCurrentProcess();

	if (_address != nullptr) {
		printStackItem(ss, process, _address, symbol, line);
	}

	for (int i = 0; i < _stackSize; ++i) {
		printStackItem(ss, process, _stackTrace[i], symbol, line);
	}

	delete[] symbol_data_raw;
	delete line;
}

void Exception::saveStackTrace() {
	HANDLE process = GetCurrentProcess();
	SymInitialize(process, NULL, true);
	_stackSize = CaptureStackBackTrace(_stackSkip, STACK_TRACE_MAX_SIZE, _stackTrace, NULL);
}

#endif

#include "stdafx.h"
#include "Helpers.h"

void Console::Log(const char *str) {
	std::cout << "[+] " << str << std::endl;
}

void Console::Warn(const char *str) {
	std::cout << "[!] " << str << std::endl;
}

void Console::Err(const char *str) {
	std::cout << "[?] " << str << std::endl;
	Helpers::Exit(0);
}

void Console::Address(const char *name, int address) {
	std::cout << "[+] " << name << ": 0x" << std::hex << address << std::dec << std::endl;
}

void Console::Bytes(const char *name, int bytes[]) {
	std::cout << "[+] " << name << ": " << std::endl;
	// for size of bytes
	// print 2 bytes in hex or smt
	std::cout << std::endl;
}

// Bad code but it works
extern HMODULE g_DLLMOD;

void Helpers::Init() {
	// Console window
	AllocConsole();
	AttachConsole(GetProcessId(GetCurrentProcess()));
	freopen("CON", "w", stdout);
	SetConsoleTitleA("Zm_By_Syc0x00");

	Console::Log("Console initialized");

	return;
}

void Helpers::Exit(int timer) {
	Sleep(timer);
	Console::Log("Detaching...");
	FreeConsole(); // This way you can close the console without closing BO2
	FreeLibraryAndExitThread(g_DLLMOD, 0);
}

bool Helpers::KillService(LPCWSTR serviceName) {
	SC_HANDLE service, scm;
	SERVICE_STATUS_PROCESS serviceStatus;
	DWORD what;

	scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
	if (!scm) {
		return 0;
	}

	service = OpenService(scm, serviceName, SERVICE_QUERY_STATUS);
	if (!service) {
		Console::Warn("The service isn't even installed asshat");
		CloseServiceHandle(scm);
		return true;
	}

	auto result = QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&serviceStatus), sizeof(SERVICE_STATUS_PROCESS), &what);

	bool ret = true;

	if (serviceStatus.dwCurrentState != SERVICE_STOPPED) {
		// TODO Kill it
		if (/*killing it*/ 1 == false)
			ret = false;
	}

	CloseServiceHandle(service);
	CloseServiceHandle(scm);

	if (ret == false)
		std::cout << "Failed to kill BE" << std::endl;
	else
		std::cout << "Killed BE" << std::endl;

	return ret;
}

#include <Tlhelp32.h>
#include <string>

// https://stackoverflow.com/questions/7956519/how-to-kill-processes-by-name-win32-api
bool Helpers::KillProc(const wchar_t *proc) {
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);

	while (hRes) {
		if (wcscmp(pEntry.szExeFile, proc) == 0) {
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL) {
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
				CloseHandle(hSnapShot);
				std::wcout << "[+] " << "Killed " << proc << std::endl;
				return true;
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}

	CloseHandle(hSnapShot);
	return false;
}

inline int Helpers::Ptr(int address) {
	int *ptr = reinterpret_cast<int *>(address);
	return *ptr; // Return the value of the pointer
}

void Helpers::HookFunction(PVOID *oFunction, PVOID pDetour) {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}
void Helpers::UnhookFunction(PVOID *oFunction, PVOID pDetour) {
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}

// Thx SMBB
const char *Helpers::VariableText(const char *format, ...) {
	va_list argptr;
	va_start(argptr, format);

	char buffer[2048];
	vsprintf(buffer, format, argptr); // It should use vsnprintf but meh doesn't matter here

	va_end(argptr);

	return buffer;
}

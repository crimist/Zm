#pragma once
#include "stdafx.h"

extern std::mutex mtx;

namespace Console {
	void Log(const char *str);
	void Warn(const char *str);
	void Err(const char *str);
	template <typename T> void Value(const char *name, T value) {
		mtx.lock();
		std::cout << "[+] " << name << ": " << value << std::endl;
		mtx.unlock();
	}
	void Address(const char *name, int address);
	void Bytes(const char *name, int bytes[]);
}

class Helpers {
public:
	static void Init();
	static void Exit(int timer);
	static bool KillService(LPCWSTR serviceName);
	static bool KillProc(const wchar_t *proc);

	static int Ptr(int address);

	static void HookFunction(PVOID *oFunction, PVOID pDetour);
	static void UnhookFunction(PVOID *oFunction, PVOID pDetour);

	static const char *VariableText(const char* format, ...);

	static void KeyPress(unsigned char keyCode);
    static ImU32 ColorGradient(float value);
    static ImU32 ColorGradient(int value);
}

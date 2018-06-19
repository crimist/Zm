// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Zm.h"
#include "Hooks.h"
#include "Helpers.h"
#include "Math.h"
#include "Offsets.h"
#include "Game.h"
#include "Menu.h"
#include "Input.h"

// My first ever other than an external one for a 2d game where I wrote some mem manip funcs including AOB in C#
// so please no bulli
// also i stole some funcs (w2s and some helpers) from online and i probably didnt credit every1 sry 

HMODULE g_DLLMOD;

void Main() {
	Helpers::Init();

	//Zm::Dump(); // Dump to test it's working

	HANDLE D3Dhandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hooks::Init, NULL, NULL, NULL);
	HANDLE TPHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Input::Thread, NULL, NULL, NULL);

	WaitForSingleObject(D3Dhandle, INFINITE); // Wait for the F10 which will undo the Hooks
	TerminateThread(TPHandle, 0); // Then remove the input thread

	CloseHandle(TPHandle); // Close the handles
	CloseHandle(D3Dhandle);

	Console::Log("Successful unhook and TerminateThread()");

	Helpers::Exit(0); // and detach
	return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	g_DLLMOD = hModule;
	DWORD ThreadID;

    switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Main, NULL, NULL, &ThreadID);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

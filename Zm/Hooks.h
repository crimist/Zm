#pragma once
#include "stdafx.h"

typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

namespace Hooks {
	// Hooked func
	HRESULT __stdcall hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	// Funcs
	void Init();
}

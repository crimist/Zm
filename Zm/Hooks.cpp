#include "stdafx.h"
#include "Helpers.h"
#include "Hooks.h"
#include "Offsets.h"
#include "Vectors.h"
#include "Math.h"
#include "Menu.h"
#include "Renderer.h"

// Thanks SMBB for the example code in R6 internal cheat

// Original func addr
typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef unsigned short(__cdecl *RegisterTag)(char *szName);
typedef signed int(_cdecl *tGetTagPos)(void *pEntity, DWORD dwMeshResult, int iTag, float *flOut);
typedef LRESULT(__stdcall *tWndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Ptrs
tD3D11Present oPresent = NULL;
RegisterTag oRegisterTag = NULL;
tGetTagPos oGetTagPos = NULL;
tWndProc oWndProc = NULL;

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplDX11_WndProcHandler(hWnd, uMsg, wParam, lParam) && (GetKeyState(VK_DELETE) & 0x0001) != 0) {
		Menu::GetInstance()->oToggle = true;

		return 1l;
	}

	Menu::GetInstance()->oToggle = false;
	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

unsigned short __cdecl hkRegisterTag(char *szName) {
	std::cout << "szName: " << static_cast<const void*>(szName) << std::endl;
	std::cout << "szName: " << szName << std::endl;
	unsigned short ret = oRegisterTag(szName);
	std::cout << "ret: " << ret << std::endl;
	return ret;
}

signed int _cdecl hkGetTagPos(void *pEntity, DWORD dwMeshResult, int iTag, float *flOut) {
	//Console::Value("iTag", iTag);
	//std::cout << "flOut: " << static_cast<void *>(flOut) << std::endl;

	return oGetTagPos(pEntity, dwMeshResult, iTag, flOut);
}

// D3D11 Shit
ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL; // ptr 0x0357e288->?
IDXGISwapChain * pSwapChain = NULL; // ptr 0x035b5da4->?
ID3D11RenderTargetView *pRenderTargetView = NULL;

bool firstTime = true;

// I just spent 6+ hours debugging why this was only called once only to find that it's cause RTSS
// conflicts with D3D hooking fml
HRESULT __stdcall Hooks::hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (firstTime) {
		Console::Log("hkD3D11Present Called");
		HWND hWindow = FindWindow(NULL, L"Call of Duty®: Black Ops II - Zombies");
		if (hWindow == NULL)
			Console::Err("Failed to find Window");

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice))) {
			Console::Log("pSwapChain->GetDevice() Success");
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}

		ID3D11Texture2D* renderTargetTexture = nullptr;
		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture)))) {
			Console::Log("pSwapChain->GetBuffer() Success");
			pDevice->CreateRenderTargetView(renderTargetTexture, NULL, &pRenderTargetView);
			renderTargetTexture->Release();
		}

		ImGui_ImplDX11_Init(hWindow, pDevice, pContext);
		ImGui_ImplDX11_CreateDeviceObjects();

		firstTime = false;
	}

	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	ImGui_ImplDX11_NewFrame();

	Renderer::GetInstance()->BeginScene();
	Renderer::GetInstance()->DrawScene();
	Renderer::GetInstance()->EndScene();

	Menu::GetInstance()->Draw();

	ImGui::Render();

	return oPresent(pSwapChain, SyncInterval, Flags);
}

void Hooks::Init() {
	Console::Log("D3D::Init()");

	Helpers::KillProc(L"RTSS.exe"); // RTSS conflics with the D3D11Present hook 
	Helpers::KillProc(L"EncoderServer.exe");
	Helpers::KillProc(L"EncoderServer64.exe");
	Helpers::KillProc(L"RTSSHooksLoader.exe");
	Helpers::KillProc(L"RTSSHooksLoader64.exe");
	// Kill (or bypass if ur cool) any anti cheat here too (ex: battleye/vac)

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HWND hWindow = FindWindow(NULL, L"Call of Duty®: Black Ops II - Zombies");
	if (hWindow == NULL)
		Console::Err("Failed to find Window");

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	// Based on API Monitoring I did
	scd.BufferDesc.Width = Offsets::Screen->Width;
	scd.BufferDesc.Height = Offsets::Screen->Height;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.OutputWindow = hWindow;
	scd.Windowed = ((GetWindowLongPtr(hWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true; // check if our game is windowed
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Also based on API Monitoring I did
	// Btw this shit ran thru nvapi cause optimus maybe? idk. anyway nvapi just does 1 arg and
	// then passes it thru anyway
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel,
							1, D3D11_SDK_VERSION/*7*/, &scd, &pSwapChain, &pDevice, 0, &pContext))) {
		Console::Warn("D3D11CreateDeviceAndSwapChain() failed");
		return;
	} else
		Console::Log("D3D11CreateDeviceAndSwapChain() Success");

	Console::Address("pDevice", reinterpret_cast<int32_t>(pDevice));
	Console::Address("pContext", reinterpret_cast<int32_t>(pContext));
	Console::Address("pSwapChain", reinterpret_cast<int32_t>(pSwapChain));
	Console::Address("pRenderTargetView", reinterpret_cast<int32_t>(pRenderTargetView));
	
	DWORD_PTR *pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChain);
	DWORD_PTR *pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDevice);
	DWORD_PTR *pContextVT = reinterpret_cast<DWORD_PTR*>(pContext);

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);
	pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);

	Console::Address("pSwapChainVT", reinterpret_cast<int32_t>(pSwapChainVT));
	Console::Address("pDeviceVT", reinterpret_cast<int32_t>(pDeviceVT));
	Console::Address("pContextVT", reinterpret_cast<int32_t>(pContextVT));

	Console::Address("D3D11SWAPCHAIN", pSwapChainVT[8]);
	Console::Address("D3D11CONTEXT", pContextVT[12]);
	Console::Address("D3D11DEVICE", pDeviceVT[24]);

	// Keep the original func ptr
	oPresent = reinterpret_cast<tD3D11Present>(pSwapChainVT[8]/*D3D11PRESENT*/);
	oRegisterTag = reinterpret_cast<RegisterTag>(0x4AE070);
	oGetTagPos = reinterpret_cast<tGetTagPos>(0x4A0EF0); // gotta find this shit // this looks to be it

	// Hooks
	oWndProc = (WNDPROC)SetWindowLongPtr(FindWindow(NULL, L"Call of Duty®: Black Ops II - Zombies"), GWL_WNDPROC, (LONG)(LONG_PTR)hkWndProc);
	Console::Log("Hooked WndProc");
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&oPresent), hkD3D11Present);
	Console::Log("Hooked D3D11Present");
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&oRegisterTag), hkRegisterTag);

	SHORT keystate; // while will assume int32 otherwise because cpp
	while (!(keystate = GetKeyState(VK_F10) & 0x8000))
		Sleep(100);

	// Unhooks
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&oPresent), hkD3D11Present);
	Console::Log("Unhooked D3D11PRESENT");
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&oRegisterTag), hkRegisterTag);
	(WNDPROC)SetWindowLongPtr(FindWindow(NULL, L"Call of Duty®: Black Ops II - Zombies"), GWL_WNDPROC, (LONG)(LONG_PTR)oWndProc);
	Console::Log("Unhooked WndProc");
}

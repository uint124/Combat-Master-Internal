#pragma once
#include <dwmapi.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

namespace GUI
{
	Present oPresent;
	HWND window = NULL;
	WNDPROC oWndProc;
	ID3D11Device* pDevice = NULL;
	ID3D11DeviceContext* pContext = NULL;
	ID3D11RenderTargetView* mainRenderTargetView;

	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}


	uint64_t FindSwapchain()
	{
		uint64_t swapchain_start = (uint64_t)(find_sig("GameOverlayRenderer64.dll", "48 8B 05 ? ? ? ? FF D0 E9 ? ? ? ? 4C 8D 3D"));
		uint32_t swapchain_offset = *(uint32_t*)(swapchain_start + 3);
		uintptr_t swapchain_next = swapchain_start + 7;
		uint64_t swapchain_address = (swapchain_next + swapchain_offset);
		return swapchain_address;
	}
}
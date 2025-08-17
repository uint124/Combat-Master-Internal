#pragma once
#include <Windows.h>

namespace Globals
{
	uint64_t ProjectModule;
	uint64_t EngineModule;
	uint64_t GameOverlayRendererModule;

	namespace Screen
	{
		Vector2 ScreenSize;
		Vector2 ScreenCenter;

		namespace ImVec
		{
			ImVec2 ScreenSize;
			ImVec2 ScreenCenter;
		}
	}

	namespace Menu
	{
		bool bIsOpen = true;

		bool bWatermark = true;

		int currentPage = 0;

		bool bEsp = true;
		bool bAimbot = true;
		bool bDrawFov = true;
		float aimbotFov = 500.f;
		float aimbotSmoothing = 10.f;
		int aimbotKey = VK_RBUTTON;

		bool bBreakAi = false;
		bool bInfiniteJump = false;

		bool bEspBoxes = true;
		bool bEspFilledBoxes = false;
		bool bEspLines = false;
		bool bEspNames = true;
		bool bEspHealth = true;
		bool bEspHealthBar = true;
		bool bEspDistance = true;
		bool bShowAimbotTarget = false;

		float espMaxDistance = 200.0f;
	}

	void Init()
	{
		ProjectModule = reinterpret_cast<uint64_t>(GetModuleHandleA("Project.dll"));
		EngineModule = reinterpret_cast<uint64_t>(GetModuleHandleA("Engine.dll"));
		GameOverlayRendererModule = reinterpret_cast<uint64_t>(GetModuleHandleA("GameOverlayRenderer64.dll"));

		Screen::ScreenSize = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };
		Screen::ScreenCenter = Screen::ScreenSize / 2;

		Screen::ImVec::ScreenSize = Screen::ScreenSize.ToImVec();
		Screen::ImVec::ScreenCenter = Screen::ScreenCenter.ToImVec();
	}

} using namespace Globals;
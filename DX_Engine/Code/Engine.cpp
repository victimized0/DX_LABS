#include "pch.h"
#include "Engine.h"
#include <windowsx.h>
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return Engine::GetPtr()->WndProc(hwnd, msg, wParam, lParam);
}

Engine* Engine::pEngine = nullptr;
Engine* Engine::GetPtr()
{
	return pEngine;
}

Engine::Engine(HINSTANCE hInstance)
	: m_wndCaption(L"Made by Salimov")
	, m_isInit(false)
	, m_isResizing(false)
{
#ifdef _DEBUG
	assert(pEngine == nullptr);
#endif
	pEngine = this;

	wchar_t buf[MAX_PATH];
	GetModuleFileName(nullptr, buf, MAX_PATH);
	std::wstring execPath = std::wstring(buf);

	gEnv.WorkingPath	= execPath.substr(0, execPath.find_last_of(L"/\\"));
	gEnv.HInstance		= hInstance;
}

Engine::~Engine() {

}

bool Engine::Initialize(int iconId, int width, int height){
	if (!m_isInit) {
		if (!InitializeWindow(iconId, width, height)) {
			return false;
		}

		if (!gEnv.Renderer()->Initialise()) {
			return false;
		}

		if (!gEnv.D2DRend()->Initialise()) {
			return false;
		}

		m_isInit = true;
		return true;
	} else {
		return false;
	}
}

bool Engine::InitializeWindow(int iconId, int width, int height) {
	DWORD style		= WS_OVERLAPPEDWINDOW | WS_VISIBLE & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
	DWORD exstyle	= WS_EX_APPWINDOW;

	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);
	int x = monitorInfo.rcMonitor.left;
	int y = monitorInfo.rcMonitor.top;
	const int monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	const int monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	RECT wndrect;
	SetRect(&wndrect, 0, 0, width, height);
	AdjustWindowRectEx(&wndrect, style, FALSE, exstyle);

	width	= wndrect.right - wndrect.left;
	height	= wndrect.bottom - wndrect.top;

	x += (monitorWidth - width) / 2;
	y += (monitorHeight - height) / 2;

	LPCWSTR className = L"MainWnd";

	WNDCLASSEXW wcex	= {};
	wcex.cbSize			= sizeof(WNDCLASSEXW);
	wcex.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= MainWndProc;
	wcex.hInstance		= gEnv.HInstance;
	wcex.hIcon			= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName	= className;

	if (!RegisterClassEx(&wcex)) {
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	HWND hWnd = CreateWindowExW(exstyle, className, m_wndCaption.c_str(), style, x, y, width, height, NULL, NULL, wcex.hInstance, NULL);

	if (hWnd == nullptr) {
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	SetFocus(hWnd);
	UpdateWindow(hWnd);

	gEnv.Width		= width;
	gEnv.Height		= height;
	gEnv.HWnd		= hWnd;
	gEnv.HInstance	= wcex.hInstance;

	return hWnd != nullptr;
}

int Engine::Run() {
	MSG msg = {};
	m_timer.Reset();

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		} else {
			m_timer.Tick();
			float dt = m_timer.GetDeltaTime();
			m_scene.Update(dt);
			Update(dt);
			UpdateFPS();

			gEnv.Renderer()->BeginFrame();
			gEnv.Renderer()->Render();
			gEnv.D2DRend()->DrawText("FPS: " + std::to_string(m_fps));
			gEnv.Renderer()->EndFrame();
		}
	}

	return (int)msg.wParam;
}

void Engine::UpdateFPS() {
	++m_framesCount;

	// Compute averages over one second period.
	if ((m_timer.GetTotalTime() - m_elapsedTime) >= 1.0f) {
		//float fps = (float)m_framesPerSecond;
		//float mspf = 1000.0f / fps;
		m_fps = m_framesCount;

		// Reset for next average.
		m_framesCount = 0;
		m_elapsedTime += 1.0f;
	}
}

LRESULT Engine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE) {
			m_timer.Stop();
		}
		else {
			m_timer.Start();
		}

		Keyboard::ProcessMessage(msg, wParam, lParam);
		Mouse::ProcessMessage(msg, wParam, lParam);
	}
	break;

	case WM_SIZE:
	break;

	case WM_ENTERSIZEMOVE:
	break;

	case WM_EXITSIZEMOVE:
	break;

	case WM_GETMINMAXINFO:
	{
		auto info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;
	}
	break;

	case WM_MENUCHAR:
	// A menu is active and the user presses a key that does not correspond
		// to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
	return MAKELRESULT(0, MNC_CLOSE);

	case WM_SYSKEYDOWN:
	{
		Keyboard::ProcessMessage(msg, wParam, lParam);
		//if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000) {
		//	SetWindowLongPtr(hWnd, GWL_STYLE, 0);
		//	SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

		//	SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		//	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
		//}
		break;

	case WM_MOUSEMOVE:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_INPUT:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_KEYDOWN:
	{
		Keyboard::ProcessMessage(msg, wParam, lParam);
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hWnd);
		}
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	}

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Scene& Engine::GetScene() {
	return m_scene;
}

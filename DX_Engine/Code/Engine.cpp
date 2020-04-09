#include "pch.h"
#include "Engine.h"
#include <windowsx.h>
#include "Events/MouseEvent.h"
#include "Events/KeyboardEvent.h"

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
	, m_isPaused(false)
	, m_isMinimized(false)
	, m_isMaximized(false)
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

		OnResize(width, height);
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

			if (!m_isPaused) {
				float dt = m_timer.GetDeltaTime();
				m_scene.Update(dt);
				Update(dt);
				gEnv.Renderer()->Render();
			} else {
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

void Engine::OnResize(uint32_t width, uint32_t height) {
	gEnv.Width = max(width, 1);
	gEnv.Height = max(height, 1);

	gEnv.Renderer()->CreateResources();
	const_cast<Camera&>(m_scene.GetMainCamera()).SetProj(XM_PIDIV4, width, height, 0.1f, 1000.0f);
}

LRESULT Engine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE) {
			m_isPaused = true;
			m_timer.Stop();
		}
		else {
			m_isPaused = false;
			m_timer.Start();
		}
	}
	break;

	case WM_SIZE:
	{
		if (gEnv.Renderer()->GetDevice()) {
			if (wParam == SIZE_MINIMIZED) {
				m_isPaused = true;
				m_isMinimized = true;
				m_isMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED) {
				m_isPaused = false;
				m_isMinimized = false;
				m_isMaximized = true;
				OnResize(LOWORD(lParam), HIWORD(lParam));
			}
			else if (wParam == SIZE_RESTORED) {
				if (m_isMinimized) {
					m_isPaused = false;
					m_isMinimized = false;
					OnResize(LOWORD(lParam), HIWORD(lParam));
				}
				else if (m_isMaximized) {
					m_isPaused = false;
					m_isMaximized = false;
					OnResize(LOWORD(lParam), HIWORD(lParam));
				}
				else if (m_isResizing) {

				}
				else {
					OnResize(LOWORD(lParam), HIWORD(lParam));
				}
			}
		}
	}
	break;

	case WM_ENTERSIZEMOVE:
	{
		m_isPaused = true;
		m_isResizing = true;
		m_timer.Stop();
	}
	break;

	case WM_EXITSIZEMOVE:
	{
		m_isPaused = false;
		m_isResizing = false;
		m_timer.Start();

		RECT rc;
		GetClientRect(hWnd, &rc);
		OnResize(rc.right - rc.left, rc.bottom - rc.top);
	}
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
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000) {
			if (gEnv.AllowFullScreen) {
				// Implements the classic ALT+ENTER fullscreen toggle
				if (gEnv.IsFullScreen) {
					SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

					int width = 800;
					int height = 600;

					ShowWindow(hWnd, SW_SHOWNORMAL);

					SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
				}
				else {
					SetWindowLongPtr(hWnd, GWL_STYLE, 0);
					SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

					SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

					ShowWindow(hWnd, SW_SHOWMAXIMIZED);
				}

				gEnv.IsFullScreen = !gEnv.IsFullScreen;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		OnEvent(MouseEvent(EventType::MouseDown, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;

	case WM_RBUTTONUP:
		OnEvent(MouseEvent(EventType::MouseUp, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;

	case WM_MOUSEMOVE:
		OnEvent(MouseEvent(EventType::MouseMove, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		break;

	case WM_MOUSEWHEEL:
		OnEvent(MouseEvent(EventType::MouseScroll, wParam));
		break;

	case WM_KEYDOWN:
	{
		OnEvent(KeyboardEvent(EventType::KeyDown, wParam));
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

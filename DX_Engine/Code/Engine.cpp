#include "pch.h"
#include "Engine.h"
#include <windowsx.h>
#include "Events/MouseEvent.h"
#include "Events/KeyboardEvent.h"
#include "D3D11Renderer.h"

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
	m_renderer = std::make_unique<D3D11Renderer>();

	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(nullptr, buf, MAX_PATH);
	std::wstring m_execPath = std::wstring(buf);
	m_execPath = m_execPath.substr(0, m_execPath.find_last_of(L"/\\"));

	Environment::Instance().SetExecPath(m_execPath);
	Environment::Instance().SetInstanceHandle(hInstance);
}

Engine::~Engine() {

}

bool Engine::Initialize(int iconId){
	if (!m_isInit) {
		if (!InitializeWindow(iconId)) {
			return false;
		}

		if (!m_renderer->Initialise()) {
			return false;
		}

		OnResize(Environment::Instance().GetWidth(), Environment::Instance().GetHeight() - 20);
		m_isInit = true;

		return true;
	} else {
		return false;
	}
}

bool Engine::InitializeWindow(int iconId) {
	WNDCLASSEX wcex		= {};
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= Environment::Instance().GetInstanceHandle();
	wcex.hIcon			= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName	= nullptr;
	wcex.lpszClassName	= L"MainWnd";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));

	if (!RegisterClassEx(&wcex)) {
		MessageBoxW(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, static_cast<LONG>(Environment::Instance().GetWidth()), static_cast<LONG>(Environment::Instance().GetHeight()) };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	HWND hWnd = CreateWindowExW(0, L"MainWnd", m_wndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, Environment::Instance().GetInstanceHandle(), 0);

	if (hWnd == nullptr) {
		MessageBoxW(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	Environment::Instance().SetWidth(width);
	Environment::Instance().SetHeight(height);
	Environment::Instance().SetWindowHandle(hWnd);

	return true;
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
				m_renderer->Render();
			} else {
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}

void Engine::OnResize(uint32_t width, uint32_t height) {
	Environment::Instance().SetWidth(max(width, 1));
	Environment::Instance().SetWidth(max(height, 1));

	//if (m_isFullScreen)
	//	m_swapChain->SetFullscreenState(true, nullptr);
	m_renderer->CreateResources();
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
		if (m_renderer->GetDevice()) {
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
			if (Environment::Instance().GetAllowFullscreen()) {
				// Implements the classic ALT+ENTER fullscreen toggle
				if (Environment::Instance().GetIsFullscreen()) {
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

				Environment::Instance().SetIsFullscreen(!Environment::Instance().GetIsFullscreen());
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

IRenderer* Engine::GetRenderer()const {
	return m_renderer.get();
}

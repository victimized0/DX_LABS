#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "Interfaces/IRenderer.h"
#include "Engine.h"

#ifdef _WIN32
#define USE_DX 1
#endif

#define SAFE_DELETE(p) { if (p) { delete (p); (p) = nullptr; } }

struct Environment {
public:
	Environment()
		: Width(800)
		, Height(600)
		, IsFullScreen(false)
		, AllowFullScreen(false)
		, HInstance(nullptr)
		, HWnd(nullptr)
	{
		m_renderer.reset(IRenderer::Create());
	}

	IRenderer* Renderer() { return m_renderer.get(); }

public:
	std::wstring				WorkingPath;

	HINSTANCE					HInstance;
	HWND						HWnd;

	int							Width;
	int							Height;

	bool						IsFullScreen;
	bool						AllowFullScreen;

private:
	std::unique_ptr<IRenderer>	m_renderer;

};

extern Environment gEnv;

#endif //_ENVIRONMENT_H_

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "D2DRenderer.h"
#include "Interfaces/IRenderer.h"
#include "Engine.h"

#define SAFE_DELETE(p) { if (p) { delete (p); (p) = nullptr; } }

struct Environment {
public:
	Environment()
		: Width(800)
		, Height(600)
		, HInstance(nullptr)
		, HWnd(nullptr)
	{
		m_renderer.reset(IRenderer::Create());
		m_d2dRenderer = std::make_unique<D2DRenderer>();
	}

	IRenderer*						Renderer() { return m_renderer.get(); }
	D2DRenderer*					D2DRend() { return m_d2dRenderer.get(); }

public:
	std::wstring					WorkingPath;

	HINSTANCE						HInstance;
	HWND							HWnd;

	int								Width;
	int								Height;

private:
	std::unique_ptr<IRenderer>		m_renderer;
	std::unique_ptr<D2DRenderer>	m_d2dRenderer;

};

extern Environment gEnv;

#endif //_ENVIRONMENT_H_

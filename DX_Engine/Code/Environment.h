#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include "ResourcesManager.h"
#include "Interfaces/IRenderer.h"
#include "Engine.h"

#define SAFE_DELETE(p) { if (p) { delete (p); (p) = nullptr; } }

struct Environment {
public:
										Environment();
										~Environment() = default;

	IRenderer*							Renderer() { return m_renderer.get(); }
	ResourcesManager*					ResManager() { return m_resMan.get(); }

public:
	std::wstring						WorkingPath;

	HINSTANCE							HInstance;
	HWND								HWnd;

	int									Width;
	int									Height;

private:
	std::unique_ptr<IRenderer>			m_renderer;
	std::unique_ptr<ResourcesManager>	m_resMan;

};

extern Environment gEnv;

#endif //_ENVIRONMENT_H_

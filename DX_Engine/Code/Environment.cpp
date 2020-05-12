#include "pch.h"
#include "Environment.h"

Environment gEnv;

Environment::Environment()
	: Width(800)
	, Height(600)
	, HInstance(nullptr)
	, HWnd(nullptr)
{
	m_renderer.reset(IRenderer::Create());
	m_resMan.reset(ResourcesManager::Create());
}



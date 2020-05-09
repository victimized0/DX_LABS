#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "StepTimer.h"
#include "Scene.h"
#include "Interfaces/IRenderer.h"
#include "Environment.h"

#ifdef USE_DX
#include "D3D11Renderer.h"
#else
#error Not implemented
#endif

class Engine {
protected:
									Engine(HINSTANCE hnstance);
	virtual							~Engine();

									Engine(const Engine&)				= delete;
									Engine(Engine&&)					= delete;
									Engine& operator=(const Engine&)	= delete;
									Engine& operator=(Engine&&)			= delete;

	virtual bool					Initialize(int iconId, int width, int height);
	virtual void					Update(float dt) = 0;

public:
	static Engine*					GetPtr();
	int								Run();
	LRESULT							WndProc(HWND, UINT, WPARAM, LPARAM);
	Scene&							GetScene();

protected:
	bool							InitializeWindow(int iconId, int width, int height);

	static Engine*					pEngine;
	std::wstring					m_wndCaption;

	bool							m_isInit;
	bool							m_isResizing;

	Timer							m_timer;
	Scene							m_scene;

};


#endif // !_ENGINE_H_
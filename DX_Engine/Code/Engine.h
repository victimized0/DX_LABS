#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "StepTimer.h"
#include "Scene.h"
#include "Events/Event.h"
#include "Interfaces/IRenderer.h"
#include "Environment.h"

#ifdef _WIN32
#include "D3D11Renderer.h"
#else
#include "NullRenderer.h"
#endif

class Engine {
protected:
									Engine(HINSTANCE hnstance);
	virtual							~Engine();

									Engine(const Engine&)				= delete;
									Engine(Engine&&)					= delete;
									Engine& operator=(const Engine&)	= delete;
									Engine& operator=(Engine&&)			= delete;

	virtual void					OnResize(uint32_t, uint32_t);
	virtual bool					Initialize(int iconId, int width, int height);
	virtual void					OnEvent(const Event& event) = 0;
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
	bool							m_isPaused;
	bool							m_isMinimized;
	bool							m_isMaximized;
	bool							m_isResizing;

	Timer							m_timer;
	Scene							m_scene;

};


#endif // !_ENGINE_H_
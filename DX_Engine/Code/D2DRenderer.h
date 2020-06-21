#ifndef _D2D_RENDERER_H_
#define _D2D_RENDERER_H_
#pragma once

// Direct2D
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#define _USE_D2D
using Microsoft::WRL::ComPtr;

class D2DRenderer {
public:
									D2DRenderer();
									~D2DRenderer();

	bool							Initialise();
	void							Render();
	void							DrawText(const std::string& str);

private:
	ComPtr<ID2D1Factory>			m_factory;
	ComPtr<ID2D1RenderTarget>		m_backBufferRT;

	ComPtr<IDWriteFactory>			m_writeFactory;
	ComPtr<IDWriteTextFormat>		m_textFormat;

	ComPtr<ID2D1SolidColorBrush>	m_whiteBrush;
	ComPtr<ID2D1SolidColorBrush>	m_blackBrush;

};

#endif // _D2D_RENDERER_H_

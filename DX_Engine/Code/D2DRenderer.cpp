#include "pch.h"
#include "D2DRenderer.h"

D2DRenderer::D2DRenderer()
	: m_factory(nullptr)
	, m_backBufferRT(nullptr)
	, m_writeFactory(nullptr)
	, m_textFormat(nullptr)
	, m_whiteBrush(nullptr)
	, m_blackBrush(nullptr)
{

}

D2DRenderer::~D2DRenderer() {
    
}

bool D2DRenderer::Initialise() {
	HRESULT hr = S_OK;

	//hr |= D2D1CreateFactory(
 //       D2D1_FACTORY_TYPE_SINGLE_THREADED,
 //       m_factory.GetAddressOf()
 //   );

    D2D1_FACTORY_OPTIONS options;
#ifdef _DEBUG
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
    options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

    hr |= D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        __uuidof(ID2D1Factory),
        &options,
        &m_factory
    );

    UINT dpi = GetDpiForWindow(gEnv.HWnd);
    D2D1_RENDER_TARGET_PROPERTIES rtProperties = {};
    rtProperties.type           = D2D1_RENDER_TARGET_TYPE_HARDWARE;
    rtProperties.pixelFormat    = { DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED };
    rtProperties.dpiX           = dpi;
    rtProperties.dpiY           = dpi;
    rtProperties.usage          = D2D1_RENDER_TARGET_USAGE_NONE;
    rtProperties.minLevel       = D2D1_FEATURE_LEVEL_DEFAULT;

    IDXGISurface* bbSurface = gEnv.Renderer()->GetBackBuffer();
    hr |= m_factory->CreateDxgiSurfaceRenderTarget(
        bbSurface,
        rtProperties,
        &m_backBufferRT
    );

    bbSurface->Release();
    if (SUCCEEDED(hr)) {
        // Create a gray brush.
        hr = m_backBufferRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &m_whiteBrush
        );
    }

    if (SUCCEEDED(hr)) {
        // Create a gray brush.
        hr = m_backBufferRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &m_blackBrush
        );
    }

    hr |= DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_writeFactory.GetAddressOf()));
    hr |= m_writeFactory->CreateTextFormat(L"Gabriola", nullptr,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        32,
        L"en-us",
        &m_textFormat
    );

	return hr == S_OK;
}

void D2DRenderer::Render() {
    m_backBufferRT->BeginDraw();
    m_backBufferRT->SetTransform(D2D1::IdentityMatrix());
    m_backBufferRT->EndDraw();
}

void D2DRenderer::DrawText(const std::string& str) {
    D2D1_SIZE_F targetSize = m_backBufferRT->GetSize();
    D2D1_RECT_F layoutRect = D2D1::RectF(
        10.0f,
        0.0f,
        targetSize.width,
        targetSize.height
    );

    m_backBufferRT->BeginDraw();
    m_backBufferRT->SetTransform(D2D1::IdentityMatrix());

    std::wstring wstr(str.begin(), str.end());
    m_backBufferRT->DrawText(
        wstr.c_str(),       // The string to render.
        wstr.size(),        // The string's length.
        m_textFormat.Get(), // The text format.
        layoutRect,         // The region of the window where the text will be rendered.
        m_blackBrush.Get()  // The brush used to draw the text.
    );

    m_backBufferRT->EndDraw();
}

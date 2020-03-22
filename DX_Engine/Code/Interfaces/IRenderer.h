#ifndef _INTERFACE_RENDERER_H_
#define _INTERFACE_RENDERER_H_

#ifdef _WIN32 
#define D3DDevice				ID3D11Device
#define D3DContext				ID3D11DeviceContext
#define D3DSwapChain			IDXGISwapChain1
#define RenderTargetView		ID3D11RenderTargetView
#define DepthStencilView		ID3D11DepthStencilView
#define D3DViewport				CD3D11_VIEWPORT
#define D3DDriverType			D3D_DRIVER_TYPE
#define D3DFeatureLevel			D3D_FEATURE_LEVEL
#define InputLayout				ID3D11InputLayout
#define RSState					ID3D11RasterizerState
#define D3DBuffer				ID3D11Buffer
#define VertexBuffer			ID3D11Buffer
#define IndexBuffer				ID3D11Buffer
#define ConstantBuffer			ID3D11Buffer
#define D3DBlob					ID3DBlob
#define D3DUsage				D3D11_USAGE
#define D3DBindFlag				D3D11_BIND_FLAG
#define D3DRSDesc				D3D11_RASTERIZER_DESC
#define InputElementDesc		D3D11_INPUT_ELEMENT_DESC
#define VertexShader			ID3D11VertexShader
#define PixelShader				ID3D11PixelShader
#define D3DBufferDesc			D3D11_BUFFER_DESC
#define D3DSubresData			D3D11_SUBRESOURCE_DATA
#define D3DMappedSubres			D3D11_MAPPED_SUBRESOURCE
#define BIND_VERTEX_BUFFER		D3D11_BIND_VERTEX_BUFFER
#define BIND_INDEX_BUFFER		D3D11_BIND_INDEX_BUFFER
#define BIND_CONSTANT_BUFFER	D3D11_BIND_CONSTANT_BUFFER
#define D3D_CULL_FRONT			D3D11_CULL_FRONT
#define D3D_CULL_BACK			D3D11_CULL_BACK
#define D3D_CULL_NONE			D3D11_CULL_NONE
#define D3D_FILL_WIREFRAME		D3D11_FILL_WIREFRAME
#define D3D_FILL_SOLID			D3D11_FILL_SOLID
#else
// TODO: Non-DX defines
#endif

#define MAX_BUFFERS_COUNT 512

enum class ConstBufferType : unsigned char {
	PER_FRAME		= 0,
	PER_OBJECT		= 1,
	PER_MATERIAL	= 2,
	TOTAL
};

class IRenderer {
public:
	static IRenderer*		Create();
	virtual bool			Initialise()				= 0;
	virtual void			Render()					= 0;

	virtual bool			CreateDevice()				= 0;
	virtual bool			CreateResources()			= 0;
	virtual bool			OnDeviceLost()				= 0;

	virtual D3DDevice*		GetDevice()					= 0;
	virtual D3DContext*		GetDeviceContext()			= 0;

	virtual HRESULT			CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, D3DBuffer** pBuffer) = 0;
	virtual HRESULT			CreateBlob(const char* path, D3DBlob** pBlob) = 0;
};

#endif //_INTERFACE_RENDERER_H_

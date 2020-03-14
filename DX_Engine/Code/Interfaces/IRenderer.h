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
	virtual bool			Initialise()				= 0;
	virtual void			Render()					= 0;

	virtual bool			CreateDevice()				= 0;
	virtual bool			CreateResources()			= 0;
	virtual bool			OnDeviceLost()				= 0;

	virtual D3DDevice*		GetDevice()					= 0;
	virtual D3DContext*		GetDeviceContext()			= 0;

	virtual VertexBuffer*	GetVertexBuffer(int vbId)	= 0;
	virtual IndexBuffer*	GetIndexBuffer(int ibId)	= 0;
	virtual InputLayout*	GetInputLayout(int ilId)	= 0;
	virtual RSState*		GetRSState(int rssId)		= 0;
	virtual VertexShader*	GetVertexShader(int vsId)	= 0;
	virtual PixelShader*	GetPixelShader(int psId)	= 0;

	virtual int				CreateVertexBuffer(size_t size, size_t strideSize, const void* pData)			= 0;
	virtual int				CreateIndexBuffer(size_t size, const void* pData)								= 0;
	virtual int				CreateInputLayout(InputElementDesc* desc, size_t arrSize, D3DBlob* shaderBlob)	= 0;
	virtual int				CreateRSState(D3DRSDesc* desc)													= 0;
	virtual int				CreateVertexShader(const char* path, D3DBlob** ppBlob)							= 0;
	virtual int				CreatePixelShader(const char* path)												= 0;
};

#endif //_INTERFACE_RENDERER_H_

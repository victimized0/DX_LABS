#ifndef _INTERFACE_RENDERER_H_
#define _INTERFACE_RENDERER_H_

#ifdef _WIN32
#define USE_DX
#else
#error Not implemented
#endif

//#ifdef USE_DX 
#define IDevice						ID3D11Device
#define IDevCon						ID3D11DeviceContext
#define ISwapChain					IDXGISwapChain
#define ISamplerState				ID3D11SamplerState
#define IShaderResView				ID3D11ShaderResourceView
#define IRenderTargetView			ID3D11RenderTargetView
#define IDepthStencilState			ID3D11DepthStencilState
#define IDepthStencilView			ID3D11DepthStencilView
#define CViewport					CD3D11_VIEWPORT
#define D3DDriverType				D3D_DRIVER_TYPE
#define D3DFeatureLevel				D3D_FEATURE_LEVEL
#define IInputLayout				ID3D11InputLayout
#define IRSState					ID3D11RasterizerState
#define IBlendState					ID3D11BlendState
#define IBuffer						ID3D11Buffer
#define IVertexBuffer				ID3D11Buffer
#define IIndexBuffer				ID3D11Buffer
#define IConstBuffer				ID3D11Buffer
#define IBlob						ID3DBlob
#define D3DUsage					D3D11_USAGE
#define D3DBindFlag					D3D11_BIND_FLAG
#define D3DRSDesc					D3D11_RASTERIZER_DESC
#define D3DInputElementDesc			D3D11_INPUT_ELEMENT_DESC
#define IVertexShader				ID3D11VertexShader
#define IPixelShader				ID3D11PixelShader
#define D3DBufferDesc				D3D11_BUFFER_DESC
#define D3DSubresData				D3D11_SUBRESOURCE_DATA
#define D3DMappedSubres				D3D11_MAPPED_SUBRESOURCE
#define D3DSamplerDesc				D3D11_SAMPLER_DESC
#define D3DPrimitiveTopology		D3D_PRIMITIVE_TOPOLOGY
#define ITexture2D					ID3D11Texture2D
#define D3DTopologyTriangleStrip	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
#define D3DTopologyTriangleList		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
#define D3DBindVertexBuffer			D3D11_BIND_VERTEX_BUFFER
#define D3DBindIndexBuffer			D3D11_BIND_INDEX_BUFFER
#define D3DBindConstBuffer			D3D11_BIND_CONSTANT_BUFFER
#define D3DCullFront				D3D11_CULL_FRONT
#define D3DCullBack					D3D11_CULL_BACK
#define D3DCullNone					D3D11_CULL_NONE
#define D3DFillWireframe			D3D11_FILL_WIREFRAME
#define D3DFillSolid				D3D11_FILL_SOLID
#define D3DShaderMacro				D3D_SHADER_MACRO
#define HRES						HRESULT
//#endif

#define MAX_BUFFERS_COUNT 512

#define RF_USE_LIGHT	1 << 0
#define RF_USE_TEXTURES	1 << 1
#define RF_DEFERRED		1 << 2

#include "../Math/SimpleMath.h"
typedef DirectX::SimpleMath::Vector2	Vector2;
typedef DirectX::SimpleMath::Vector3	Vector3;
typedef DirectX::SimpleMath::Vector4	Vector4;
typedef DirectX::SimpleMath::Matrix		Matrix;
typedef DirectX::SimpleMath::Quaternion	Quaternion;

struct Texture {
	int			Width;
	int			Height;
	std::string	Path;
};

struct Material {
	Texture							Diffuse;
	Texture							Normal;
	Texture							Specular;
	DirectX::SimpleMath::Vector3	AmbientColor;
	DirectX::SimpleMath::Vector3	DiffuseColor;
	DirectX::SimpleMath::Vector4	SpecularColor;
};

struct DirLight {
	DirectX::SimpleMath::Vector4	LightCol;
	DirectX::SimpleMath::Vector4	LightAmb;
	DirectX::SimpleMath::Vector3	LightDir;
};

struct PointLight {
	DirectX::SimpleMath::Vector4	Diffuse;
	DirectX::SimpleMath::Vector4	Ambient;
	DirectX::SimpleMath::Vector3	Position;
	float							Range;
	DirectX::SimpleMath::Vector3	Attenuation;
};

struct RenderInfo {
	Microsoft::WRL::ComPtr<ISamplerState>	pSamplerState;
	Microsoft::WRL::ComPtr<IShaderResView>	pDiffuseView;
	Microsoft::WRL::ComPtr<IShaderResView>	pNormalView;
	Microsoft::WRL::ComPtr<IShaderResView>	pSpecularView;

	Microsoft::WRL::ComPtr<IVertexBuffer>	pVertexBuffer;
	Microsoft::WRL::ComPtr<IIndexBuffer>	pIndexBuffer;
	Microsoft::WRL::ComPtr<IRSState>		pRSState;

	IVertexShader*							pVertexShader;
	IPixelShader*							pPixelShader;
	IInputLayout*							pInputLayout;

	D3DPrimitiveTopology					Topology = D3DTopologyTriangleList;
};

enum class RenderPass : unsigned char {
	Geometry	= 0,
	Light		= 1
};

#include "..\Managers\ShadersManager.h"

class IRenderer {
public:
	static IRenderer*		Create();

	virtual					~IRenderer()				{}
	virtual bool			Initialise()				= 0;
	virtual void			Render()					= 0;

	virtual bool			CreateDevice()				= 0;
	virtual bool			OnDeviceLost()				= 0;

	virtual IDevice*		GetDevice()					= 0;
	virtual IDevCon*		GetContext()				= 0;
	virtual void			ClearFrame()				= 0;
	virtual void			ClearGBuffer()				= 0;

	virtual ShadersManager* GetShadersManager()			= 0;
	virtual void			SetBackColor(float r, float g, float b)		= 0;

	virtual HRES			CreateBuffer(size_t size, size_t strideSize, const void* pData, D3DBindFlag bindFlag, IBuffer** pBuffer) = 0;
};

#endif //_INTERFACE_RENDERER_H_

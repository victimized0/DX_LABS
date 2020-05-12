#ifndef _SHADER_H_
#define _SHADER_H_

#include "VertexFormats.h"
#include "Interfaces/IRenderer.h"

using Microsoft::WRL::ComPtr;

enum class ShaderType : unsigned char {
	Vertex,
	Pixel,
	Geometry
};

class Shader : public IBaseResource {
protected:
	typedef VertexPosColNmlTex Vertex;

public:
									Shader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile, ShaderType type);
	virtual							~Shader();

	virtual bool					Initialise(IDevice* device, IDevCon* context)override;
	void							GenerateHash()override;

	IInputLayout*					GetInputLayout();
	IBlob*							GetBlob();

	virtual void					BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS) = 0;
	virtual void					BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV) = 0;
	virtual void					BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer) = 0;
	virtual void					BindToPipeline() = 0;

protected:
	HRES							CreateInputLayout(IDevice* device);
	HRES							Compile();

protected:
	IDevCon*						m_pContext;

	ComPtr<IBlob>					m_blob;
	ComPtr<IInputLayout>			m_inputLayout;

	std::vector<D3DShaderMacro>		m_macros;
	std::string						m_entry;
	std::string						m_profile;
	UINT							m_flags;
	ShaderType						m_type;

};

class VertexShader final : public Shader {
public:
									VertexShader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile = "vs_5_0");
	virtual							~VertexShader();

	bool							Initialise(IDevice* device, IDevCon* context)final;
	void							BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS)final;
	void							BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV)final;
	void							BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer)final;
	void							BindToPipeline()final;

private:
	ComPtr<IVertexShader>			m_vertexShader;

};

class PixelShader final : public Shader {
public:
									PixelShader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile = "ps_5_0");
	virtual							~PixelShader();

	bool							Initialise(IDevice* device, IDevCon* context)final;
	void							BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS)final;
	void							BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV)final;
	void							BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer)final;
	void							BindToPipeline()final;

private:
	ComPtr<IPixelShader>			m_pixelShader;

};

class GeometryShader final : public Shader {
public:
									GeometryShader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile = "gs_5_0");
	virtual							~GeometryShader();

	bool							Initialise(IDevice* device, IDevCon* context)final;
	void							BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS)final;
	void							BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV)final;
	void							BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer)final;
	void							BindToPipeline()final;

private:
	ComPtr<IGeometryShader>			m_geometryShader;

};

#endif //_SHADER_H_

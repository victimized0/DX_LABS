#ifndef _SHADER_H_
#define _SHADER_H_

#include "Interfaces/IRenderer.h"
using Microsoft::WRL::ComPtr;

enum class ShaderType : unsigned char {
	Vertex,
	Pixel
};

struct VertexPosColNmlTex;

class Shader {
protected:
	using Vertex = VertexPosColNmlTex;

public:
							Shader(std::string const& name, ShaderType type);
	virtual					~Shader() = default;

	virtual void			Create(IDevice* device, const char* entryPoint, const std::vector<D3DShaderMacro>& macros) = 0;
	IInputLayout*			GetVertexDecl() const { return m_inputLayout.Get(); }
	IBlob*					GetBlob() const { return m_blob.Get(); }

protected:
	void					Create(IDevice* device, const char* entryPoint, const char* profile, const std::vector<D3DShaderMacro>& macros);

protected:
	std::string				m_name;
	ShaderType				m_type;
	ComPtr<IBlob>			m_blob;
	ComPtr<IInputLayout>	m_inputLayout;

};

class VertexShader : public Shader {
public:
							VertexShader(std::string const& name);
	virtual					~VertexShader() = default;

	void					Create(IDevice* device, const char* entryPoint, const std::vector<D3DShaderMacro>& macros) final;
	IVertexShader*			GetShader() const { return m_vertexShader.Get(); }

private:
	ComPtr<IVertexShader>	m_vertexShader;
};

class PixelShader : public Shader {
public:
							PixelShader(std::string const& name);
	virtual					~PixelShader() = default;

	void					Create(IDevice* device, const char* entryPoint, const std::vector<D3DShaderMacro>& macros) final;
	IPixelShader*			GetShader() const { return m_pixelShader.Get(); }

private:
	ComPtr<IPixelShader>	m_pixelShader;
};

#endif //_SHADER_H_

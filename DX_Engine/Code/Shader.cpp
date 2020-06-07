#include "pch.h"
#include "Shader.h"

Shader::Shader(std::string const& name, ShaderType type)
	: m_name(name)
	, m_type(type)
	, m_blob(nullptr)
	, m_inputLayout(nullptr)
{

}

void Shader::Create(IDevice* device, const char* entryPoint, const char* profile, const std::vector<D3DShaderMacro>& macros) {
	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	if (m_name.empty() || entryPoint == nullptr) {
		return;
	}

	m_name.append(".hlsl");
	std::wstring path(m_name.begin(), m_name.end());
	std::string projDir(SHADERS_DIR);
	path = std::wstring(projDir.begin(), projDir.end()) + path;
	ComPtr<ID3DBlob> errorBlob;

	HRES hr = D3DCompileFromFile(	path.c_str(),
									macros.data(),
									D3D_COMPILE_STANDARD_FILE_INCLUDE,
									entryPoint,
									profile,
									flags,
									0,
									&m_blob,
									&errorBlob);

	if (FAILED(hr)) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (m_blob) {
			m_blob->Release();
		}
	}
}

VertexShader::VertexShader(std::string const& name)
	: Shader(name, ShaderType::Vertex)
{

}

void VertexShader::Create(IDevice* device, const char* entryPoint, const std::vector<D3DShaderMacro>& macros) {
	Shader::Create(device, entryPoint, "vs_5_0", macros);

	ThrowIfFailed(
		device->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader)
	);

	device->CreateInputLayout(	Vertex::InputElements,
								Vertex::ElementsCount,
								m_blob->GetBufferPointer(),
								m_blob->GetBufferSize(),
								&m_inputLayout);
}

PixelShader::PixelShader(std::string const& name)
	: Shader(name, ShaderType::Pixel)
{

}

void PixelShader::Create(IDevice* device, const char* entryPoint, const std::vector<D3DShaderMacro>& macros) {
	Shader::Create(device, entryPoint, "ps_5_0", macros);

	ThrowIfFailed(
		device->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_pixelShader)
	);
}

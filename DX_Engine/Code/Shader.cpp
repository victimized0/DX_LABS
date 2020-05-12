#include "pch.h"
#include "Shader.h"

#include "crc32c.h"

////////////////////////////// BASE SHADER CLASS

Shader::Shader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile, ShaderType type)
	: IBaseResource(name)
	, m_blob(nullptr)
	, m_inputLayout(nullptr)
	, m_pContext(nullptr)
	, m_entry(entryPoint)
	, m_profile(profile)
	, m_type(type)
	, m_flags(0)
{
#ifdef _DEBUG
	m_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	m_macros.assign(macros, macros + macrosSize);
}

Shader::~Shader() {

}

bool Shader::Initialise(IDevice* device, IDevCon* context) {
	m_pContext = context;

	HRES hr = Compile();
	if ( FAILED(hr) ) {
		return false;
	}

	GenerateHash();

	return true;
}

HRES Shader::Compile() {
	if ( m_name.empty() || m_entry.empty() ) {
		return E_INVALIDARG;
	}

	std::wstring path( m_name.begin(), m_name.end() );
	std::string projDir( SHADERS_DIR );
	path = std::wstring( projDir.begin(), projDir.end() ) + path;
	ComPtr<ID3DBlob> errorBlob;

	HRES hr = D3DCompileFromFile(	path.c_str(), m_macros.data(),
									D3D_COMPILE_STANDARD_FILE_INCLUDE,
									m_entry.c_str(), m_profile.c_str(),
									m_flags, 0,
									&m_blob, &errorBlob
	);

	if ( FAILED(hr) ) {
		if (errorBlob) {
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (m_blob) {
			m_blob->Release();
		}
	}

	return hr;
}

IInputLayout* Shader::GetInputLayout() {
	return m_inputLayout.Get();
}

IBlob* Shader::GetBlob() {
	return m_blob.Get();
}

HRES Shader::CreateInputLayout(IDevice* device) {
	if ( !m_blob ) {
		return E_FAIL;
	}

	device->CreateInputLayout(	Vertex::InputElements, Vertex::ElementsCount,
								m_blob->GetBufferPointer(), m_blob->GetBufferSize(),
								&m_inputLayout);

	return S_OK;
}

void Shader::GenerateHash() {
	const uint8_t* data = reinterpret_cast<uint8_t*>(m_blob->GetBufferPointer());
	m_crc32c = crc32c_append(0, data, m_blob->GetBufferSize());
}

////////////////////////////// VERTEX SHADER CLASS

VertexShader::VertexShader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile)
	: Shader(name, macros, macrosSize, entryPoint, profile, ShaderType::Vertex)
{

}

VertexShader::~VertexShader() {

}

bool VertexShader::Initialise(IDevice* device, IDevCon* context) {
	if ( !Shader::Initialise(device, context) ) {
		return false;
	}

	HRES hr = device->CreateVertexShader( m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader );
	if ( FAILED(hr) ) {
		return false;
	}

	return true;
}

void VertexShader::BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV) {
	m_pContext->VSSetShaderResources(startSlot, nViews, ppSRV);
}

void VertexShader::BindToPipeline() {
	m_pContext->IASetInputLayout(m_inputLayout.Get());
	m_pContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
}

void VertexShader::BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS) {
	m_pContext->VSSetSamplers(startSlot, nViews, ppSS);
}

void VertexShader::BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer) {
	m_pContext->VSSetConstantBuffers(startSlot, nViews, ppCBuffer);
}

////////////////////////////// PIXEL SHADER CLASS

PixelShader::PixelShader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile)
	: Shader(name, macros, macrosSize, entryPoint, profile, ShaderType::Pixel)
{

}

PixelShader::~PixelShader() {

}

bool PixelShader::Initialise(IDevice* pDev, IDevCon* pCon) {
	if ( !Shader::Initialise(pDev, pCon) ) {
		return false;
	}

	HRES hr = pDev->CreatePixelShader( m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_pixelShader );
	if ( FAILED(hr) ) {
		return false;
	}

	return true;
}

void PixelShader::BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV) {
	m_pContext->PSSetShaderResources(startSlot, nViews, ppSRV);
}

void PixelShader::BindToPipeline() {
	m_pContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

void PixelShader::BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS) {
	m_pContext->PSSetSamplers(startSlot, nViews, ppSS);
}

void PixelShader::BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer) {
	m_pContext->PSSetConstantBuffers(startSlot, nViews, ppCBuffer);
}

////////////////////////////// GEOMETRY SHADER CLASS

GeometryShader::GeometryShader(const std::string& name, const D3DShaderMacro* macros, size_t macrosSize, const std::string& entryPoint, const std::string& profile)
	: Shader(name, macros, macrosSize, entryPoint, profile, ShaderType::Geometry)
{

}

GeometryShader::~GeometryShader() {

}

bool GeometryShader::Initialise(IDevice* pDev, IDevCon* pCon) {
	if ( !Shader::Initialise(pDev, pCon) ) {
		return false;
	}

	HRES hr = pDev->CreateGeometryShader( m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_geometryShader );
	if ( FAILED(hr) ) {
		return false;
	}

	return true;
}

void GeometryShader::BindResourceViews(UINT startSlot, UINT nViews, IShaderResView* const* ppSRV) {
	m_pContext->GSSetShaderResources(startSlot, nViews, ppSRV);
}

void GeometryShader::BindToPipeline() {
	m_pContext->GSSetShader(m_geometryShader.Get(), nullptr, 0);
}

void GeometryShader::BindSamplers(UINT startSlot, UINT nViews, ISamplerState* const* ppSS) {
	m_pContext->GSSetSamplers(startSlot, nViews, ppSS);
}

void GeometryShader::BindConstBuffers(UINT startSlot, UINT nViews, IBuffer* const* ppCBuffer) {
	m_pContext->GSSetConstantBuffers(startSlot, nViews, ppCBuffer);
}

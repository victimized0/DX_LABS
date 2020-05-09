#include "pch.h"
#include "Mesh.h"

#include "Math/SimpleMath.h"
#include "DDSTextureLoader.h"
#include <d3dcompiler.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

Mesh::Mesh(const std::string& name)
	: m_name(name)
	, m_transform(Matrix::Identity)
{

}

Matrix Mesh::GetMatrix()const {
	return m_transform;
}

void Mesh::Initialise(IDevice* device) {
	assert(m_vertices.size() > 0);
	assert(m_indices.size() > 0);

	IBlob* pVsBlob = nullptr;
	IBlob* pPsBlob = nullptr;

	for (size_t idx = 0; idx < m_vertices.size(); ++idx) {
		m_vertices[idx].Color = Vector3::One;
	}

	m_constBuffer.Create(device);

	UINT shaderFlags = 0;
#ifdef _DEBUG
	shaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	if (m_renderInfo.pDiffuseView != nullptr) {
		std::vector<D3DShaderMacro> macros = {
			{ "USE_DIR_LIGHT", "1" },
			{ NULL, NULL }
		};

		if (FAILED( gEnv.Renderer()->CompileShader(L"textured_vs.hlsl", "main", "vs_5_0", macros, shaderFlags, &pVsBlob) )) return;
		if (FAILED( gEnv.Renderer()->CompileShader(L"textured_ps.hlsl", "main", "ps_5_0", macros, shaderFlags, &pPsBlob) )) return;
	}
	else {
		if (FAILED(gEnv.Renderer()->CreateBlob("/Data/Shaders/standard_vs.cso", &pVsBlob))) return;
		if (FAILED(gEnv.Renderer()->CreateBlob("/Data/Shaders/standard_ps.cso", &pPsBlob))) return;
	}

	ThrowIfFailed(
		gEnv.Renderer()->CreateBuffer(m_vertices.size(), sizeof(Vertex), &m_vertices[0], D3DBindVertexBuffer, &m_renderInfo.pVertexBuffer)
	);

	ThrowIfFailed(
		gEnv.Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], D3DBindIndexBuffer, &m_renderInfo.pIndexBuffer)
	);

	ThrowIfFailed(
		device->CreateVertexShader(pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), nullptr, &m_renderInfo.pVertexShader)
	);

	ThrowIfFailed(
		device->CreatePixelShader(pPsBlob->GetBufferPointer(), pPsBlob->GetBufferSize(), nullptr, &m_renderInfo.pPixelShader)
	);

	device->CreateInputLayout(Vertex::InputElements, Vertex::ElementsCount,
							  pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(),
							  &m_renderInfo.pInputLayout);
}

void Mesh::Draw(IDevCon* context) {
	if (m_renderInfo.pVertexBuffer == nullptr ||
		m_renderInfo.pIndexBuffer  == nullptr ||
		m_renderInfo.pVertexShader == nullptr ||
		m_renderInfo.pPixelShader  == nullptr ||
		m_renderInfo.pInputLayout  == nullptr)
	{
		return;
	}

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetPrimitiveTopology(m_renderInfo.Topology);
	context->IASetInputLayout(m_renderInfo.pInputLayout.Get());
	context->IASetVertexBuffers(0, 1, m_renderInfo.pVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_renderInfo.pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	context->VSSetShader(m_renderInfo.pVertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_renderInfo.pPixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_renderInfo.pDiffuseView.GetAddressOf());
	context->PSSetSamplers(0, 1, m_renderInfo.pSamplerState.GetAddressOf());

	CBPerMaterial cbPerMaterial = {};
	cbPerMaterial.AmbientColor	= Vector4(m_material.AmbientColor, 0.0f);
	cbPerMaterial.DiffuseColor	= Vector4(m_material.DiffuseColor, 0.0f);
	cbPerMaterial.SpecularColor = m_material.SpecularColor;
	m_constBuffer.SetData(context, cbPerMaterial);
	IConstBuffer* cb = m_constBuffer.GetBuffer();

	context->PSSetConstantBuffers((UINT)CBPerMaterial::Slot, 1, &cb);

	if (m_renderInfo.pNormalView != nullptr) {
		context->PSSetShaderResources(1, 1, m_renderInfo.pNormalView.GetAddressOf());
	}

	if (m_renderInfo.pSpecularView != nullptr) {
		context->PSSetShaderResources(2, 1, m_renderInfo.pSpecularView.GetAddressOf());
	}

	if (m_renderInfo.pRSState != nullptr) {
		context->RSSetState(m_renderInfo.pRSState.Get());
	}

	context->DrawIndexed((UINT)m_indices.size(), 0, 0);
}

void Mesh::SetMaterial(const Material& mat) {
	m_material = mat;

	std::wstring diffusePath(mat.Diffuse.Path.begin(), mat.Diffuse.Path.end());
	std::wstring normalPath(mat.Normal.Path.begin(), mat.Normal.Path.end());
	std::wstring specularPath(mat.Specular.Path.begin(), mat.Specular.Path.end());

	if (FAILED(DirectX::CreateDDSTextureFromFile(gEnv.Renderer()->GetDevice(), diffusePath.c_str(), nullptr, &m_renderInfo.pDiffuseView))) return;
	DirectX::CreateDDSTextureFromFile(gEnv.Renderer()->GetDevice(), normalPath.c_str(), nullptr, &m_renderInfo.pNormalView);
	DirectX::CreateDDSTextureFromFile(gEnv.Renderer()->GetDevice(), specularPath.c_str(), nullptr, &m_renderInfo.pSpecularView);

	D3D11_SAMPLER_DESC samplerDesc	= {};
	samplerDesc.Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias			= 0.0f;
	samplerDesc.MaxAnisotropy		= 4;
	samplerDesc.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD				= -FLT_MAX;
	samplerDesc.MaxLOD				= FLT_MAX;

	ThrowIfFailed(gEnv.Renderer()->GetDevice()->CreateSamplerState(&samplerDesc, &m_renderInfo.pSamplerState));
}

void Mesh::CreateVertices(std::vector<Vertex>& vertices) {
	assert(m_vertices.size() == 0);

	m_vertices = vertices;
	gEnv.Renderer()->CreateBuffer(m_vertices.size(), sizeof(Vertex), &m_vertices[0], D3DBindVertexBuffer, &m_renderInfo.pVertexBuffer);
}

void Mesh::CreateIndices(std::vector<UINT>& indices) {
	assert(m_indices.size() == 0);

	m_indices = indices;
	gEnv.Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], D3DBindIndexBuffer, &m_renderInfo.pIndexBuffer);
}

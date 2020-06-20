#include "pch.h"
#include "Mesh.h"

#include "Math/SimpleMath.h"
#include "DDSTextureLoader.h"
#include <d3dcompiler.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

Mesh::Mesh()
	: Mesh("")
{

}

Mesh::Mesh(const std::string& name)
	: m_name(name)
	, m_transform(Matrix::Identity)
	, m_renderFlags(RF_DEFERRED)
{

}

void Mesh::SetRenderFlags(int flags) {
	m_renderFlags = flags;
}

void Mesh::SetColor(const Vector3& vCol) {
	assert(m_vertices.size() > 0);

	for (size_t index = 0; index < m_vertices.size(); ++index) {
		m_vertices[index].Color = vCol;
	}
}

void Mesh::SetVertexShader(const VertexShader& vs) {
	m_renderInfo.pVertexShader = vs.GetShader();
	m_renderInfo.pInputLayout = vs.GetVertexDecl();
}

void Mesh::SetPixelShader(const PixelShader& ps) {
	m_renderInfo.pPixelShader = ps.GetShader();
}

Matrix Mesh::GetMatrix()const {
	return m_transform;
}

void Mesh::Initialise(IDevice* device) {
	assert(m_vertices.size() > 0);
	assert(m_indices.size() > 0);

	m_constBuffer.Create(device);

	ThrowIfFailed(
		gEnv.Renderer()->CreateBuffer(m_vertices.size(), sizeof(Vertex), &m_vertices[0], D3DBindVertexBuffer, &m_renderInfo.pVertexBuffer)
	);

	ThrowIfFailed(
		gEnv.Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], D3DBindIndexBuffer, &m_renderInfo.pIndexBuffer)
	);

	if (m_renderInfo.pVertexShader == nullptr) {
		if ((m_renderFlags & (RF_USE_LIGHT | RF_USE_TEXTURES | RF_DEFERRED)) == (RF_USE_LIGHT | RF_USE_TEXTURES | RF_DEFERRED)) {
			m_renderInfo.pVertexShader	= gEnv.Renderer()->GetShadersManager()->TexturedWithLightVS.GetShader();
			m_renderInfo.pInputLayout	= gEnv.Renderer()->GetShadersManager()->TexturedWithLightVS.GetVertexDecl();
			if (m_renderInfo.pPixelShader == nullptr) {
				m_renderInfo.pPixelShader = gEnv.Renderer()->GetShadersManager()->DeferredTexturedWithLightPS.GetShader();
			}
		}
		else if ((m_renderFlags & (RF_USE_LIGHT | RF_USE_TEXTURES)) == (RF_USE_LIGHT | RF_USE_TEXTURES)) {
			m_renderInfo.pVertexShader	= gEnv.Renderer()->GetShadersManager()->TexturedWithLightVS.GetShader();
			m_renderInfo.pInputLayout	= gEnv.Renderer()->GetShadersManager()->TexturedWithLightVS.GetVertexDecl();
			if (m_renderInfo.pPixelShader == nullptr) {
				m_renderInfo.pPixelShader = gEnv.Renderer()->GetShadersManager()->TexturedWithLightPS.GetShader();
			}
		}
		else if (m_renderFlags & RF_USE_TEXTURES) {
			m_renderInfo.pVertexShader	= gEnv.Renderer()->GetShadersManager()->TexturedNoLightVS.GetShader();
			m_renderInfo.pInputLayout	= gEnv.Renderer()->GetShadersManager()->TexturedNoLightVS.GetVertexDecl();
			if (m_renderInfo.pPixelShader == nullptr) {
				m_renderInfo.pPixelShader = gEnv.Renderer()->GetShadersManager()->TexturedNoLightPS.GetShader();
			}
		}
		else if (m_renderFlags & RF_USE_LIGHT) {
			m_renderInfo.pVertexShader	= gEnv.Renderer()->GetShadersManager()->StandardWithLightVS.GetShader();
			m_renderInfo.pInputLayout	= gEnv.Renderer()->GetShadersManager()->StandardWithLightVS.GetVertexDecl();
			if (m_renderInfo.pPixelShader == nullptr) {
				m_renderInfo.pPixelShader = gEnv.Renderer()->GetShadersManager()->StandardWithLightPS.GetShader();
			}
		}
		else {
			m_renderInfo.pVertexShader	= gEnv.Renderer()->GetShadersManager()->StandardNoLightVS.GetShader();
			m_renderInfo.pInputLayout	= gEnv.Renderer()->GetShadersManager()->StandardNoLightVS.GetVertexDecl();
			if (m_renderInfo.pPixelShader == nullptr) {
				m_renderInfo.pPixelShader = gEnv.Renderer()->GetShadersManager()->StandardNoLightPS.GetShader();
			}
		}
	}
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
	context->IASetInputLayout(m_renderInfo.pInputLayout);
	context->IASetVertexBuffers(0, 1, m_renderInfo.pVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_renderInfo.pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	context->VSSetShader(m_renderInfo.pVertexShader, nullptr, 0);
	context->PSSetShader(m_renderInfo.pPixelShader, nullptr, 0);

	if (m_renderInfo.pDiffuseView.Get() != nullptr) {
		context->PSSetShaderResources(0, 1, m_renderInfo.pDiffuseView.GetAddressOf());
		//context->PSSetSamplers(0, 1, m_renderInfo.pSamplerState.GetAddressOf());
	}

	CBPerMaterial cbPerMaterial = {};
	cbPerMaterial.AmbientColor	= Vector4(m_material.AmbientColor, 0.0f);
	cbPerMaterial.DiffuseColor	= Vector4(m_material.DiffuseColor, 0.0f);
	cbPerMaterial.SpecularColor = m_material.SpecularColor;
	m_constBuffer.SetData(context, cbPerMaterial);
	IConstBuffer* cb = m_constBuffer.GetBuffer();

	context->PSSetConstantBuffers((UINT)CBPerMaterial::Slot, 1, &cb);

	if (m_renderInfo.pSpecularView != nullptr) {
		context->PSSetShaderResources(1, 1, m_renderInfo.pSpecularView.GetAddressOf());
	}

	if (m_renderInfo.pNormalView != nullptr) {
		context->PSSetShaderResources(2, 1, m_renderInfo.pNormalView.GetAddressOf());
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

	if (FAILED(DirectX::CreateDDSTextureFromFile(gEnv.Renderer()->GetDevice(), diffusePath.c_str(), nullptr, &m_renderInfo.pDiffuseView, true))) return;
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

void Mesh::CreateVertices(const std::vector<Vertex>& vertices) {
	assert(m_vertices.size() == 0);

	m_vertices = vertices;
	gEnv.Renderer()->CreateBuffer(m_vertices.size(), sizeof(Vertex), &m_vertices[0], D3DBindVertexBuffer, &m_renderInfo.pVertexBuffer);
}

void Mesh::CreateIndices(const std::vector<UINT>& indices) {
	assert(m_indices.size() == 0);

	m_indices = indices;
	gEnv.Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], D3DBindIndexBuffer, &m_renderInfo.pIndexBuffer);
}

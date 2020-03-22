#include "pch.h"
#include <climits>
#include "GeometryObject.h"
#include "../Helper.h"
#include "../Engine.h"

using namespace DirectX;

GeometryObject::GeometryObject(const std::string& name, DirectX::XMFLOAT3 position)
	: SceneObject::SceneObject(name, position)
	, m_aabb(0, 0, 0, 0)
{
	m_rendInfo = {};
	m_constBuffer.Create(Environment::Instance().Renderer()->GetDevice());
}

GeometryObject::GeometryObject(const std::string& name)
	: GeometryObject::GeometryObject(name, XMFLOAT3(0.0f, 0.0f, 0.0f))
{

}

void GeometryObject::FindMax(const std::vector<VertexType>& verts, float& maxX, float& maxY) const {
	maxX = 0;
	maxY = 0;

	for each (auto& v in verts) {
		if (maxX < v.Position.x) {
			maxX = v.Position.x;
		}

		if (maxY < v.Position.y) {
			maxY = v.Position.y;
		}
	}
}

void GeometryObject::FindMin(const std::vector<VertexType>& verts, float& minX, float& minY) const {
	minX = (std::numeric_limits<float>::max)();
	minY = (std::numeric_limits<float>::max)();

	for each (auto & v in verts) {
		if (minX > v.Position.x) {
			minX = v.Position.x;
		}

		if (minY > v.Position.y) {
			minY = v.Position.y;
		}
	}
}

void GeometryObject::CreateVertices(std::vector<VertexType>& vertices) {
	if (m_vertices.size() > 0) {
		// GeometryObject isn't supposed to be changed during it's lifespan
		return;
	}

	m_vertices = vertices;

	Environment::Instance().Renderer()->CreateBuffer(m_vertices.size(), sizeof(VertexType), &m_vertices[0], BIND_VERTEX_BUFFER, &m_rendInfo.pVertexBuffer);

	float maxX = 0, maxY = 0, minX = 0, minY = 0;
	FindMax(vertices, maxX, maxY);
	FindMin(vertices, minX, minY);
	m_aabb.Create(minX, minY, maxX, maxY);
}

void GeometryObject::CreateIndices(std::vector<UINT>& indices) {
	if (m_indices.size() > 0) {
		// GeometryObject isn't supposed to be changed during it's lifespan
		return;
	}

	m_indices = indices;
	Environment::Instance().Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], BIND_INDEX_BUFFER, &m_rendInfo.pIndexBuffer);
}

void GeometryObject::Update(float dt) {
	m_aabb.Update(GetPosition());
}

void GeometryObject::Initialise() {
	IRenderer* pRend = Environment::Instance().Renderer();

	D3DBlob* pVsBlob;
	D3DBlob* pPsBlob;

	if (FAILED(pRend->CreateBlob("/data/shaders/standard_vs.cso", &pVsBlob))) return;
	if (FAILED(pRend->CreateBlob("/data/shaders/standard_ps.cso", &pPsBlob))) return;

	ThrowIfFailed(pRend->GetDevice()->CreateVertexShader(pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), nullptr, &m_rendInfo.pVertexShader));
	ThrowIfFailed(pRend->GetDevice()->CreatePixelShader(pPsBlob->GetBufferPointer(), pPsBlob->GetBufferSize(), nullptr, &m_rendInfo.pPixelShader));

	pRend->GetDevice()->CreateInputLayout(	VertexType::InputElements,
											VertexType::ElementsCount,
											pVsBlob->GetBufferPointer(),
											pVsBlob->GetBufferSize(),
											&m_rendInfo.pInputLayout);

	D3DRSDesc rsDesc = {};
	rsDesc.CullMode = D3D_CULL_BACK;
	rsDesc.FillMode = D3D_FILL_SOLID;

	if (FAILED(pRend->GetDevice()->CreateRasterizerState(&rsDesc, &m_rendInfo.pRSState))) return;
}

const void* GeometryObject::Vertices()const {
	return &m_vertices[0];
}

const void* GeometryObject::Indices()const {
	return &m_indices[0];
}

void GeometryObject::Scale(float delta) {
	XMMATRIX transform		= XMLoadFloat4x4(&m_transform);
	XMMATRIX scaleMat		= XMMatrixScaling(delta, delta, delta);
	XMMATRIX newTransform	= XMMatrixMultiply(transform, scaleMat);
	XMStoreFloat4x4(&m_transform, newTransform);
}

void GeometryObject::Rotate(float dx, float dy, float dz) {
	XMMATRIX transform		= XMLoadFloat4x4(&m_transform);
	XMMATRIX rotMat			= XMMatrixRotationRollPitchYaw(dx, dy, dz);
	XMMATRIX newTransform	= XMMatrixMultiply(transform, rotMat);
	XMStoreFloat4x4(&m_transform, newTransform);
}

void GeometryObject::Translate(float dx, float dy, float dz) {
	XMMATRIX transform		= XMLoadFloat4x4(&m_transform);
	XMMATRIX translationMat	= XMMatrixTranslation(dx, dy, dz);
	XMMATRIX newTransform	= XMMatrixMultiply(transform, translationMat);
	XMStoreFloat4x4(&m_transform, newTransform);
}

ConstantBuffer* GeometryObject::GetConstBuffer(D3DContext* context, FXMMATRIX viewMat, FXMMATRIX projMat) {
	CBPerObject bufferData	= {};
	XMStoreFloat4x4(&bufferData.worldViewProj, GetWorldTransform() * viewMat * projMat);

	m_constBuffer.SetData(context, bufferData);
	return m_constBuffer.GetBuffer();
}

#include "pch.h"
#include <climits>
#include "GeometryObject.h"
#include "../Helper.h"
#include "../Engine.h"

using namespace DirectX;

GeometryObject::GeometryObject(const std::string& name, DirectX::XMFLOAT3 position)
	: SceneObject::SceneObject(name, position)
	, m_vertexBufferId(-1)
	, m_indexBufferId(-1)
	, m_inputLayoutId(-1)
	, m_vertexShaderId(-1)
	, m_pixelShaderId(-1)
	, m_rsStateId(0)
	, m_aabb(0, 0, 0, 0)
{
	m_constBuffer.Create(Engine::GetPtr()->GetRenderer()->GetDevice());
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
	m_vertexBufferId = Engine::GetPtr()->GetRenderer()->CreateVertexBuffer(m_vertices.size(), sizeof(VertexType), &m_vertices[0]);
	
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
	m_indexBufferId = Engine::GetPtr()->GetRenderer()->CreateIndexBuffer(m_indices.size(), &m_indices[0]);
}

void GeometryObject::Update(float dt) {
	m_aabb.Update(GetPosition());
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

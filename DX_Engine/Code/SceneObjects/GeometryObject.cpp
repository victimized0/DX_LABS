#include "pch.h"
#include "GeometryObject.h"
#include "../Helper.h"

using namespace DirectX;

GeometryObject::GeometryObject(const std::string& name, DirectX::XMFLOAT3 position)
	: SceneObject::SceneObject(name, position)
	, m_vertexBuffer(0)
	, m_indexBuffer(0)
{}

GeometryObject::GeometryObject(const std::string& name)
	: GeometryObject::GeometryObject(name, XMFLOAT3(0.0f, 0.0f, 0.0f))
{}

void GeometryObject::SetVertices(const std::vector<SimpleVertexColour>& vertices) {
	m_vertices.clear();

	for each (auto& vertex in vertices) {
		auto position = XMFLOAT3(m_pos.x + vertex.Position.x,
								 m_pos.y + vertex.Position.y,
								 m_pos.z + vertex.Position.z);

		m_vertices.push_back(SimpleVertexColour(position, vertex.Colour));
	}
}

void GeometryObject::SetIndices(std::vector<UINT>& indices) {
	m_indices.swap(indices);
}

const std::vector<SimpleVertexColour>& GeometryObject::GetVertices()const {
	return m_vertices;
}

const std::vector<UINT>& GeometryObject::GetIndices()const {
	return m_indices;
}

ID3D11Buffer* GeometryObject::GetVertexBuffer()const {
	return m_vertexBuffer.Get();
}

ID3D11Buffer* GeometryObject::GetIndexBuffer()const {
	return m_indexBuffer.Get();
}

void GeometryObject::CreateVertexBuffer(ID3D11Device* device, D3D11_USAGE usage) {
	::CreateBuffer(device, m_vertices, D3D11_BIND_VERTEX_BUFFER, usage, &m_vertexBuffer);
}

void GeometryObject::CreateIndexBuffer(ID3D11Device* device, D3D11_USAGE usage) {
	::CreateBuffer(device, m_indices, D3D11_BIND_INDEX_BUFFER, usage, &m_indexBuffer);
}

const void* GeometryObject::Vertices()const {
	return &m_vertices[0];
}

const void* GeometryObject::Indices()const {
	return &m_indices[0];
}

void GeometryObject::Translate(float dx, float dy, float dz) {
	m_pos.x += dx;
	m_pos.y += dy;
	m_pos.z += dz;

	for (size_t i = 0; i < m_vertices.size(); ++i) {
		m_vertices[i].Position = XMFLOAT3(m_vertices[i].Position.x + dx,
										  m_vertices[i].Position.y + dy,
										  m_vertices[i].Position.z + dz);
	}
}

void GeometryObject::UpdateVertexBuffer(ID3D11DeviceContext* context) {
	D3D11_BUFFER_DESC desc = {};
	m_vertexBuffer->GetDesc(&desc);
	if (desc.Usage != D3D11_USAGE_DYNAMIC) {
		return;
	}

	D3D11_MAPPED_SUBRESOURCE subresource;
	context->Map(m_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, &m_vertices[0], m_vertices.size() * sizeof(SimpleVertexColour));
	context->Unmap(m_vertexBuffer.Get(), 0);
}
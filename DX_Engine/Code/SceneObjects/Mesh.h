#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "../VertexFormats.h"
#include "../ConstBuffer.h"

class Mesh {
private:
	using Matrix		= DirectX::SimpleMath::Matrix;
	using Vector2		= DirectX::SimpleMath::Vector2;
	using Vector3		= DirectX::SimpleMath::Vector3;
	using Vector4		= DirectX::SimpleMath::Vector4;
	using Quaternion	= DirectX::SimpleMath::Quaternion;

public:
	using Vertex		= VertexPosColNmlTex;

public:
									Mesh(const std::string& name);
	virtual							~Mesh() = default;

									Mesh(const Mesh&) = default;
									Mesh& operator=(const Mesh&) = default;
									//Mesh(Mesh&&);
									//Mesh& operator=(Mesh&&);

	void							Initialise(IDevice* device);
	void							Draw(IDevCon* context);
	Matrix							GetMatrix()const;
	
	RenderInfo&						GetRenderInfo() { return m_renderInfo; }
	const RenderInfo&				GetRenderInfo()const { return m_renderInfo; }
	void							SetRenderInfo(const RenderInfo& ri) { m_renderInfo = ri; }
	void							SetMaterial(const Material& mat);

	void							CreateVertices(std::vector<Vertex>& vertices);
	void							CreateIndices(std::vector<UINT>& indices);
	const std::vector<Vertex>&		GetVertices()const { return m_vertices; }
	const std::vector<UINT>&		GetIndices()const { return m_indices; }
	const void*						Vertices()const { &m_vertices[0]; }
	const void*						Indices()const { &m_indices[0]; }

private:
	std::vector<Vertex>				m_vertices;
	std::vector<UINT>				m_indices;

	RenderInfo						m_renderInfo;
	ConstBuffer<CBPerMaterial>		m_constBuffer;

	Matrix							m_transform;
	Material						m_material;
	std::string						m_name;

};

#endif //_MESH_H_

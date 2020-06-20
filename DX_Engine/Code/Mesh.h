#ifndef _MESH_H_
#define _MESH_H_
#pragma once

#include "ConstBuffer.h"

class Mesh {
public:
	using Vertex = VertexPosColNmlTex;

public:
									Mesh();
									Mesh(const std::string& name);
	virtual							~Mesh()							= default;

									Mesh(const Mesh&)				= default;
									Mesh& operator=(const Mesh&)	= default;
									Mesh(Mesh&&)					= default;
									Mesh& operator=(Mesh&&)			= default;

	void							Initialise(IDevice* device);
	void							Draw(IDevCon* context);
	Matrix							GetMatrix()const;
	
	RenderInfo&						GetRenderInfo() { return m_renderInfo; }
	const RenderInfo&				GetRenderInfo()const { return m_renderInfo; }
	void							SetRenderInfo(const RenderInfo& ri) { m_renderInfo = ri; }
	void							SetVertexShader(const VertexShader& vs);
	void							SetPixelShader(const PixelShader& ps);
	void							SetMaterial(const Material& mat);

	void							SetRenderFlags(int flags);
	void							SetColor(const Vector3& vCol);

	void							CreateVertices(const std::vector<Vertex>& vertices);
	void							CreateIndices(const std::vector<UINT>& indices);
	const std::vector<Vertex>&		Vertices()const { return m_vertices; }
	const std::vector<UINT>&		Indices()const { return m_indices; }
	std::vector<Vertex>&			Vertices() { return m_vertices; }
	std::vector<UINT>&				Indices() { return m_indices; }

private:
	std::vector<Vertex>				m_vertices;
	std::vector<UINT>				m_indices;

	int								m_renderFlags;
	RenderInfo						m_renderInfo;
	ConstBuffer<CBPerMaterial>		m_constBuffer;

	Matrix							m_transform;
	Material						m_material;
	std::string						m_name;

};

#endif //_MESH_H_

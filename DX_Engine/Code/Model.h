#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include "ConstBuffer.h"
#include "Mesh.h"

class Model {
public:
	using Vertex = VertexPosColNmlTex;

public:
									Model(const std::string& path);
	virtual							~Model()						= default;

									Model(Model const&)				= delete;
									Model& operator=(Model const&)	= delete;
									Model(Model&&)					= delete;
									Model& operator=(Model&&)		= delete;

	void							Initialise(IDevice* device);
	void							Draw(IDevCon* context, const Matrix& world);
	bool							LoadFromFile(std::string path);

	void							AddMesh(const Mesh& mesh);
	void							SetRenderFlags(int flags);
	const std::vector<Vertex>&		GetVertices()const { return m_vertices; }
	const std::string&				GetPath()const { return m_path; }

private:
	std::vector<Mesh>				m_meshes;
	std::vector<Vertex>				m_vertices;
	std::string						m_path;
	ConstBuffer<CBPerInstance>		m_constBuffer;
};

#endif //_MODEL_H_

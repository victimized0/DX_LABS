#ifndef _MODEL_H_
#define _MODEL_H_
#pragma once

#include "Mesh.h"

#include "../ConstBuffer.h"
#include "../VertexFormats.h"
#include "../Math/SimpleMath.h"
#include "../MeshLoaders/ObjLoader.h"

class Model {
private:
	using Matrix		= DirectX::SimpleMath::Matrix;
	using Vector2		= DirectX::SimpleMath::Vector2;
	using Vector3		= DirectX::SimpleMath::Vector3;
	using Vector4		= DirectX::SimpleMath::Vector4;
	using Quaternion	= DirectX::SimpleMath::Quaternion;

public:
	using Vertex		= VertexPosColNmlTex;

public:
									Model(const std::string& path);
	virtual							~Model() = default;

	void							Initialise(IDevice* device);
	void							Draw(IDevCon* context, const Matrix& world);
	bool							LoadFromFile(std::string path);

	void							AddMesh(const Mesh& mesh);
	const std::vector<Vertex>&		GetVertices()const { return m_vertices; }
	const std::string&				GetPath()const { return m_path; }

private:
	std::vector<Mesh>				m_meshes;
	std::vector<Vertex>				m_vertices;
	std::string						m_path;
	ConstBuffer<CBPerInstance>		m_constBuffer;

};

#endif //_MODEL_H_

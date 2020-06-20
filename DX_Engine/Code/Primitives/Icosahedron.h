#ifndef _ICOSAHEDRON_H_
#define _ICOSAHEDRON_H_
#pragma once

#include <map>
#include <array>

#include "../Mesh.h"
#include "../Math/SimpleMath.h"

class Icosahedron {
private:
	typedef DirectX::SimpleMath::Vector2 Vector2;
	typedef DirectX::SimpleMath::Vector3 Vector3;

public:
										Icosahedron(int nSubdiv);
										~Icosahedron();

	void								ToMesh(Mesh& outMesh, float scale) const;

private:
	int									VertexForEdge(int vi1, int vi2);

	void								Normalise();
	void								Subdivide();
	void								CalculateUVs();

public:
	std::vector<Vector3>				Vertices;
	std::vector<Vector2>				TexCoords;
	std::vector<std::array<int, 3>>		Triangles;

private:
	std::map<std::pair<int, int>, int>	m_lookup;

};

#endif //_ICOSAHEDRON_H_

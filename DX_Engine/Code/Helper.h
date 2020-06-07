#ifndef _HELPER_H_
#define _HELPER_H_

#include "pch.h"
#include "VertexFormats.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace {
	template<typename T>
	T wrap_angle(T theta) noexcept
	{
		const T mod = (T)fmod(theta, XM_2PI);
		if (mod > (T)XM_PI)
		{
			return mod - XM_2PI;
		}
		else if (mod < -(T)XM_PI)
		{
			return mod + XM_2PI;
		}
		return mod;
	}

	template <typename Vertex>
	void CreateIcosahedron(float radius, std::vector<Vertex>& ppVerts, std::vector<UINT>& ppInds) {
		const float t = (radius + (float)std::sqrt(5.0)) / 2.0f;

		// Vertices
		std::vector<Vertex> vertices(12);
		vertices[0].Position	= Vector3(-radius, t, 0.0f);
		vertices[1].Position	= Vector3(radius, t, 0.0f);
		vertices[2].Position	= Vector3(-radius, -t, 0.0f);
		vertices[3].Position	= Vector3(radius, -t, 0.0f);
		vertices[4].Position	= Vector3(0.0f, -radius, t);
		vertices[5].Position	= Vector3(0.0f, radius, t);
		vertices[6].Position	= Vector3(0.0f, -radius, -t);
		vertices[7].Position	= Vector3(0.0f, radius, -t);
		vertices[8].Position	= Vector3(t, 0.0f, -radius);
		vertices[9].Position	= Vector3(t, 0.0f, radius);
		vertices[10].Position	= Vector3(-t, 0.0f, -radius);
		vertices[11].Position	= Vector3(-t, 0.0f, radius);

		std::vector<Vector3> positions = {
			vertices[0].Position,
			vertices[1].Position,
			vertices[2].Position,
			vertices[3].Position,
			vertices[4].Position,
			vertices[5].Position,
			vertices[6].Position,
			vertices[7].Position,
			vertices[8].Position,
			vertices[9].Position,
			vertices[10].Position,
			vertices[11].Position
		};

		// Faces
		std::vector<UINT> indices = {
			0,	11, 5,
			0,	5,	1,
			0,	1,	7,
			0,	7,	10,
			0,	10, 11,
			1,	5,	9,
			5,	11,	4,
			11,	10,	2,
			10, 7,	6,
			7,	1,	8,
			3,	9,	4,
			3,	4,	2,
			3,	2,	6,
			3,	6,	8,
			3,	8,	9,
			4,	9,	5,
			2,	4,	11,
			6,	2,	10,
			8,	6,	7,
			9,	8,	1
		};

		BoundingBox bb;
		BoundingBox::CreateFromPoints(bb, positions.size(), positions.data(), sizeof(Vector3));

		vertices[0].Normal = bb.Center - positions[0];
		vertices[1].Normal = bb.Center - positions[1];
		vertices[2].Normal = bb.Center - positions[2];
		vertices[3].Normal = bb.Center - positions[3];
		vertices[4].Normal = bb.Center - positions[4];
		vertices[5].Normal = bb.Center - positions[5];
		vertices[6].Normal = bb.Center - positions[6];
		vertices[7].Normal = bb.Center - positions[7];
		vertices[8].Normal = bb.Center - positions[8];
		vertices[9].Normal = bb.Center - positions[9];
		vertices[10].Normal = bb.Center - positions[10];
		vertices[11].Normal = bb.Center - positions[11];

		ppVerts	= std::move( vertices );
		ppInds	= std::move( indices );
	}

}

#endif //_HELPER_H_

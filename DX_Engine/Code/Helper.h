#ifndef _HELPER_H_
#define _HELPER_H_

#include "pch.h"
#include "VertexFormats.h"
#include "SceneObjects/GeometryObject.h"

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

	void CreateIcosahedron(GeometryObject** geoObj, float radius, float r = 1.0f, float g = 1.0f, float b = 1.0f) {
		const float t = (radius + (float)std::sqrt(5.0)) / 2.0f;

		// Vertices
		std::vector<GeometryObject::VertexType> vertices = {
			{ DirectX::XMFLOAT3(-radius, t, 0.0f), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(radius, t, 0.0f), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(-radius, -t, 0.0f), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(radius, -t, 0.0f), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(0.0f, -radius, t), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(0.0f, radius, t), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(0.0f, -radius, -t), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(0.0f, radius, -t), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(t, 0.0f, -radius), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(t, 0.0f, radius), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(-t, 0.0f, -radius), DirectX::XMFLOAT4(r, g, b, 1.0f) },
			{ DirectX::XMFLOAT3(-t, 0.0f, radius), DirectX::XMFLOAT4(r, g, b, 1.0f) }
		};

		// Faces
		std::vector<UINT> indices = {
			0, 11, 5,
			0, 5, 1,
			0, 1, 7,
			0, 7, 10,
			0, 10, 11,
			1, 5, 9,
			5, 11, 4,
			11, 10, 2,
			10, 7, 6,
			7, 1, 8,
			3, 9, 4,
			3, 4, 2,
			3, 2, 6,
			3, 6, 8,
			3, 8, 9,
			4, 9, 5,
			2, 4, 11,
			6, 2, 10,
			8, 6, 7,
			9, 8, 1
		};

		(*geoObj)->CreateVertices(vertices);
		(*geoObj)->CreateIndices(indices);
	}

}

#endif //_HELPER_H_

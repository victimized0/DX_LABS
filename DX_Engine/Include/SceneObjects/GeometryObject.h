#ifndef _GEOMETY_OBJECT_H_
#define _GEOMETY_OBJECT_H_

#include "SceneObject.h"
#include "../ConstBuffer.h"
#include "../VertexFormats.h"

using Microsoft::WRL::ComPtr;

struct AABB {
	float X;
	float Y;
	float Width;
	float Height;

	explicit AABB(float x, float y, float w, float h)
		: X(x)
		, Y(y)
		, Width(w)
		, Height(h)
	{}

	void Create(float minx, float miny, float maxx, float maxy) {
		Width  = fabs(maxx - minx);
		Height = fabs(maxy - miny);
		X = Width / 2;
		Y = Height / 2;
	}

	bool Collides(const AABB& other)const {
		return	X < other.X + other.Width &&
				X + Width > other.X &&
				Y < other.Y + other.Height &&
				Y + Height > other.Y;
	}

	void Update(const DirectX::XMFLOAT3& pos) {
		X = pos.x;
		Y = pos.y;
	}
};

class GeometryObject : public SceneObject {
public:

	using VertexType = VertexPositionColour;

											GeometryObject(const std::string& name, DirectX::XMFLOAT3 position);
											GeometryObject(const std::string& name);
											~GeometryObject() {}

	void									CreateVertices(std::vector<VertexType>& vertices);
	void									CreateIndices(std::vector<UINT>& indices);

	const std::vector<VertexType>&			GetVertices()const { return m_vertices; }
	const std::vector<UINT>&				GetIndices()const { return m_indices; }

	const int								GetVertexShaderId()	const	{ return m_vertexShaderId;	}
	const int								GetPixelShaderId()	const	{ return m_pixelShaderId;	}
	const int								GetVertexBufferId()	const	{ return m_vertexBufferId;	}
	const int								GetIndexBufferId()	const	{ return m_indexBufferId;	}
	const int								GetInputLayoutId()	const	{ return m_inputLayoutId;	}
	const int								GetRSStateId()		const	{ return m_rsStateId;		}

	ConstantBuffer*							GetConstBuffer(D3DContext* context, DirectX::FXMMATRIX viewMat, DirectX::FXMMATRIX projMat);
	DirectX::XMMATRIX						GetWorldTransform() const	{ return XMLoadFloat4x4(&m_transform); }
	AABB&									GetAABB() { return m_aabb; }

	void									SetVertexShaderId	(int id)	{ m_vertexShaderId	= id; }
	void									SetPixelShaderId	(int id)	{ m_pixelShaderId	= id; }
	void									SetVertexBufferId	(int id)	{ m_vertexBufferId	= id; }
	void									SetIndexBufferId	(int id)	{ m_indexBufferId	= id; }
	void									SetInputLayoutId	(int id)	{ m_inputLayoutId	= id; }
	void									SetRSStateId		(int id)	{ m_rsStateId		= id; }

	const void*								Vertices()const;
	const void*								Indices()const;

	void									Update(float dt)override;

	void									Scale(float delta);
	void									Rotate(float dx, float dy, float dz);
	void									Translate(float dx, float dy, float dz);

private:
	void									FindMax(const std::vector<VertexType>& verts, float& maxX, float& maxY) const;
	void									FindMin(const std::vector<VertexType>& verts, float& minX, float& minY) const;

protected:
	int										m_vertexShaderId;
	int										m_pixelShaderId;
	int										m_vertexBufferId;
	int										m_indexBufferId;
	int										m_inputLayoutId;
	int										m_rsStateId;

	ConstBuffer<CBPerObject>				m_constBuffer;

	std::vector<VertexType>					m_vertices;
	std::vector<UINT>						m_indices;

	AABB									m_aabb;

};

#endif //_GEOMETY_OBJECT_H_

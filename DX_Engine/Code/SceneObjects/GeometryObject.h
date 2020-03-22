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
		float threshold = 0.1f;

		float a_wDiv2	= Width / 2;
		float a_hDiv2	= Height / 2;
		float a_left	= X + a_wDiv2;
		float a_right	= X - a_wDiv2;
		float a_top		= Y + a_hDiv2;
		float a_bottom	= Y - a_hDiv2;

		float b_wDiv2	= other.Width / 2;
		float b_hDiv2	= other.Height / 2;
		float b_left	= other.X + b_wDiv2;
		float b_right	= other.X - b_wDiv2;
		float b_top		= other.Y + b_hDiv2;
		float b_bottom	= other.Y - b_hDiv2;

		bool b_left_to_a_right = fabs(b_left - a_right) < threshold;
		bool b_right_to_a_left = fabs(b_right - a_left) < threshold;

		bool b_bottom_to_a_top = fabs(b_bottom - a_top) < Height;
		bool b_top_to_a_bottom = fabs(b_top - a_bottom) < Height;
		bool collides_top = b_bottom_to_a_top && b_top_to_a_bottom;

		return (b_left_to_a_right && collides_top) || (b_right_to_a_left && collides_top);
	}

	void Update(const DirectX::XMFLOAT3& pos) {
		X = pos.x;
		Y = pos.y;
	}
};

struct RenderInfo {
	ComPtr<VertexBuffer>	pVertexBuffer;
	ComPtr<IndexBuffer>		pIndexBuffer;
	ComPtr<InputLayout>		pInputLayout;
	ComPtr<VertexShader>	pVertexShader;
	ComPtr<PixelShader>		pPixelShader;
	ComPtr<RSState>			pRSState;
};

class GeometryObject : public SceneObject {
public:

	using VertexType = VertexPositionColour;

											GeometryObject(const std::string& name, DirectX::XMFLOAT3 position);
											GeometryObject(const std::string& name);
	virtual									~GeometryObject() {}

	void									CreateVertices(std::vector<VertexType>& vertices);
	void									CreateIndices(std::vector<UINT>& indices);

	const std::vector<VertexType>&			GetVertices()const { return m_vertices; }
	const std::vector<UINT>&				GetIndices()const { return m_indices; }

	RenderInfo&								GetRenderInfo() { return m_rendInfo;	}
	const RenderInfo&						GetRenderInfo()const { return m_rendInfo;	}

	ConstantBuffer*							GetConstBuffer(D3DContext* context, DirectX::FXMMATRIX viewMat, DirectX::FXMMATRIX projMat);
	DirectX::XMMATRIX						GetWorldTransform() const	{ return XMLoadFloat4x4(&m_transform); }
	AABB&									GetAABB() { return m_aabb; }

	void									SetRenderInfo(const RenderInfo& ri) { m_rendInfo = ri; }

	const void*								Vertices()const;
	const void*								Indices()const;

	void									Update(float dt)override;
	void									Initialise()override;
	//virtual void							Draw();

	void									Scale(float delta);
	void									Rotate(float dx, float dy, float dz);
	void									Translate(float dx, float dy, float dz);

private:
	void									FindMax(const std::vector<VertexType>& verts, float& maxX, float& maxY) const;
	void									FindMin(const std::vector<VertexType>& verts, float& minX, float& minY) const;

protected:
	AABB									m_aabb;
	RenderInfo								m_rendInfo;
	ConstBuffer<CBPerObject>				m_constBuffer;

	std::vector<VertexType>					m_vertices;
	std::vector<UINT>						m_indices;

};

#endif //_GEOMETY_OBJECT_H_

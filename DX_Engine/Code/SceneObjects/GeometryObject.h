#ifndef _GEOMETY_OBJECT_H_
#define _GEOMETY_OBJECT_H_

#include "SceneObject.h"
#include "../ConstBuffer.h"
#include "../VertexFormats.h"
#include "../Math/SimpleMath.h"

using Microsoft::WRL::ComPtr;

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

	using VertexType = VertexPosCol;

											GeometryObject(const std::string& name, const DirectX::SimpleMath::Vector3& position);
											GeometryObject(const std::string& name);
	virtual									~GeometryObject() {}

	void									CreateVertices(std::vector<VertexType>& vertices);
	void									CreateIndices(std::vector<UINT>& indices);

	const std::vector<VertexType>&			GetVertices()const { return m_vertices; }
	const std::vector<UINT>&				GetIndices()const { return m_indices; }

	RenderInfo&								GetRenderInfo() { return m_rendInfo; }
	const RenderInfo&						GetRenderInfo()const { return m_rendInfo; }

	ConstantBuffer*							GetConstBuffer(D3DContext* context, const DirectX::SimpleMath::Matrix& viewMat, const DirectX::SimpleMath::Matrix& projMat);
	DirectX::SimpleMath::Matrix				GetWorldTransform() const	{ return Transform.ToMatrix(); }
	DirectX::BoundingBox&					GetBoundingBox() { return m_boundingBox; }

	void									SetRenderInfo(const RenderInfo& ri) { m_rendInfo = ri; }

	const void*								Vertices()const;
	const void*								Indices()const;

	void									Update(float dt)override;
	void									Initialise()override;
	//virtual void							Draw();

	void									Scale(float delta);
	void									Translate(float dx, float dy, float dz);
	void									Rotate(float dx, float dy, float dz);
	void									Orbit(float angle, const DirectX::SimpleMath::Vector3& target);

private:
	DirectX::SimpleMath::Vector2			FindMax(const std::vector<VertexType>& verts) const;
	DirectX::SimpleMath::Vector2			FindMin(const std::vector<VertexType>& verts) const;

protected:
	DirectX::BoundingBox					m_boundingBox;
	RenderInfo								m_rendInfo;
	ConstBuffer<CBPerObject>				m_constBuffer;

	std::vector<VertexType>					m_vertices;
	std::vector<UINT>						m_indices;

};

#endif //_GEOMETY_OBJECT_H_

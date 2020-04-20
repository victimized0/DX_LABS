#ifndef _GEOMETY_OBJECT_H_
#define _GEOMETY_OBJECT_H_

#include "SceneObject.h"
#include "../ConstBuffer.h"
#include "../VertexFormats.h"
#include "../Math/SimpleMath.h"
#include "../MeshLoaders/ObjLoader.h"

using Microsoft::WRL::ComPtr;

struct RenderInfo {
	ComPtr<IVertexBuffer>	pVertexBuffer;
	ComPtr<IIndexBuffer>	pIndexBuffer;
	ComPtr<IInputLayout>	pInputLayout;
	ComPtr<IVertexShader>	pVertexShader;
	ComPtr<IPixelShader>	pPixelShader;
	ComPtr<IRSState>		pRSState;
	D3DPrimitiveTopology	Topology = D3DTopologyTriangleList;
};

class GeometryObject : public SceneObject {
protected:
	using Vector2 = DirectX::SimpleMath::Vector2; 
	using Vector3 = DirectX::SimpleMath::Vector3; 
	using Vector4 = DirectX::SimpleMath::Vector4; 
	using Matrix = DirectX::SimpleMath::Matrix;

public:
	using VertexType = VertexPosCol;

public:
											GeometryObject(const std::string& name, const Vector3& m_position);
											GeometryObject(const std::string& name, const objloader::Mesh& obj, const Vector3& m_position);
											GeometryObject(const std::string& name);
	virtual									~GeometryObject() {}

	void									CreateVertices(std::vector<VertexType>& vertices);
	void									CreateIndices(std::vector<UINT>& indices);
	const std::vector<VertexType>&			GetVertices()const { return m_vertices; }
	const std::vector<UINT>&				GetIndices()const { return m_indices; }
	const void*								Vertices()const;
	const void*								Indices()const;

	RenderInfo&								GetRenderInfo() { return m_rendInfo; }
	const RenderInfo&						GetRenderInfo()const { return m_rendInfo; }
	void									SetRenderInfo(const RenderInfo& ri) { m_rendInfo = ri; }

	DirectX::BoundingBox&					GetBoundingBox() { return m_boundingBox; }

	void									Initialise()override;
	void									Update(float dt)override;
	virtual void							Draw(IDevCon* context);

	void									Translate(const Vector3& dPos);
	void									Rotate(float dRoll, float dPitch, float dYaw);
	Matrix									GetWorld()const;

protected:
	Vector2									FindMax(const std::vector<VertexType>& verts) const;
	Vector2									FindMin(const std::vector<VertexType>& verts) const;

protected:
	DirectX::BoundingBox					m_boundingBox;
	RenderInfo								m_rendInfo;
	ConstBuffer<CBPerInstance>				m_constBuffer;

	std::vector<VertexType>					m_vertices;
	std::vector<UINT>						m_indices;

public:
	Vector3									Position;
	float									Yaw;
	float									Pitch;
	float									Roll;

};

#endif //_GEOMETY_OBJECT_H_

#ifndef _GEOMETY_OBJECT_H_
#define _GEOMETY_OBJECT_H_

#include "SceneObject.h"

using Microsoft::WRL::ComPtr;

class GeometryObject : public SceneObject {
public:
	GeometryObject(const std::string& name, DirectX::XMFLOAT3 position);
	GeometryObject(const std::string& name);
	virtual ~GeometryObject() {}

	void										SetVertices(const std::vector<SimpleVertexColour>& vertices);
	void										SetIndices(std::vector<UINT>& indices);

	const std::vector<SimpleVertexColour>&		GetVertices()const;
	const std::vector<UINT>&					GetIndices()const;

	ID3D11Buffer*								GetVertexBuffer()const;
	ID3D11Buffer*								GetIndexBuffer()const;

	void										CreateVertexBuffer(ID3D11Device* device, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
	void										CreateIndexBuffer(ID3D11Device* device, D3D11_USAGE usage = D3D11_USAGE_IMMUTABLE);
	void										UpdateVertexBuffer(ID3D11DeviceContext* context);

	const void*									Vertices()const;
	const void*									Indices()const;

	//void										Initialise()override;
	//void										Update(float dt)override;
	virtual void								Translate(float dx, float dy, float dz);

protected:
	ComPtr<ID3D11Buffer>						m_vertexBuffer;
	ComPtr<ID3D11Buffer>						m_indexBuffer;

	std::vector<SimpleVertexColour>				m_vertices;
	std::vector<UINT>							m_indices;

};

#endif //_GEOMETY_OBJECT_H_

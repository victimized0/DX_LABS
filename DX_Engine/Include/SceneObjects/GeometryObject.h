#ifndef _GEOMETY_OBJECT_H_
#define _GEOMETY_OBJECT_H_

#include "SceneObject.h"
#include "../ConstBuffer.h"
#include "../VertexFormats.h"

using Microsoft::WRL::ComPtr;

class GeometryObject : public SceneObject {
public:
											GeometryObject(const std::string& name, DirectX::XMFLOAT3 position);
											GeometryObject(const std::string& name);
											~GeometryObject() {}

	void									CreateVertices(std::vector<DirectX::XMFLOAT3>& vertices);
	void									CreateIndices(std::vector<UINT>& indices);

	const std::vector<DirectX::XMFLOAT3>&	GetVertices()const;
	const std::vector<UINT>&				GetIndices()const;

	const int								GetVertexShaderId()	const	{ return m_vertexShaderId;	}
	const int								GetPixelShaderId()	const	{ return m_pixelShaderId;	}
	const int								GetVertexBufferId()	const	{ return m_vertexBufferId;	}
	const int								GetIndexBufferId()	const	{ return m_indexBufferId;	}
	const int								GetInputLayoutId()	const	{ return m_inputLayoutId;	}
	const int								GetRSStateId()		const	{ return m_rsStateId;		}

	ConstantBuffer*							GetConstBuffer(D3DContext* context, DirectX::FXMMATRIX viewMat, DirectX::FXMMATRIX projMat);
	DirectX::XMMATRIX						GetWorldTransform() const	{ return XMLoadFloat4x4(&m_transform); }

	void									SetVertexShaderId	(int id)	{ m_vertexShaderId	= id; }
	void									SetPixelShaderId	(int id)	{ m_pixelShaderId	= id; }
	void									SetVertexBufferId	(int id)	{ m_vertexBufferId	= id; }
	void									SetIndexBufferId	(int id)	{ m_indexBufferId	= id; }
	void									SetInputLayoutId	(int id)	{ m_inputLayoutId	= id; }
	void									SetRSStateId		(int id)	{ m_rsStateId		= id; }

	const void*								Vertices()const;
	const void*								Indices()const;

	void									Scale(float delta);
	void									Rotate(float dx, float dy, float dz);
	void									Translate(float dx, float dy, float dz);

protected:
	int										m_vertexShaderId;
	int										m_pixelShaderId;
	int										m_vertexBufferId;
	int										m_indexBufferId;
	int										m_inputLayoutId;
	int										m_rsStateId;

	ConstBuffer<CBPerObject>				m_constBuffer;

	std::vector<DirectX::XMFLOAT3>			m_vertices;
	std::vector<UINT>						m_indices;

};

#endif //_GEOMETY_OBJECT_H_

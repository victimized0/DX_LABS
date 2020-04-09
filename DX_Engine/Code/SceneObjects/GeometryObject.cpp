#include "pch.h"
#include "GeometryObject.h"
#include "../Helper.h"
#include "../Engine.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

GeometryObject::GeometryObject(const std::string& name, const Vector3& position)
	: SceneObject::SceneObject(name, position)
{
	m_rendInfo = {};
	m_constBuffer.Create(gEnv.Renderer()->GetDevice());
}

GeometryObject::GeometryObject(const std::string& name)
	: GeometryObject::GeometryObject(name, Vector3())
{

}

Vector2 GeometryObject::FindMax(const std::vector<VertexType>& verts) const {
	Vector2 max;
	for each (auto& v in verts) {
		if (max.x < v.Position.x) {
			max.x = v.Position.x;
		}

		if (max.y < v.Position.y) {
			max.y = v.Position.y;
		}
	}

	return max;
}

Vector2 GeometryObject::FindMin(const std::vector<VertexType>& verts) const {
	Vector2 min;

	for each (auto & v in verts) {
		if (min.x > v.Position.x) {
			min.x = v.Position.x;
		}

		if (min.y > v.Position.y) {
			min.y = v.Position.y;
		}
	}

	return min;
}

void GeometryObject::CreateVertices(std::vector<VertexType>& vertices) {
	if (m_vertices.size() > 0) {
		// GeometryObject isn't supposed to be changed during it's lifespan
		return;
	}

	m_vertices = vertices;

	gEnv.Renderer()->CreateBuffer(m_vertices.size(), sizeof(VertexType), &m_vertices[0], BIND_VERTEX_BUFFER, &m_rendInfo.pVertexBuffer);

	Vector2 max = FindMax(vertices);
	Vector2 min = FindMin(vertices);
	BoundingBox::CreateFromPoints(m_boundingBox, min, max);
}

void GeometryObject::CreateIndices(std::vector<UINT>& indices) {
	if (m_indices.size() > 0) {
		// GeometryObject isn't supposed to be changed during it's lifespan
		return;
	}

	m_indices = indices;
	gEnv.Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], BIND_INDEX_BUFFER, &m_rendInfo.pIndexBuffer);
}

void GeometryObject::Update(float dt) {
	m_boundingBox.Center = GetPosition();
}

void GeometryObject::Initialise() {
	IRenderer* pRend = gEnv.Renderer();

	D3DBlob* pVsBlob;
	D3DBlob* pPsBlob;

	if (FAILED(pRend->CreateBlob("/data/shaders/standard_vs.cso", &pVsBlob))) return;
	if (FAILED(pRend->CreateBlob("/data/shaders/standard_ps.cso", &pPsBlob))) return;

	ThrowIfFailed(pRend->GetDevice()->CreateVertexShader(pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), nullptr, &m_rendInfo.pVertexShader));
	ThrowIfFailed(pRend->GetDevice()->CreatePixelShader(pPsBlob->GetBufferPointer(), pPsBlob->GetBufferSize(), nullptr, &m_rendInfo.pPixelShader));

	pRend->GetDevice()->CreateInputLayout(	VertexType::InputElements,
											VertexType::ElementsCount,
											pVsBlob->GetBufferPointer(),
											pVsBlob->GetBufferSize(),
											&m_rendInfo.pInputLayout);

	D3DRSDesc rsDesc = {};
	rsDesc.CullMode = D3D_CULL_BACK;
	rsDesc.FillMode = D3D_FILL_SOLID;

	if (FAILED(pRend->GetDevice()->CreateRasterizerState(&rsDesc, &m_rendInfo.pRSState))) return;
}

const void* GeometryObject::Vertices()const {
	return &m_vertices[0];
}

const void* GeometryObject::Indices()const {
	return &m_indices[0];
}

ConstantBuffer* GeometryObject::GetConstBuffer(D3DContext* context, const Matrix& viewMat, const Matrix& projMat) {
	CBPerObject bufferData	= {};
	bufferData.worldViewProj = GetWorldTransform() * viewMat * projMat;

	m_constBuffer.SetData(context, bufferData);
	return m_constBuffer.GetBuffer();
}

void GeometryObject::Scale(float factor) {
	Transform.Scale({ factor, factor, factor });
}

void GeometryObject::Translate(float dx, float dy, float dz) {
	Transform.Translate({ dx, dy, dz });
}

void GeometryObject::Rotate(float yaw, float pitch, float roll) {
	Transform.Rotate(Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll));
}

void GeometryObject::Orbit(float angle, const Vector3& target) {
	Transform.Orbit(angle, target);
}

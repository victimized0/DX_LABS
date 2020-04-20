#include "pch.h"
#include "GeometryObject.h"
#include "../Helper.h"
#include "../Engine.h"

using std::vector;
using namespace DirectX;
using namespace DirectX::SimpleMath;

GeometryObject::GeometryObject(const std::string& name, const Vector3& m_position)
	: SceneObject::SceneObject(name, m_position)
	, Position(m_position)
	, Yaw(0.0f)
	, Pitch(0.0f)
	, Roll(0.0f)
{
	m_rendInfo = {};
	m_constBuffer.Create(gEnv.Renderer()->GetDevice());
}

GeometryObject::GeometryObject(const std::string& name)
	: GeometryObject::GeometryObject(name, Vector3())
{

}

GeometryObject::GeometryObject(const std::string& name, const objloader::Mesh& mesh, const DirectX::SimpleMath::Vector3& m_position)
	: GeometryObject::GeometryObject(name, m_position)
{
	vector<VertexType>	vertices;
	vector<UINT>		indices = mesh.Indices;

	vertices.resize(mesh.Vertices.size());
	for(int index = 0; index < mesh.Vertices.size(); ++index) {
		vertices[index].Position = Vector3(mesh.Vertices[index].m_position.X, mesh.Vertices[index].m_position.Y, mesh.Vertices[index].m_position.Z);
		vertices[index].Colour = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
	}

	CreateVertices(vertices);
	CreateIndices(indices);
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

	gEnv.Renderer()->CreateBuffer(m_vertices.size(), sizeof(VertexType), &m_vertices[0], D3DBindVertexBuffer, &m_rendInfo.pVertexBuffer);

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
	gEnv.Renderer()->CreateBuffer(m_indices.size(), sizeof(UINT), &m_indices[0], D3DBindIndexBuffer, &m_rendInfo.pIndexBuffer);
}

void GeometryObject::Update(float dt) {
	m_boundingBox.Center = Position;
}

void GeometryObject::Initialise() {
	IRenderer* pRend = gEnv.Renderer();

	IBlob* pVsBlob;
	IBlob* pPsBlob;

	if (FAILED(pRend->CreateBlob("/Data/Shaders/standard_vs.cso", &pVsBlob))) return;
	if (FAILED(pRend->CreateBlob("/Data/Shaders/standard_ps.cso", &pPsBlob))) return;

	ThrowIfFailed(pRend->GetDevice()->CreateVertexShader(pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), nullptr, &m_rendInfo.pVertexShader));
	ThrowIfFailed(pRend->GetDevice()->CreatePixelShader(pPsBlob->GetBufferPointer(), pPsBlob->GetBufferSize(), nullptr, &m_rendInfo.pPixelShader));

	pRend->GetDevice()->CreateInputLayout(	VertexType::InputElements,
											VertexType::ElementsCount,
											pVsBlob->GetBufferPointer(),
											pVsBlob->GetBufferSize(),
											&m_rendInfo.pInputLayout);

	//D3DRSDesc rsDesc = {};
	//rsDesc.CullMode = D3DCullBack;
	//rsDesc.FillMode = D3DFillSolid;

	//if (FAILED(pRend->GetDevice()->CreateRasterizerState(&rsDesc, &m_rendInfo.pRSState))) return;
}

const void* GeometryObject::Vertices()const {
	return &m_vertices[0];
}

const void* GeometryObject::Indices()const {
	return &m_indices[0];
}

void GeometryObject::Draw(IDevCon* context) {
	if (m_rendInfo.pVertexBuffer == nullptr ||
		m_rendInfo.pIndexBuffer  == nullptr ||
		m_rendInfo.pVertexShader == nullptr ||
		m_rendInfo.pPixelShader  == nullptr ||
		m_rendInfo.pInputLayout  == nullptr)
	{
		return;
	}

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	context->IASetPrimitiveTopology(m_rendInfo.Topology);
	context->IASetInputLayout(m_rendInfo.pInputLayout.Get());
	context->IASetVertexBuffers(0, 1, m_rendInfo.pVertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_rendInfo.pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	CBPerInstance cbPerInstance = {};
	auto camera = Engine::GetPtr()->GetScene().GetMainCamera();
	cbPerInstance.WorldViewProj = GetWorld() * camera->GetView() * camera->GetProj();
	m_constBuffer.SetData(context, cbPerInstance);
	IConstBuffer* cb = m_constBuffer.GetBuffer();

	context->VSSetShader(m_rendInfo.pVertexShader.Get(), nullptr, 0);
	context->VSSetConstantBuffers((UINT)CBPerInstance::Slot, 1, &cb);

	context->PSSetShader(m_rendInfo.pPixelShader.Get(), nullptr, 0);

	if (m_rendInfo.pRSState != nullptr) {
		context->RSSetState(m_rendInfo.pRSState.Get());
	}

	context->DrawIndexed((UINT)m_indices.size(), 0, 0);
}

Matrix GeometryObject::GetWorld()const {
	return Matrix::CreateFromRollPitchYaw(Pitch, Yaw, Roll) *
		   Matrix::CreateTranslation(Position);
}

void GeometryObject::Translate(const Vector3& dPos) {
	Position += Vector3::Transform(dPos, Matrix::CreateFromRollPitchYaw(Yaw, Roll, Pitch));
}

void GeometryObject::Rotate(float dx, float dy, float dz) {
	float rotationSpeed  = 0.004f;
	Yaw = wrap_angle(Yaw + dx * rotationSpeed);
	Pitch = std::clamp(Pitch + dy * rotationSpeed, 0.995f * -XM_PI / 2.0f, 0.995f * XM_PI / 2.0f);
	Roll = wrap_angle(Roll + dz * rotationSpeed);
}

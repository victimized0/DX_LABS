#include "pch.h"
#include "Katamari.h"

using std::vector;

Katamari::Katamari(const std::string& name, const objloader::Mesh& mesh, const DirectX::SimpleMath::Vector3& position)
	: GeometryObject::GeometryObject(name, mesh, position)
{
	//vector<VertexType>	vertices;
	//vector<UINT>		indices = mesh.Indices;

	//vertices.resize(mesh.Vertices.size());
	//for (int index = 0; index < mesh.Vertices.size(); ++index) {
	//	vertices[index].Position	= Vector3(mesh.Vertices[index].m_position.X, mesh.Vertices[index].m_position.Y, mesh.Vertices[index].m_position.Z);
	//	vertices[index].Normal		= Vector3(mesh.Vertices[index].Normal.X, mesh.Vertices[index].Normal.Y, mesh.Vertices[index].Normal.Z);
	//	vertices[index].TexCoords	= Vector2(mesh.Vertices[index].TextureCoordinate.X, mesh.Vertices[index].TextureCoordinate.Y);
	//}

	////CreateVertices(vertices);
	//CreateIndices(indices);
}

void Katamari::Initialise() {
	GeometryObject::Initialise();
	//IRenderer* pRend = gEnv.Renderer();

	//IBlob* pVsBlob;
	//IBlob* pPsBlob;

	//if ( FAILED( pRend->CreateBlob("/Data/Shaders/textured_vs.cso", &pVsBlob) ) ) return;
	//if ( FAILED( pRend->CreateBlob("/Data/Shaders/textured_ps.cso", &pPsBlob) ) ) return;

	//ThrowIfFailed(
	//	pRend->GetDevice()->CreateVertexShader( pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(), nullptr, &m_rendInfo.pVertexShader )
	//);

	//ThrowIfFailed(
	//	pRend->GetDevice()->CreatePixelShader( pPsBlob->GetBufferPointer(), pPsBlob->GetBufferSize(), nullptr, &m_rendInfo.pPixelShader )
	//);

	//pRend->GetDevice()->CreateInputLayout(	VertexType::InputElements, VertexType::ElementsCount,
	//										pVsBlob->GetBufferPointer(), pVsBlob->GetBufferSize(),
	//										&m_rendInfo.pInputLayout);
}

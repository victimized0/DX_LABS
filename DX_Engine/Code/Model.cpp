#include "pch.h"
#include "Model.h"

#include "Math/SimpleMath.h"
#include "MeshLoaders/ObjLoader.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Model::Model(const std::string& path)
	: m_path(path)
{
	LoadFromFile(path);
}

void Model::AddMesh(const Mesh& mesh) {
	m_meshes.push_back(mesh);
	m_vertices.insert(m_vertices.end(), mesh.Vertices().begin(), mesh.Vertices().end());
}

bool Model::LoadFromFile(std::string path) {
	if (path.size() < 5) {
		return false;
	}

	ObjLoader obj;
	obj.LoadFile(path);

	m_vertices.resize(obj.LoadedVertices.size());
	for (size_t vIndex = 0; vIndex < m_vertices.size(); ++vIndex) {
		m_vertices[vIndex].Position		= Vector3(obj.LoadedVertices[vIndex].Position.X, obj.LoadedVertices[vIndex].Position.Y, obj.LoadedVertices[vIndex].Position.Z);
		m_vertices[vIndex].Normal		= Vector3(obj.LoadedVertices[vIndex].Normal.X, obj.LoadedVertices[vIndex].Normal.Y, obj.LoadedVertices[vIndex].Normal.Z);
		m_vertices[vIndex].TexCoords	= Vector2(obj.LoadedVertices[vIndex].TextureCoordinate.X, obj.LoadedVertices[vIndex].TextureCoordinate.Y);
	}

	for (size_t index = 0; index < obj.LoadedMeshes.size(); ++index) {
		Mesh mesh(obj.LoadedMeshes[index].MeshName);

		std::vector<Mesh::Vertex> verts;
		verts.resize(obj.LoadedMeshes[index].Vertices.size());
		for (size_t vIndex = 0; vIndex < verts.size(); ++vIndex) {
			verts[vIndex].Position	= Vector3(obj.LoadedMeshes[index].Vertices[vIndex].Position.X, obj.LoadedMeshes[index].Vertices[vIndex].Position.Y, obj.LoadedMeshes[index].Vertices[vIndex].Position.Z);
			verts[vIndex].Normal	= Vector3(obj.LoadedMeshes[index].Vertices[vIndex].Normal.X, obj.LoadedMeshes[index].Vertices[vIndex].Normal.Y, obj.LoadedMeshes[index].Vertices[vIndex].Normal.Z);
			verts[vIndex].TexCoords	= Vector2(obj.LoadedMeshes[index].Vertices[vIndex].TextureCoordinate.X, -obj.LoadedMeshes[index].Vertices[vIndex].TextureCoordinate.Y);
		}

		Material mat = {};
		mat.Diffuse.Path	= obj.LoadedMaterials[index].map_Kd;
		mat.Normal.Path		= obj.LoadedMaterials[index].map_bump;
		mat.Specular.Path	= obj.LoadedMaterials[index].map_Ks;
		mat.AmbientColor	= Vector3(obj.LoadedMaterials[index].Ka.X, obj.LoadedMaterials[index].Ka.Y, obj.LoadedMaterials[index].Ka.Z);
		mat.DiffuseColor	= Vector3(obj.LoadedMaterials[index].Kd.X, obj.LoadedMaterials[index].Kd.Y, obj.LoadedMaterials[index].Kd.Z);
		mat.SpecularColor	= Vector4(obj.LoadedMaterials[index].Ks.X, obj.LoadedMaterials[index].Ks.Y, obj.LoadedMaterials[index].Ks.Z, obj.LoadedMaterials[index].Ns);
		
		mesh.CreateVertices(verts);
		mesh.CreateIndices(obj.LoadedMeshes[index].Indices);
		mesh.SetMaterial(mat);

		m_meshes.push_back(mesh);
	}

	return true;
}

void Model::Initialise(IDevice* device) {
	for (size_t index = 0; index < m_meshes.size(); ++index) {
		m_meshes[index].Initialise(device);
	}

	m_constBuffer.Create(device);
}

void Model::Draw(IDevCon* context, const Matrix& world) {
	if (m_constBuffer.GetBuffer() == nullptr) return;

	CBPerInstance cbPerInstance = {};
	auto camera = Engine::GetPtr()->GetScene().GetMainCamera();
	cbPerInstance.WorldViewProj	= world * camera->GetView() * camera->GetProj(gEnv.Width, gEnv.Height);
	cbPerInstance.WorldView		= world * camera->GetView();
	cbPerInstance.World			= world;
	m_constBuffer.SetData(context, cbPerInstance);
	IConstBuffer* cb = m_constBuffer.GetBuffer();

	context->VSSetConstantBuffers((UINT)CBPerInstance::Slot, 1, &cb);

	for (size_t index = 0; index < m_meshes.size(); ++index) {
		m_meshes[index].Draw(context);
	}
}

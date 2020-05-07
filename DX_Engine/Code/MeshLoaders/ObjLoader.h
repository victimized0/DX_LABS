#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

#pragma once
#include "../Interfaces/IMeshLoader.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>

namespace objloader {
	struct Vector2
	{
		// Default Constructor
		Vector2();
		// Variable Set Constructor
		Vector2(float X_, float Y_);
		// Bool Equals Operator Overload
		bool operator==(const Vector2& other) const;
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector2& other) const;
		// Addition Operator Overload
		Vector2 operator+(const Vector2& right) const;
		// Subtraction Operator Overload
		Vector2 operator-(const Vector2& right) const;
		// Float Multiplication Operator Overload
		Vector2 operator*(const float& other) const;

		// Positional Variables
		float X;
		float Y;
	};
	
	struct Vector3
	{
		// Default Constructor
		Vector3();
		// Variable Set Constructor
		Vector3(float X_, float Y_, float Z_);
		// Bool Equals Operator Overload
		bool operator==(const Vector3& other) const;
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector3& other) const;
		// Addition Operator Overload
		Vector3 operator+(const Vector3& right) const;
		// Subtraction Operator Overload
		Vector3 operator-(const Vector3& right) const;
		// Float Multiplication Operator Overload
		Vector3 operator*(const float& other) const;
		// Float Division Operator Overload
		Vector3 operator/(const float& other) const;

		// Positional Variables
		float X;
		float Y;
		float Z;
	};

	struct Vertex
	{
		// Position Vector
		Vector3 Position;
		// Normal Vector
		Vector3 Normal;
		// Texture Coordinate Vector
		Vector2 TextureCoordinate;
	};

	struct Material
	{
		Material()
		{
			name;
			Ns = 0.0f;
			Ni = 0.0f;
			d = 0.0f;
			illum = 0;
		}

		// Material Name
		std::string name;
		// Ambient Color
		Vector3 Ka;
		// Diffuse Color
		Vector3 Kd;
		// Specular Color
		Vector3 Ks;
		// Specular Exponent
		float Ns;
		// Optical Density
		float Ni;
		// Dissolve
		float d;
		// Illumination
		int illum;
		// Ambient Texture Map
		std::string map_Ka;
		// Diffuse Texture Map
		std::string map_Kd;
		// Specular Texture Map
		std::string map_Ks;
		// Specular Hightlight Map
		std::string map_Ns;
		// Alpha Texture Map
		std::string map_d;
		// Bump Map
		std::string map_bump;
	};

	struct Mesh
	{
		// Default Constructor
		Mesh()
		{

		}
		// Variable Set Constructor
		Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
		{
			Vertices = _Vertices;
			Indices = _Indices;
		}
		// Mesh Name
		std::string MeshName;
		// Vertex List
		std::vector<Vertex> Vertices;
		// Index List
		std::vector<unsigned int> Indices;

		// Material
		Material MeshMaterial;
	};
}

// Class: Loader
//
// Description: The OBJ Model Loader
class ObjLoader : public IMeshLoader {
	typedef objloader::Vertex	Vertex;
	typedef objloader::Mesh		Mesh;
	typedef objloader::Material Material;
	typedef objloader::Vector2	Vector2;
	typedef objloader::Vector3	Vector3;

public:
	ObjLoader();
	~ObjLoader();

	// Load a file into the loader
	//
	// If file is loaded return true
	//
	// If the file is unable to be found
	// or unable to be loaded return false
	bool LoadFile(std::string Path)override;

	// Loaded Mesh Objects
	std::vector<Mesh> LoadedMeshes;
	// Loaded Vertex Objects
	std::vector<Vertex> LoadedVertices;
	// Loaded Index Positions
	std::vector<unsigned int> LoadedIndices;
	// Loaded Material Objects
	std::vector<Material> LoadedMaterials;

private:
	// Generate vertices from a list of positions, 
	//	tcoords, normals and a face line
	void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts, const std::vector<Vector3>& iPositions, const std::vector<Vector2>& iTCoords, const std::vector<Vector3>& iNormals, std::string icurline);

	// Triangulate a list of vertices into a face by printing
	//	inducies corresponding with triangles within it
	void VertexTriangluation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts);

	// Load Materials from .mtl file
	bool LoadMaterials(std::string path);
};

#endif //_OBJ_LOADER_H_

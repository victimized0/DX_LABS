#include "pch.h"
#include "Icosahedron.h"

Icosahedron::Icosahedron(int nSubdiv) {
    const float x = 0.525731f;
    const float z = 0.850651f;
    const float n = 0.f;

    Vertices = {
        {-x,n,z},
        {x,n,z},
        {-x,n,-z},
        {x,n,-z},
        {n,z,x},
        {n,z,-x},
        {n,-z,x},
        {n,-z,-x},
        {z,x,n},
        {-z,x,n},
        {z,-x,n},
        {-z,-x,n}
    };

    Triangles = {
        {0,4,1},
        {0,9,4},
        {9,5,4},
        {4,5,8},
        {4,8,1},
        {8,10,1},
        {8,3,10},
        {5,3,8},
        {5,2,3},
        {2,7,3},
        {7,10,3},
        {7,6,10},
        {7,11,6},
        {11,0,6},
        {0,1,6},
        {6,1,10},
        {9,0,11},
        {9,11,2},
        {9,2,5},
        {7,2,11}
    };

    Normalise();

    for (size_t i = 0; i < nSubdiv; ++i) {
        Subdivide();
    }

    CalculateUVs();
}

Icosahedron::~Icosahedron() {

}

int Icosahedron::VertexForEdge(int vi1, int vi2) {
    int a = std::min(vi1, vi2);
    int b = std::max(vi1, vi2);
    std::pair<int, int> key(a, b);

    auto inserted = m_lookup.insert({ key, Vertices.size() });

    if (inserted.second) {
        Vector3& edge0 = Vertices[a];
        Vector3& edge1 = Vertices[b];
        auto point = edge0 + edge1;
        point.Normalize();
        Vertices.push_back(point);
    }

    return inserted.first->second;
}

void Icosahedron::Normalise() {
    for (Vector3& vertex : Vertices) {
        vertex.Normalize();
    }
}

void Icosahedron::Subdivide() {
    std::vector<std::array<int, 3>> swapSphere;
    swapSphere.reserve(Triangles.size() * 3);

    for (auto&& triangle : Triangles) {
        int mid[3];

        for (size_t i = 0; i < 3; ++i) {
            mid[i] = VertexForEdge(triangle[i], triangle[(i + 1) % 3]);
        }

        swapSphere.push_back({ triangle[0], mid[0], mid[2] });
        swapSphere.push_back({ triangle[1], mid[1], mid[0] });
        swapSphere.push_back({ triangle[2], mid[2], mid[1] });
        swapSphere.push_back({ mid[0], mid[1], mid[2] });
    }

    Triangles.swap(swapSphere);
    m_lookup.clear();
}

void Icosahedron::CalculateUVs() {
    TexCoords.resize(Vertices.size());

    for (size_t i = 0; i < Vertices.size(); ++i) {
        Vector3& normal = Vertices[i];
        Vector2& uv = TexCoords[i];

        const float& x = normal.x;
        const float& y = normal.y;
        const float& z = normal.z;

        float normalisedX = 0;
        float normalisedZ = -1;

        if (((x * x) + (z * z)) > 0) {
            normalisedX = sqrt((x * x) / ((x * x) + (z * z)));
            if (x < 0) {
                normalisedX = -normalisedX;
            }

            normalisedZ = sqrt((z * z) / ((x * x) + (z * z)));
            if (z < 0) {
                normalisedZ = -normalisedZ;
            }
        }

        if (normalisedZ == 0) {
            uv.x = ((normalisedX * DirectX::XM_PI) / 2);
        }

        else {
            uv.x = atan(normalisedX / normalisedZ);

            if (normalisedX < 0) {
                uv.x = DirectX::XM_PI - uv.x;
            }

            if (normalisedZ < 0)
            {
                uv.x += DirectX::XM_PI;
            }
        }

        if (uv.x < 0) {
            uv.x += 2 * DirectX::XM_PI;
        }

        uv.x /= 2 * DirectX::XM_PI;
        uv.y = (-y + 1) / 2;
    }
}

void Icosahedron::ToMesh(Mesh& outMesh, float scale) const {
    outMesh.Vertices().resize(Vertices.size());
    outMesh.Indices().resize(Triangles.size() * 3);

    for (size_t i = 0; i < outMesh.Vertices().size(); ++i) {
        outMesh.Vertices()[i].Position  = Vertices[i] * scale;
        outMesh.Vertices()[i].Normal    = Vertices[i];
        outMesh.Vertices()[i].TexCoords = TexCoords[i];
    }
    
    for (size_t i = 0; i < outMesh.Indices().size(); i+=3) {
        outMesh.Indices()[i + 0] = Triangles[i / 3][0];
        outMesh.Indices()[i + 1] = Triangles[i / 3][2];
        outMesh.Indices()[i + 2] = Triangles[i / 3][1];
    }
}

#ifndef _KATAMARI_H_
#define _KATAMARI_H_

#pragma once
#include "SceneObjects/GeometryObject.h"

class Katamari : public GeometryObject {
private:
	using VertexType = VertexPosNmlTex;

public:
					Katamari(const std::string& name, const objloader::Mesh& mesh, const DirectX::SimpleMath::Vector3& position);
	virtual			~Katamari() = default;

	void			Initialise()override;

private:
	BoundingSphere	m_boundingSphere;


};

#endif //_KATAMARI_H_

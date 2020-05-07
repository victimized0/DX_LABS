#ifndef _KATAMARI_H_
#define _KATAMARI_H_
#pragma once

#include "SceneObjects/GameObject.h"

class Katamari final : public GameObject {
public:
									Katamari(const std::string& name, const DirectX::SimpleMath::Vector3& position, std::string modelPath, float scale = 1.0f);
	virtual							~Katamari() = default;

	void							Attach(std::unique_ptr<GameObject>&& pickup);
	void							Initialise()final;
	bool							Contains(GameObject* child);

public:
	float							Size;

};

#endif //_KATAMARI_H_

#ifndef _PICKUP_H_
#define _PICKUP_H_
#pragma once

#include "SceneObjects/GameObject.h"

class Pickup: public GameObject {
public:
			Pickup(const std::string& name, const DirectX::SimpleMath::Vector3& position, std::string modelPath = "", float scale = 1.0f);
			~Pickup() = default;

			Pickup(const Pickup&) = default;
			Pickup& operator=(const Pickup&) = default;
			Pickup(Pickup&&) = default;
			Pickup& operator=(Pickup&&) = default;

	void	Initialise()final;

public:
	float	Size;

};

#endif //_PICKUP_H_

#ifndef _SOLAR_OBJECT_H_
#define _SOLAR_OBJECT_H_
#pragma once

#include "SceneObjects/GeometryObject.h"

class SolarObject : public GeometryObject {
	typedef DirectX::SimpleMath::Vector3 Vector3;

public:
	SolarObject(const std::string& name, const Vector3& m_position);
	SolarObject(const std::string& name);

public:
	Vector3		ThetaPhiChi;
	Vector3		YawPitchRoll;
	float		Radius;

};

#endif //_SOLAR_OBJECT_H_

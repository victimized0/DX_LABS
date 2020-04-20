#include "pch.h"
#include "SolarObject.h"

SolarObject::SolarObject(const std::string& name)
	: SolarObject(name, Vector3::Zero)
{

}

SolarObject::SolarObject(const std::string& name, const Vector3& m_position)
	: GeometryObject(name, m_position)
	, YawPitchRoll(Vector3::Zero)
	, ThetaPhiChi(Vector3::Zero)
	, Radius(0)
{

}


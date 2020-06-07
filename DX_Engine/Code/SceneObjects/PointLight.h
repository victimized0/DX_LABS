#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_
#pragma once

#include "../ConstBuffer.h"
#include "GameObject.h"
#include "../Mesh.h"

class CPointLight final : public GameObject {
public:
								CPointLight(PointLight const& pData);
	virtual						~CPointLight();

	void						Initialise(IDevice* device);
	void						Update(float dt) final;
	void						Draw(IDevCon* context, Matrix const& world = Matrix::Identity) final;

private:
	PointLight					m_rLightData;
	Mesh						m_lightSphere;
	ConstBuffer<CBPerLight>		m_cbLight;
	ConstBuffer<CBPerInstance>	m_cbInstance;

};

#endif // _POINT_LIGHT_H_

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_
#pragma once

#include "../ConstBuffer.h"
#include "GameObject.h"
#include "../Mesh.h"

class CPointLight final : public GameObject {
public:
								CPointLight(PointLightData const& pData);
	virtual						~CPointLight();

	void						Initialise(IDevice* device);
	void						Update(float dt) final;
	void						Draw(IDevCon* context, Matrix const& world = Matrix::Identity, RenderPass pass = RenderPass::Geometry) final;
	const PointLightData&		GetData() const { return m_lightData; }

private:
	PointLightData				m_lightData;
	Mesh						m_lightSphere;
	Mesh						m_lightVolume;
	ConstBuffer<CBPerInstance>	m_cbPerInst;
	ConstBuffer<CBPerLight>		m_cbPerLight;

};

#endif // _POINT_LIGHT_H_

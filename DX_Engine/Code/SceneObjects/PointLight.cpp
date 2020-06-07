#include "pch.h"
#include "PointLight.h"
#include "../Helper.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

CPointLight::CPointLight(PointLight const& pData)
	: GameObject("", pData.Position)
	, m_rLightData(pData)
{
	::CreateIcosahedron( 3.0f, m_lightSphere.Vertices(), m_lightSphere.Indices() );
	m_lightSphere.SetColor({ pData.Diffuse.x, pData.Diffuse.y, pData.Diffuse.z });
}

CPointLight::~CPointLight() {

}

void CPointLight::Initialise(IDevice* device) {
	m_cbLight.Create( device );
	m_cbInstance.Create( device );
	m_lightSphere.Initialise( device );
}

void CPointLight::Update(float dt) {

}

void CPointLight::Draw(IDevCon* context, Matrix const& world) {
	if (m_cbInstance.GetBuffer() == nullptr) return;

	CBPerInstance cbPerInstance = {};
	auto camera = Engine::GetPtr()->GetScene().GetMainCamera();
	Matrix w = world * GetWorld();

	cbPerInstance.WorldViewProj	= w * camera->GetView() * camera->GetProj(gEnv.Width, gEnv.Height);
	cbPerInstance.WorldView		= w * camera->GetView();
	cbPerInstance.World			= w;

	m_cbInstance.SetData(context, cbPerInstance);
	IConstBuffer* pConstBuf		= m_cbInstance.GetBuffer();

	context->VSSetConstantBuffers( (UINT)CBPerInstance::Slot, 1, &pConstBuf );

	m_lightSphere.Draw( context );
}

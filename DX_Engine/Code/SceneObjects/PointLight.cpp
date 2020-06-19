#include "pch.h"
#include "PointLight.h"
#include "../Helper.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

CPointLight::CPointLight(PointLightData const& pData)
	: GameObject("", pData.Position)
	, m_lightData(pData)
{
	::CreateIcosahedron( 3.0f, m_lightSphere.Vertices(), m_lightSphere.Indices() );
	m_lightSphere.SetColor({ pData.Diffuse.x, pData.Diffuse.y, pData.Diffuse.z });
}

CPointLight::~CPointLight() {

}

void CPointLight::Initialise(IDevice* device) {
	m_cbPerInst.Create( device );
	m_cbPerLight.Create( device );
	m_lightSphere.Initialise( device );
}

void CPointLight::Update(float dt) {

}

void CPointLight::Draw(IDevCon* context, Matrix const& world, RenderPass pass) {
	if (m_cbPerInst.GetBuffer() == nullptr) return;

	if (pass == RenderPass::Geometry) {
		CBPerInstance cbpi = {};
		auto camera = Engine::GetPtr()->GetScene().GetMainCamera();
		Matrix w = world * GetWorld();

		cbpi.WorldViewProj	= w * camera->GetView() * camera->GetProj(gEnv.Width, gEnv.Height);
		cbpi.WorldView		= w * camera->GetView();
		cbpi.World			= w;

		m_cbPerInst.SetData(context, cbpi);
		IConstBuffer* pCbpi = m_cbPerInst.GetBuffer();

		context->VSSetConstantBuffers((UINT)CBPerInstance::Slot, 1, &pCbpi);
		m_lightSphere.Draw(context);
	}

	if (pass == RenderPass::Light) {
		CBPerLight cbpl = {};

		cbpl.Diffuse	= m_lightData.Diffuse;
		cbpl.AttRange	= Vector4(m_lightData.Attenuation, m_lightData.Range);
		cbpl.Ambient	= m_lightData.Ambient;
		cbpl.Position	= m_lightData.Position;

		m_cbPerLight.SetData(context, cbpl);
		IConstBuffer* pCbpl = m_cbPerLight.GetBuffer();

		ShadersManager* pShadersMan = gEnv.Renderer()->GetShadersManager();

		context->PSSetConstantBuffers((UINT)CBPerLight::Slot, 1, &pCbpl);
		context->PSSetShader(pShadersMan->PointLightPS.GetShader(), nullptr, 0);
		context->Draw(3, 0);
	}

}

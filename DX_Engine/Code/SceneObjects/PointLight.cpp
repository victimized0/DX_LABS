#include "pch.h"
#include "PointLight.h"
#include "../Helper.h"
#include "../Primitives/Icosahedron.h"

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

CPointLight::CPointLight(PointLightData const& pData)
	: GameObject("", pData.Position)
	, m_lightData(pData)
{
	Icosahedron lightMesh(2);
	lightMesh.ToMesh(m_lightSphere, 3.0f);
	m_lightSphere.SetColor({ pData.Diffuse.x, pData.Diffuse.y, pData.Diffuse.z });

	lightMesh.ToMesh(m_lightVolume, pData.Range);
}

CPointLight::~CPointLight() {

}

void CPointLight::Initialise(IDevice* device) {
	m_cbPerInst.Create( device );
	m_cbPerLight.Create( device );
	m_lightSphere.Initialise( device );
	m_lightVolume.Initialise( device );
}

void CPointLight::Update(float dt) {

}

void CPointLight::Draw(IDevCon* context, Matrix const& world, RenderPass pass) {
	if (m_cbPerInst.GetBuffer() == nullptr) return;

	CBPerInstance cbpi	= {};
	auto camera			= Engine::GetPtr()->GetScene().GetMainCamera();
	Matrix w			= world * GetWorld();

	cbpi.WorldViewProj	= w * camera->GetView() * camera->GetProj(gEnv.Width, gEnv.Height);
	cbpi.WorldView		= w * camera->GetView();
	cbpi.World			= w;

	m_cbPerInst.SetData(context, cbpi);
	IConstBuffer* pCbpi = m_cbPerInst.GetBuffer();

	context->VSSetConstantBuffers((UINT)CBPerInstance::Slot, 1, &pCbpi);

	if (pass == RenderPass::Light)
	{
		CBPerLight cbpl = {};

		cbpl.Diffuse	= m_lightData.Diffuse;
		cbpl.Ambient	= m_lightData.Ambient;
		cbpl.PosRange	= Vector4(m_lightData.Position, m_lightData.Range);
		
		m_cbPerLight.SetData(context, cbpl);
		IConstBuffer* pCbpl = m_cbPerLight.GetBuffer();

		ShadersManager* pShadersMan = gEnv.Renderer()->GetShadersManager();

		context->PSSetConstantBuffers((UINT)CBPerLight::Slot, 1, &pCbpl);

		m_lightVolume.SetVertexShader(pShadersMan->StandardNoLightVS);
		m_lightVolume.SetPixelShader(pShadersMan->PointLightPS);
		m_lightVolume.Draw(context);
	}

	if (pass == RenderPass::Geometry) {
		m_lightSphere.Draw(context);
	}
}

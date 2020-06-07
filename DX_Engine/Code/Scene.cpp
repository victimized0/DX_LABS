#include "pch.h"
#include "Scene.h"

using namespace DirectX::SimpleMath;

Scene::Scene()
    : m_pDirLight(nullptr)
{
    Root        = std::make_unique<GameObject>("Root", Vector3::Zero);
    m_camera    = std::make_unique<Camera>("Main Camera");
}

Scene::~Scene() {

}

void Scene::Initialise(IDevice* device) {
    m_cbPerFrame.Create( device );
}

void Scene::SetSunLight(DirLight const* pDirLight) {
    m_pDirLight = pDirLight;
}

void Scene::AddLight(std::unique_ptr<CPointLight>&& pLight) {
    pLight->Initialise( gEnv.Renderer()->GetDevice() );
    m_vPointLights.push_back( std::move(pLight) );
}

//const std::vector<CPointLight>& Scene::GetAllLights() {
//    return m_vPointLights;
//}

void Scene::ReparentObject(const std::string& key, GameObject* pNewParent) {
    Root->Reparent(key, pNewParent);
}

void Scene::AddObject(std::unique_ptr<GameObject>&& obj, GameObject* parent) {
    if (!obj->IsInit()) {
        obj->Initialise();
    }

    if (parent == nullptr) {
        Root->AddChild( std::move(obj) );
    }
    else {
        parent->AddChild( std::move(obj) );
    }
}

void Scene::DeleteObject(const std::string& key) {
    Root->RemoveChild(key);
}

GameObject* Scene::GetSceneObject(const std::string& key)const {
    return Root->Find(key);
}

void Scene::Update(float dt) {

}

void Scene::RenderScene(IDevCon* context, RenderPass pass) {
    if (pass == RenderPass::Geometry) {
        Root->Draw(context, DirectX::SimpleMath::Matrix::Identity);

        for (auto& pointLight : m_vPointLights) {
            pointLight->Draw(context);
        }
    }

    if (pass == RenderPass::Light) {
        CBPerFrame cbpf = {};
        cbpf.EyePos = DirectX::SimpleMath::Vector4(GetMainCamera()->GetPosition(), 1.0f);
        cbpf.DirLight = *m_pDirLight;
        cbpf.PointLightsArr[0] = m_vPointLights[0]->GetData();
        cbpf.PointLightsArr[1] = m_vPointLights[1]->GetData();
        cbpf.PointLightsArr[2] = m_vPointLights[2]->GetData();

        //cbpf.PointLightsArr[0].Position = Vector3::Transform(cbpf.PointLightsArr[0].Position, m_vPointLights[0]->GetWorld());
        //cbpf.PointLightsArr[1].Position = Vector3::Transform(cbpf.PointLightsArr[1].Position, m_vPointLights[1]->GetWorld());
        //cbpf.PointLightsArr[2].Position = Vector3::Transform(cbpf.PointLightsArr[2].Position, m_vPointLights[2]->GetWorld());

        m_cbPerFrame.SetData(context, cbpf);
        IConstBuffer* cb = m_cbPerFrame.GetBuffer();

        context->PSSetConstantBuffers((UINT)CBPerFrame::Slot, 1, &cb);

        //draw screen-aligned quad
        context->Draw(3, 0);
    }
}

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

        if (m_pDirLight != nullptr) {
            cbpf.LightCol = m_pDirLight->LightCol;
            cbpf.LightAmb = m_pDirLight->LightAmb;
            cbpf.LightDir = DirectX::SimpleMath::Vector4(m_pDirLight->LightDir, 0.0f);
        }

        m_cbPerFrame.SetData(context, cbpf);
        IConstBuffer* cb = m_cbPerFrame.GetBuffer();

        context->VSSetConstantBuffers((UINT)CBPerFrame::Slot, 1, &cb);
        context->PSSetConstantBuffers((UINT)CBPerFrame::Slot, 1, &cb);

        //draw screen-aligned quad
        context->Draw(3, 0);
    }
}

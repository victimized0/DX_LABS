#include "pch.h"
#include "Scene.h"

using namespace DirectX::SimpleMath;

Scene::Scene() {
    Root = std::make_unique<GameObject>("Root", Vector3::Zero);
    m_camera = std::make_unique<Camera>("Main Camera");
}

Scene::~Scene() {

}


void Scene::ReparentObject(const std::string& key, GameObject* pNewParent) {
    Root->Reparent(key, pNewParent);
}

void Scene::AddObject(std::unique_ptr<GameObject>&& obj, GameObject* parent) {
    obj->Initialise();
    if (parent == nullptr) {
        Root->AddChild( std::move(obj) );
    }
    else {
        parent->AddChild( std::move(obj) );
    }
}

void Scene::DeleteObject(const std::string& key) {
    Root->RemoveChild(key);
    //for (size_t index = 0; index < m_sceneTree.size(); ++index) {
    //    if (m_sceneObjects[index].get() == obj) {
    //        m_sceneObjects.erase(m_sceneObjects.begin() + index);
    //        return;
    //    }
    //}
}

GameObject* Scene::GetSceneObject(const std::string& key)const {
    return Root->Find(key);
    //for (size_t index = 0; index < m_sceneTree.size(); ++index) {
    //    if (m_sceneObjects[index]->GetName() == key) {
    //        return m_sceneObjects[index].get();
    //    }
    //}
    //return nullptr;
}

void Scene::Update(float dt) {

}

void Scene::RenderScene(IDevCon* context) {
    Root->Draw(context, DirectX::SimpleMath::Matrix::Identity);
}

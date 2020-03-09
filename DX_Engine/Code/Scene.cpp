#include "pch.h"
#include "Scene.h"

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::AddObject(const std::shared_ptr<SceneObject>& pObj) {
    pObj->Initialise();
    m_sceneObjects.push_back(pObj);
}

std::shared_ptr<SceneObject> Scene::GetSceneObject(const std::string& key)const {
    for (size_t index = 0; index < m_sceneObjects.size(); ++index) {
        if (m_sceneObjects[index]->GetName() == key) {
            return m_sceneObjects[index];
        }
    }
    return nullptr;
}

void Scene::Initialise() {
    for (size_t index = 0; index < m_sceneObjects.size(); ++index) {
        m_sceneObjects[index]->Initialise();
    }
}

void Scene::Update(float dt) {
    for (size_t index = 0; index < m_sceneObjects.size(); ++index) {
        m_sceneObjects[index]->Update(dt);
    }
}

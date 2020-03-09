#include "pch.h"
#include "SceneObject.h"

using namespace DirectX;

SceneObject::SceneObject(const std::string& name, DirectX::XMFLOAT3 position)
	: m_name(name)
	, m_pos(position)
{

}

SceneObject::SceneObject(const std::string& name)
	: SceneObject(name, XMFLOAT3(0.0f, 0.0f, 0.0f))
{

}

SceneObject::~SceneObject() {

}

void SceneObject::Initialise() {

}

void SceneObject::Update(float dt) {

}

void SceneObject::SetPosition(float x, float y, float z) {
	m_pos = XMFLOAT3(x, y, z);
}

const std::string& SceneObject::GetName()const {
	return m_name;
}

const DirectX::XMFLOAT3& SceneObject::GetPosition()const {
	return m_pos;
}

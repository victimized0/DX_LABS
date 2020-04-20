#include "pch.h"
#include "SceneObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using std::string;

SceneObject::SceneObject(const string& name, const Vector3& position)
	: m_name(name)
{

}

SceneObject::SceneObject(const string& name)
	: SceneObject(name, XMFLOAT3(0.0f, 0.0f, 0.0f))
{

}

SceneObject::~SceneObject() {

}

void SceneObject::Initialise() {

}

void SceneObject::Update(float dt) {

}

const string& SceneObject::GetName()const {
	return m_name;
}


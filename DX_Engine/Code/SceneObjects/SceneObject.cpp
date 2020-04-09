#include "pch.h"
#include "SceneObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using std::string;

SceneObject::SceneObject(const string& name, const Vector3& position)
	: m_name(name)
	, Transform()
{
	SetPosition(position);
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

void SceneObject::SetPosition(float x, float y, float z) {
	SetPosition(Vector3(x, y, z));
}

void SceneObject::SetPosition(const Vector3& pos) {
	Transform.Translate(pos);
}

const string& SceneObject::GetName()const {
	return m_name;
}

Vector3 SceneObject::GetPosition()const {
	return Transform.ToMatrix().Translation();
}

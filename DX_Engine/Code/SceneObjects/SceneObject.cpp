#include "pch.h"
#include "SceneObject.h"

using namespace DirectX;

SceneObject::SceneObject(const std::string& name, DirectX::XMFLOAT3 position)
	: m_name(name)
{
	XMStoreFloat4x4(&m_transform, XMMatrixIdentity());
	SetPosition(position.x, position.y, position.z);
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
	auto pos = GetPosition();
	float dx = pos.x - x;
	float dy = pos.y - y;
	float dz = pos.z - z;

	XMMATRIX translationMat = XMMatrixTranslation(dx, dy, dz);
	XMMATRIX transform		= XMLoadFloat4x4(&m_transform);
	XMMATRIX newTransform	= XMMatrixMultiply(transform, translationMat);
	XMStoreFloat4x4(&m_transform, newTransform);
}

const std::string& SceneObject::GetName()const {
	return m_name;
}

DirectX::XMFLOAT3 SceneObject::GetPosition()const {
	XMFLOAT3 translation(0,0,0);
	XMVECTOR outScale;
	XMVECTOR outRot;
	XMVECTOR outTrans;

	XMMATRIX transform = XMLoadFloat4x4(&m_transform);
	if (XMMatrixDecompose(&outScale, &outRot, &outTrans, transform)) {
		XMStoreFloat3(&translation, outTrans);
	}

	return translation;
}

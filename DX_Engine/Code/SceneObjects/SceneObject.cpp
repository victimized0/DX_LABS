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
	m_transform._41 = x;
	m_transform._42 = y;
	m_transform._43 = z;
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

DirectX::XMVECTOR SceneObject::GetPositionV()const {
	XMVECTOR outScale;
	XMVECTOR outRot;
	XMVECTOR outTrans;

	XMMATRIX transform = XMLoadFloat4x4(&m_transform);
	XMMatrixDecompose(&outScale, &outRot, &outTrans, transform);

	return outTrans;
}

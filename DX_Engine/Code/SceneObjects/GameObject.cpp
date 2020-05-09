#include "pch.h"
#include "GameObject.h"

#include "../Helper.h"
#include "../Engine.h"
#include "../DDSTextureLoader.h"

using namespace DirectX;

GameObject::GameObject(const std::string& name)
	: GameObject::GameObject(name, Vector3::Zero, "", 1.0f)
{

}

GameObject::GameObject(const std::string& name, const Vector3& position, std::string modelPath, float scale)
	: m_name(name)
	, m_model(modelPath)
	, Scale(scale)
	, Position(position)
	, Rotation(Quaternion::Identity)
{

}

void GameObject::Update(float dt) {

}

void GameObject::Reparent(const std::string& name, GameObject* pNewParent) {
	for (size_t index = 0; index < m_children.size(); ++index) {
		GameObject* obj = m_children[index].get();
		if (obj->GetName() == name) {
			pNewParent->AddChild( std::move(m_children[index]) );
			m_children.erase(m_children.begin() + index);
			break;
		}
		obj->Reparent(name, pNewParent);
	}
}

void GameObject::AddChild(std::unique_ptr<GameObject>&& child) {
	m_children.push_back( std::move(child) );
}

void GameObject::RemoveChild(const std::string& name) {
	for (size_t index = 0; index < m_children.size(); ++index) {
		GameObject* obj = m_children[index].get();
		if (obj->GetName() == name) {
			m_children.erase(m_children.begin() + index);
			break;
		}
		obj->RemoveChild(name);
	}
}

GameObject* GameObject::Find(const std::string& name) {
	for (size_t index = 0; index < m_children.size(); ++index) {
		GameObject* obj = m_children[index].get();
		if (obj->GetName() == name) {
			return obj;
		}
		obj->Find(name);
	}
	return nullptr;
}

void GameObject::Initialise() {
	m_model.Initialise(gEnv.Renderer()->GetDevice());
	CreateBoundingBox();
}

void GameObject::Draw(IDevCon* context, const Matrix& parentTransfom) {
	m_model.Draw(context, GetWorld() * parentTransfom);

	for (size_t index = 0; index < m_children.size(); ++index) {
		GameObject* obj = m_children[index].get();
		obj->Draw(context, GetWorld() * parentTransfom);
	}
}

Matrix GameObject::GetWorld()const {
	return
		Matrix::CreateScale(Scale) *
		Matrix::CreateFromQuaternion(Rotation) *
		Matrix::CreateTranslation(Position);
}

void GameObject::SetTransform(Matrix& mat) {
	Vector3 pos, scale;
	Quaternion rot;

	if (mat.Decompose(scale, rot, pos)) {
		Position	= pos;
		Rotation	= rot;
		Scale		= scale.x;
	}
}

void GameObject::Rescale(float dScale) {
	Scale *= dScale;
}

void GameObject::Move(const Vector3& dPos) {
	Position += dPos;
	m_boundingBox.Center = Position;
}

void GameObject::Orbit(const Vector3& dRot, float angle) {
	Rotation *= Quaternion::CreateFromAxisAngle(dRot, angle);
}

void GameObject::Orbit(float dx, float dy, float dz, float angle) {
	Orbit(Vector3(dx, dy, dz), angle);
}

bool GameObject::Intersects(GameObject* other) {
	return m_boundingBox.Intersects(other->m_boundingBox);
}

void GameObject::CreateBoundingBox() {
	std::vector<Vector3> pVerts(m_model.GetVertices().size());
	for (size_t index = 0; index < pVerts.size(); ++index) {
		pVerts[index] = Vector3::Transform(m_model.GetVertices()[index].Position, GetWorld());
	}

	BoundingBox::CreateFromPoints(m_boundingBox, pVerts.size(), pVerts.data(), sizeof(Vector3));
	m_boundingBox.Center = Position;
}

std::vector<GameObject*> GameObject::GetIntersected(GameObject* obj) {
	std::vector<GameObject*> interstected;

	for (size_t index = 0; index < m_children.size(); ++index) {
		GameObject* intObj = m_children[index].get();
		if (intObj->Intersects(obj)) {
			bool contains = false;

			for (size_t i = 0; i < interstected.size(); ++i) {
				contains = interstected[i] == intObj;
				if (contains) break;
			}

			if (!contains) {
				interstected.push_back(intObj);
			}
		}

		std::vector<GameObject*> inheritedInters = intObj->GetIntersected(obj);
		interstected.insert(interstected.begin(), inheritedInters.begin(), inheritedInters.end());
	}

	return interstected;
}

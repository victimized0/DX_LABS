#include "pch.h"
#include "Katamari.h"

Katamari::Katamari(const std::string& name, const DirectX::SimpleMath::Vector3& position, std::string modelPath, float scale)
	: GameObject::GameObject(name, position, modelPath, scale)
	, Size(0)
{

}

void Katamari::Attach(std::unique_ptr<GameObject>&& pickup) {

}

void Katamari::Initialise() {
	GameObject::Initialise();
	Size = 2 * GetBoundingBox().Extents.x * 2 * GetBoundingBox().Extents.y * 2 * GetBoundingBox().Extents.z;
}

bool Katamari::Contains(GameObject* child) {
	for (size_t i = 0; i < m_children.size(); i++) {
		if (m_children[i].get() == child) return true;
	}

	return false;
}

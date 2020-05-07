#include "pch.h"
#include "Pickup.h"

Pickup::Pickup(const std::string& name, const DirectX::SimpleMath::Vector3& position, std::string modelPath, float scale)
	: GameObject::GameObject(name, position, modelPath, scale)
	, Size(0)
{

}

void Pickup::Initialise() {
	GameObject::Initialise();
	Size = 2 * GetBoundingBox().Extents.x * 2 * GetBoundingBox().Extents.y * 2 * GetBoundingBox().Extents.z;
}

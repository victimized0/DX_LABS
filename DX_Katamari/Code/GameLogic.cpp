#include "pch.h"
#include "GameLogic.h"
#include "GameObjects/Katamari.h"
#include "GameObjects/Pickup.h"
#include <random>

GameLogic::GameLogic()
	: m_pScene(nullptr)
{

}

GameLogic::~GameLogic() {

}

bool GameLogic::Initialise(Scene* pScene) {
	if (pScene == nullptr) {
		return false;
	}

	m_pScene = pScene;
	return true;
}

void GameLogic::Update(float dt) {
	auto player = dynamic_cast<Katamari*>(m_pScene->GetSceneObject("Katamari"));

	auto intersected = m_pScene->Root->GetIntersected(player);
	for each (auto obj in intersected) {
		auto pickup = dynamic_cast<Pickup*>(obj);
		if (pickup != nullptr && player->Size > pickup->Size) {
			if (player->Contains(pickup)) continue;

			float range = 2 * player->GetBoundingBox().Extents.x;

			std::random_device dev;
			std::mt19937 rng(dev());
			std::uniform_real_distribution<float> dist(-range, range);

			pickup->Position.x = 2 * dist(rng);
			pickup->Position.y = 2 * dist(rng);
			pickup->Position.z = 2 * dist(rng);

			pickup->Scale /= player->Scale;

			m_pScene->ReparentObject(pickup->GetName(), player);
		}
	}
}

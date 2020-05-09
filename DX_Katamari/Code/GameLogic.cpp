#include "pch.h"
#include "GameLogic.h"

#include "GameObjects/Katamari.h"
#include "GameObjects/Pickup.h"

bool GameLogic::Initialise(Scene* pScene) {
	if (pScene == nullptr) {
		return false;
	}

	m_pScene = pScene;
	return true;
}

void GameLogic::Update(float dt) {
	auto player			= dynamic_cast<Katamari*>(m_pScene->GetSceneObject("Katamari"));
	auto intersected	= m_pScene->Root->GetIntersected(player);

	for each (auto obj in intersected) {
		auto pickup = dynamic_cast<Pickup*>(obj);

		if (pickup != nullptr && player->Size > pickup->Size) {
			if (player->Contains(pickup)) continue;

			player->Attach(pickup);
			m_pScene->ReparentObject(pickup->GetName(), player);	// TODO: Refactor this shit
		}
	}
}

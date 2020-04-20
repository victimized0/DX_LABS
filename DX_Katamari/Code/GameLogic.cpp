#include "pch.h"
#include "GameLogic.h"
#include "SceneObjects/GeometryObject.h"

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

}

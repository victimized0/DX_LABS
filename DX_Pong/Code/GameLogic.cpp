#include "pch.h"
#include "GameLogic.h"
#include "SceneObjects/GameObject.h"
#include "GameObjects/Ball.h"

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
	auto ball    = dynamic_cast<Ball*>(m_pScene->GetSceneObject(NAME_BALL));
	auto lPaddle = dynamic_cast<GameObject*>(m_pScene->GetSceneObject(NAME_LPADDLE));
	auto rPaddle = dynamic_cast<GameObject*>(m_pScene->GetSceneObject(NAME_RPADDLE));

	const BoundingBox& ballBB = ball->GetBoundingBox();

	bool collidesBorder = ballBB.Center.y + ballBB.Extents.y >= GRID_TOP_BORDER || ballBB.Center.y - ballBB.Extents.y <= GRID_BOTTOM_BORDER;
	if (collidesBorder) {
		ball->InverseSpeedY();
	}

	if (lPaddle->GetBoundingBox().Intersects(ball->GetBoundingBox()) ||
		rPaddle->GetBoundingBox().Intersects(ball->GetBoundingBox()))
	{
		ball->InverseSpeedX();
	}

	float dx = ball->GetSpeedX() * dt;
	float dy = ball->GetSpeedY() * dt;
	ball->Move( { dx, dy, 0.0f } );

	bool crossedBorderRight	= ballBB.Center.x < GRID_RIGHT_BORDER;
	bool crossedBorderLeft	= ballBB.Center.x > GRID_LEFT_BORDER;
	if (crossedBorderRight || crossedBorderLeft) {
		ball->Reset();
	}

}

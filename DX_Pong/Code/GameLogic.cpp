#include "pch.h"
#include "GameLogic.h"
#include "Events/KeyboardEvent.h"
#include "SceneObjects/GeometryObject.h"
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

void GameLogic::OnEvent(const Event& event) {
	switch (event.GetType())
	{
		case EventType::KeyDown:
		{
			auto& _event = const_cast<Event&>(event);
			auto& kbEvent = reinterpret_cast<KeyboardEvent&>(_event);
			KeyCode key = kbEvent.GetKeyCode();

			switch (key)
			{
				case KeyCode::W:
				{
					auto player = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_LPADDLE).get());
					float distToTop = fabs(player->GetPosition().y + player->GetAABB().Height / 2 - GRID_TOP_BORDER);
					if (distToTop > 0.1f) {
						player->Translate(0.0f, 1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::S:
				{
					auto paddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_LPADDLE).get());
					float distToBottom = fabs(paddle->GetPosition().y - paddle->GetAABB().Height / 2 - GRID_BOTTOM_BORDER);
					if (distToBottom > 0.1f) {
						paddle->Translate(0.0f, -1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::Up:
				{
					auto player = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_RPADDLE).get());
					float distToTop = fabs(player->GetPosition().y + player->GetAABB().Height / 2 - GRID_TOP_BORDER);
					if (distToTop > 0.1f) {
						player->Translate(0.0f, 1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::Down:
				{
					auto paddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_RPADDLE).get());
					float distToBottom = fabs(paddle->GetPosition().y - paddle->GetAABB().Height / 2 - GRID_BOTTOM_BORDER);
					if (distToBottom > 0.1f) {
						paddle->Translate(0.0f, -1.0f, 0.0f);
					}
				}
				break;
			}
		}
		break;
	}
}

void GameLogic::Update(float dt) {
	auto ball    = dynamic_cast<Ball*>(  m_pScene->GetSceneObject(NAME_BALL).get());
	auto lPaddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_LPADDLE).get());
	auto rPaddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_RPADDLE).get());

	const AABB& ballAABB = ball->GetAABB();
	float threshold = 0.1f;

	bool collidesBorder = fabs(ballAABB.Y - GRID_TOP_BORDER) < threshold || fabs(ballAABB.Y - GRID_BOTTOM_BORDER) < threshold;
	if (collidesBorder) {
		ball->InverseSpeedY();
	}

	if (lPaddle->GetAABB().Collides(ball->GetAABB()) ||
		rPaddle->GetAABB().Collides(ball->GetAABB()))
	{
		ball->InverseSpeedX();
	}

	float dx = ball->GetSpeedX() * dt;
	float dy = ball->GetSpeedY() * dt;
	ball->Translate(dx, dy, 0.0f);

	bool crossedBorderRight = fabs(ballAABB.X - GRID_RIGHT_BORDER) < threshold;
	bool crossedBorderLeft  = fabs(ballAABB.X - GRID_LEFT_BORDER)  < threshold;
	if (crossedBorderRight || crossedBorderLeft) {
		ball->Reset();
	}
}

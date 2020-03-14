#include "pch.h"
#include "GameLogic.h"
#include "Events/KeyboardEvent.h"
#include "SceneObjects/GeometryObject.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Paddle.h"

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
					auto player = dynamic_cast<Paddle*>(m_pScene->GetSceneObject(NAME_LPADDLE).get());
					float distToTop = fabs(player->GetPosition().y + player->GetSizeY() / 2 - GRID_TOP_BORDER);
					if (distToTop > 0.1f) {
						player->Translate(0.0f, 1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::S:
				{
					auto paddle = dynamic_cast<Paddle*>(m_pScene->GetSceneObject(NAME_LPADDLE).get());
					float distToBottom = fabs(paddle->GetPosition().y - paddle->GetSizeY() / 2 - GRID_BOTTOM_BORDER);
					if (distToBottom > 0.1f) {
						paddle->Translate(0.0f, -1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::Up:
				{
					auto player = dynamic_cast<Paddle*>(m_pScene->GetSceneObject(NAME_RPADDLE).get());
					float distToTop = fabs(player->GetPosition().y + player->GetSizeY() / 2 - GRID_TOP_BORDER);
					if (distToTop > 0.1f) {
						player->Translate(0.0f, 1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::Down:
				{
					auto paddle = dynamic_cast<Paddle*>(m_pScene->GetSceneObject(NAME_RPADDLE).get());
					float distToBottom = fabs(paddle->GetPosition().y - paddle->GetSizeY() / 2 - GRID_BOTTOM_BORDER);
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
	auto lPaddle = dynamic_cast<Paddle*>(m_pScene->GetSceneObject(NAME_LPADDLE).get());
	auto rPaddle = dynamic_cast<Paddle*>(m_pScene->GetSceneObject(NAME_RPADDLE).get());

	BoundingBox ballBB = ball->GetBoundingBox();
	float threshold	   = 0.15f;

	bool collidesBorder = fabs(ballBB.Top - GRID_TOP_BORDER) < threshold || fabs(ballBB.Bottom - GRID_BOTTOM_BORDER) < threshold;
	if (collidesBorder) {
		ball->InverseSpeedY();
	}

	if (lPaddle->Collides(ball->GetBoundingBox(), ball->GetSize(), ball->GetSize(), threshold) ||
		rPaddle->Collides(ball->GetBoundingBox(), ball->GetSize(), ball->GetSize(), threshold))
	{
		ball->InverseSpeedX();
	}

	float dx = ball->GetSpeedX() * dt;
	float dy = ball->GetSpeedY() * dt;
	ball->Translate(dx, dy, 0.0f);

	bool crossedBorderRight = fabs(ballBB.Right - GRID_RIGHT_BORDER) < threshold;
	bool crossedBorderLeft  = fabs(ballBB.Left  - GRID_LEFT_BORDER)  < threshold;
	if (crossedBorderRight || crossedBorderLeft) {
		ball->Reset();
	}
}

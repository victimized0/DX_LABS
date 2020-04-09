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
					auto paddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_LPADDLE));
					float distToTop = fabs(paddle->GetPosition().y + 2 * paddle->GetBoundingBox().Extents.y / 2 - GRID_TOP_BORDER);
					if (distToTop > 0.1f) {
						paddle->Translate(0.0f, 1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::S:
				{
					auto paddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_LPADDLE));
					float distToBottom = fabs(paddle->GetPosition().y - 2 * paddle->GetBoundingBox().Extents.y / 2 - GRID_BOTTOM_BORDER);
					if (distToBottom > 0.1f) {
						paddle->Translate(0.0f, -1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::Up:
				{
					auto paddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_RPADDLE));
					float distToTop = fabs(paddle->GetPosition().y + 2 * paddle->GetBoundingBox().Extents.y / 2 - GRID_TOP_BORDER);
					if (distToTop > 0.1f) {
						paddle->Translate(0.0f, 1.0f, 0.0f);
					}
				}
				break;

				case KeyCode::Down:
				{
					auto paddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_RPADDLE));
					float distToBottom = fabs(paddle->GetPosition().y - 2 * paddle->GetBoundingBox().Extents.y / 2 - GRID_BOTTOM_BORDER);
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
	auto ball    = dynamic_cast<Ball*>(m_pScene->GetSceneObject(NAME_BALL));
	auto lPaddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_LPADDLE));
	auto rPaddle = dynamic_cast<GeometryObject*>(m_pScene->GetSceneObject(NAME_RPADDLE));

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
	ball->Translate(dx, dy, 0.0f);

	bool crossedBorderRight	= ballBB.Center.x < GRID_RIGHT_BORDER;
	bool crossedBorderLeft	= ballBB.Center.x > GRID_LEFT_BORDER;
	if (crossedBorderRight || crossedBorderLeft) {
		ball->Reset();
	}

}

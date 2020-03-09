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
					auto player = m_pScene->GetSceneObject("player1");
					dynamic_cast<GeometryObject*>(player.get())->Translate(0.0f, 1.0f, 0.0f);
				}
				break;

				case KeyCode::S:
				{
					auto player = m_pScene->GetSceneObject("player1");
					dynamic_cast<GeometryObject*>(player.get())->Translate(0.0f, -1.0f, 0.0f);
				}
				break;

				case KeyCode::Up:
				{
					auto player = m_pScene->GetSceneObject("player2");
					dynamic_cast<GeometryObject*>(player.get())->Translate(0.0f, 1.0f, 0.0f);
				}
				break;

				case KeyCode::Down:
				{
					auto player = m_pScene->GetSceneObject("player2");
					dynamic_cast<GeometryObject*>(player.get())->Translate(0.0f, -1.0f, 0.0f);
				}
				break;
			}
		}
		break;
	}
}

void GameLogic::Update(float dt) {

}

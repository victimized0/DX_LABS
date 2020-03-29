#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "Math/SimpleMath.h"

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
{
	m_wndCaption = L"DX Katamari";
}

Game::~Game() {

}

bool Game::Initialize(int iconId, int width, int height) {
	if (!Engine::Initialize(iconId, width, height)) {
		return false;
	}

	CreateScene();
	return true;
}

void Game::CreateScene() {

}

void Game::OnEvent(const Event& event) {
	Event& _event = const_cast<Event&>(event);
	MouseEvent& mouseEvent = reinterpret_cast<MouseEvent&>(_event);
}

void Game::Update(float dt) {

}

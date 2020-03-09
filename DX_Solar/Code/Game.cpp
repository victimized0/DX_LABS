#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "SceneObjects\GeometryObject.h"

using std::vector;
using std::wstring;

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
{
	m_wndCaption = L"DX Solar System";
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
}

Game::~Game() {

}

bool Game::Initialize() {
	if (!Engine::Initialize(IDI_ICON1)) {
		return false;
	}

	InitializeBuffers();

	return true;
}

void Game::OnEvent(const Event& event) {

}

void Game::Update(float dt) {
	UNREFERENCED_PARAMETER(dt);
}

void Game::InitializeBuffers() {

}

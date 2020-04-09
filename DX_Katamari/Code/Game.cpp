#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "Math/SimpleMath.h"

using std::vector;

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

	if (!m_gameLogic.Initialise(&m_scene)) {
		return false;
	}

	gEnv.Renderer()->SetBackColor(0.529f, 0.808f, 0.922f);
	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto ground = std::make_shared<GeometryObject>(NAME_GROUND, XMFLOAT3(0.0f, 0.0f, 0.0f));
	
	vector<GeometryObject::VertexType> groundVertices =
	{
		{ XMFLOAT3(-GROUND_WIDTH,	0,	-GROUND_HEIGHT), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(GROUND_WIDTH,	0,	-GROUND_HEIGHT), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-GROUND_WIDTH,	0,	GROUND_HEIGHT), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(GROUND_WIDTH,	0,	GROUND_HEIGHT), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	vector<UINT> groundIndices =
	{
		0, 2, 1,
		2, 3, 1
	};

	ground->CreateVertices(groundVertices);
	ground->CreateIndices(groundIndices);

	m_scene.AddObject(ground);
}

void Game::OnEvent(const Event& event) {
	m_gameLogic.OnEvent(event);
}

void Game::Update(float dt) {
	m_gameLogic.Update(dt);
}

#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "GameObjects\Ball.h"
#include "SceneObjects\GeometryObject.h"

using std::vector;

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
{
	m_wndCaption = L"DX Pong";
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
}

Game::~Game() {

}

bool Game::Initialize(int iconId) {
	if (!Engine::Initialize(iconId)) {
		return false;
	}

	if (!m_gameLogic.Initialise(&m_scene)) {
		return false;
	}

	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto ball	 = std::make_shared<Ball>( NAME_BALL, XMFLOAT3(0.0f, 0.0f, 0.0f) );
	auto lPaddle = std::make_shared<GeometryObject>( NAME_LPADDLE, XMFLOAT3(-27.0f, 0.0f, 0.0f) );
	auto rPaddle = std::make_shared<GeometryObject>( NAME_RPADDLE, XMFLOAT3(27.0f, 0.0f, 0.0f) );

	vector<GeometryObject::VertexType> lPaddleVertices =
	{
		{ XMFLOAT3(-1.0f,	4.0f,	0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,	4.0f,	0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,	-4.0f,	0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,	-4.0f,	0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	vector<UINT> lPaddleIndices =
	{
		0, 2, 1,
		2, 3, 1
	};

	vector<GeometryObject::VertexType> rPaddleVertices(lPaddleVertices);
	vector<UINT> rPaddleIndices(lPaddleIndices);

	vector<GeometryObject::VertexType> ballVertices =
	{
		{ XMFLOAT3(-0.5f,	0.5f,	0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f,	0.5f,	0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f,	-0.5f,	0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f,	-0.5f,	0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};
	vector<UINT> ballIndices(lPaddleIndices);

	lPaddle->CreateVertices(lPaddleVertices);
	lPaddle->CreateIndices(lPaddleIndices);

	rPaddle->CreateVertices(rPaddleVertices);
	rPaddle->CreateIndices(rPaddleIndices);
	
	ball->CreateVertices(ballVertices);
	ball->CreateIndices(ballIndices);

	m_scene.AddObject(lPaddle);
	m_scene.AddObject(rPaddle);
	m_scene.AddObject(ball);
}

void Game::OnEvent(const Event& event) {
	m_gameLogic.OnEvent(event);
}

void Game::Update(float dt) {
	m_gameLogic.Update(dt);
}

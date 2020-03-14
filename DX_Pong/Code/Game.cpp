#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "GameObjects\Paddle.h"
#include "GameObjects\Ball.h"
#include "SceneObjects\GeometryObject.h"

using std::vector;
using std::wstring;

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
	CreateShaders();

	return true;
}

void Game::CreateShaders() {
	D3DBlob* vsBlob;
	int vsId = m_renderer->CreateVertexShader("/data/shaders/standard_vs.cso", &vsBlob);
	int ilId = m_renderer->CreateInputLayout(const_cast<InputElementDesc*>(SimpleVertexColour::InputElements),
											SimpleVertexColour::ElementsCount, vsBlob);

	int psId = m_renderer->CreatePixelShader("/data/shaders/standard_ps.cso");

	auto pBall	= m_scene.GetSceneObject("ball").get();
	auto pP1	= m_scene.GetSceneObject("player1").get();
	auto pP2	= m_scene.GetSceneObject("player2").get();

	dynamic_cast<GeometryObject*>(pBall)->SetVertexShaderId(vsId);
	dynamic_cast<GeometryObject*>(pBall)->SetPixelShaderId(psId);
	dynamic_cast<GeometryObject*>(pBall)->SetInputLayoutId(ilId);

	dynamic_cast<GeometryObject*>(pP1)->SetVertexShaderId(vsId);
	dynamic_cast<GeometryObject*>(pP1)->SetPixelShaderId(psId);
	dynamic_cast<GeometryObject*>(pP1)->SetInputLayoutId(ilId);

	dynamic_cast<GeometryObject*>(pP2)->SetVertexShaderId(vsId);
	dynamic_cast<GeometryObject*>(pP2)->SetPixelShaderId(psId);
	dynamic_cast<GeometryObject*>(pP2)->SetInputLayoutId(ilId);
}

void Game::CreateScene() {
	auto ball	 = std::make_shared<Ball>("ball", XMFLOAT3(0.0f, 0.0f, 0.0f));
	auto player1 = std::make_shared<Paddle>("player1", XMFLOAT3(28.0f, 0.0f, 0.0f));
	auto player2 = std::make_shared<Paddle>("player2", XMFLOAT3(-28.0f, 0.0f, 0.0f));

	vector<SimpleVertexColour>	player1Vertices =
	{
		{ XMFLOAT3(-1.0f,	4.0f,	0.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,	4.0f,	0.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,	-4.0f,	0.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,	-4.0f,	0.0f),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	vector<UINT>				player1Indices =
	{
		0, 2, 1,
		2, 3, 1
	};

	vector<SimpleVertexColour>	player2Vertices(player1Vertices);
	vector<UINT>				player2Indices(player1Indices);

	vector<SimpleVertexColour>	ballVertices =
	{
		{ XMFLOAT3(-0.5f,	0.5f,	0.0f),	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f,	0.5f,	0.0f),	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f,	-0.5f,	0.0f),	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f,	-0.5f,	0.0f),	XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};
	vector<UINT>				ballIndices(player1Indices);

	player1->CreateVertices(player1Vertices);
	player1->CreateIndices(player1Indices);
	player1->SetSize(2, 8);

	player2->CreateVertices(player2Vertices);
	player2->CreateIndices(player2Indices);
	player2->SetSize(2, 8);
	
	ball->CreateVertices(ballVertices);
	ball->CreateIndices(ballIndices);
	ball->SetSize(1);

	player1->SetBall(ball.get());
	player2->SetBall(ball.get());

	m_scene.AddObject(player1);
	m_scene.AddObject(player2);
	m_scene.AddObject(ball);
}

void Game::OnEvent(const Event& event) {
	m_gameLogic.OnEvent(event);
}

void Game::Update(float dt) {
	UNREFERENCED_PARAMETER(dt);
	m_gameLogic.Update(dt);
}

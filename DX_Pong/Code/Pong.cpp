#include "pch.h"
#include "Pong.h"
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

bool Game::Initialize() {
	if (!Engine::Initialize(IDI_ICON1)) {
		return false;
	}

	if (!m_gameLogic.Initialise(&m_scene)) {
		return false;
	}

	auto player1 = std::make_shared<Paddle>("player1", XMFLOAT3(28.0f, 0.0f, 0.0f));
	auto player2 = std::make_shared<Paddle>("player2", XMFLOAT3(-28.0f, 0.0f, 0.0f));
	auto ball    = std::make_shared<Ball>("ball", XMFLOAT3(0.0f, 0.0f, 0.0f));

	vector<SimpleVertexColour>	player1Vertices =
	{
		{ XMFLOAT3(	-1.0f,	4.0f,	0.0f	),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(	1.0f,	4.0f,	0.0f	),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(	-1.0f,	-4.0f,	0.0f	),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(	1.0f,	-4.0f,	0.0f	),	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
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

	player1->SetVertices(player1Vertices);
	player1->SetIndices(player1Indices);
	player1->SetSize(2, 8);

	player2->SetVertices(player2Vertices);
	player2->SetIndices(player2Indices);
	player2->SetSize(2, 8);

	ball->SetVertices(ballVertices);
	ball->SetIndices(ballIndices);
	ball->SetSize(1);

	player1->SetBall(ball.get());
	player2->SetBall(ball.get());

	m_scene.AddObject(player1);
	m_scene.AddObject(player2);
	m_scene.AddObject(ball);

	InitializeBuffers();

	return true;
}

void Game::OnEvent(const Event& event) {
	m_gameLogic.OnEvent(event);
}

void Game::Update(float dt) {
	UNREFERENCED_PARAMETER(dt);
	m_gameLogic.Update(dt);
}

void Game::InitializeBuffers() {
	auto ball = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject("ball").get());
	auto player1 = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject("player1").get());
	auto player2 = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject("player2").get());

	ball->CreateVertexBuffer(m_renderer->GetDevice(), D3D11_USAGE_DYNAMIC);
	ball->CreateIndexBuffer(m_renderer->GetDevice());

	player1->CreateVertexBuffer(m_renderer->GetDevice(), D3D11_USAGE_DYNAMIC);
	player1->CreateIndexBuffer(m_renderer->GetDevice());

	player2->CreateVertexBuffer(m_renderer->GetDevice(), D3D11_USAGE_DYNAMIC);
	player2->CreateIndexBuffer(m_renderer->GetDevice());
}

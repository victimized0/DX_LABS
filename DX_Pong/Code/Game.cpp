#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "GameObjects\Paddle.h"
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
	CreateShaders();

	return true;
}

void Game::CreateShaders() {
	D3DBlob* vsBlob;
	int vsId = m_renderer->CreateVertexShader("/data/shaders/standard_vs.cso", &vsBlob);
	int psId = m_renderer->CreatePixelShader("/data/shaders/standard_ps.cso");

	int ilId = m_renderer->CreateInputLayout(const_cast<InputElementDesc*>(GeometryObject::VertexType::InputElements),
											GeometryObject::VertexType::ElementsCount, vsBlob);

	auto pBall	= dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_BALL).get());
	auto pP1	= dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_LPADDLE).get());
	auto pP2	= dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_RPADDLE).get());

	pBall->SetVertexShaderId(vsId);
	pBall->SetPixelShaderId(psId);
	pBall->SetInputLayoutId(ilId);

	pP1->SetVertexShaderId(vsId);
	pP1->SetPixelShaderId(psId);
	pP1->SetInputLayoutId(ilId);

	pP2->SetVertexShaderId(vsId);
	pP2->SetPixelShaderId(psId);
	pP2->SetInputLayoutId(ilId);

	//D3DRSDesc desc = {};
	//desc.CullMode = D3D11_CULL_BACK;
	//desc.FillMode = D3D11_FILL_WIREFRAME;

	//int wfRSId = m_renderer->CreateRSState(&desc);
	//pBall->SetRSStateId(wfRSId);
}

void Game::CreateScene() {
	auto ball	 = std::make_shared<Ball>(  NAME_BALL,	  XMFLOAT3(0.0f, 0.0f, 0.0f));
	auto lPaddle = std::make_shared<Paddle>(NAME_LPADDLE, XMFLOAT3(-27.0f, 0.0f, 0.0f));
	auto rPaddle = std::make_shared<Paddle>(NAME_RPADDLE, XMFLOAT3(27.0f, 0.0f, 0.0f));

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
	lPaddle->SetSize(2, 8);

	rPaddle->CreateVertices(rPaddleVertices);
	rPaddle->CreateIndices(rPaddleIndices);
	rPaddle->SetSize(2, 8);
	
	ball->CreateVertices(ballVertices);
	ball->CreateIndices(ballIndices);
	ball->SetSize(1);

	m_scene.AddObject(lPaddle);
	m_scene.AddObject(rPaddle);
	m_scene.AddObject(ball);
}

void Game::OnEvent(const Event& event) {
	m_gameLogic.OnEvent(event);
}

void Game::Update(float dt) {
	UNREFERENCED_PARAMETER(dt);
	m_gameLogic.Update(dt);
}

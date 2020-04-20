#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "GameObjects/Ball.h"
#include "SceneObjects/GeometryObject.h"
using std::vector;

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
	, m_paddleSpeed(25.0f)
{
	m_wndCaption = L"DX Pong";
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
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
	
	m_keyboard = std::make_unique<Keyboard>();

	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto ball	 = std::make_shared<Ball>( NAME_BALL, Vector3(0.0f, 0.0f, 0.0f) );
	auto lPaddle = std::make_shared<GeometryObject>( NAME_LPADDLE, Vector3(26.0f, 0.0f, 0.0f) );
	auto rPaddle = std::make_shared<GeometryObject>( NAME_RPADDLE, Vector3(-26.0f, 0.0f, 0.0f) );

	vector<GeometryObject::VertexType> lPaddleVertices =
	{
		{ Vector3(-1.0f, 4.0f,	0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f,	4.0f,	0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-1.0f, -4.0f,	0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(1.0f,	-4.0f,	0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) }
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
		{ Vector3(-0.5f, 0.5f,	0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f,	0.5f,	0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-0.5f, -0.5f,	0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(0.5f,	-0.5f,	0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) }
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

	//m_scene.GetMainCamera()->Update();
}

void Game::Update(float dt) {
	auto lpaddle = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_LPADDLE));
	auto rpaddle = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_RPADDLE));

	m_gameLogic.Update(dt);
	auto kb = m_keyboard->GetState();

	float dv = m_paddleSpeed * dt;

	if (kb.W) {
		float distToTop = fabs(lpaddle->GetWorld().Translation().y + 2 * lpaddle->GetBoundingBox().Extents.y / 2 - GRID_TOP_BORDER);
		if (distToTop > dv) {
			lpaddle->Translate(Vector3::Up * dv);
		}
	}

	if (kb.S) {
		float distToBottom = fabs(lpaddle->GetWorld().Translation().y - 2 * lpaddle->GetBoundingBox().Extents.y / 2 - GRID_BOTTOM_BORDER);
		if (distToBottom > dv) {
			lpaddle->Translate(-Vector3::Up * dv);
		}
	}

	if (kb.Up) {
		float distToTop = fabs(rpaddle->GetWorld().Translation().y + 2 * rpaddle->GetBoundingBox().Extents.y / 2 - GRID_TOP_BORDER);
		if (distToTop > dv) {
			rpaddle->Translate(Vector3::Up * dv);
		}
	}

	if (kb.Down) {
		float distToBottom = fabs(rpaddle->GetWorld().Translation().y - 2 * rpaddle->GetBoundingBox().Extents.y / 2 - GRID_BOTTOM_BORDER);
		if (distToBottom > dv) {
			rpaddle->Translate(-Vector3::Up * dv);
		}
	}
}

#include "pch.h"
#include "Game.h"
#include "Helper.h"
#include <random>

#include "Math/SimpleMath.h"
#include "GameObjects/Katamari.h"
#include "GameObjects/Pickup.h"
#include "MeshLoaders/ObjLoader.h"

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

	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(gEnv.HWnd);
	m_mouse->SetMode(Mouse::MODE_RELATIVE);

	gEnv.Renderer()->SetBackColor(0.529f, 0.808f, 0.922f);

	m_dirLight.LightCol = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_dirLight.LightAmb = DirectX::SimpleMath::Vector4(0.15f, 0.15f, 0.15f, 1.0f);
	m_dirLight.LightDir = DirectX::SimpleMath::Vector3(-1.0f, -1.0f, -1.0f);

	gEnv.Renderer()->SetSunLight(&m_dirLight);

	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto ground = std::make_unique<GameObject>(NAME_TERRAIN);

	vector<VertexPosColNmlTex> groundVertices =
	{
		{ Vector3(-TERRAIN_SIZE, 0, -TERRAIN_SIZE),	Vector3::One, Vector3::One, Vector2::Zero },
		{ Vector3(-TERRAIN_SIZE, 0, TERRAIN_SIZE),	Vector3::One, Vector3::One, Vector2::Zero },
		{ Vector3(TERRAIN_SIZE, 0, TERRAIN_SIZE),	Vector3::One, Vector3::One, Vector2::Zero },
		{ Vector3(TERRAIN_SIZE, 0, -TERRAIN_SIZE),	Vector3::One, Vector3::One, Vector2::Zero }
	};

	vector<UINT> groundIndices =
	{
		0, 1, 3,
		1, 2, 3
	};

	Mesh groundMesh("terrain");
	groundMesh.CreateVertices(groundVertices);
	groundMesh.CreateIndices(groundIndices);
	ground->GetModel().AddMesh(groundMesh);

	auto player = std::make_unique<Katamari>(NAME_PLAYER, Vector3(0, 7, 0), MeshesPath + "Apple.obj", 0.2f);

	m_scene.AddObject( std::move(ground) );
	m_scene.AddObject( std::move(player) );

	GeneratePickups();
}

void Game::GeneratePickups() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(-1000, 1000);

	int objectsToGenerate = 30;

	////////////////////// Generate rings

	for (size_t i = 0; i < objectsToGenerate; ++i) {
		auto obj = std::make_unique<Pickup>("ring_" + std::to_string(i), Vector3(dist(rng), 0, dist(rng)), MeshesPath + "Ring.obj");
		m_scene.AddObject( std::move(obj) );
	}

	////////////////////// Generate apples

	for (size_t i = 0; i < objectsToGenerate; ++i) {
		auto obj = std::make_unique<Pickup>("apple_" + std::to_string(i), Vector3(dist(rng), 4.0f, dist(rng)), MeshesPath + "Apple.obj", 0.1f);
		m_scene.AddObject( std::move(obj) );
	}

	////////////////////// Generate eggs

	for (size_t i = 0; i < objectsToGenerate; ++i) {
		auto obj = std::make_unique<Pickup>("egg_" + std::to_string(i), Vector3(dist(rng), 0, dist(rng)), MeshesPath + "Egg_OBJ.obj");
		m_scene.AddObject( std::move(obj) );
	}

	auto well = std::make_unique<Pickup>("well_01", Vector3(dist(rng), 0, dist(rng)), MeshesPath + "Well.obj", 10.0f);
	m_scene.AddObject( std::move(well) );

}

void Game::Update(float dt) {
	OnMouseUpdate(dt);
	OnKeyboardUpdate(dt);
	m_gameLogic.Update(dt);
}

void Game::OnMouseUpdate(float dt) {
	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == Mouse::MODE_RELATIVE) {
		Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0) * dt;

		auto camera = m_scene.GetMainCamera();
		float dx = -delta.x * 1.5f;
		float dy = -delta.y * 0.2f;
		camera->Rotate(dx, dy);
	}
}

void Game::OnKeyboardUpdate(float dt) {
	auto camera = m_scene.GetMainCamera();
	auto player = dynamic_cast<Katamari*>(m_scene.GetSceneObject(NAME_PLAYER));
	auto kb = m_keyboard->GetState();
	float moveSpeed = 2.0f;
	float spinSpeed = 0.1f;

	Vector3 viewFwd = Vector3::TransformNormal(Vector3::Forward, camera->GetView().Invert());
	Vector3 moveFwd = Vector3(viewFwd.x, 0, viewFwd.z);
	Vector3 viewRight = Vector3::TransformNormal(Vector3::Right, camera->GetView().Invert());

	if (kb.LeftShift) {
		moveSpeed = 10.0f;
	}
	else {
		moveSpeed = 2.0f;
	}

	if (kb.W) {
		player->Translate(moveFwd * moveSpeed);
		player->Rotate(viewRight, spinSpeed);
	}

	if (kb.A) {
		player->Translate(-viewRight * moveSpeed);
		player->Rotate(viewFwd, spinSpeed);
	}

	if (kb.S) {
		player->Translate(-moveFwd * moveSpeed);
		player->Rotate(-viewRight, spinSpeed);
	}

	if (kb.D) {
		player->Translate(viewRight * moveSpeed);
		player->Rotate(-viewFwd, spinSpeed);
	}

	if (kb.Z) {
		player->Rescale(1.1f);
	}

	if (kb.X) {
		player->Rescale(0.9f);
	}

	if (kb.Left) {
		m_dirLight.LightDir.x -= 0.1f;
	}

	if (kb.Right) {
		m_dirLight.LightDir.x += 0.1f;
	}

	if (kb.Up) {
		m_dirLight.LightDir.z += 0.1f;
	}

	if (kb.Down) {
		m_dirLight.LightDir.z -= 0.1f;
	}

	if (kb.LeftControl && kb.R) {
		m_dirLight.LightDir = DirectX::SimpleMath::Vector3(-1.0f, -1.0f, -1.0f);
	}

	camera->SetTarget(player->Position);
}

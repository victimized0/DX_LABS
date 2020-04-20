#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "Math/SimpleMath.h"
#include "GameObjects/Katamari.h"
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

	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto ground = std::make_shared<GeometryObject>(NAME_TERRAIN);
	m_scene.AddObject(ground);

	vector<GeometryObject::VertexType> groundVertices =
	{
		{ Vector3(-TERRAIN_SIZE, 0, -TERRAIN_SIZE),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(-TERRAIN_SIZE, 0, TERRAIN_SIZE),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(TERRAIN_SIZE, 0, TERRAIN_SIZE),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Vector3(TERRAIN_SIZE, 0, -TERRAIN_SIZE),	Vector4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	vector<UINT> groundIndices =
	{
		0, 1, 3,
		1, 2, 3
	};

	ground->CreateVertices(groundVertices);
	ground->CreateIndices(groundIndices);
	
	ObjLoader test;
	test.LoadFile(MeshesPath + "Mars_2K.obj");

	auto player = std::make_shared<Katamari>(NAME_PLAYER, test.LoadedMeshes[0], Vector3(0, 3.25f, 0));
	m_scene.AddObject(player);

	GeneratePickups();
}

void Game::GeneratePickups() {
	ObjLoader test;
	test.LoadFile(MeshesPath + "Apricot_02_hi_poly.obj");

	string objName = "$obj_" + m_scene.GetSceneObjects().size();
	auto obj = std::make_shared<GeometryObject>(objName, test.LoadedMeshes[0], Vector3(10.2f, 0, 3.4f));
	m_scene.AddObject(obj);
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
	float speed = 1.0f;

	Vector3 viewFwd = Vector3::TransformNormal(Vector3::Forward, camera->GetView().Invert());
	Vector3 moveFwd = Vector3(viewFwd.x, 0, viewFwd.z);
	Vector3 viewLeft = Vector3::TransformNormal(Vector3::Left, camera->GetView().Invert());

	if (kb.W) {
		player->Translate(moveFwd * speed);
	}

	if (kb.A) {
		player->Translate(viewLeft * speed);
	}

	if (kb.S) {
		player->Translate(-moveFwd * speed);
	}

	if (kb.D) {
		player->Translate(-viewLeft * speed);
	}

	camera->SetTarget(player->Position);
}

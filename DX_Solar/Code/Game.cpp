#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "SceneObjects/Model.h"
#include "SceneObjects/Mesh.h"

#include "VertexFormats.h"
#include "Math/SimpleMath.h"
#include "Helper.h"

using std::vector;
using std::wstring;

using namespace DirectX;
using namespace DirectX::SimpleMath;

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
{
	m_wndCaption = L"DX Solar System";
}

Game::~Game() {

}

bool Game::Initialize(int iconId, int width, int height) {
	if (!Engine::Initialize(iconId, width, height)) {
		return false;
	}

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(gEnv.HWnd);

	CreateScene();
	return true;
}

void Game::CreateScene() {
	Mesh sunMesh("sun_mesh");
	::CreateIcosahedron(20.0f, sunMesh.Vertices(), sunMesh.Indices());
	sunMesh.Initialise(gEnv.Renderer()->GetDevice());

	auto sun = std::make_unique<Model>(NAME_SUN);
	sun->AddMesh(std::move(sunMesh));
	m_scene.AddObject(std::move(sun));

	//Mesh<VertexType::VertexPosCol> earthMesh("earth_mesh", EVertexType::P3F_C3F);
	//::CreateIcosahedron(5.0f, earthMesh.Vertices(), earthMesh.Indices());
	//earthMesh.Initialise(gEnv.Renderer()->GetDevice());

	//auto earth = std::make_unique<Model>(NAME_EARTH, Vector3(75, 0, 0));
	//earth->AddMesh(earthMesh);
	//m_scene.AddObject(std::move(earth));

	//auto moon = std::make_shared<SolarObject>(NAME_MOON, Vector3(65, 0, 0));
	//auto pMoon = dynamic_cast<GameObject*>(moon.get());
	//::CreateIcosahedron(&pMoon, 1.f, 0.2f, 0.2f, 0.2f);

	//m_scene.AddObject(earth);
	//m_scene.AddObject(moon);

	m_scene.GetMainCamera()->SetRadius(200);
}

void Game::Update(float dt) {
	auto mouse = m_mouse->GetState();

	if (mouse.positionMode == Mouse::MODE_RELATIVE) {
		auto camera = m_scene.GetMainCamera();
		Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f) * 0.1f;

		camera->Orbit(-delta.x, -delta.y);
	}

	m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

	//auto pSun = dynamic_cast<Model*>(m_scene.GetSceneObject(NAME_SUN));
}
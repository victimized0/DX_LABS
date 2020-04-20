#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "SceneObjects/GeometryObject.h"
#include "GameObjects/SolarObject.h"
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
	auto sun = std::make_shared<SolarObject>(NAME_SUN, Vector3(0, 0, 0));
	auto pSun = dynamic_cast<GeometryObject*>(sun.get());
	::CreateIcosahedron(&pSun, 20.0f, 1.0f, 1.0f, 0.0f);

	auto earth = std::make_shared<SolarObject>(NAME_EARTH, Vector3(75, 0, 0));
	auto pEarth = dynamic_cast<GeometryObject*>(earth.get());
	::CreateIcosahedron(&pEarth, 5.f, 0.0f, 0.0f, 1.0f);
	
	auto moon = std::make_shared<SolarObject>(NAME_MOON, Vector3(65, 0, 0));
	auto pMoon = dynamic_cast<GeometryObject*>(moon.get());
	::CreateIcosahedron(&pMoon, 1.f, 0.2f, 0.2f, 0.2f);

	m_scene.AddObject(sun);
	m_scene.AddObject(earth);
	m_scene.AddObject(moon);

	m_scene.GetMainCamera()->SetRadius(200);
}

void Game::Update(float dt) {
	auto mouse = m_mouse->GetState();

	if (mouse.positionMode == Mouse::MODE_RELATIVE) {
		auto camera = m_scene.GetMainCamera();
		Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f) * 0.1f;

		camera->Rotate(-delta.x, -delta.y);
	}

	m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

	auto pSun	= dynamic_cast<SolarObject*>(m_scene.GetSceneObject(NAME_SUN));
	auto pEarth = dynamic_cast<SolarObject*>(m_scene.GetSceneObject(NAME_EARTH));
	auto pMoon	= dynamic_cast<SolarObject*>(m_scene.GetSceneObject(NAME_MOON));

	pSun->Rotate(0, 0, XM_PI);
	pEarth->Rotate(0, 0, XM_PI);
	pMoon->Rotate(0, 0, XM_PI);
}
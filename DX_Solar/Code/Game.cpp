#include "pch.h"
#include "Game.h"
#include "Helper.h"

#include "SceneObjects\GeometryObject.h"

using std::vector;
using std::wstring;

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
{
	m_wndCaption = L"DX Solar System";
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
}

Game::~Game() {

}

bool Game::Initialize(int iconId, int width, int height) {
	if (!Engine::Initialize(iconId, width, height)) {
		return false;
	}

	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto sun = std::make_shared<GeometryObject>(NAME_SUN, XMFLOAT3(0, 0, 0));
	auto pSun = sun.get();
	::CreateIcosahedron(&pSun, 20.0f, 1.0f, 1.0f, 0.0f);

	auto earth = std::make_shared<GeometryObject>(NAME_EARTH, XMFLOAT3(75, 0, 0));
	auto pEarth = earth.get();
	::CreateIcosahedron(&pEarth, 5.f, 0.0f, 0.0f, 1.0f);
	
	auto moon = std::make_shared<GeometryObject>(NAME_MOON, XMFLOAT3(65, 0, 0));
	auto pMoon = moon.get();
	::CreateIcosahedron(&pMoon, 1.f, 0.2f, 0.2f, 0.2f);

	m_scene.AddObject(sun);
	m_scene.AddObject(earth);
	m_scene.AddObject(moon);

	m_scene.GetMainCamera().SetRadius(200);
}

void Game::OnEvent(const Event& event) {
	Event& _event = const_cast<Event&>(event);
	MouseEvent& mouseEvent = reinterpret_cast<MouseEvent&>(_event);

	switch (event.GetType()) {
	case EventType::MouseDown:
		OnMouseDown(mouseEvent);
		break;
	case EventType::MouseUp:
		OnMouseUp(mouseEvent);
		break;
	case EventType::MouseMove:
		OnMouseMove(mouseEvent);
		break;
	case EventType::MouseScroll:
		OnMouseScroll(mouseEvent);
		break;
	}
}

void Game::Update(float dt) {
	auto pSun	= dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_SUN));
	auto pEarth = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_EARTH));
	auto pMoon	= dynamic_cast<GeometryObject*>(m_scene.GetSceneObject(NAME_MOON));
	
	pSun->Rotate(sinf(dt) * 2.f, 0, 0);
	pEarth->Orbit(sinf(dt) * 2.f, pSun->GetPosition());
	pMoon->Orbit(sinf(dt) * 1.5f, pEarth->GetPosition());
}

void Game::OnMouseDown(const MouseEvent& event) {
	m_lastMousePos.x = event.GetPosX();
	m_lastMousePos.y = event.GetPosY();
	SetCapture(gEnv.HWnd);
}

void Game::OnMouseUp(const MouseEvent& event) {
	UNREFERENCED_PARAMETER(event);
	ReleaseCapture();
}

void Game::OnMouseMove(const MouseEvent& event) {
	auto& camera = m_scene.GetMainCamera();

	if (event.GetKeyState() == MouseKeyState::LButton) {
		float dx = XMConvertToRadians(0.5f * static_cast<float>(event.GetPosX() - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.5f * static_cast<float>(event.GetPosY() - m_lastMousePos.y));
		camera.Rotate(-dx, -dy);
	}
	else if (event.GetKeyState() == MouseKeyState::RButton) {
		float dy = XMConvertToRadians(0.5f * static_cast<float>(event.GetPosY() - m_lastMousePos.y));
		camera.Move(dy);
	}

	m_lastMousePos.x = event.GetPosX();
	m_lastMousePos.y = event.GetPosY();
}

void Game::OnMouseScroll(const MouseEvent& event) {
	auto& camera = m_scene.GetMainCamera();
	camera.Zoom(10 * -0.002f * GET_WHEEL_DELTA_WPARAM(event.GetParam()));
}

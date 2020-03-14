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

bool Game::Initialize() {
	if (!Engine::Initialize(IDI_ICON1)) {
		return false;
	}

	auto sun = std::make_shared<GeometryObject>("TheSun", XMFLOAT3(0,0,0));
	auto pSun = sun.get();
	::CreateIcosahedron(&pSun, 2.0f);

	pSun->CreateVertexBuffer(m_renderer->GetDevice(), D3D11_USAGE_DYNAMIC);
	pSun->CreateIndexBuffer(m_renderer->GetDevice());

	auto earth = std::make_shared<GeometryObject>("Earth", XMFLOAT3(0, 5.0f, 0));
	auto pEarth = earth.get();
	::CreateIcosahedron(&pEarth, 1.0f);

	pEarth->CreateVertexBuffer(m_renderer->GetDevice(), D3D11_USAGE_DYNAMIC);
	pEarth->CreateIndexBuffer(m_renderer->GetDevice());

	m_scene.AddObject(sun);
	m_scene.AddObject(earth);

	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.CullMode = D3D11_CULL_BACK;

	if (!m_renderer->SetRasterizerState(rastDesc)) {
		return false;
	}

	return true;
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
	UNREFERENCED_PARAMETER(dt);
	auto pSun = dynamic_cast<GeometryObject*>(m_scene.GetSceneObject("TheSun").get());
	pSun->Translate(dt, 0, 0);
}

void Game::OnMouseDown(const MouseEvent& event) {
	m_lastMousePos.x = event.GetPosX();
	m_lastMousePos.y = event.GetPosY();
	SetCapture(Environment::Instance().GetWindowHandle());
}

void Game::OnMouseUp(const MouseEvent& event) {
	ReleaseCapture();
}

void Game::OnMouseMove(const MouseEvent& event) {
	auto camera = dynamic_cast<Camera*>(m_scene.GetSceneObject("Main Camera").get());

	if (event.GetKeyState() == MouseKeyState::LButton) {
		float dx = XMConvertToRadians(0.5f * static_cast<float>(event.GetPosX() - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.5f * static_cast<float>(event.GetPosY() - m_lastMousePos.y));
		camera->Rotate(-dx, -dy);
	}
	else if (event.GetKeyState() == MouseKeyState::RButton) {
		float dy = XMConvertToRadians(0.5f * static_cast<float>(event.GetPosY() - m_lastMousePos.y));
		camera->Move(dy);
	}

	m_lastMousePos.x = event.GetPosX();
	m_lastMousePos.y = event.GetPosY();
}

void Game::OnMouseScroll(const MouseEvent& event) {
	auto camera = dynamic_cast<Camera*>(m_scene.GetSceneObject("Main Camera").get());
	camera->Zoom(-0.002f * GET_WHEEL_DELTA_WPARAM(event.GetParam()));
}

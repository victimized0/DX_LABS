#include "pch.h"
#include "Game.h"

#include "Model.h"
#include "Mesh.h"
#include "Helper.h"
#include "Math/SimpleMath.h"

using std::vector;
using std::wstring;

using namespace DirectX;
using namespace DirectX::SimpleMath;

Game::Game(HINSTANCE hInstance)
	: Engine(hInstance)
{
	m_wndCaption = L"DX Solar System";
}

bool Game::Initialize(int iconId, int width, int height) {
	if (!Engine::Initialize(iconId, width, height)) {
		return false;
	}

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(gEnv.HWnd);
	m_mouse->SetMode(Mouse::MODE_RELATIVE);

	CreateScene();
	return true;
}

void Game::CreateScene() {
	auto sun	= std::make_unique<GameObject>(NAME_SUN);
	auto earth	= std::make_unique<GameObject>(NAME_EARTH, Vector3(200, 0, 0));
	auto moon	= std::make_unique<GameObject>(NAME_MOON,  Vector3(25, 0, 0));

	Mesh sunMesh	( "sun_mesh" );
	Mesh earthMesh	( "earth_mesh" );
	Mesh moonMesh	( "moon_mesh" );

	::CreateIcosahedron(50.0f, sunMesh.Vertices(),	 sunMesh.Indices());
	::CreateIcosahedron(10.0f, earthMesh.Vertices(), earthMesh.Indices());
	::CreateIcosahedron(2.0f,  moonMesh.Vertices(),  moonMesh.Indices());

	sunMesh.SetColor( Vector3(1.0f, 1.0f, 0.0f) );
	earthMesh.SetColor( Vector3(0.0f, 0.0f, 1.0f) );
	moonMesh.SetColor( Vector3(1.0f, 1.0f, 1.0f) );

	sunMesh.SetRenderFlags(0);
	earthMesh.SetRenderFlags(0);
	moonMesh.SetRenderFlags(0);

	sun->GetModel().AddMesh(sunMesh);
	earth->GetModel().AddMesh(earthMesh);
	moon->GetModel().AddMesh(moonMesh);

	earth->AddChild( std::move(moon) );
	sun->AddChild( std::move(earth) );

	m_scene.AddObject( std::move(sun) );
	m_scene.GetMainCamera()->Radius = 600;
}

void Game::Update(float dt) {
	ProcessMouse();

	auto pSun	= dynamic_cast<GameObject*>( m_scene.GetSceneObject(NAME_SUN) );
	auto pEarth	= dynamic_cast<GameObject*>( m_scene.GetSceneObject(NAME_EARTH) );
	auto pMoon	= dynamic_cast<GameObject*>( m_scene.GetSceneObject(NAME_MOON) );

	if (pSun)	pSun->Rotate(Vector3::Up, 0.01f);
	if (pEarth)	pEarth->Rotate(Vector3::Up, 0.1f);
	if (pMoon)	pMoon->Rotate(Vector3::Up, 0.1f);
}

void Game::ProcessMouse() {
	auto camera	= m_scene.GetMainCamera();
	auto mouse	= m_mouse->GetState();

	if (mouse.leftButton) {
		auto delta = Vector3( float(mouse.x), float(mouse.y), 0.f ) * 0.1f;
		camera->Rotate(-delta.x, -delta.y);
	}

	if (mouse.rightButton) {
		float dy = XMConvertToRadians(5 * float(mouse.y));
		camera->Move(dy);
	}

}

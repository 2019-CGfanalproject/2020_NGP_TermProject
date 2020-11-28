#include "pch.h"
#include "LoginScene.h"
#include "GameFramework.h"


LoginScene::LoginScene(GameFramework* framework)
	: Scene(framework, SceneID::LOGIN)
{
}

LoginScene::~LoginScene()
{
}

void LoginScene::Initialize()
{
    m_Framework->m_Objects.AddStaticObject(BitmapKey::TITLE, Vector2(0, 80));
    m_Framework->m_Objects.AddStaticObject(BitmapKey::IPADDR_EDITBAR, Vector2(150, 600));
}

void LoginScene::Destroy()
{

}

void LoginScene::Update()
{
}

void LoginScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			TryConnecting();
			m_Framework->m_SceneManager.ChangeScene(SceneID::LOBBY);
			break;
		default:
			break;
		}
		break;
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}
}

std::string loopback{ "127.0.0.1" };
std::string shphone{ "192.168.43.216" };

void LoginScene::TryConnecting()
{
	// ip 체크
	// nickname 체크
	bool connected = m_Framework->m_Communicator.Connect(shphone.c_str(), TEXT("혜리무"));

	if (connected) {
		m_Framework->m_SceneManager.ChangeScene(SceneID::GAME);
	}
	else {
		// 연결이 실패했다는 것을 사용자에게 알려준다
		OutputDebugString(TEXT("연결실패\n"));
	}
}

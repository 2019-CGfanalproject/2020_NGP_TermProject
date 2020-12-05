#include "pch.h"
#include "LoginScene.h"
#include "GameFramework.h"
#include "GameObject.h"

std::string loopback{ "127.0.0.1" };
std::string shphone{ "192.168.43.216" };
std::string wireless{ "192.168.122.264" };

LoginScene::LoginScene(GameFramework* framework)
	: Scene(framework, SceneID::LOGIN)
{
	m_IPAddrText = m_Framework->m_Objects.AddText();
	m_IPAddrText->m_Text.clear();
	m_IPAddrText->m_Left = 400;
	m_IPAddrText->m_Top = 400;
	m_IPAddrText->m_Right = 800;
	m_IPAddrText->m_Bottom = 500;
}

LoginScene::~LoginScene()
{
}

void LoginScene::Initialize()
{
	m_Framework->m_Objects.AddStaticObject(BitmapKey::LOGIN, Vector2(0, 0));
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
		case VK_SPACE: {
			// ����׸� ���� ������ ���� Ű
			bool connected = m_Framework->m_Communicator.Connect(loopback.c_str(), TEXT("������"));
			if (connected) m_Framework->m_SceneManager.ChangeScene(SceneID::LOBBY);
			break;
		}
		case VK_RETURN:
			TryConnecting();
			break;
		default:
			break;
		}
		break;
	case WM_CHAR: {
		m_IPAddrText->m_Text.push_back((const wchar_t)wParam);
		//m_Framework->m_Objects.
		//OutputDebugStringW(m_Text.c_str());
		//OutputDebugStringW(L"\n");
		break;
	}
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}
}


void LoginScene::TryConnecting()
{
	// ANSI�� ��ȯ
	std::string addr;
	addr.assign(m_IPAddrText->m_Text.begin(), m_IPAddrText->m_Text.end());

	// ����
	bool connected = m_Framework->m_Communicator.Connect(addr.c_str(), TEXT("������"));

	// ����ó��
	if (connected) {
		m_Framework->m_SceneManager.ChangeScene(SceneID::LOBBY);
	}
	else {
		// ������ �����ߴٴ� ���� ����ڿ��� �˷��ش�
		OutputDebugString(TEXT("�������\n"));
	}
}

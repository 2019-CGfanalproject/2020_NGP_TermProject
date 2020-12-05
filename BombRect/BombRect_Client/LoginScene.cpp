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

	m_NicknameText = m_Framework->m_Objects.AddText();
	m_NicknameText->m_Text.clear();
	m_NicknameText->m_Left = 400;
	m_NicknameText->m_Top = 500;
	m_NicknameText->m_Right = 800;
	m_NicknameText->m_Bottom = 600;

	m_CurrFocusText = m_IPAddrText;
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
		case VK_F12: {
			// 디버그를 위한 루프백 연결 키
			bool connected = m_Framework->m_Communicator.Connect(loopback.c_str(), TEXT("혜리무"));
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
		switch (wParam) {
		case 8: {	// backspace
			if(0 < m_CurrFocusText->m_Text.size())
				m_CurrFocusText->m_Text.pop_back();
			break;
		}
		case '\t': break;
		case '\n': break;
		case ' ': break;
		default:
			m_CurrFocusText->m_Text.push_back((const wchar_t)wParam);
			break;
		}

		break;
	}
	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_TAB:
			if (m_CurrFocusText == m_IPAddrText) m_CurrFocusText = m_NicknameText;
			else m_CurrFocusText = m_IPAddrText;
			break;
		}
		break;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}
}

void LoginScene::TryConnecting()
{
	// ANSI로 변환
	std::string addr;
	addr.assign(m_IPAddrText->m_Text.begin(), m_IPAddrText->m_Text.end());

	// 연결
	bool connected = m_Framework->m_Communicator.Connect(addr.c_str(), m_NicknameText->m_Text);

	// 예외처리
	if (connected) {
		m_Framework->m_SceneManager.ChangeScene(SceneID::LOBBY);
	}
	else {
		// 연결이 실패했다는 것을 사용자에게 알려준다
		OutputDebugString(TEXT("연결실패\n"));
	}
}

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
	m_IPAddrText->m_Left = 450;
	m_IPAddrText->m_Top = 350 + 16;
	m_IPAddrText->m_Right = 800;
	m_IPAddrText->m_Bottom = 450;
	m_IPAddrText->font = FontKey::LOGIN;

	m_NicknameText = m_Framework->m_Objects.AddText();
	m_NicknameText->m_Text.clear();
	m_NicknameText->m_Left = 450;
	m_NicknameText->m_Top = 450 + 16;
	m_NicknameText->m_Right = 800;
	m_NicknameText->m_Bottom = 550;
	m_NicknameText->font = FontKey::LOGIN;

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
	m_Framework->m_Objects.m_DynamicObjects.clear();

	if (m_CurrFocusText == m_IPAddrText) {
		m_Framework->m_Objects.AddDynamicObject(BitmapKey::IPADDR_SELECTED_BAR, Vector2(290, 350));
		m_Framework->m_Objects.AddDynamicObject(BitmapKey::NICKNAME_BAR, Vector2(290, 450));
	}
	else {
		m_Framework->m_Objects.AddDynamicObject(BitmapKey::IPADDR_BAR, Vector2(290, 350));
		m_Framework->m_Objects.AddDynamicObject(BitmapKey::NICKNAME_SELECTED_BAR, Vector2(290, 450));
	}
	
}

void LoginScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYUP:
		switch (wParam) {
		case VK_F12: {
			// 디버그를 위한 루프백 연결 키
			bool connected = m_Framework->m_Communicator.Connect(shphone.c_str(), TEXT("혜리무"));
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
	case WM_IME_COMPOSITION: {
		int len;
		wchar_t letter;
		HIMC hIMC = ImmGetContext(m_Framework->m_Wnd);

		// 전에 들어온 문자가 조합중인 문자였다면, 마지막 글자를 뺀다.
		if (m_IsPreStrComp && 0 != m_CurrFocusText->m_Text.size()) {
			m_CurrFocusText->m_Text.pop_back();
		}

		if (lParam & GCS_RESULTSTR) {       // 완성된 글자만 가져온다
			len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
			ImmGetCompositionString(hIMC, GCS_RESULTSTR, &letter, len);
			m_CurrFocusText->m_Text.push_back(letter);
			m_IsPreStrComp = false;
		}

		else if (lParam & GCS_COMPSTR) {         // 자모 단위로 가져온다
			len = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);		// 길이를 가져온다
			ImmGetCompositionString(hIMC, GCS_COMPSTR, &letter, len);
			m_CurrFocusText->m_Text.push_back(letter);
			m_IsPreStrComp = true;
		}

		ImmReleaseContext(m_Framework->m_Wnd, hIMC);
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

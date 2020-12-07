#include "pch.h"
#include "LobbyScene.h"
#include "GameFramework.h"
#include "GameObject.h"

Vector2 slot_nickname_pos[4] = {
{325,	25 + 15 },
{575,	25 + 15 },
{325,	325 + 15 },
{575,	325 + 15 },
};

void LobbyScene::Initialize()
{
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::CHATTING_REGION,
		Vector2(20, 20)
	);

	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::CHATTING_INPUT_REGION,
		Vector2(20, 660)
	);

	m_ChattingText = m_Framework->m_Objects.AddText();
	m_ChattingText->m_Text = m_ChattingText->m_Text;
	m_ChattingText->m_Left = 20;
	m_ChattingText->m_Top = 660;
	m_ChattingText->m_Right = 300;
	m_ChattingText->m_Bottom = 780;
	m_ChattingText->font = FontKey::CHATTING;

	Vector2 slot_pos[4] = {
		{ 325, 25 }, { 575, 25 },
		{ 325, 325 }, { 575, 325 },
	};

	for (int i = 0; i < 4; ++i) {
		m_Framework->m_Objects.AddStaticObject(
			BitmapKey::PLAYER_SLOT,
			slot_pos[i]
		);

		m_Framework->m_Objects.m_ReadyState[i] = false;

		if (100 != m_Framework->m_Objects.m_Nicknames[i].id) {
			TextObject* name = m_Framework->m_Objects.AddText();
			name->m_Text = m_Framework->m_Objects.m_Nicknames[i].name;
			name->m_Left = slot_nickname_pos[i].x;
			name->m_Top = slot_nickname_pos[i].y;
			name->m_Right = slot_nickname_pos[i].x + 200;
			name->m_Bottom = slot_nickname_pos[i].y + 50;
			name->font = FontKey::NICKNAME;
			name->is_align_center = true;
		}
	}

	Vector2 left_origin{ 300, 600 };
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::READY_BUTTON,
		Vector2(50, 50) + left_origin
	);
}

void LobbyScene::Destroy()
{
}

void LobbyScene::Update()
{
	m_Framework->m_Objects.UpdateLobby();
}

void LobbyScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CHAR: {
		switch (wParam) {
		case 8: {	// backspace
			if (0 < m_ChattingText->m_Text.size())
				m_ChattingText->m_Text.pop_back();
			break;
		}
		case '\t': break;
		case '\r': break;
		// case ' ': break;
		default:
			m_ChattingText->m_Text.push_back((const wchar_t)wParam);
			break;
		}
		break;
	}
	case WM_IME_COMPOSITION: {
		int len;
		wchar_t letter;
		HIMC hIMC = ImmGetContext(m_Framework->m_Wnd);

		// 전에 들어온 문자가 조합중인 문자였다면, 마지막 글자를 뺀다.
		if (m_IsPreStrComp && 0 != m_ChattingText->m_Text.size()) {
			m_ChattingText->m_Text.pop_back();
		}

		if (lParam & GCS_RESULTSTR) {       // 완성된 글자만 가져온다
			len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
			ImmGetCompositionString(hIMC, GCS_RESULTSTR, &letter, len);
			m_ChattingText->m_Text.push_back(letter);
			m_IsPreStrComp = false;
		}

		else if (lParam & GCS_COMPSTR) {         // 자모 단위로 가져온다
			len = ImmGetCompositionStringW(hIMC, GCS_COMPSTR, NULL, 0);		// 길이를 가져온다
			ImmGetCompositionString(hIMC, GCS_COMPSTR, &letter, len);
			m_ChattingText->m_Text.push_back(letter);
			m_IsPreStrComp = true;
		}

		ImmReleaseContext(m_Framework->m_Wnd, hIMC);
		break;
	}
	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			// m_Framework->m_Communicator.SendChatting(m_ChattingText->m_Text);
			m_Framework->m_Communicator.SendReady();
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

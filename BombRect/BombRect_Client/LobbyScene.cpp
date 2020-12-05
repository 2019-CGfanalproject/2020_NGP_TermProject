#include "pch.h"
#include "LobbyScene.h"
#include "GameFramework.h"

void LobbyScene::Initialize()
{
	Vector2 slot_pos[4] = {
		{ 325, 25 }, { 575, 25 },
		{ 325, 325 }, { 575, 325 },
	};

	for (int i = 0; i < 4; ++i) {
		m_Framework->m_Objects.AddStaticObject(
			BitmapKey::PLAYER_SLOT,
			slot_pos[i]
		);
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
	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
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

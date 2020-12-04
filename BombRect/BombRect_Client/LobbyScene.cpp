#include "pch.h"
#include "LobbyScene.h"
#include "GameFramework.h"

void LobbyScene::Initialize()
{
	Vector2 slot_origin{ 300, 0 };
	Vector2 left_origin{ 300, 600 };
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::READY_BUTTON, 
		Vector2(50, 50) + left_origin
	);
	
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::PLAYER_SLOT, 
		Vector2(25,25) + slot_origin
	);
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::PLAYER_SLOT,
		Vector2(250 + 25, 25) + slot_origin
	);
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::PLAYER_SLOT,
		Vector2(25, 300 + 25) + slot_origin
	);
	m_Framework->m_Objects.AddStaticObject(
		BitmapKey::PLAYER_SLOT,
		Vector2(250 + 25, 300 + 25) + slot_origin
	);
}

void LobbyScene::Destroy()
{
}

void LobbyScene::Update()
{
	m_Framework->m_Objects.NicknameUpdate();	
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

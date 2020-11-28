#include "pch.h"
#include "LobbyScene.h"
#include "GameFramework.h"

void LobbyScene::Initialize()
{
	m_Framework->m_Objects.AddStaticObject(BitmapKey::READY_BUTTON, Vector2(300 + 10, 500 + 10));
}

void LobbyScene::Destroy()
{
}

void LobbyScene::Update()
{
}

void LobbyScene::Update(PlayerInfo& info)
{
}

void LobbyScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			m_Framework->m_Communicator.SendReady();
			// m_Framework->m_SceneManager.ChangeScene(SceneID::GAME);
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

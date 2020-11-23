#include "pch.h"
#include "GameFramework.h"

GameFramework::GameFramework()
{
}

GameFramework::~GameFramework()
{
}

void GameFramework::Initialize(HINSTANCE instance, HWND wnd)
{
	m_Inst = instance;
	m_Wnd = wnd;

	m_Renderer.Initailize(instance, wnd);
	m_SceneManager.Initialize(this);
	// CreateThread(NULL, 0, NetworkCommunicator::ServerMain, (LPVOID)&m_Communicator, 0, NULL);

	// m_Communicator.PushMessage(NETWORK_MASSAGE::CONNECT);
}

void GameFramework::Release()
{
}

void GameFramework::Update()
{
	m_SceneManager.UpdateCurrentScene();
}

void GameFramework::Render()
{
	m_Renderer.Render(m_Objects);
}

void GameFramework::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	// for test
	switch (message) {
	case WM_KEYUP:
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			m_SceneManager.ChangeScene(SceneID::GAME);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}

	// 여기서 바로 커뮤니케이터에게 메세지 넣어도 되지 않을까?
	m_SceneManager.HandleInput();
}

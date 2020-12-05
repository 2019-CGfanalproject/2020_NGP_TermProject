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
	m_Communicator.Initialize(this);
}

void GameFramework::Release()
{
}

void GameFramework::Update()
{
	m_Objects.Update();
	m_SceneManager.UpdateCurrentScene();
}

void GameFramework::Render()
{
	Update();
	m_Renderer.Render(m_Objects);
}

void GameFramework::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	// for develop mode
	switch (message) {
	case WM_KEYUP:
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F1:
			m_SceneManager.ChangeScene(SceneID::LOGIN);
			break;
		case VK_F2:
			m_SceneManager.ChangeScene(SceneID::LOBBY);
			break;
		case VK_F3:
			m_SceneManager.ChangeScene(SceneID::GAME);
			break;
		case VK_F4:
			m_SceneManager.ChangeScene(SceneID::RESULT);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}

	m_SceneManager.HandleInput(message, wParam, lParam);
}

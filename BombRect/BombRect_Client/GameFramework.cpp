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
	CreateThread(NULL, 0, NetworkCommunicator::ServerMain, (LPVOID)&m_Communicator, 0, NULL);
	m_Communicator.SetFramework(this);

	//std::string loopback{ "127.0.0.1" };
	//std::string shphone{ "192.168.43.216" };

	//m_Communicator.Connect(shphone.c_str(), TEXT("Çý¸®¹«"));
	//m_Communicator.PushMessage(CommunicateMessage::CONNECT);
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
	// m_Renderer.TestRender();
	Update();
	m_Renderer.Render(m_Objects);
}

void GameFramework::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	// for test
	switch (message) {
	case WM_KEYUP:
		switch (wParam) {
		case VK_RETURN:
			m_Communicator.SendReady();
			// m_SceneManager.ChangeScene(SceneID::GAME);
			break;
		}
		break;
	case WM_KEYDOWN:
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}

	m_SceneManager.HandleInput(message, wParam, lParam);
}

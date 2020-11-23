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

	m_Renderer.RenderScene(m_SceneManager.GetCurrScene());
}

void GameFramework::HandleInput()
{
	m_SceneManager.HandleInput();
}

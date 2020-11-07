#include "pch.h"
#include "GameFramework.h"
#include "SceneManager.h"

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
	m_SceneManager.RenderCurrentScene();
}

void GameFramework::HandleInput()
{
	m_SceneManager.HandleInput();
}

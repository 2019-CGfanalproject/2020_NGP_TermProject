#pragma once
#include "SceneManager.h"
#include "Renderer.h"
#include "NetworkCommunicator.h"

class GameFramework
{
	HINSTANCE m_Inst;
	HWND m_Wnd;

public:
	Renderer			m_Renderer;
	SceneManager		m_SceneManager;
	NetworkCommunicator m_Communicator;

public:
	GameFramework();
	~GameFramework();

	void Initialize(HINSTANCE, HWND);
	void Release();

	void Update();
	void Render();
	void HandleInput();
};

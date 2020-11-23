#pragma once
#include "SceneManager.h"
#include "Renderer.h"
#include "NetworkCommunicator.h"
#include "ObjectContainer.h"

class GameFramework
{
	HINSTANCE m_Inst;
public:
	HWND m_Wnd;

	Renderer			m_Renderer;
	SceneManager		m_SceneManager;
	ObjectContainer		m_Objects;
	NetworkCommunicator m_Communicator;

public:
	GameFramework();
	~GameFramework();

	void Initialize(HINSTANCE, HWND);
	void Release();

	void Update();
	void Render();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
};

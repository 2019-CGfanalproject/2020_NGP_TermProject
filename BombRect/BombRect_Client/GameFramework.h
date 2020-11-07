#pragma once
#include "SceneManager.h"

class GameFramework
{
	HINSTANCE m_Inst;
	HWND m_Wnd;

	SceneManager m_SceneManager;

	bool instantiated = false;

public:
	GameFramework();
	~GameFramework();

	void Initialize(HINSTANCE, HWND);
	void Release();

	void Update();
	void Render();
	void HandleInput();
};

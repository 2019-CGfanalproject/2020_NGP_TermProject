#pragma once
#include "SceneManager.h"
#include "Renderer.h"
class GameFramework
{
	HINSTANCE m_Inst;
	HWND m_Wnd;

	Renderer m_Renderer;
	SceneManager m_SceneManager;

public:
	GameFramework();
	~GameFramework();

	void Initialize(HINSTANCE, HWND);
	void Release();

	void Update();
	void Render();
	void HandleInput();
};

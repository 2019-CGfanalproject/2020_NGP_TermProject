#pragma once
#include "Scene.h"

class SceneManager
{
	std::array<Scene*, (int)SceneID::COUNT> m_Scenes;
	Scene* m_CurScene = nullptr;

public:
	SceneManager();
	~SceneManager();

	void UpdateCurrentScene();
	void RenderCurrentScene();
	void HandleInput();

	// Scene�� ��ȯ�� ���� �޼ҵ� ����
};

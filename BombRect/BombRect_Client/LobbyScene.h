#pragma once
#include "Scene.h"

class TextObject;
class LobbyScene : public Scene
{
	// ready ��ư ������Ʈ
	TextObject* m_ChattingText;
	bool m_IsPreStrComp = false;

public:
	LobbyScene(GameFramework* framework)
		:Scene(framework, SceneID::LOBBY) { }
	virtual ~LobbyScene() { }

	void Initialize();
	void Destroy() override;

	void Update();
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) override;
};


#pragma once
#include "Scene.h"

// 현재 씬에서의 객체 생성과 소멸 관리를 함
// 게임 겍체를 저장한다.
class GameScene : public Scene
{
public:
	GameScene();
	virtual ~GameScene();

public:
	void Initialize() override;
	void Update() override;
	void HandleInput() override;
};


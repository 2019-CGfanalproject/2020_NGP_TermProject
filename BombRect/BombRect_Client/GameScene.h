#pragma once
#include "Scene.h"

// ���� �������� ��ü ������ �Ҹ� ������ ��
// ���� ��ü�� �����Ѵ�.
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


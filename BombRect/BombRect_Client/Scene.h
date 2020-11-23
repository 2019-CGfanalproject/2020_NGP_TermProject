#pragma once

// ���� �������� ��ü ������ �Ҹ� ������ ��
class GameFramework;
class Scene abstract
{
protected:
	// Scene Interface class
	SceneID m_ID;
	GameFramework* m_Framework;

public:
	Scene(GameFramework* framework, SceneID id) 
		: m_Framework(framework), m_ID(id) { }
	virtual ~Scene() { }

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void HandleInput() = 0;
};

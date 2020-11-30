#pragma once

class GameFramework;
class InputHandler;

// 按眉 积己苞 家戈 包府
class Scene abstract
{
protected:
	// Scene Interface class
	GameFramework* m_Framework;
	const SceneID m_ID;

public:
	Scene(GameFramework* framework, SceneID id) 
		: m_Framework(framework), m_ID(id) { }
	virtual ~Scene() { }

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;
	virtual void HandleInput(InputHandler* inputHandler) { }
	virtual void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) { }

public:
	virtual const SceneID GetID() const { 
		return m_ID;
	}
};

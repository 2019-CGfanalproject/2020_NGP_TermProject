#pragma once
#include "GameObject.h"

class Scene abstract
{
protected:
	// Scene Interface class
	SceneID m_ID;
	std::list<GameObject> m_Objects;

public:
	Scene() { };
	virtual ~Scene() { };

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void HandleInput() = 0;
	std::list<GameObject>* GetObjectList() {
		return &m_Objects;
	}
};

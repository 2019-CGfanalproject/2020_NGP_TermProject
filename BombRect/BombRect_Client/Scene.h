#pragma once
class Scene abstract
{
	// Scene Interface class
	SceneID m_ID;

public:
	Scene();
	virtual ~Scene();

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput() = 0;
};


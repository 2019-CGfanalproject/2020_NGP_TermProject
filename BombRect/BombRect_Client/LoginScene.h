#pragma once
#include "Scene.h"

class TextObject;
class LoginScene : public Scene
{
	String m_Text;
	TextObject* m_IPAddrText;

public:
	LoginScene(GameFramework* framework);
	virtual ~LoginScene();

public:
	void Initialize() override;
	void Destroy() override;

	void Update() override;
	void HandleInput(InputHandler* inputHandler) { };
	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	void TryConnecting();
};


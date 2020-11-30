#pragma once

constexpr int INPUT_COUNT = 5;
enum class KeyInput {
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT,
	SPACEBAR,
};

class SceneManager;
class InputHandler
{
private:
	SceneManager* m_SceneManager;
	WPARAM m_LastInput;
	bool m_InputControls[INPUT_COUNT];

public:
	InputHandler(SceneManager* sceneManager)
		: m_SceneManager(sceneManager)
	{
		Reset();
	}

	void HandleInput(UINT message, WPARAM wParam, LPARAM lParam);

	void Reset() {
		for (int i = 0; i < INPUT_COUNT; ++i)
			m_InputControls[i] = false;
	}

	PlayerState CalcPlayerState();

	bool CheckSpace() {
		if (m_InputControls[(int)KeyInput::SPACEBAR]) {
			m_InputControls[(int)KeyInput::SPACEBAR] = false;
			return true;
		}
		return false;
	}
};

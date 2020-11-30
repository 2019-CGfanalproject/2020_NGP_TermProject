#include "pch.h"
#include "InputHandler.h"
#include "SceneManager.h"

void InputHandler::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYUP: {
		switch (wParam) {
		case VK_LEFT:
			m_InputControls[(int)KeyInput::ARROW_LEFT] = false;
			break;
		case VK_UP:
			m_InputControls[(int)KeyInput::ARROW_UP] = false;
			break;
		case VK_RIGHT:
			m_InputControls[(int)KeyInput::ARROW_RIGHT] = false;
			break;
		case VK_DOWN:
			m_InputControls[(int)KeyInput::ARROW_DOWN] = false;
			break;
		case VK_SPACE:
			m_InputControls[(int)KeyInput::SPACEBAR] = false;
			break;
		}
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			m_InputControls[(int)KeyInput::SPACEBAR] = true;
			m_LastInput = wParam;
			// m_Framework->m_Communicator.SendBomb();
			break;
		case VK_LEFT:
			m_InputControls[(int)KeyInput::ARROW_LEFT] = true;
			m_LastInput = wParam;
			break;
		case VK_UP:
			m_InputControls[(int)KeyInput::ARROW_UP] = true;
			m_LastInput = wParam;
			break;
		case VK_RIGHT:
			m_InputControls[(int)KeyInput::ARROW_RIGHT] = true;
			m_LastInput = wParam;
			break;
		case VK_DOWN:
			m_InputControls[(int)KeyInput::ARROW_DOWN] = true;
			m_LastInput = wParam;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}
}

PlayerState InputHandler::CalcPlayerState()
{
	// 키가 동시에 눌려 있다면
	// 나중에 눌린 키에 따라서 플레이어의 상태를 바꿔준다.

	bool is_idle = true;
	for (int i = 0; i < 4; ++i) {
		if (m_InputControls[i]) is_idle = false;
	}
	if (is_idle) return PlayerState::IDLE;

	switch (m_LastInput) {
	case VK_LEFT:
		return PlayerState::LEFT;
	case VK_UP:
		return PlayerState::UP;
	case VK_RIGHT:
		return PlayerState::RIGHT;
	case VK_DOWN:
		return PlayerState::DOWN;
	}
}

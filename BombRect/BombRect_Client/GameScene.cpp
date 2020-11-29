#include "pch.h"
#include "GameScene.h"
#include "GameFramework.h"
#include "GameObject.h"

const TilePos CLOSED_TILE_POS[] = {
	{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0},
	{0, 1},															{8, 1},
	{0, 2},		    {2, 2},			{4, 2},			{6, 2},			{8, 2},
	{0, 3},															{8, 3},
	{0, 4},		    {2, 4},			{4, 4},			{6, 4},			{8, 4},
	{0, 5},															{8, 5},
	{0, 6},		    {2, 6},			{4, 6},			{6, 6},			{8, 6},
	{0, 7},															{8, 7},
	{0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8}, {6, 8}, {7, 8}, {8, 8},
};

GameScene::GameScene(GameFramework* framework)
	: Scene(framework, SceneID::GAME)
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	// 맵 만들고
	// closed tile 만들고
	Vector2 padding{ 40, 40 };

	m_Framework->m_Objects.AddStaticObject(BitmapKey::BACKGOURND, padding);

	for (auto& pos : CLOSED_TILE_POS) {
		m_Framework->
			m_Objects.AddStaticObject(
			BitmapKey::CLOSED_TILE, 
			padding + Vector2(pos.r * 80, pos.c * 80)
			);
	}

	// m_Framework->m_Objects.AddCharater()

	// 플레이어 세팅
	auto player = m_Framework->m_Objects.AddDynamicObject(BitmapKey::CHARACTER_RED, Vector2(0, 0));
	m_Player = new Player(player);
}

void GameScene::Destroy()
{
}

//PlayerInfo g_lobby_state[4] = {
//	{0, nullptr},{1, nullptr},
//	{2, nullptr},{3, nullptr},
//}

void GameScene::Update()
{
}

void GameScene::Update(PlayerInfo& info)
{
	m_Player->SetPos(info.pos);
}

// 함수에 인자로 넘겨주는 건 좀...
void GameScene::Update(PlayerInfo players[], SendBombInfo bombs[])
{
	m_Player->SetPos(players[0].pos);
	for (int i = 0; i < 12; ++i) {
		if (-1 == bombs[i].bomb_count_down) break;

		Vector2 pos{ (int)bombs[i].pos.r * 80, (int)bombs[i].pos.c * 80 };
		m_Framework->m_Objects.AddDynamicObject(BitmapKey::BOMB, pos);
	}
}

void GameScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{
	// 인풋으로 플레이어의 state를 결정하고 네트워크 메시지를 보냄
	switch (message) {
	case WM_KEYUP: {
		switch (wParam) {
		case VK_LEFT:
			m_Player->m_InputControl[(int)Input::ARROW_LEFT] = false;
			break;
		case VK_UP:
			m_Player->m_InputControl[(int)Input::ARROW_UP] = false;
			break;
		case VK_RIGHT:
			m_Player->m_InputControl[(int)Input::ARROW_RIGHT] = false;
			break;
		case VK_DOWN:
			m_Player->m_InputControl[(int)Input::ARROW_DOWN] = false;
			break;
		}
		break;
	}
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			m_Framework->m_Communicator.SendBomb();
			break;
		case VK_LEFT:
			m_Player->m_InputControl[(int)Input::ARROW_LEFT] = true;
			break;
		case VK_UP:
			m_Player->m_InputControl[(int)Input::ARROW_UP] = true;
			break;
		case VK_RIGHT:
			m_Player->m_InputControl[(int)Input::ARROW_RIGHT] = true;
			break;
		case VK_DOWN:
			m_Player->m_InputControl[(int)Input::ARROW_DOWN] = true;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		break;
	}

	PlayerState cur_state = m_Player->CalcState(wParam);
	if (m_Player->state != cur_state) {
		m_Player->state = cur_state;

		switch (cur_state) {
		case PlayerState::UP:
			m_Framework->m_Communicator.SendPlayerState(PlayerState::UP);
			break;
		case PlayerState::DOWN:
			m_Framework->m_Communicator.SendPlayerState(PlayerState::DOWN);
			break;
		case PlayerState::LEFT:
			m_Framework->m_Communicator.SendPlayerState(PlayerState::LEFT);
			break;
		case PlayerState::RIGHT:
			m_Framework->m_Communicator.SendPlayerState(PlayerState::RIGHT);
			break;
		case PlayerState::IDLE:
			m_Framework->m_Communicator.SendPlayerState(PlayerState::IDLE);
		}
	}
}

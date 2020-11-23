#include "pch.h"
#include "GameScene.h"
#include "GameFramework.h"

constexpr TilePos CLOSED_TILE_POS[] = {
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
}

//PlayerInfo g_lobby_state[4] = {
//	{0, nullptr},{1, nullptr},
//	{2, nullptr},{3, nullptr},
//}

void GameScene::Update()
{
}

void GameScene::HandleInput()
{
}

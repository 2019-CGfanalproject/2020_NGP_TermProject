#include "pch.h"
#include "LoginScene.h"
#include "GameFramework.h"

#define ID_EDITCHILD 50

LoginScene::LoginScene(GameFramework* framework)
	: Scene(framework, SceneID::LOGIN)
{
}

LoginScene::~LoginScene()
{
}

void LoginScene::Initialize()
{
    m_Framework->m_Objects.AddStaticObject(BitmapKey::TITLE,
        Vector2(0, 0));

    //HWND hwndEdit = CreateWindowEx(
    //    0, L"EDIT",   // predefined class 
    //    NULL,         // no window title 
    //    WS_CHILD | WS_VISIBLE | WS_VSCROLL |
    //    ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
    //    0, 0, 200, 100,   // set size in WM_SIZE message 
    //    m_Framework->m_Wnd,         // parent window 
    //    (HMENU)ID_EDITCHILD,   // edit control ID 
    //    (HINSTANCE)GetWindowLongPtr(m_Framework->m_Wnd, GWLP_HINSTANCE),
    //    NULL);        // pointer not needed 

    //// Add text to the window. 
    //SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)L"test");
}

void LoginScene::Update()
{
}

void LoginScene::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
{

}

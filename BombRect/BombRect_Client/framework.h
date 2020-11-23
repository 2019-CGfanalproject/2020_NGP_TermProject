#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <CommCtrl.h>		// Windows Control을 사용하기 위한 헤더
#include <WinSock2.h>
#pragma comment(lib, "ws2_32")

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <array>
#include <list>
#include <queue>
#include <map>

#include <string>
#ifdef UNICODE
typedef std::wstring String;
#else
typedef std::string String;
#endif

// DirectX
#include <d2d1.h>
#pragma comment(lib, "D2D1")
#include <wincodec.h>

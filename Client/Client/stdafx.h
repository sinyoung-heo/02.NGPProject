// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// Windows Socket 헤더 파일
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

// C 런타임 헤더 파일입니다.
#include <iostream>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <ctime>
#include <io.h>

using namespace std;

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

/* FMOD */
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

#include "Define.h"
#include "Enum.h"
#include "Exertn.h"
#include "Function.h"
#include "Struct.h"
#include "Typedef.h"

#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "MathMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "BmpMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "MouseMgr.h"
#include "TileMgr.h"
#include "SoundMgr.h"

/*알파 블랜딩 사용하기 위해 추가한 변수.*/
#pragma comment (lib, "msimg32.lib")


// 메모리 누수체크
//#include <vld.h>


//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif
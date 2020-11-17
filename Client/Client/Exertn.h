#pragma once

class CObj;
extern HWND g_hWnd;
extern bool g_bIsActive;

/* 서버 관련 변수 */
extern SOCKET g_hSocket;
extern int g_iServerID;
extern char g_szServerName[64];
extern unordered_map<int, CObj*> g_umap_serverObj;

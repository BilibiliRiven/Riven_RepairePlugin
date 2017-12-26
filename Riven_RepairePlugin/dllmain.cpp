// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "Plugin.h"
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "OLLYDBG.LIB")
//#pragma comment(lib, "OLLYDBG.EXE")

HANDLE g_hModule;
int Debug();
DWORD __stdcall MyGetClassLong(HWND hWnd, int iIndex);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extc int  _export cdecl ODBG_Plugindata(char shortname[32]){
	strcpy_s(shortname, 32,"Riven's Plugin");
	return PLUGIN_VERSION;
}

HWND g_hWndMain;

extc int  _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw,
	ulong *features){
	if (ollydbgversion < PLUGIN_VERSION){
		return -1;
	}

	g_hWndMain = hw;


	Addtolist(0, 0, "Riven's(李家诚) plugin used to repaire OD meessage break point bug.");
	Addtolist(0, -1, "Copyright (C) 2017 Riven");

	
	return Debug();
}

int Debug(){
	DWORD OldProtect = 0;
	LPVOID  pGetClassLong = (LPVOID)0x50d858;
	BOOL bRet = VirtualProtect(pGetClassLong, 4, PAGE_EXECUTE_READWRITE, &OldProtect);
	if (!bRet){
		return -1;
	}

	*(DWORD *)pGetClassLong = (DWORD)MyGetClassLong;

	bRet = VirtualProtect((LPVOID)0x50d858, 4, OldProtect, &OldProtect);
	if (!bRet){
		return -1;
	}

	return 0;
}


DWORD __stdcall MyGetClassLong(HWND hWnd, int iIndex){
	if (IsWindowUnicode(hWnd)){
		return GetClassLongPtrW(hWnd, iIndex);
	}
	return GetClassLongPtrA(hWnd, iIndex);
}



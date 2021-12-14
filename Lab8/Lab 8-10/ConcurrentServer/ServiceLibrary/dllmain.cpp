﻿// dllmain.cpp: определяет точку входа для приложения DLL.
#include "pch.h"
#include "Windows.h"
#include "DefineTableService.h" // макро для TableService
#include "EchoServer.h"
#include "RandServer.h"
#include "TimeServer.h"

BEGIN_TABLESERVICE              // таблица 
ENTRYSERVICE("Echo", EchoServer),
ENTRYSERVICE("Time", TimeServer),
ENTRYSERVICE("Rand", RandServer)
END_TABLESERVICE;

extern "C" __declspec(dllexport) HANDLE  SSS(char* id, LPVOID prm)
{
    HANDLE rc = NULL;
    int  i = 0;
    while (i < SIZETS && strcmp(TABLESERVICE_ID(i), id) != 0)i++;
    if (i < SIZETS)
        rc = CreateThread(NULL, NULL, TABLESERVICE_FN(i), prm, NULL, NULL);
    return rc;
};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


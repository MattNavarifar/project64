/****************************************************************************
*                                                                           *
* Project64 - A Nintendo 64 emulator.                                      *
* http://www.pj64-emu.com/                                                  *
* Copyright (C) 2012 Project64. All rights reserved.                        *
*                                                                           *
* License:                                                                  *
* GNU/GPLv2 http://www.gnu.org/licenses/gpl-2.0.html                        *
*                                                                           *
****************************************************************************/
#include "StdAfx.h"
#include <Project64-core/N64System/SystemGlobals.h>
#include <Project64-core/N64System/N64RomClass.h>
#include <Project64-core/N64System/Mips/MemoryVirtualMem.h>
#include <Project64-core/N64System/N64Class.h>
#include <iostream>
#include "NetplayPlugin.h"

CNetplayPlugin::CNetplayPlugin(void)
{

}

CNetplayPlugin::~CNetplayPlugin()
{
	UnloadPlugin();
}

void CNetplayPlugin::StartServer()
{
	bool(CALL * StartServer)(void);
	_LoadFunction("StartServer", StartServer);
	if (StartServer == NULL)
	{
		WriteTrace(TraceNetplayPlugin, TraceDebug, "Could not find the StartServer function within the netplay DLL");
		return;
	}
	StartServer();
}

void CNetplayPlugin::StartClient()
{
	bool(CALL * StartClient)(void);
	_LoadFunction("StartClient", StartClient);
	if (StartClient == NULL)
	{
		WriteTrace(TraceNetplayPlugin, TraceDebug, "Could not find the StartClient function within the netplay DLL");
		return;
	}
	StartClient();
}

bool CNetplayPlugin::LoadFunctions(void)
{
	return true;
}

bool CNetplayPlugin::Initiate(CPlugins* Plugins, CN64System* System)
{
	typedef struct
	{
		uint8_t* RDRAM;
	} NETPLAY_INFO;

	CMipsMemoryVM& MMU = System->m_MMU_VM;
	int32_t(CALL* Initialize)(NETPLAY_INFO Netplay_Info);
	_LoadFunction("Initialize", Initialize);

	if (Initialize == NULL)
	{
		WriteTrace(TraceNetplayPlugin, TraceDebug, "Could not find the Initialize function within the netplay DLL");
		return false;
	}

	NETPLAY_INFO Info = { MMU.Rdram() };

	Initialize(Info);

	return true;
}


void CNetplayPlugin::UnloadPluginDetails(void)
{

	bool(CALL * StopServer)(void);
	_LoadFunction("StopServer", StopServer);
	if (StopServer == NULL)
	{
		WriteTrace(TraceNetplayPlugin, TraceDebug, "Could not find the StopServer function within the netplay DLL");
		return;
	}
	StopServer();

	bool(CALL * StopClient)(void);
	_LoadFunction("StopClient", StopClient);
	if (StopClient == NULL)
	{
		WriteTrace(TraceNetplayPlugin, TraceDebug, "Could not find the StopClient function within the netplay DLL");
		return;
	}
	StopClient();
	
}
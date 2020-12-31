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
#include <iostream>
#include "NetplayPlugin.h"

CNetplayPlugin::CNetplayPlugin(void)
{
	std::cout << "Hello World!" << std::endl;
}

CNetplayPlugin::~CNetplayPlugin()
{
	UnloadPlugin();
}

bool CNetplayPlugin::LoadFunctions(void)
{
	return true;
}

bool CNetplayPlugin::Initiate(CPlugins* Plugins, CN64System* System)
{
	bool(CALL * StartServer)(void);
	_LoadFunction("StartServer", StartServer);
	if (StartServer == NULL)
	{
		WriteTrace(TraceNetplayPlugin, TraceDebug, "Could not find the StartServer function within the netplay DLL");
		return false;
	}
	StartServer();
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
}
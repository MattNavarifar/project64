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

void CNetplayPlugin::UnloadPluginDetails(void)
{
}

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
	return true;
}

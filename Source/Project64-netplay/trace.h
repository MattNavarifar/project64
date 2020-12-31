/****************************************************************************
*                                                                           *
* Project64-Netplay					                                        *
* http://www.pj64-emu.com/                                                  *
* Copyright (C) 2021 Project64. All rights reserved.                        *
*                                                                           *
* License:                                                                  *
* GNU/GPLv2 http://www.gnu.org/licenses/gpl-2.0.html                        *
*																			*
* @author - Matt Navarifar													*
*                                                                           *
****************************************************************************/
#pragma once
#include <Common/TraceModulesCommon.h>
#include <Common/Trace.h>

enum TraceModuleNetPlay
{
    TraceNetPlayServer = 0,
    TraceDLL,
    MaxTraceModuleNetPlay,
};

void SetupTrace(void);
void StartTrace(void);
void StopTrace(void);

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
#include "trace.h"

#include <string.h>
#include <Common/Trace.h>
#include <Common/path.h>
#include <Common/LogClass.h>
#include "settings.h"

#include "Config.h"

static CTraceFileLog* g_LogFile = NULL;

void SetupTrace(void)
{
    if (g_LogFile != NULL)
    {
        return;
    }

#ifdef _DEBUG
    TraceSetMaxModule(MaxTraceModuleNetPlay, TraceDebug);
#else
    TraceSetMaxModule(MaxTraceModuleNetPlay, TraceInfo);
#endif

    TraceSetModuleName(TraceNetPlayServer, "NetPlayServer");
    TraceSetModuleName(TraceDLL, "NetPlayDLL");
}

void StartTrace(void)
{
    const char* log_dir = g_settings ? g_settings->log_dir() : NULL;
    if (log_dir == NULL || log_dir[0] == '\0')
    {
        return;
    }

    CPath LogFilePath(log_dir, "Project64-netplay.log");
    if (!LogFilePath.DirectoryExists())
    {
        LogFilePath.DirectoryCreate();
    }
    g_LogFile = new CTraceFileLog(LogFilePath, g_settings->FlushLogs(), CLog::Log_New, 500);
    TraceAddModule(g_LogFile);
}

void StopTrace(void)
{
    if (g_LogFile)
    {
        TraceRemoveModule(g_LogFile);
        delete g_LogFile;
        g_LogFile = NULL;
    }
}

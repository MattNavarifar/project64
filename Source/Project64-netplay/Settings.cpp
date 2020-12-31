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

#include <Settings/Settings.h>
#include "trace.h"
#include "Settings.h"
#include "SettingsID.h"

#include <Common/StdString.h>


CSettings::CSettings() :
	m_Set_log_dir(0),
	m_Set_log_flush(0),
	m_FlushLogs(false)
{
	memset(m_log_dir, 0, sizeof(m_log_dir));
	m_Set_log_flush = FindSystemSettingId("Log Auto Flush");

	m_Set_log_dir = FindSystemSettingId("Dir:Log");

	if (m_Set_log_dir != 0)
	{
		GetSystemSettingSz(m_Set_log_dir, m_log_dir, sizeof(m_log_dir));
	}

	m_FlushLogs = m_Set_log_flush != 0 ? GetSystemSetting(m_Set_log_flush) != 0 : false;
}

CSettings::~CSettings()
{

}

void CSettings::LogLevelChanged(void)
{
    g_ModuleLogLevel[TraceMD5] = GetSetting(Set_Logging_MD5);
}

#ifdef _WIN32
#include <Windows.h>
#endif
void UseUnregisteredSetting(int /*SettingID*/)
{
#ifdef _WIN32
	DebugBreak();
#endif
}
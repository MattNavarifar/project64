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

class CSettings
{
public:
    CSettings();
    ~CSettings();

    inline const char* log_dir(void) const { return m_log_dir; }
    inline bool FlushLogs(void) const { return m_FlushLogs; }

private:
    void LogLevelChanged(void);

private:
    bool m_FlushLogs;
    short m_Set_log_dir;
    short m_Set_log_flush;
    char m_log_dir[260];
    
};

extern CSettings* g_settings;

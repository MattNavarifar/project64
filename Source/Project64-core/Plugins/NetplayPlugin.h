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
#pragma once
#include <Project64-core/Plugins/PluginBase.h>

class CNetplayPlugin : public CPlugin
{

private:
	CNetplayPlugin(const CNetplayPlugin&);				// Disable copy constructor
	CNetplayPlugin& operator=(const CNetplayPlugin&);	// Disable assignment

	virtual int32_t GetDefaultSettingStartRange() const { return FirstNetplayDefaultSet; }
	virtual int32_t GetSettingStartRange() const { return FirstNetplaySettings; }
	PLUGIN_TYPE type() { return PLUGIN_TYPE_NETPLAY; }
	virtual void UnloadPluginDetails(void);

public:
	CNetplayPlugin(void);
	~CNetplayPlugin();

	void StartServer();
	void StartClient();
	bool LoadFunctions(void);
	bool Initiate(CPlugins* Plugins, CN64System* System);

private:
	uint8_t* m_Ram;

};
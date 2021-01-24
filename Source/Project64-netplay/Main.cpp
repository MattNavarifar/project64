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
#include "Netplay_1.0.h"
#include "Version.h"
#include "NetServer.h"
#include "NetClient.h"
#include <Settings/Settings.h>

#include <Common/CriticalSection.h>
//#include <Common/DateTimeClass.h>
//#include <Common/path.h>
//#include <Common/SmartPointer.h>

#include "Config.h"

#ifdef _WIN32
#include <Windows.h>
#include <commctrl.h>
#endif

#ifdef _WIN32
HINSTANCE hinstDLL = NULL;
#endif

#include "trace.h"

NNetServer* NetServer;
NNetClient* NetClient;

CSettings* g_settings = NULL;

NETPLAY_INFO net;

/******************************************************************
Function: GetDllInfo
Purpose:  This function allows the emulator to gather information
about the dll by filling in the PluginInfo structure.
input:    a pointer to a PLUGIN_INFO stucture that needs to be
filled by the function. (see def above)
output:   none
*******************************************************************/
void CALL GetDllInfo(PLUGIN_INFO* PluginInfo)
{
    PluginInfo->Version = 0x0100;     // Set to 0x0101
    PluginInfo->Type = PLUGIN_TYPE_NETPLAY;  // Set to PLUGIN_TYPE_GFX
#ifdef _DEBUG
    sprintf(PluginInfo->Name, "Project64 NetPlay Plugin (Debug): %s", VER_FILE_VERSION_STR);
#else
    sprintf(PluginInfo->Name, "Project64 NetPlay Plugin: %s", VER_FILE_VERSION_STR);
#endif

    // If DLL supports memory these memory options then set them to TRUE or FALSE
    //  if it does not support it
    PluginInfo->NormalMemory = FALSE;  // a normal uint8_t array
    PluginInfo->MemoryBswaped = TRUE; // a normal uint8_t array where the memory has been pre
    // bswap on a dword (32 bits) boundry
}

/******************************************************************
Function: CloseDLL
Purpose:  This function is called when the emulator is closing
down allowing the dll to de-initialise.
input:    none
output:   none
*******************************************************************/
void CALL CloseDLL(void)
{
    WriteTrace(TraceDLL, TraceInfo, "Closing DLL");


    if (g_settings)
    {
        delete g_settings;
        g_settings = NULL;
    }
    StopTrace();
}

void CALL PluginLoaded(void)
{
    SetupTrace();
    if (g_settings == NULL)
    {
        g_settings = new CSettings;
    }
    StartTrace();

    WriteTrace(TraceDLL, TraceInfo, "Start");
    WriteTrace(TraceDLL, TraceInfo, "Done");
}

//Todo: remove the 64-core dependency (additional include and linked lib) from netplay if we're not pulling in the mips object.
int CALL Initialize(NETPLAY_INFO NetplayInfo)
{
    uint8_t* ram = NetplayInfo.RDRAM;
    uint8_t val = ram[0 + 0x18EE00];

    net = NetplayInfo;

    if (NetServer)
    {
        NetServer->SendMemoryRange(MemRange{ 0x18EE00, 10, ram });
    }
    
    
    WriteTrace(TraceDLL, TraceInfo, "Val is: %d", val);
    return 0;
}

bool CALL StartServer(void)
{
    NetServer = new NNetServer();
    return true;
}

bool CALL StopServer(void)
{
    delete NetServer;
    return true;
}

bool CALL StartClient(void)
{
    NetClient = new NNetClient(net.RDRAM);
    return false;
}

bool CALL StopClient(void)
{
    delete NetClient;
    return false;
}

/******************************************************************
Function: RomClosed
Purpose:  This function is called when a rom is closed.
input:    none
output:   none
*******************************************************************/
void CALL RomClosed(void)
{

#ifdef ANDROID
    vbo_disable();
#endif
    //TODO: Release resources here.
}


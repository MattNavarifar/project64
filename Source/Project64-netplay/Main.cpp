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
#ifdef _WIN32
#include <Windows.h>
#include <commctrl.h>
#endif

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
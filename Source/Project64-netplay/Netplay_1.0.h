#pragma once
/**********************************************************************************
Netplay plugin spec, version #1.0
@Author - Matt Navarifar
**********************************************************************************/

#include <Common/stdtypes.h>
#include <stdio.h>

/* Plugin types */
#define PLUGIN_TYPE_NETPLAY				5

#if defined(_WIN32)
#define EXPORT      extern "C" __declspec(dllexport)
#define CALL        __cdecl
#else
#define EXPORT      extern "C" __attribute__((visibility("default")))
#define CALL
#endif

/***** Structures *****/
typedef struct
{
    uint16_t Version;        /* Should be set to 0x0101 */
    uint16_t Type;           /* Set to PLUGIN_TYPE_AUDIO */
    char Name[100];          /* Name of the DLL */
    int32_t NormalMemory;
    int32_t MemoryBswaped;
} PLUGIN_INFO;


/******************************************************************
Function: RomClosed
Purpose:  This function is called when a rom is closed.
input:    none
output:   none
*******************************************************************/
EXPORT void CALL RomClosed(void);

/******************************************************************
Function: RomOpen
Purpose:  This function is called when a rom is open. (from the
emulation thread)
input:    none
output:   none
*******************************************************************/
EXPORT void CALL RomOpen(void);

/******************************************************************
    Function: GetDllInfo
    Purpose:  This function allows the emulator to gather information
    about the dll by filling in the PluginInfo structure.
    input:    a pointer to a PLUGIN_INFO stucture that needs to be
    filled by the function. (see def above)
    output:   none
    *******************************************************************/
EXPORT void CALL GetDllInfo(PLUGIN_INFO* PluginInfo);
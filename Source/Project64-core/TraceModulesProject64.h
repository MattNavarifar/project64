#pragma once
#include <Common/TraceModulesCommon.h>

enum TraceModuleProject64
{
    TraceSettings = MaxTraceModuleCommon,
    TraceUnknown,
    TraceAppInit,
    TraceAppCleanup,
    TraceN64System,
    TracePlugins,
    TraceGFXPlugin,
    TraceAudioPlugin,
    TraceControllerPlugin,
    TraceNetplayPlugin,
    TraceRSPPlugin,
    TraceRSP,
    TraceAudio,
    TraceRegisterCache,
    TraceRecompiler,
    TraceTLB,
    TraceProtectedMem,
    TraceUserInterface,
    TraceRomList,
    TraceExceptionHandler,
    MaxTraceModuleProject64,
};
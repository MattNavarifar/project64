
#include "Netplay_1.0.h"
#include "Config.h"
#include "trace.h"
#include <Common/StdString.h>
#include <Settings/Settings.h>
#include "SettingsID.h"

#ifdef _WIN32
#include <Common/CriticalSection.h>
#include <atlbase.h>
#include <wtl/atlapp.h>

#include <atlwin.h>
#include <wtl/atldlgs.h>
#include <wtl/atlctrls.h>
#include <wtl/atlcrack.h>

extern HINSTANCE hinstDLL;

extern CriticalSection* g_ProcessDListCS;

class CProject64NetplayWtlModule :
    public CAppModule
{
public:
    CProject64NetplayWtlModule(HINSTANCE hinst)
    {
        Init(NULL, hinst);
    }
    virtual ~CProject64NetplayWtlModule(void)
    {
        Term();
    }
};

CProject64NetplayWtlModule* WtlModule = NULL;

void ConfigInit(void* hinst)
{
    WtlModule = new CProject64NetplayWtlModule((HINSTANCE)hinst);
}

void ConfigCleanup(void)
{
    if (WtlModule)
    {
        delete WtlModule;
        WtlModule = NULL;
    }
}

#endif
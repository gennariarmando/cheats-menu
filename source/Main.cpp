#include <plugin_vc.h>
#include "CMenuSystem.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        CMenuSystem::InjectPatches();
        return true;
        break;
    case DLL_PROCESS_DETACH:
        return false;
        break;
    };
}
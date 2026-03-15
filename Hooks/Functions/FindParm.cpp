#include "../Hooks.h"
#include "../../Features/Misc/Misc.h"

int __stdcall Hooks::FindParmHook(const char* psz)
{
	if (strcmp(psz, "-insecure") == 0 || strcmp(psz, "-tools") == 0)
	{
		static uint8_t* g_bAllowSecureServers = *reinterpret_cast<uint8_t**>((memory::PatternScan("engine.dll", "80 3D ? ? ? ? ? 75 46")) + 2);

		static bool allow = false;
		if (!allow && Config::Misc::bInsecure)
		{
			*g_bAllowSecureServers = true;
		}
		else if(allow && !Config::Misc::bInsecure)
		{
			*g_bAllowSecureServers = false;
		}

		allow = Config::Misc::bInsecure;
	}

	return Hooks::FindParmOriginal(interfaces::commandline, psz);
}
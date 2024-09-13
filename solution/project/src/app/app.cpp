#include "app.hpp"

constexpr int errc_hooks{ 1 };

int app::load()
{
	if (MH_Initialize() != MH_OK) {
		return errc_hooks;
	}

	for (Hook *const h : getInsts<Hook>())
	{
		if (!h->init()) {
			return errc_hooks;
		}
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
		return errc_hooks;
	}

	return 0;
}

int app::unload()
{
	if (MH_Uninitialize() != MH_OK) {
		return errc_hooks;
	}

	return 0;
}
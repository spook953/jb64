#include "../game/game.hpp"

namespace cfg
{
	bool no_view_punch{ true };
	bool no_pain_sounds{ true };
	bool no_resupply_sound{ true };
	bool no_damage_indicator{ true };
	bool no_damage{ true };
	bool instant_capture{ true };
	float custom_fov{ 120.0f };
}

MAKE_HOOK(
	C_BasePlayer_CalcPlayerView,
	mem_utils::findBytes("client.dll", "48 89 5C 24 ? 56 57 41 54 48 83 EC 50").get(),
	void,
	void *rcx, Vector &eyeOrigin, QAngle &eyeAngles, float &fov)
{
	if (cfg::no_view_punch && rcx)
	{
		Vector &punch{ *reinterpret_cast<Vector *>(reinterpret_cast<uintptr_t>(rcx) + 4796ull) };
		const Vector old_punch{ punch };
		punch = {};
		CALL_ORIGINAL(rcx, eyeOrigin, eyeAngles, fov);
		punch = old_punch;
		return;
	}

	CALL_ORIGINAL(rcx, eyeOrigin, eyeAngles, fov);
}

MAKE_HOOK(
	CTFPlayer_PainSound,
	mem_utils::findBytes("server.dll", "E8 ? ? ? ? 48 8B 07 48 8B CF FF 90 ? ? ? ? 66 0F 6E C0").fixRip().get(),
	void,
	void *rcx, void *info)
{
	if (cfg::no_pain_sounds) {
		return;
	}

	CALL_ORIGINAL(rcx, info);
}

MAKE_HOOK(
	CBaseEntity_EmitSound,
	mem_utils::findBytes("server.dll", "E8 ? ? ? ? 83 FD FE").fixRip().get(),
	void,
	void *filter, int iEntIndex, const char *soundname, void *pOrigin, float soundtime, float *duration)
{
	if (cfg::no_resupply_sound && HASH_RT(soundname) == HASH_CT("Regenerate.Touch")) {
		return;
	}

	CALL_ORIGINAL(filter, iEntIndex, soundname, pOrigin, soundtime, duration);
}

MAKE_HOOK(
	CHudDamageIndicator_ShouldDraw,
	mem_utils::findBytes("client.dll", "40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 84 C0 74 10").get(),
	bool,
	void *rcx)
{
	if (cfg::no_damage_indicator) {
		return false;
	}

	return CALL_ORIGINAL(rcx);
}

MAKE_HOOK(
	CMoveHelperServer_PlayerFallingDamage,
	mem_utils::findBytes("server.dll", "48 8B C4 53 48 81 EC ? ? ? ? 48 8B D9 0F 29 70 E8").get(),
	bool,
	void *rcx)
{
	if (cfg::no_damage) {
		return true;
	}

	return CALL_ORIGINAL(rcx);
}

MAKE_HOOK(
	CTriggerAreaCapture_CaptureThink,
	mem_utils::findBytes("server.dll", "E8 ? ? ? ? 80 BB ? ? ? ? ? 4C 8B 74 24 ?").fixRip().get(),
	void,
	void *rcx)
{
	if (cfg::instant_capture && rcx) {
		*reinterpret_cast<float *>(reinterpret_cast<uintptr_t>(rcx) + 1564) = 0.0f; //CTriggerAreaCapture::m_flCapTime
	}

	CALL_ORIGINAL(rcx);
}

MAKE_HOOK(
	C_BasePlayer_GetFOV,
	mem_utils::findBytes("client.dll", "40 53 48 83 EC ? 48 8B D9 0F 29 74 24 ? 48 8B 0D ? ? ? ? 48 8B 01").get(),
	float,
	void *rcx)
{
	return cfg::custom_fov;
}
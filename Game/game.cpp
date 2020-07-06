#include "Game.h"
#include <cstdint>

namespace RainbowSix
{
	bool esp()
	{
		uintptr_t game_manager = RPM<uintptr_t>(base_address + 0x6C21B48);
		if (!game_manager)
			return NULL;

		uintptr_t entity_list = RPM<uintptr_t>(game_manager + 0x98) + 0xE60F6CF8784B5E96;
		int entity_count = RPM<uint32_t>(game_manager + 0xA0) & 0x3fffffff + 0xE60F6CF8784B5E96;
		if (entity_count == NULL) return false;
		for (int i = 0; i < entity_count; i++)
		{
			uintptr_t entity_object = RPM<uintptr_t>(entity_list + i * 0x8);
			uintptr_t addr2 = RPM<uintptr_t>(entity_object + 0x18);
			if (addr2 == NULL) continue;
			uintptr_t addr3 = RPM<uintptr_t>(addr2 + 0xD8);
			if (addr3 == NULL) continue;
			for (auto current_component = 0x80; current_component < 0xF0; current_component += sizeof(std::uintptr_t))
			{
				uintptr_t addr4 = RPM<uintptr_t>(addr3 + current_component);
				if (addr4 == NULL) continue;
				if (RPM<uintptr_t>(addr4) != (base_address + 0x4C93080))
					continue;
				WPM(addr4 + 0x62a, bCaveiraESPStatus);
			}
		}
	}
}
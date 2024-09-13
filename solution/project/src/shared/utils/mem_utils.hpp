#pragma once

#include "mem_addr.hpp"

#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <string_view>

namespace mem_utils
{
	MemAddr findBytes(std::string_view dll, std::string_view bytes);
}
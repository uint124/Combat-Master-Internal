#pragma once

uint64_t* find_sig(std::string module_name, const std::string& byte_array)
{
	static std::unordered_map<std::string, HMODULE> modules;

	HMODULE module = 0;

	auto it = modules.find(module_name);
	if (it != modules.end()) {
		module = it->second;
		printf("[FindSignature] Reusing module %s\n", module_name.c_str());
	}
	else {
		module = GetModuleHandleA(module_name.c_str());
		modules.insert(std::make_pair(module_name, module));
		printf("[FindSignature] Emplaced module %s for the first time\n", module_name.c_str());
	}

	if (!module)
		return nullptr;

	static const auto pattern_to_byte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + std::strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.push_back(-1);
				}
				else
				{
					bytes.push_back(std::strtoul(current, &current, 16));
				}
			}

			return bytes;
		};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module) + dos_header->e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte(byte_array.c_str());
	const auto scan_bytes = reinterpret_cast<std::uint8_t*>(module);

	const auto pattern_size = pattern_bytes.size();
	const auto pattern_data = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - pattern_size; ++i)
	{
		auto found = true;

		for (auto j = 0ul; j < pattern_size; ++j)
		{
			if (scan_bytes[i + j] == pattern_data[j] || pattern_data[j] == -1)
				continue;
			found = false;
			break;
		}
		if (!found)
			continue;
		return reinterpret_cast<uint64_t*>(&scan_bytes[i]);
	}

	return nullptr;
}
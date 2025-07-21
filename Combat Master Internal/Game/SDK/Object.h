#pragma once

class Object
{
public:
	uint64_t* GetCachedPtr()
	{
		return *(uint64_t**)((DWORD64)this + Offsets::Object::cachedPtr);
	}
};
#pragma once

class PlayerHealth : public Object
{
public:
	float GetHealthPercent()
	{
		// health ranges from 1-0 
		using Fn = float(__fastcall*)(PlayerHealth* playerHealth);
		return reinterpret_cast<Fn>(ProjectModule + 0x3c820c0)(this);
	}
};
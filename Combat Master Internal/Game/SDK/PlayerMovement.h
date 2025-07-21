#pragma once

class PlayerMovement
{
public:
	MovementState GetState()
	{
		return *(MovementState*)((DWORD64)this + 0x5C);
	}
};
#pragma once

class CameraController
{
public:
	Camera* GetCamera()
	{
		return *(Camera**)((DWORD64)this + 0x98);
	}
};
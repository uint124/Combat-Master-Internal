#pragma once

class Camera : public GameObject
{
public:
	ViewMatrix GetViewMatrix()
	{
		uint64_t* cachedPtr = GetCachedPtr();
		if (!cachedPtr) return {};

		return *(ViewMatrix*)((DWORD64)cachedPtr + Offsets::viewMatrix);
	}

	Vector3 GetCameraPosition()
	{
		uint64_t* cachedPtr = GetCachedPtr();
		if (!cachedPtr) return {};

		return *(Vector3*)((DWORD64)cachedPtr + 0x444);
	}

	float GetFov()
	{
		uint64_t* cachedPtr = GetCachedPtr();
		if (!cachedPtr) return 0;

		return *(float*)(cachedPtr + 0x170);
	}

	static void SetFov(__int64 camera, float Fov)
	{
		using Fn = void(__fastcall*)(__int64 camera, float fov);
		auto func = reinterpret_cast<Fn>(EngineModule + 0x313670);
		func(camera, Fov);
	}
};
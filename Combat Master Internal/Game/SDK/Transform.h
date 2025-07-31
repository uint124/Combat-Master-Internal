#pragma once

class TransformData
{
public:
	Vector3 GetRootPosition()
	{
		return *(Vector3*)((DWORD64)this + Offsets::TransformData::rootPosition);
	}
};

class Transform : public Object
{
public:
	TransformData* GetTransformData()
	{
		uint64_t* cachedPtr = GetCachedPtr();
		if (!cachedPtr) return 0;

		return *(TransformData**)((DWORD64)cachedPtr + Offsets::Transform::transformData);
	}

	Vector3 get_PositionInjected()
	{
		uint64_t* cachedPtr = this->GetCachedPtr();
		if (!cachedPtr) return {};

		using Fn = __int64(__fastcall*)(uint64_t* transform, Vector3& outVector);
		auto func = reinterpret_cast<Fn>(EngineModule + 0xB7FC0);
		Vector3 outVec;
		func(cachedPtr, outVec);

		return outVec;
		
	}

	void Internal_LookAtInjected(Vector3 position)
	{
		uint64_t* cachedPtr = this->GetCachedPtr();
		if (!cachedPtr) return;

		//void __fastcall sub_B8570(__int64 a1, float* a2, int* a3)
		using Fn = void(__fastcall*)(uint64_t* transform, Vector3& worldPosition, Vector3& worldUp);
		auto func = reinterpret_cast<Fn>(EngineModule + 0xB8570);

		static Vector3 upVector = Vector3(0, 1, 0);
		func(cachedPtr, position, upVector);
	}
};
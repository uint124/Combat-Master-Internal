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
};
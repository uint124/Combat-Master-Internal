#pragma once

template<typename T>
class List
{
private:
	char pad_0x18[0x10];
	uint64_t* Data;
	int ListSize;

public:
	T* Get(int i)
	{
		return reinterpret_cast<T*>((DWORD64)this->Data[4 + i]); // List starts at 0x20
		//return *(T**)(*(uint64_t*)((DWORD64)this + Offsets::List::listData) + 0x20 + (i * 0x8));
	}

	int Size()
	{
		return *(int*)((DWORD64)this + Offsets::List::listSize);
	}
};
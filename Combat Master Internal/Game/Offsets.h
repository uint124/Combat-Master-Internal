#pragma once

namespace Offsets
{
	constexpr uint64_t playerRoot = 0x50EEE20; // Player root class
	
	constexpr uint64_t il2cppStaticField = 0xB8;

	// Main Camera
	constexpr uint64_t viewMatrix = 0x2FC; // Viewmatrix

	namespace List
	{
		constexpr uint64_t listSize = 0x18;
		constexpr uint64_t listData = 0x10;
	}

	namespace PlayerRoot
	{
		// Static Fields
		constexpr uint64_t localPlayer = 0x8;
		constexpr uint64_t allPlayers = 0x18;

		constexpr uint64_t playerHealth = 0xB8;
		constexpr uint64_t playerAI = 0xC8;

		// Functions
		constexpr uint64_t getHealthPercent = 0x3c820c0; // 12
		constexpr uint64_t getTeamId = 0x3b39460; // 6

	}

	namespace PlayerAI
	{
		constexpr uint64_t aiBrainInfo = 0x30;
	}

	namespace AIBrainInfo
	{
		
	}

	namespace PlayerHealth
	{
		constexpr uint64_t currentHealth = 0xD0;
	}

	namespace Object
	{
		constexpr uint64_t cachedPtr = 0x10;
	}

	namespace TransformData
	{
		// Transform Data
		constexpr uint64_t rootPosition = 0x90;
	}

	namespace Transform
	{
		// Transform
		constexpr uint64_t transformData = 0x28; // class TransformAccess*
	}
}
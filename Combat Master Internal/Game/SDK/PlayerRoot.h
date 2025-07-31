#pragma once

class PlayerRoot : public GameObject
{
private:
	static PlayerRoot* GetInstance()
	{
		auto klass = *(uint64_t**)(ProjectModule + Offsets::playerRoot);
		return *(PlayerRoot**)((DWORD64)klass + Offsets::il2cppStaticField);
	}

public:
	static PlayerRoot* GetLocalPlayer()
	{
		return *(PlayerRoot**)(GetInstance() + Offsets::PlayerRoot::localPlayer);
	}

	static List<PlayerRoot>* GetAllPlayers()
	{
		return *(List<PlayerRoot>**)(GetInstance() + Offsets::PlayerRoot::allPlayers);
	}

	Vector3 GetNeckPosition()
	{
		Transform* neckTransform = *(Transform**)((DWORD64)this + 0x30);
		if (!neckTransform) return {};

		return neckTransform->get_PositionInjected();
	}

	Transform* GetNeckTransform()
	{
		return *(Transform**)((DWORD64)this + 0x30);
	}

	PlayerMovement* GetPlayerMovement()
	{
		return *(PlayerMovement**)((DWORD64)this + 0xB0);
	}

	int GetTeamId()
	{
		using Fn = int(__fastcall*)(PlayerRoot* playerRoot);
		return reinterpret_cast<Fn>(ProjectModule + 0x3B39460)(this);
	}

	bool IsTeammate()
	{
		PlayerRoot* localPlayer = PlayerRoot::GetLocalPlayer();
		if (!localPlayer) return false;

		return this->GetTeamId() == localPlayer->GetTeamId();
	}

	PlayerHealth* GetPlayerHealth()
	{
		return *(PlayerHealth**)((DWORD64)this + 0xB8);
	}


	bool IsDead()
	{
		// True for invalid pointers!?
		PlayerHealth* playerHealth = GetPlayerHealth();
		if (!playerHealth) return true;

		return playerHealth->GetHealthPercent() <= 0.0000f;
	}

	CameraController* GetCameraController()
	{
		return *(CameraController**)((DWORD64)this + 0x48);
	}

	Vector3 GetRootPosition()
	{
		auto transform = this->GetNeckTransform();

		if (!transform) return {};

		auto transformData = transform->GetTransformData();

		if (!transformData) return {};

		return transformData->GetRootPosition();
	}

	Camera* GetCamera()
	{
		CameraController* cameraController = this->GetCameraController();
		if (!cameraController) return nullptr;

		Camera* camera = cameraController->GetCamera();
		if (!camera) return nullptr;

		return camera;

	}

	// I like to live life on the edge
	void BreakAIBrain()
	{
		uint64_t* playerAI = *(uint64_t**)((DWORD64)this + Offsets::PlayerRoot::playerAI);
		if (!playerAI) return;

		uint64_t* aiBrainInfo = *(uint64_t**)((DWORD64)playerAI + Offsets::PlayerAI::aiBrainInfo);
		if (!aiBrainInfo) return;

		for (int i = 5; i < 30; i++)
		{
			*(int*)((DWORD64)aiBrainInfo + (i * 0x4)) = 0;
		}

		for (int i = 10; i < 50; i++)
		{
			*(int*)((DWORD64)playerAI + (i * 0x4)) = 0;
		}
	}

	// Project.dll+7C825E  get entity list
};
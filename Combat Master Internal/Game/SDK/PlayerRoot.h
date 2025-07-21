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

	Transform* GetTransform()
	{
		return *(Transform**)((DWORD64)this + 0x30);
	}

	PlayerMovement* GetPlayerMovement()
	{
		return *(PlayerMovement**)((DWORD64)this + 0xB0);
	}

	CameraController* GetCameraController()
	{
		return *(CameraController**)((DWORD64)this + 0x48);
	}

	Vector3 GetRootPosition()
	{
		auto transform = this->GetTransform();

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
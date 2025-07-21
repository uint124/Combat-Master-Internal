#pragma once

void LogDebugInformation()
{
	List<PlayerRoot>* players = PlayerRoot::GetAllPlayers();
	PlayerRoot* localPlayer = PlayerRoot::GetLocalPlayer();

	printf("Player Root Klass: 0x%llx\n", ProjectModule + Offsets::playerRoot);
	printf("Local Player: 0x%llx\n", localPlayer);
	printf("Global Player List<>: 0x%llx\n", players);
	printf("Number Of Players: %i\n", players->Size());
}
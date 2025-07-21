#pragma once

bool WorldToScreen(const Vector3& position, Vector2* outPos, ViewMatrix viewMatrix)
{
    float m00 = viewMatrix.matrix[0][0], m01 = viewMatrix.matrix[0][1], m02 = viewMatrix.matrix[0][2], m03 = viewMatrix.matrix[0][3];
    float m10 = viewMatrix.matrix[1][0], m11 = viewMatrix.matrix[1][1], m12 = viewMatrix.matrix[1][2], m13 = viewMatrix.matrix[1][3];
    float m20 = viewMatrix.matrix[2][0], m21 = viewMatrix.matrix[2][1], m22 = viewMatrix.matrix[2][2], m23 = viewMatrix.matrix[2][3];
    float m30 = viewMatrix.matrix[3][0], m31 = viewMatrix.matrix[3][1], m32 = viewMatrix.matrix[3][2], m33 = viewMatrix.matrix[3][3];

    float w = m03 * position.x + m13 * position.y + m23 * position.z + m33;

    if (w < 1.f)
        return false;

    float invW = 1.0f / w;

    float x = m00 * position.x + m10 * position.y + m20 * position.z + m30;
    float y = m01 * position.x + m11 * position.y + m21 * position.z + m31;

    float screenX = (Screen::ScreenCenter.x) * (1.f + x * invW);
    float screenY = (Screen::ScreenCenter.y) * (1.f - y * invW);

    outPos->x = screenX;
    outPos->y = screenY;

    return true;
}

std::vector<PlayerRoot*> GetEntities() 
{
    std::vector<PlayerRoot*> out;

    auto localPlayer = PlayerRoot::GetLocalPlayer();
    if (!localPlayer) return {};

    auto Players = PlayerRoot::GetAllPlayers();

    int numPlayers = Players->Size();

    for (int i = 0; i < numPlayers; i++)
    {
        auto entity = Players->Get(i);
        if (!entity)
            continue;

        if (entity == localPlayer)
            continue;

        //printf("Entity %i -> %llx\n", i, entity);
        out.push_back(entity);
    }

    return out;
}

PlayerRoot* ClosestInFOV(float fov = 250.f)
{
    auto localPlayer = PlayerRoot::GetLocalPlayer();
    if (!localPlayer) return nullptr;

    auto localCamera = localPlayer->GetCamera();
    if (!localCamera) return nullptr;

    auto players = GetEntities();

    PlayerRoot* bestPlayer = nullptr;
    float closestDistance = fov;

    for (auto& player : players)
    {
        if (player == localPlayer) {
            continue;
        }

        Vector2 w2sPos;
        if (!WorldToScreen(player->GetRootPosition(), &w2sPos, localCamera->GetViewMatrix()))
            continue;
       
        float distance = Vector2::Distance(w2sPos, Screen::ScreenCenter);

        if (distance < closestDistance)
        {
            bestPlayer = player;
            closestDistance = distance;
        }
    }
    
    return bestPlayer;
}

// char __fastcall sub_36DF970(int *a1, __int64 a2, unsigned __int64 *a3, __int64 a4, unsigned int a5, int a6)

bool Raycast(Vector3 origin, Vector3 direction, float maxDistance, int32_t layerMask, int32_t queryTriggerInteraction, const uintptr_t* method)
{
    using Fn = bool(__fastcall*)(Vector3, Vector3, float, int32_t, int32_t, const uintptr_t*);
    auto raycastFunc = reinterpret_cast<Fn>(ProjectModule + 0x36DF970);

    return raycastFunc(origin, direction, maxDistance, layerMask, queryTriggerInteraction, method);
}
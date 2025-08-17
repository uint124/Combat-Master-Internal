#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdint>

#include "List.h"

namespace Globals {
    extern uint64_t ProjectModule;
}

class Unlocker
{
public:
    struct GameConfiguration
    {
        char pad_0x00[0x20];
        void* GdInfo;                       // 0x20 - GDInfo instance
    };

    struct GDInfo
    {
        char pad_0x00[0x30];
        void* levelsInfo;                   // 0x30 - LevelsInfo
        char pad_0x38[0x48];
        void* matches;                      // 0x80 - MatchesGDInfoSection
        char pad_0x88[0x38];
        void* loot;                         // 0x70 - LootGDInfoSection
    };

    struct LevelsInfo
    {
        char pad_0x00[0x18];
        List<uint32_t>* xpOfLevel;          // 0x18 - List<uint> _xpOfLevel
        char pad_0x20[0x8];
    };

    struct MatchInfo
    {
        char pad_0x00[0x54];
        uint16_t HighlightExpMultiplier;    // 0x54
        char pad_0x56[0x2];
        uint16_t VictoryExpBonus;           // 0x58
        uint16_t FirstPlaceExpBonus;        // 0x5A
        uint16_t SecondPlaceExpBonus;       // 0x5C
        uint16_t ThirdPlaceExpBonus;        // 0x5E
        char pad_0x60[0x8];
    };

    struct MatchesGDInfoSection
    {
        char pad_0x00[0x18];
        List<MatchInfo>* allMatches;        // 0x18 - List<MatchInfo>
    };

    struct BattlePassItem
    {
        char pad_0x00[0x20];
        uint32_t ExpToGetItem;              // 0x20
        bool IsFree;                        // 0x24
        char pad_0x25[0x3];
    };

    struct Price
    {
        uint32_t currency;                  // 0x00
        uint32_t amount;                    // 0x04
    };

    struct ShopLootInfo
    {
        char pad_0x00[0x30];
        void* StoreKey;                     // 0x30
        char pad_0x38[0xC];
        Price BuyPrice;                     // 0x44
        char pad_0x4C[0x4];
    };

    struct StoreBundleLootInfo : ShopLootInfo
    {
        char pad_0x50[0x18];
    };

    struct BattlePassInfo
    {
        char pad_0x00[0x78];
        void* LevelsBattlePassItems;        // 0x78 - Array of BattlePassItem
        char pad_0x80[0x18];
        List<BattlePassItem>* AllBattlePassItems; // 0x98 - List<BattlePassItem>
    };

    struct LootGDInfoSection
    {
        char pad_0x00[0x48];
        List<StoreBundleLootInfo>* shopBundleLootInfos; // 0x48 - List<StoreBundleLootInfo>
        char pad_0x50[0x10];
        List<BattlePassInfo>* battlePassInfos;          // 0x58 - List<BattlePassInfo>
    };

    static constexpr uint64_t GameConfiguration_get_Instance = 0x3d8db40;
    static constexpr uint64_t GDInfo_get_LevelsInfo = 0x5d7860;
    static constexpr uint64_t GDInfo_get_Matches = 0x5d8cc0;
    static constexpr uint64_t GDInfo_get_Loot = 0x5d8be0;
    static constexpr uint64_t MatchesGDInfoSection_get_AllMatches = 0x5d8ef0;
    static constexpr uint64_t LootGDInfoSection_get_ShopBundleLootInfos = 0x5d7280;
    static constexpr uint64_t LootGDInfoSection_get_BattlePassInfos = 0x5c96a0;

    static void XPBoost()
    {        
        try
        {
            List<MatchInfo>* allMatches = GetAllMatches();
            if (!allMatches)
            {
                return;
            }

            const uint16_t maxBonus = 32767;
            int matchCount = allMatches->Size();
            
            for (int i = 0; i < matchCount; i++)
            {
                MatchInfo* matchInfo = allMatches->Get(i);
                if (matchInfo)
                {
                    matchInfo->FirstPlaceExpBonus = maxBonus;
                    matchInfo->HighlightExpMultiplier = maxBonus;
                    matchInfo->SecondPlaceExpBonus = maxBonus;
                    matchInfo->ThirdPlaceExpBonus = maxBonus;
                    matchInfo->VictoryExpBonus = maxBonus;
                }
            }
            
            printf("[Unlocker] XP boost LOADED (%d matches modified)\n", matchCount);
        }
        catch (...)
        {
        }
    }

    static void FreeBundles()
    {    
        try
        {
            List<StoreBundleLootInfo>* shopBundleInfos = GetShopBundleLootInfos();
            if (!shopBundleInfos)
            {
                printf("[Unlocker] Failed to get ShopBundleLootInfos\n");
                return;
            }

            int bundleCount = shopBundleInfos->Size();
            
            void* freeStoreKey = nullptr;
            for (int i = 0; i < bundleCount; i++)
            {
                StoreBundleLootInfo* bundleInfo = shopBundleInfos->Get(i);
                if (bundleInfo && bundleInfo->BuyPrice.amount == 0)
                {
                    freeStoreKey = bundleInfo->StoreKey;
                    break;
                }
            }
            
            for (int i = 0; i < bundleCount; i++)
            {
                StoreBundleLootInfo* bundleInfo = shopBundleInfos->Get(i);
                if (bundleInfo)
                {
                    bundleInfo->BuyPrice.currency = 0;
                    bundleInfo->BuyPrice.amount = 0;
                    
                    if (freeStoreKey)
                    {
                        bundleInfo->StoreKey = freeStoreKey;
                    }
                }
            }
            
            printf("[Unlocker] Free bundles LOADED (%d bundles modified)\n", bundleCount);
        }
        catch (...)
        {
        }
    }

private:
    static GameConfiguration* GetGameConfigurationInstance()
    {
        typedef GameConfiguration* (*GetInstanceFunc)();
        GetInstanceFunc getInstance = reinterpret_cast<GetInstanceFunc>(Globals::ProjectModule + GameConfiguration_get_Instance);
        return getInstance();
    }

    static GDInfo* GetGDInfo()
    {
        GameConfiguration* gameConfig = GetGameConfigurationInstance();
        if (!gameConfig)
            return nullptr;
        
        return reinterpret_cast<GDInfo*>(gameConfig->GdInfo);
    }

    static LevelsInfo* GetLevelsInfo()
    {
        GDInfo* gdInfo = GetGDInfo();
        if (!gdInfo)
            return nullptr;
        
        typedef LevelsInfo* (*GetLevelsInfoFunc)(GDInfo*);
        GetLevelsInfoFunc getLevelsInfo = reinterpret_cast<GetLevelsInfoFunc>(Globals::ProjectModule + GDInfo_get_LevelsInfo);
        return getLevelsInfo(gdInfo);
    }

    static List<MatchInfo>* GetAllMatches()
    {
        GDInfo* gdInfo = GetGDInfo();
        if (!gdInfo)
            return nullptr;
        
        typedef MatchesGDInfoSection* (*GetMatchesFunc)(GDInfo*);
        GetMatchesFunc getMatches = reinterpret_cast<GetMatchesFunc>(Globals::ProjectModule + GDInfo_get_Matches);
        MatchesGDInfoSection* matchesSection = getMatches(gdInfo);
        
        if (!matchesSection)
            return nullptr;
        
        typedef List<MatchInfo>* (*GetAllMatchesFunc)(MatchesGDInfoSection*);
        GetAllMatchesFunc getAllMatches = reinterpret_cast<GetAllMatchesFunc>(Globals::ProjectModule + MatchesGDInfoSection_get_AllMatches);
        return getAllMatches(matchesSection);
    }

    static List<BattlePassInfo>* GetBattlePassInfos()
    {
        GDInfo* gdInfo = GetGDInfo();
        if (!gdInfo)
            return nullptr;
        
        typedef LootGDInfoSection* (*GetLootFunc)(GDInfo*);
        GetLootFunc getLoot = reinterpret_cast<GetLootFunc>(Globals::ProjectModule + GDInfo_get_Loot);
        LootGDInfoSection* lootSection = getLoot(gdInfo);
        
        if (!lootSection)
            return nullptr;
        
        typedef List<BattlePassInfo>* (*GetBattlePassInfosFunc)(LootGDInfoSection*);
        GetBattlePassInfosFunc getBattlePassInfos = reinterpret_cast<GetBattlePassInfosFunc>(Globals::ProjectModule + LootGDInfoSection_get_BattlePassInfos);
        return getBattlePassInfos(lootSection);
    }

    static List<StoreBundleLootInfo>* GetShopBundleLootInfos()
    {
        GDInfo* gdInfo = GetGDInfo();
        if (!gdInfo)
            return nullptr;
        
        typedef LootGDInfoSection* (*GetLootFunc)(GDInfo*);
        GetLootFunc getLoot = reinterpret_cast<GetLootFunc>(Globals::ProjectModule + GDInfo_get_Loot);
        LootGDInfoSection* lootSection = getLoot(gdInfo);
        
        if (!lootSection)
            return nullptr;
        
        typedef List<StoreBundleLootInfo>* (*GetShopBundleLootInfosFunc)(LootGDInfoSection*);
        GetShopBundleLootInfosFunc getShopBundleLootInfos = reinterpret_cast<GetShopBundleLootInfosFunc>(Globals::ProjectModule + LootGDInfoSection_get_ShopBundleLootInfos);
        return getShopBundleLootInfos(lootSection);
    }

    template<typename T>
    static T CallFunction(uint64_t address, void* instance = nullptr)
    {
        if (instance)
        {
            typedef T (*FuncWithInstance)(void*);
            FuncWithInstance func = reinterpret_cast<FuncWithInstance>(Globals::ProjectModule + address);
            return func(instance);
        }
        else
        {
            typedef T (*FuncStatic)();
            FuncStatic func = reinterpret_cast<FuncStatic>(Globals::ProjectModule + address);
            return func();
        }
    }
};
#include "WorldSession.h"
#include "Common.h"
#include "DBCStores.h"
#include "Log.h"
#include "ObjectAccessor.h"
#include "Pet.h"
#include "Player.h"
#include "WorldPacket.h"
#include "World.h"
#include "Log.h"
#include "CharacterCache.h"
#include "Chat.h"

#ifndef ARENA_1V1_H
#define ARENA_1V1_H

// TalentTab.dbc -> TalentTabID
const uint32 FORBIDDEN_TALENTS_IN_1V1_ARENA[] =
{
    // Healer
    201, // PriestDiscipline
    202, // PriestHoly
    382, // PaladinHoly
    262, // ShamanRestoration
    282, // DruidRestoration

    // Tanks
    //383, // PaladinProtection
    //163, // WarriorProtection

    0 // End
};


// Return false, if player have invested more than 35 talentpoints in a forbidden talenttree.
static bool Arena1v1CheckTalents(Player* player)
{
    if(!player)
        return false;

    if(sWorld->getBoolConfig(CONFIG_ARENA_1V1_BLOCK_FORBIDDEN_TALENTS) == false)
        return true;

    uint32 count = 0;
    for (uint32 talentId = 0; talentId < sTalentStore.GetNumRows(); ++talentId)
    {
        TalentEntry const* talentInfo = sTalentStore.LookupEntry(talentId);

        if (!talentInfo)
            continue;

        for (int8 rank = MAX_TALENT_RANK-1; rank >= 0; --rank)
        {
            if (talentInfo->SpellRank[rank] == 0)
                continue;

            if (player->HasTalent(talentInfo->SpellRank[rank], player->GetActiveSpec()))
            {
                for(int8 i = 0; FORBIDDEN_TALENTS_IN_1V1_ARENA[i] != 0; i++)
                    if(FORBIDDEN_TALENTS_IN_1V1_ARENA[i] == talentInfo->TabID)
                        count += rank + 1;
            }
        }
    }

    if(count >= 36)
    {
        ChatHandler(player->GetSession()).SendSysMessage("You can't join, because you have invested too many points in a forbidden talent. Please edit your talents.");
        return false;
    }
    else
        return true;
}

#endif

/* Copyright (C) 2009 - 2011 Hellscream Network <http://hellscream-wow.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* Name     : custom_npc_resetlevel.cpp
 * Category : NPCs
 * Complete : 100%
 * Author   : Wortex @ Hellscream Network <http://www.hellscream-wow.com/>
 * Comment  : Level Reseter with rewards.
 */

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../config.h"
#include "Database/DatabaseEnv.h"

extern DatabaseType SD2Database;

bool GossipHello_custom_npc_resetlevel(Player* pPlayer, Creature* pCreature)
{
    Config SD2Config;
    if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
        error_log("SD2: Unable to open configuration file");

    pPlayer->ADD_GOSSIP_ITEM(0, "Do you want to reset your character ?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(0, "You're not interested.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID());
    return true;
}

bool GossipSelect_custom_npc_resetlevel(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    Config SD2Config;
    if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
        error_log("SD2: Unable to open configuration file");

    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        if (pPlayer->getLevel() == (SD2Config.GetFloatDefault("ResetMaster.ResetMinimumLevel",0)))
        {
            Config SD2Config;
            if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG))
                error_log("SD2: Unable to open configuration file");

            pCreature->MonsterWhisper("You have been reset!", pPlayer, false);
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->ModifyMoney(+(SD2Config.GetFloatDefault("ResetMaster.ResetAddMoney",0)));
            pPlayer->SetLevel((SD2Config.GetFloatDefault("ResetMaster.LevelAfterReset",0)));
            QueryResult *result = SD2Database.PQuery("SELECT `count` FROM `char_resets` WHERE `id` = %u", pPlayer, false);
            if (result)
            {
                Field *fields = result->Fetch();
                int count = fields[1].GetInt32();
                delete result;
                SD2Database.PQuery("UPDATE `char_resets` SET `count` = %i WHERE `id` = %u;", count, pPlayer, false);
            }
            else
            {
                SD2Database.PQuery("INSERT INTO `char_resets` VALUES (%u, %s, '1');", pPlayer, false, pPlayer->GetName());
            }
        }
        else
        {
            pCreature->MonsterWhisper("Your level is too low or too high to reset!", pPlayer, false);
            pPlayer->CLOSE_GOSSIP_MENU();
        }
    }

    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pCreature->MonsterWhisper("Mh ok. See you around!", pPlayer, false);
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    
    return true;
}

void AddSC_custom_npc_resetlevel()
{
    Script *newscript;

    newscript                   = new Script;
    newscript->Name             = "custom_npc_resetlevel";
    newscript->pGossipHello     = &GossipHello_custom_npc_resetlevel;
    newscript->pGossipSelect    = &GossipSelect_custom_npc_resetlevel;
    newscript->RegisterSelf();
}
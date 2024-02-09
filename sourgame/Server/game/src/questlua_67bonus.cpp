#include "stdafx.h"
#include "questmanager.h"
#include "char.h"
#include "item.h"
#include "67bonusnew.h"

namespace quest
{
	
	ALUA(get_w_bonus67)
	{

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		LPITEM get_space = ch->GetBonus67NewItem();

		if(!get_space)
		{
			lua_pushboolean(L, false);
			return 1;
		}

		lua_pushboolean(L, true);
		return 1;
	}

	ALUA(get_vnum_bonus67)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		LPITEM get_space = ch->GetBonus67NewItem();

		if(!get_space)
		{
			lua_pushnumber(L, 0);
		}else{
			lua_pushnumber(L, get_space->GetVnum());
		}

		return 1;
	}

	ALUA(get_time_bonus67)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		LPITEM get_space = ch->GetBonus67NewItem();

		if(!get_space)
		{
			lua_pushnumber(L, 0);
		}else{
			lua_pushnumber(L, C67BonusNew::instance().GetTimeAddAttr(ch));
		}

		return 1;
	}

	ALUA(get_pos_bonus67)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		lua_pushboolean(L,C67BonusNew::instance().GetPosGetItem(ch));
		return 1;
	}


	ALUA(get_item_bonus67)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		lua_pushnumber(L,C67BonusNew::instance().GetItemAttr(ch));
		return 1;
	}


	void Register67BonusNewFunctionTable()
	{
		luaL_reg bonus67_info_functions[] =
		{
			{	"get_bonus67",	get_w_bonus67	},
			{	"get_vnum_bonus67", get_vnum_bonus67 }, 
			{	"get_time_bonus67", get_time_bonus67 },
			{	"get_pos_bonus67", get_pos_bonus67},
			{	"get_item_bonus67", get_item_bonus67},
			{	NULL,			NULL			}
		};

		CQuestManager::instance().AddLuaFunctionTable("bonus67", bonus67_info_functions);
	}
}


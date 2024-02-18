#pragma once
#include "../GameLib/ItemData.h"
#include "../UserInterface/Locale_inc.h"

struct SAffects
{
	enum
	{
		AFFECT_MAX_NUM = 32,
	};

	SAffects() : dwAffects(0) {}
	SAffects(const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}
	int operator = (const DWORD & c_rAffects)
	{
		__SetAffects(c_rAffects);
	}

	BOOL IsAffect(BYTE byIndex)
	{
		return dwAffects & (1 << byIndex);
	}

	void __SetAffects(const DWORD & c_rAffects)
	{
		dwAffects = c_rAffects;
	}

	DWORD dwAffects;
};

#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
const DWORD c_skillbook_slot_max = 10;
#endif

extern std::string g_strGuildSymbolPathName;

const DWORD c_Name_Max_Length = 64;
const DWORD c_FileName_Max_Length = 128;
const DWORD c_Short_Name_Max_Length = 32;

const DWORD c_Inventory_Page_Column = 5;
const DWORD c_Inventory_Page_Row = 9;
const DWORD c_Inventory_Page_Size = c_Inventory_Page_Column*c_Inventory_Page_Row; // x*y
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
const DWORD c_Inventory_Page_Count = 4;
#else
const DWORD c_Inventory_Page_Count = 2;
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
const DWORD c_Special_Inventory_Page_Size = 5*9; // x*y
const DWORD c_Special_Inventory_Page_Count = 5;
const DWORD c_Special_ItemSlot_Count = c_Special_Inventory_Page_Size * c_Special_Inventory_Page_Count;
#endif
const DWORD c_ItemSlot_Count = c_Inventory_Page_Size * c_Inventory_Page_Count;
const DWORD c_Equipment_Count = 12;

const DWORD c_Equipment_Start = c_ItemSlot_Count;

const DWORD c_Equipment_Body	= c_Equipment_Start + 0;
const DWORD c_Equipment_Head	= c_Equipment_Start + 1;
const DWORD c_Equipment_Shoes	= c_Equipment_Start + 2;
const DWORD c_Equipment_Wrist	= c_Equipment_Start + 3;
const DWORD c_Equipment_Weapon	= c_Equipment_Start + 4;
const DWORD c_Equipment_Neck	= c_Equipment_Start + 5;
const DWORD c_Equipment_Ear		= c_Equipment_Start + 6;
const DWORD c_Equipment_Unique1	= c_Equipment_Start + 7;
const DWORD c_Equipment_Unique2	= c_Equipment_Start + 8;
const DWORD c_Equipment_Arrow	= c_Equipment_Start + 9;
const DWORD c_Equipment_Shield	= c_Equipment_Start + 10;

//
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	const DWORD c_New_Equipment_Start = c_Equipment_Start + 25;
#ifdef ENABLE_PENDANT
		const DWORD c_New_Equipment_Count = 4;
	#else
		const DWORD c_New_Equipment_Count = 3;
#endif
	const DWORD c_Equipment_Ring1 = c_New_Equipment_Start + 0;
	const DWORD c_Equipment_Ring2 = c_New_Equipment_Start + 1;
	const DWORD c_Equipment_Belt  = c_New_Equipment_Start + 2;
#ifdef ENABLE_PENDANT
	const DWORD c_Equipment_Pendant  = c_New_Equipment_Start + 3; 
#endif
#endif

#ifdef ENABLE_SKILL_COLOR_SYSTEM
enum ESkillColorLength
{
	MAX_SKILL_COUNT = 6,
	MAX_EFFECT_COUNT = 5,
	BUFF_BEGIN = MAX_SKILL_COUNT,
#ifdef ENABLE_WOLFMAN_CHARACTER
	MAX_BUFF_COUNT = 6,
#else
	MAX_BUFF_COUNT = 5,
#endif
};
#endif

enum EDragonSoulDeckType
{
	DS_DECK_1,
	DS_DECK_2,
	DS_DECK_MAX_NUM = 2,
};

enum EDragonSoulGradeTypes
{
	DRAGON_SOUL_GRADE_NORMAL,
	DRAGON_SOUL_GRADE_BRILLIANT,
	DRAGON_SOUL_GRADE_RARE,
	DRAGON_SOUL_GRADE_ANCIENT,
	DRAGON_SOUL_GRADE_LEGENDARY,
#ifdef ENABLE_DS_GRADE_MYTH
	DRAGON_SOUL_GRADE_MYTH,
#endif
	DRAGON_SOUL_GRADE_MAX,
};

enum EDragonSoulStepTypes
{
	DRAGON_SOUL_STEP_LOWEST,
	DRAGON_SOUL_STEP_LOW,
	DRAGON_SOUL_STEP_MID,
	DRAGON_SOUL_STEP_HIGH,
	DRAGON_SOUL_STEP_HIGHEST,
	DRAGON_SOUL_STEP_MAX,
};

#ifdef ENABLE_COSTUME_SYSTEM
	const DWORD c_Costume_Slot_Start	= c_Equipment_Start + 19;	// [ÁÖÀÇ] ¼ýÀÚ(19) ÇÏµåÄÚµù ÁÖÀÇ. ÇöÀç ¼­¹ö¿¡¼­ ÄÚ½ºÃõ ½½·ÔÀº 19ºÎÅÍÀÓ. ¼­¹ö common/length.h ÆÄÀÏÀÇ EWearPositions ¿­°ÅÇü Âü°í.
	const DWORD	c_Costume_Slot_Body		= c_Costume_Slot_Start + 0;
	const DWORD	c_Costume_Slot_Hair		= c_Costume_Slot_Start + 1;
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	const DWORD	c_Costume_Slot_Weapon	= c_Costume_Slot_Start + 2;
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	const DWORD	c_Costume_Slot_Mount	= c_Costume_Slot_Start + 3;
#endif
#ifdef ENABLE_ACCE_SYSTEM
	const DWORD	c_Costume_Slot_Acce		= c_Costume_Slot_Start + 4;
#endif
#ifdef ENABLE_AURA_SYSTEM
	const DWORD c_Costume_Slot_Aura		= c_Costume_Slot_Start + 5;
#endif

	const DWORD c_Costume_Slot_Count = 6;

	const DWORD c_Costume_Slot_End		= c_Costume_Slot_Start + c_Costume_Slot_Count;
#endif

const DWORD c_Wear_Max = 32;
const DWORD c_DragonSoul_Equip_Start = c_ItemSlot_Count + c_Wear_Max;
const DWORD c_DragonSoul_Equip_Slot_Max = 6;
const DWORD c_DragonSoul_Equip_End = c_DragonSoul_Equip_Start + c_DragonSoul_Equip_Slot_Max * DS_DECK_MAX_NUM;

const DWORD c_DragonSoul_Equip_Reserved_Count = c_DragonSoul_Equip_Slot_Max * 3;

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	const DWORD c_Belt_Inventory_Slot_Start = c_DragonSoul_Equip_End + c_DragonSoul_Equip_Reserved_Count;
	const DWORD c_Belt_Inventory_Width = 4;
	const DWORD c_Belt_Inventory_Height= 4;
	const DWORD c_Belt_Inventory_Slot_Count = c_Belt_Inventory_Width * c_Belt_Inventory_Height;
	const DWORD c_Belt_Inventory_Slot_End = c_Belt_Inventory_Slot_Start + c_Belt_Inventory_Slot_Count;

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	const DWORD c_Skill_Book_Inventory_Slot_Start = c_Belt_Inventory_Slot_End;
	const DWORD c_Skill_Book_Inventory_Slot_Count = c_Special_ItemSlot_Count;
	const DWORD c_Skill_Book_Inventory_Slot_End = c_Skill_Book_Inventory_Slot_Start + c_Skill_Book_Inventory_Slot_Count;

	const DWORD c_Upgrade_Items_Inventory_Slot_Start = c_Skill_Book_Inventory_Slot_End;
	const DWORD c_Upgrade_Items_Inventory_Slot_Count = c_Special_ItemSlot_Count;
	const DWORD c_Upgrade_Items_Inventory_Slot_End = c_Upgrade_Items_Inventory_Slot_Start + c_Upgrade_Items_Inventory_Slot_Count;

	const DWORD c_Stone_Inventory_Slot_Start = c_Upgrade_Items_Inventory_Slot_End;
	const DWORD c_Stone_Inventory_Slot_Count = c_Special_ItemSlot_Count;
	const DWORD c_Stone_Inventory_Slot_End = c_Stone_Inventory_Slot_Start + c_Stone_Inventory_Slot_Count;

	const DWORD c_GiftBox_Inventory_Slot_Start = c_Stone_Inventory_Slot_End;
	const DWORD c_GiftBox_Inventory_Slot_Count = c_Special_ItemSlot_Count;
	const DWORD c_GiftBox_Inventory_Slot_End = c_GiftBox_Inventory_Slot_Start + c_GiftBox_Inventory_Slot_Count;
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	const DWORD c_Inventory_Count = c_GiftBox_Inventory_Slot_End;
#else
	const DWORD c_Inventory_Count = c_Belt_Inventory_Slot_End;
#endif
#else
	const DWORD c_Inventory_Count	= c_DragonSoul_Equip_End;
#endif

const DWORD c_DragonSoul_Inventory_Start = 0;
const DWORD c_DragonSoul_Inventory_Box_Size = 32;
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Box_Size;
const DWORD c_DragonSoul_Inventory_End = c_DragonSoul_Inventory_Start + c_DragonSoul_Inventory_Count;

enum ESlotType
{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_SKILL,
	SLOT_TYPE_EMOTION,
	SLOT_TYPE_SHOP,
	SLOT_TYPE_EXCHANGE_OWNER,
	SLOT_TYPE_EXCHANGE_TARGET,
	SLOT_TYPE_QUICK_SLOT,
	SLOT_TYPE_SAFEBOX,
	SLOT_TYPE_PRIVATE_SHOP,
	SLOT_TYPE_MALL,
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_SWITCHBOT
	SLOT_TYPE_SWITCHBOT,
#endif
#ifdef ENABLE_AURA_SYSTEM
	SLOT_TYPE_AURA,
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	SLOT_TYPE_SKILL_BOOK_INVENTORY,
	SLOT_TYPE_UPGRADE_ITEMS_INVENTORY,
	SLOT_TYPE_STONE_INVENTORY,
	SLOT_TYPE_GIFT_BOX_INVENTORY,
#endif
	SLOT_TYPE_MAX,
};

#ifdef ENABLE_AURA_SYSTEM
const BYTE c_AuraMaxLevel = 250;

enum EAuraRefineInfoSlot
{
	AURA_REFINE_INFO_SLOT_CURRENT,
	AURA_REFINE_INFO_SLOT_NEXT,
	AURA_REFINE_INFO_SLOT_EVOLVED,
	AURA_REFINE_INFO_SLOT_MAX
};

enum EAuraWindowType
{
	AURA_WINDOW_TYPE_ABSORB,
	AURA_WINDOW_TYPE_GROWTH,
	AURA_WINDOW_TYPE_EVOLVE,
	AURA_WINDOW_TYPE_MAX,
};

enum EAuraSlotType
{
	AURA_SLOT_MAIN,
	AURA_SLOT_SUB,
	AURA_SLOT_RESULT,
	AURA_SLOT_MAX
};

enum EAuraRefineInfoType
{
	AURA_REFINE_INFO_STEP,
	AURA_REFINE_INFO_LEVEL_MIN,
	AURA_REFINE_INFO_LEVEL_MAX,
	AURA_REFINE_INFO_NEED_EXP,
	AURA_REFINE_INFO_MATERIAL_VNUM,
	AURA_REFINE_INFO_MATERIAL_COUNT,
	AURA_REFINE_INFO_NEED_GOLD,
	AURA_REFINE_INFO_EVOLVE_PCT,
	AURA_REFINE_INFO_MAX
};
#endif

enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
#ifdef ENABLE_AURA_SYSTEM
	AURA_REFINE,
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	SKILL_BOOK_INVENTORY,
	UPGRADE_ITEMS_INVENTORY,
	STONE_INVENTORY,
	GIFT_BOX_INVENTORY,
#endif
	BELT_INVENTORY,
#ifdef ENABLE_SWITCHBOT
	SWITCHBOT,
#endif
	GROUND,
	
#ifdef ENABLE_GROWTH_PET_SYSTEM
	PET_FEED,
#endif
	
	WINDOW_TYPE_MAX,
};

#ifdef WJ_ENABLE_TRADABLE_ICON
enum ETopWindowTypes
{
	ON_TOP_WND_NONE,
	ON_TOP_WND_SHOP,
	ON_TOP_WND_EXCHANGE,
	ON_TOP_WND_SAFEBOX,
	ON_TOP_WND_PRIVATE_SHOP,
	ON_TOP_WND_ITEM_COMB,
#ifdef ENABLE_GROWTH_PET_SYSTEM
	ON_TOP_WND_PET_FEED,
	ON_TOP_WND_PET_ATTR_CHANGE, 
	ON_TOP_WND_PET_PRIMIUM_FEEDSTUFF,
#endif

	ON_TOP_WND_MAX,
};
#endif

enum EDSInventoryMaxNum
{
	DS_INVENTORY_MAX_NUM = c_DragonSoul_Inventory_Count,
	DS_REFINE_WINDOW_MAX_NUM = 15,
};

#ifdef ENABLE_GROWTH_PET_SYSTEM
enum EPet
{
	PET_FEED_SLOT_MAX = 9,
	PET_GROWTH_EVOL_MAX = 4,
	PET_GROWTH_SKILL_LEVEL_MAX = 20,
	PET_GROWTH_SKILL_OPEN_EVOL_LEVEL = 4,
	PET_REVIVE_MATERIAL_SLOT_MAX = 10,
	PET_SKILL_COUNT_MAX = 3,
	LIFE_TIME_FLASH_MIN_TIME = 3600,
	ITEM_SLOT_COUNT = 180,
	SPECIAL_EVOL_MIN_AGE = 2592000,
	PET_ATTR_DETERMINE_ITEM = 55032,
	PET_ATTR_CHANGE_ITEM = 55033,
};

enum EPetButtonEvent
{
	FEED_LIFE_TIME_EVENT,
	FEED_EVOL_EVENT,
	FEED_EXP_EVENT,
	FEED_BUTTON_MAX,
};

enum EPetWindowType
{
	PET_WINDOW_INFO,
	PET_WINDOW_ATTR_CHANGE,
	PET_WINDOW_PRIMIUM_FEEDSTUFF,
};

enum EPetAttrChange
{
	PET_WND_SLOT_ATTR_CHANGE,
	PET_WND_SLOT_ATTR_CHANGE_ITEM,
	PET_WND_SLOT_ATTR_CHANGE_RESULT,
	PET_WND_SLOT_ATTR_CHANGE_MAX,
};

enum EQuickSlotError
{
	QUICK_SLOT_POS_ERROR,
	QUICK_SLOT_ITEM_USE_SUCCESS,
	QUICK_SLOT_IS_NOT_ITEM,
	QUICK_SLOT_PET_ITEM_USE_SUCCESS,
	QUICK_SLOT_PET_ITEM_USE_FAILED,
	QUICK_SLOT_CAN_NOT_USE_PET_ITEM,
};
#endif

#ifdef ENABLE_REFINE_ELEMENT
enum ERefineElement
{
	REFINE_ELEMENT_MAX = 3,
	ELEMENT_MIN_REFINE_LEVEL = 7,
	REFINE_ELEMENT_UPGRADE_YANG = 3000000,
	REFINE_ELEMENT_DOWNGRADE_YANG = 10000000,
	REFINE_ELEMENT_CHANGE_YANG = 10000000,
	
	REFINE_ELEMENT_TYPE_UPGRADE = 0,
	REFINE_ELEMENT_TYPE_DOWNGRADE = 1,
	REFINE_ELEMENT_TYPE_CHANGE = 2,
	REFINE_ELEMENT_TYPE_UPGRADE_SUCCES = 10,
	REFINE_ELEMENT_TYPE_UPGRADE_FAIL = 11,
	REFINE_ELEMENT_TYPE_DOWNGRADE_SUCCES = 12,
	REFINE_ELEMENT_TYPE_CHANGE_SUCCES = 13,

	REFINE_ELEMENT_RANDOM_VALUE_MIN = 1,
	REFINE_ELEMENT_RANDOM_VALUE_MAX = 8,
	REFINE_ELEMENT_RANDOM_BONUS_VALUE_MIN = 2,
	REFINE_ELEMENT_RANDOM_BONUS_VALUE_MAX = 12,
};
#endif

#pragma pack (push, 1)
#define WORD_MAX 0xffff

#ifdef ENABLE_SWITCHBOT
enum ESwitchbotValues
{
	SWITCHBOT_SLOT_COUNT = 5,
	SWITCHBOT_ALTERNATIVE_COUNT = 2,
	MAX_NORM_ATTR_NUM = 5,
};

enum EAttributeSet
{
	ATTRIBUTE_SET_WEAPON,
	ATTRIBUTE_SET_BODY,
	ATTRIBUTE_SET_WRIST,
	ATTRIBUTE_SET_FOOTS,
	ATTRIBUTE_SET_NECK,
	ATTRIBUTE_SET_HEAD,
	ATTRIBUTE_SET_SHIELD,
	ATTRIBUTE_SET_EAR,
	ATTRIBUTE_SET_MAX_NUM,
};

#endif

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
    SItemPos ()
    {
		window_type =     INVENTORY;
		cell = WORD_MAX;
    }
	SItemPos (BYTE _window_type, WORD _cell)
    {
        window_type = _window_type;
        cell = _cell;
    }

  //  int operator=(const int _cell)
  //  {
		//window_type = INVENTORY;
  //      cell = _cell;
  //      return cell;
  //  }
	bool IsValidCell()
	{
		switch (window_type)
		{
		case INVENTORY:
			return cell < c_Inventory_Count;
			break;
		case EQUIPMENT:
			return cell < c_DragonSoul_Equip_End;
			break;
		case DRAGON_SOUL_INVENTORY:
			return cell < (DS_INVENTORY_MAX_NUM);
			break;
#ifdef ENABLE_SWITCHBOT
		case SWITCHBOT:
			return cell < SWITCHBOT_SLOT_COUNT;
			break;
#endif
		default:
			return false;
		}
	}
	bool IsEquipCell()
	{
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			return (c_Equipment_Start + c_Wear_Max > cell) && (c_Equipment_Start <= cell);
			break;

		case BELT_INVENTORY:
		case DRAGON_SOUL_INVENTORY:
			return false;
			break;

		default:
			return false;
		}
	}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	bool IsBeltInventoryCell()
	{
		bool bResult = c_Belt_Inventory_Slot_Start <= cell && c_Belt_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	bool IsSkillBookInventoryCell()
	{
		bool bResult = c_Skill_Book_Inventory_Slot_Start <= cell && c_Skill_Book_Inventory_Slot_End > cell;
		return bResult;
	}

	bool IsUpgradeItemsInventoryCell()
	{
		bool bResult = c_Upgrade_Items_Inventory_Slot_Start <= cell && c_Upgrade_Items_Inventory_Slot_End > cell;
		return bResult;
	}

	bool IsStoneInventoryCell()
	{
		bool bResult = c_Stone_Inventory_Slot_Start <= cell && c_Stone_Inventory_Slot_End > cell;
		return bResult;
	}

	bool IsGiftBoxInventoryCell()
	{
		bool bResult = c_GiftBox_Inventory_Slot_Start <= cell && c_GiftBox_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

	bool IsNPOS()
	{
		return (window_type == RESERVED_WINDOW && cell == WORD_MAX);
	}

	bool operator==(const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}

	bool operator<(const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
} TItemPos;
const TItemPos NPOS(RESERVED_WINDOW, WORD_MAX);
#pragma pack(pop)

const DWORD c_QuickBar_Line_Count = 3;
const DWORD c_QuickBar_Slot_Count = 12;

const float c_Idle_WaitTime = 5.0f;

const int c_Monster_Race_Start_Number = 6;
const int c_Monster_Model_Start_Number = 20001;

const float c_fAttack_Delay_Time = 0.2f;
const float c_fHit_Delay_Time = 0.1f;
const float c_fCrash_Wave_Time = 0.2f;
const float c_fCrash_Wave_Distance = 3.0f;

const float c_fHeight_Step_Distance = 50.0f;

enum
{
	DISTANCE_TYPE_FOUR_WAY,
	DISTANCE_TYPE_EIGHT_WAY,
	DISTANCE_TYPE_ONE_WAY,
	DISTANCE_TYPE_MAX_NUM,
};

const float c_fMagic_Script_Version = 1.0f;
const float c_fSkill_Script_Version = 1.0f;
const float c_fMagicSoundInformation_Version = 1.0f;
const float c_fBattleCommand_Script_Version = 1.0f;
const float c_fEmotionCommand_Script_Version = 1.0f;
const float c_fActive_Script_Version = 1.0f;
const float c_fPassive_Script_Version = 1.0f;

// Used by PushMove
const float c_fWalkDistance = 175.0f;
const float c_fRunDistance = 310.0f;

#define FILE_MAX_LEN 128

enum
{
	ITEM_SOCKET_SLOT_MAX_NUM = 3,
	// refactored attribute slot begin
	ITEM_ATTRIBUTE_SLOT_NORM_NUM	= 5,
	ITEM_ATTRIBUTE_SLOT_RARE_NUM	= 2,

	ITEM_ATTRIBUTE_SLOT_NORM_START	= 0,
	ITEM_ATTRIBUTE_SLOT_NORM_END	= ITEM_ATTRIBUTE_SLOT_NORM_START + ITEM_ATTRIBUTE_SLOT_NORM_NUM,

	ITEM_ATTRIBUTE_SLOT_RARE_START	= ITEM_ATTRIBUTE_SLOT_NORM_END,
	ITEM_ATTRIBUTE_SLOT_RARE_END	= ITEM_ATTRIBUTE_SLOT_RARE_START + ITEM_ATTRIBUTE_SLOT_RARE_NUM,

	ITEM_ATTRIBUTE_SLOT_MAX_NUM		= ITEM_ATTRIBUTE_SLOT_RARE_END, // 7
	// refactored attribute slot end
};

#pragma pack(push)
#pragma pack(1)

typedef struct SQuickSlot
{
	BYTE Type;
	BYTE Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
	BYTE		bType;
	short		sValue;
} TPlayerItemAttribute;

typedef struct packet_item
{
	DWORD		vnum;
	BYTE		count;
	DWORD		dwRefineElement;
	DWORD		flags;
	DWORD		anti_flags;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TItemData;

typedef struct packet_item2
{
	DWORD		vnum;
	BYTE		count;
	DWORD		flags;
	DWORD		anti_flags;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TItemData2;

typedef struct packet_shop_item
{
	DWORD		vnum;
#ifdef ENABLE_FULL_YANG
	long long	price;
#else
	DWORD		price;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD		cheque;
#endif 
	BYTE		count;
	BYTE		display_pos;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD		dwRefineElement;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_BUY_ITEMS_WORLDARD
	DWORD 		item_vnum_buy;
	BYTE 		item_count_buy;
#endif
} TShopItemData;

#pragma pack(pop)

inline float GetSqrtDistance(int ix1, int iy1, int ix2, int iy2) // By sqrt
{
	float dx, dy;

	dx = float(ix1 - ix2);
	dy = float(iy1 - iy2);

	return sqrtf(dx*dx + dy*dy);
}

// DEFAULT_FONT
void DefaultFont_Startup();
void DefaultFont_Cleanup();
void DefaultFont_SetName(const char * c_szFontName);
CResource* DefaultFont_GetResource();
CResource* DefaultItalicFont_GetResource();
// END_OF_DEFAULT_FONT

void SetGuildSymbolPath(const char * c_szPathName);
const char * GetGuildSymbolFileName(DWORD dwGuildID);
BYTE SlotTypeToInvenType(BYTE bSlotType);

#ifdef ENABLE_AURA_SYSTEM
int* GetAuraRefineInfo(BYTE bLevel);
#endif

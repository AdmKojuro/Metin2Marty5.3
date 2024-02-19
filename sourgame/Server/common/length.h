#ifndef __INC_METIN_II_LENGTH_H__
#define __INC_METIN_II_LENGTH_H__

#include "CommonDefines.h"

#define WORD_MAX 0xffff

#ifdef ENABLE_REFINE_ELEMENT
enum ERefineElementType
{
	REFINE_ELEMENT_CATEGORY_NONE,
	REFINE_ELEMENT_CATEGORY_ELECT,
	REFINE_ELEMENT_CATEGORY_FIRE,
	REFINE_ELEMENT_CATEGORY_ICE,
	REFINE_ELEMENT_CATEGORY_WIND,
	REFINE_ELEMENT_CATEGORY_EARTH,
	REFINE_ELEMENT_CATEGORY_DARK,
	REFINE_ELEMENT_CATEGORY_MAX,
};

enum ERefineElement
{
	REFINE_ELEMENT_MAX = 3,
	ELEMENT_MIN_REFINE_LEVEL = 7,
	REFINE_ELEMENT_UPGRADE_YANG = 3000000,
	REFINE_ELEMENT_DOWNGRADE_YANG = 10000000,
	REFINE_ELEMENT_CHANGE_YANG = 10000000,
	
	REFINE_ELEMENT_UPGRADE_PROBABILITY = 100,
	REFINE_ELEMENT_DOWNGRADE_PROBABILITY = 100,
	REFINE_ELEMENT_CHANGE_PROBABILITY = 100,
	
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
enum EMisc
{
	MAX_HOST_LENGTH			= 15,
	IP_ADDRESS_LENGTH		= 15,
	LOGIN_MAX_LEN			= 30,
	PASSWD_MAX_LEN			= 16,
#ifdef ENABLE_PLAYER_PER_ACCOUNT5
	PLAYER_PER_ACCOUNT		= 5,
#else
	PLAYER_PER_ACCOUNT		= 4,
#endif
	ACCOUNT_STATUS_MAX_LEN	= 8,
	CHARACTER_NAME_MAX_LEN	= 24,
	SHOP_SIGN_MAX_LEN		= 32,
	INVENTORY_PAGE_COLUMN	= 5, // 5 (default)
	INVENTORY_PAGE_ROW		= 9, // 9 (default)
	INVENTORY_PAGE_SIZE		= INVENTORY_PAGE_COLUMN*INVENTORY_PAGE_ROW,
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
	INVENTORY_PAGE_COUNT	= 4, // 2 (default)
#else
	INVENTORY_PAGE_COUNT	= 2, // 2 (default)
#endif
	INVENTORY_MAX_NUM		= INVENTORY_PAGE_SIZE*INVENTORY_PAGE_COUNT, // 90 (default)
	ABILITY_MAX_NUM			= 50,
	EMPIRE_MAX_NUM			= 4,
	BANWORD_MAX_LEN			= 24,
	SMS_MAX_LEN				= 80,
	MOBILE_MAX_LEN			= 32,
	SOCIAL_ID_MAX_LEN		= 18,

	GUILD_NAME_MAX_LEN		= 12,
	QUEST_NAME_MAX_NUM		= 64,

	SHOP_HOST_ITEM_MAX_NUM	= 100,
	SHOP_GUEST_ITEM_MAX_NUM = 18,

	SHOP_PRICELIST_MAX_NUM	= 100,

	CHAT_MAX_LEN			= 512,

	QUICKSLOT_MAX_NUM		= 36,

	JOURNAL_MAX_NUM			= 2,

	QUERY_MAX_LEN			= 8192,

	FILE_MAX_LEN			= 128,

	PLAYER_EXP_TABLE_MAX	= 120,
	PLAYER_MAX_LEVEL_CONST	= 250,

	GUILD_MAX_LEVEL			= 20,
	MOB_MAX_LEVEL			= 100,

	ATTRIBUTE_MAX_VALUE		= 20,
	CHARACTER_PATH_MAX_NUM	= 64,
	SKILL_MAX_NUM			= 255,
	SKILLBOOK_DELAY_MIN		= 64800,
	SKILLBOOK_DELAY_MAX		= 108000,
	SKILL_MAX_LEVEL			= 40,

	APPLY_NAME_MAX_LEN		= 32,
	EVENT_FLAG_NAME_MAX_LEN = 32,

	MOB_SKILL_MAX_NUM		= 5,

    POINT_MAX_NUM = 255,
	DRAGON_SOUL_BOX_SIZE = 32,
	DRAGON_SOUL_BOX_COLUMN_NUM = 8,
	DRAGON_SOUL_BOX_ROW_NUM = DRAGON_SOUL_BOX_SIZE / DRAGON_SOUL_BOX_COLUMN_NUM,
	DRAGON_SOUL_REFINE_GRID_SIZE = 15,
	MAX_AMOUNT_OF_MALL_BONUS	= 20,

	WEAR_MAX_NUM				= 32,

	//LIMIT_GOLD
	GOLD_MAX = 2000000000,
#ifdef __ENABLE_CHEQUE_SYSTEM__
	CHEQUE_MAX = 1000,
#endif // __ENABLE_CHEQUE_SYSTEM__
#ifdef ENABLE_GAYA_SYSTEM
	GAYA_MAX = 999999,
#endif
	MAX_PASSPOD = 8 ,

#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	c_skillbook_slot_max = 10,
	GOLD_COMB_SKILLBOOK = 100000,
	BONUS_67_SLOT_MAX = 1,
#endif

	//END_LIMIT_GOLD

	OPENID_AUTHKEY_LEN = 32,

	SHOP_TAB_NAME_MAX = 32,
	SHOP_TAB_COUNT_MAX = 3,

	BELT_INVENTORY_SLOT_WIDTH = 4,
	BELT_INVENTORY_SLOT_HEIGHT= 4,

	BELT_INVENTORY_SLOT_COUNT = BELT_INVENTORY_SLOT_WIDTH * BELT_INVENTORY_SLOT_HEIGHT,

#ifdef __SPECIAL_INVENTORY_SYSTEM__
	SPECIAL_INVENTORY_PAGE_SIZE = 45,
	SPECIAL_INVENTORY_PAGE_COUNT = 5, // 225
	SPECIAL_INVENTORY_MAX_NUM = SPECIAL_INVENTORY_PAGE_SIZE * SPECIAL_INVENTORY_PAGE_COUNT,

	SKILL_BOOK_INVENTORY_MAX_NUM = SPECIAL_INVENTORY_MAX_NUM,
	UPGRADE_ITEMS_INVENTORY_MAX_NUM = SPECIAL_INVENTORY_MAX_NUM,
	STONE_INVENTORY_MAX_NUM = SPECIAL_INVENTORY_MAX_NUM,
	GIFT_BOX_INVENTORY_MAX_NUM = SPECIAL_INVENTORY_MAX_NUM,
#endif

};

#ifdef ENABLE_SWITCHBOT

enum SwitchbotValues
{
	SWITCHBOT_SLOT_COUNT = 5,
	SWITCHBOT_ALTERNATIVE_COUNT = 2,
	//1 = Switching Item
	//2 = Yang
	SWITCHBOT_PRICE_TYPE = 1,
	//Amount Swtichting Items the Yang-Price
	SWITCHBOT_PRICE_AMOUNT = 1,
};

const DWORD c_arSwitchingItems[3] =
{
	39028,
	71084,
	76014,
};
#endif

enum EGrowthPet
{
	PET_MAX_LEVEL = 105,

	PET_ATTR_CHANGE_ITEMVNUM = 55033,
	PET_NAME_MAX_SIZE = 20,
	PET_NAME_MIN_SIZE = 4,
	PET_HATCHING_MONEY = 100000,
	PET_SKILL_COUNT_MAX = 3,
	PET_GROWTH_SKILL_LEVEL_MAX = 20,
	PET_SKILL_UPGRADE_PRICE = 2000000,
	PET_GROWTH_SKILL_OPEN_EVOL_LEVEL = 4,
	PET_FEED_SLOT_MAX = 9,
	PET_REVIVE_MATERIAL_SLOT_MAX = 10,

	EGG_TO_UPBRINGING_DELTA = 300,
	MELEY_PET_EGG_VNUM = 55406,

	EXP_TYPE_MOB = 1,
	EXP_TYPE_ITEM = 2,

	PET_EVOL_MAX_ITEM_COUNT = 7,
	PET_LAST_EVOL_MIN_DAY_COUNT = 30,

	PET_MONKEY = 55701,
	PET_SPIDER = 55702,
	PET_RAZADOR = 55703,
	PET_NEMERE = 55704,
	PET_BLUE_DRAGON = 55705,
	PET_RED_DRAGON = 55706,
	PET_AZRAEL = 55707,
	PET_MINI_EXECUTOR = 55708,
	PET_BASHIIDO = 55709,
	PET_NESSIE = 55710,
	PET_EXEDYAR = 55711,
	PET_MAX_NUM = 11,

	PET_MAX_BONUS_NUM = 8,
};

enum EPetState
{
	STATE_NONE,
	STATE_UPBRINGING,
	STATE_BAG,
	STATE_SAFEBOX,
};

enum EEmpire
{
	ALL_EMPIRES,
	SHINSOO_EMPIRE,
	CHUNJO_EMPIRE,
	JINNO_EMPIRE
};

enum EMatrixCard
{
	MATRIX_CODE_MAX_LEN		= 192,
	MATRIX_ANSWER_MAX_LEN	= 8,
};

enum EWearPositions
{
	WEAR_BODY = 0,		// 0
	WEAR_HEAD = 1,		// 1
	WEAR_FOOTS = 2,		// 2
	WEAR_WRIST = 3,		// 3
	WEAR_WEAPON = 4,	// 4
	WEAR_NECK = 5,		// 5
	WEAR_EAR = 6,		// 6
	WEAR_UNIQUE1 = 7,	// 7
	WEAR_UNIQUE2 = 8,	// 8
	WEAR_ARROW = 9,		// 9
	WEAR_SHIELD = 10,	// 10	
    WEAR_ABILITY1 = 11,  // 11
    WEAR_ABILITY2 = 12,  // 12
    WEAR_ABILITY3 = 13,  // 13
    WEAR_ABILITY4 = 14,  // 14
    WEAR_ABILITY5 = 15,  // 15
    WEAR_ABILITY6 = 16,  // 16
    WEAR_ABILITY7 = 17,  // 17
    WEAR_ABILITY8 = 18,  // 18	
	WEAR_COSTUME_BODY = 19,	// 19
	WEAR_COSTUME_HAIR = 20,	// 20
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	WEAR_COSTUME_WEAPON = 21,	//21
#endif
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	WEAR_COSTUME_MOUNT = 22, //22
#endif
#ifdef ENABLE_ACCE_SYSTEM
	WEAR_COSTUME_ACCE = 23, //23
#endif
#ifdef __AURA_SYSTEM__
	WEAR_COSTUME_AURA = 24, //24
#endif

	WEAR_RING1 = 25,			// 25	: ½Å±Ô ¹ÝÁö½½·Ô1 (¿ÞÂÊ)
	WEAR_RING2 = 26,			// 26	: ½Å±Ô ¹ÝÁö½½·Ô2 (¿À¸¥ÂÊ)
	WEAR_BELT = 27,			// 27	: ½Å±Ô º§Æ®½½·Ô	
#ifdef __PENDANT_SYSTEM__
	WEAR_PENDANT = 28,// 28
#endif
	WEAR_MAX = 32	// 
};

enum EDragonSoulDeckType
{
	DRAGON_SOUL_DECK_0,
	DRAGON_SOUL_DECK_1,
	DRAGON_SOUL_DECK_MAX_NUM = 2,

	DRAGON_SOUL_DECK_RESERVED_MAX_NUM = 3,
};

enum ESex
{
	SEX_MALE,
	SEX_FEMALE
};

enum EDirection
{
	DIR_NORTH,
	DIR_NORTHEAST,
	DIR_EAST,
	DIR_SOUTHEAST,
	DIR_SOUTH,
	DIR_SOUTHWEST,
	DIR_WEST,
	DIR_NORTHWEST,
	DIR_MAX_NUM
};

#define ABILITY_MAX_LEVEL	10

enum EAbilityDifficulty
{
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
	DIFFICULTY_VERY_HARD,
	DIFFICULTY_NUM_TYPES
};

enum EAbilityCategory
{
	CATEGORY_PHYSICAL,
	CATEGORY_MENTAL,
	CATEGORY_ATTRIBUTE,
	CATEGORY_NUM_TYPES
};

enum EJobs
{
	JOB_WARRIOR,
	JOB_ASSASSIN,
	JOB_SURA,
	JOB_SHAMAN,
#ifdef ENABLE_WOLFMAN_CHARACTER
	JOB_WOLFMAN,
#endif
	JOB_MAX_NUM
};

enum ESkillGroups
{
	SKILL_GROUP_MAX_NUM = 2,
};

enum ERaceFlags
{
	RACE_FLAG_ANIMAL	= (1 << 0),
	RACE_FLAG_UNDEAD	= (1 << 1),
	RACE_FLAG_DEVIL		= (1 << 2),
	RACE_FLAG_HUMAN		= (1 << 3),
	RACE_FLAG_ORC		= (1 << 4),
	RACE_FLAG_MILGYO	= (1 << 5),
	RACE_FLAG_INSECT	= (1 << 6),
	RACE_FLAG_FIRE		= (1 << 7),
	RACE_FLAG_ICE		= (1 << 8),
	RACE_FLAG_DESERT	= (1 << 9),
	RACE_FLAG_TREE		= (1 << 10),
	RACE_FLAG_ATT_ELEC	= (1 << 11),
	RACE_FLAG_ATT_FIRE	= (1 << 12),
	RACE_FLAG_ATT_ICE	= (1 << 13),
	RACE_FLAG_ATT_WIND	= (1 << 14),
	RACE_FLAG_ATT_EARTH	= (1 << 15),
	RACE_FLAG_ATT_DARK	= (1 << 16),
#ifdef __ELEMENT_SYSTEM__
	RACE_FLAG_ATT_CZ = (1 << 17),
#endif
#ifdef __ATTR_METIN_BOSS__
	RACE_FLAG_METIN = (1 << 18),
	RACE_FLAG_BOSS = (1 << 19),
#endif
};

enum ELoads
{
	LOAD_NONE,
	LOAD_LIGHT,
	LOAD_NORMAL,
	LOAD_HEAVY,
	LOAD_MASSIVE
};

enum
{
	QUICKSLOT_TYPE_NONE,
	QUICKSLOT_TYPE_ITEM,
	QUICKSLOT_TYPE_SKILL,
	QUICKSLOT_TYPE_COMMAND,
	QUICKSLOT_TYPE_MAX_NUM,
};

enum EParts
{
	PART_MAIN,
	PART_WEAPON,
	PART_HEAD,
	PART_HAIR,

#ifdef ENABLE_ACCE_SYSTEM
	PART_ACCE,
#endif
#ifdef __AURA_SYSTEM__
	PART_AURA,
#endif
	PART_MAX_NUM,
	PART_WEAPON_SUB,
};

enum EChatType
{
	CHAT_TYPE_TALKING,
	CHAT_TYPE_INFO,
	CHAT_TYPE_NOTICE,
	CHAT_TYPE_PARTY,
	CHAT_TYPE_GUILD,
	CHAT_TYPE_COMMAND,
	CHAT_TYPE_SHOUT,
	CHAT_TYPE_WHISPER,
	CHAT_TYPE_BIG_NOTICE,
	CHAT_TYPE_MONARCH_NOTICE,
#ifdef ENABLE_DICE_SYSTEM
	CHAT_TYPE_DICE_INFO, //11
#endif
	CHAT_TYPE_MAX_NUM
};

enum EWhisperType
{
	WHISPER_TYPE_NORMAL		= 0,
	WHISPER_TYPE_NOT_EXIST		= 1,
	WHISPER_TYPE_TARGET_BLOCKED	= 2,
	WHISPER_TYPE_SENDER_BLOCKED	= 3,
	WHISPER_TYPE_ERROR		= 4,
	WHISPER_TYPE_GM			= 5,
	WHISPER_TYPE_SYSTEM		= 0xFF
};

enum ECharacterPosition
{
	POSITION_GENERAL,
	POSITION_BATTLE,
	POSITION_DYING,
	POSITION_SITTING_CHAIR,
	POSITION_SITTING_GROUND,
	POSITION_INTRO,
	POSITION_MAX_NUM
};

enum EGMLevels
{
	GM_PLAYER,
	GM_LOW_WIZARD,
	GM_WIZARD,
	GM_HIGH_WIZARD,
	GM_GOD,
	GM_IMPLEMENTOR,
	GM_DISABLE,
};

enum EMobRank
{
	MOB_RANK_PAWN,
	MOB_RANK_S_PAWN,
	MOB_RANK_KNIGHT,
	MOB_RANK_S_KNIGHT,
	MOB_RANK_BOSS,
	MOB_RANK_KING,
	MOB_RANK_MAX_NUM
};

enum ECharType
{
	CHAR_TYPE_MONSTER,
	CHAR_TYPE_NPC,
	CHAR_TYPE_STONE,
	CHAR_TYPE_WARP,
	CHAR_TYPE_DOOR,
	CHAR_TYPE_BUILDING,
	CHAR_TYPE_PC,
	CHAR_TYPE_POLYMORPH_PC,
	CHAR_TYPE_HORSE,
	CHAR_TYPE_GOTO,
	CHAR_TYPE_PET_PAY,
	CHAR_TYPE_PET,
};

enum EBattleType
{
	BATTLE_TYPE_MELEE,
	BATTLE_TYPE_RANGE,
	BATTLE_TYPE_MAGIC,
	BATTLE_TYPE_SPECIAL,
	BATTLE_TYPE_POWER,
	BATTLE_TYPE_TANKER,
	BATTLE_TYPE_SUPER_POWER,
	BATTLE_TYPE_SUPER_TANKER,
	BATTLE_TYPE_MAX_NUM
};

enum EApplyTypes
{
	APPLY_NONE,			// 0
	APPLY_MAX_HP,		// 1
	APPLY_MAX_SP,		// 2
	APPLY_CON,			// 3
	APPLY_INT,			// 4
	APPLY_STR,			// 5
	APPLY_DEX,			// 6
	APPLY_ATT_SPEED,	// 7
	APPLY_MOV_SPEED,	// 8
	APPLY_CAST_SPEED,	// 9
	APPLY_HP_REGEN,		// 10
	APPLY_SP_REGEN,		// 11
	APPLY_POISON_PCT,	// 12
	APPLY_STUN_PCT,		// 13
	APPLY_SLOW_PCT,		// 14
	APPLY_CRITICAL_PCT,		// 15
	APPLY_PENETRATE_PCT,	// 16
	APPLY_ATTBONUS_HUMAN,	// 17
	APPLY_ATTBONUS_ANIMAL,	// 18
	APPLY_ATTBONUS_ORC,		// 19
	APPLY_ATTBONUS_MILGYO,	// 20
	APPLY_ATTBONUS_UNDEAD,	// 21
	APPLY_ATTBONUS_DEVIL,	// 22
	APPLY_STEAL_HP,			// 23
	APPLY_STEAL_SP,			// 24
	APPLY_MANA_BURN_PCT,	// 25
	APPLY_DAMAGE_SP_RECOVER,	// 26
	APPLY_BLOCK,			// 27
	APPLY_DODGE,			// 28
	APPLY_RESIST_SWORD,		// 29
	APPLY_RESIST_TWOHAND,	// 30
	APPLY_RESIST_DAGGER,	// 31
	APPLY_RESIST_BELL,		// 32
	APPLY_RESIST_FAN,		// 33
	APPLY_RESIST_BOW,		// 34
	APPLY_RESIST_FIRE,		// 35
	APPLY_RESIST_ELEC,		// 36
	APPLY_RESIST_MAGIC,		// 37
	APPLY_RESIST_WIND,		// 38
	APPLY_REFLECT_MELEE,	// 39
	APPLY_REFLECT_CURSE,	// 40
	APPLY_POISON_REDUCE,	// 41
	APPLY_KILL_SP_RECOVER,	// 42
	APPLY_EXP_DOUBLE_BONUS,	// 43
	APPLY_GOLD_DOUBLE_BONUS,	// 44
	APPLY_ITEM_DROP_BONUS,	// 45
	APPLY_POTION_BONUS,		// 46
	APPLY_KILL_HP_RECOVER,	// 47
	APPLY_IMMUNE_STUN,		// 48
	APPLY_IMMUNE_SLOW,		// 49
	APPLY_IMMUNE_FALL,		// 50
	APPLY_SKILL,			// 51
	APPLY_BOW_DISTANCE,		// 52
	APPLY_ATT_GRADE_BONUS,	// 53
	APPLY_DEF_GRADE_BONUS,	// 54
	APPLY_MAGIC_ATT_GRADE,	// 55
	APPLY_MAGIC_DEF_GRADE,	// 56
	APPLY_CURSE_PCT,		// 57
	APPLY_MAX_STAMINA,		// 58
	APPLY_ATTBONUS_WARRIOR,	// 59
	APPLY_ATTBONUS_ASSASSIN,	// 60
	APPLY_ATTBONUS_SURA,	// 61
	APPLY_ATTBONUS_SHAMAN,	// 62
	APPLY_ATTBONUS_MONSTER,	// 63
	APPLY_MALL_ATTBONUS,
	APPLY_MALL_DEFBONUS,
	APPLY_MALL_EXPBONUS,
	APPLY_MALL_ITEMBONUS,
	APPLY_MALL_GOLDBONUS,
	APPLY_MAX_HP_PCT,
	APPLY_MAX_SP_PCT,
	APPLY_SKILL_DAMAGE_BONUS,
	APPLY_NORMAL_HIT_DAMAGE_BONUS,
	APPLY_SKILL_DEFEND_BONUS,
	APPLY_NORMAL_HIT_DEFEND_BONUS,
	APPLY_PC_BANG_EXP_BONUS,
	APPLY_PC_BANG_DROP_BONUS,

	APPLY_EXTRACT_HP_PCT,

	APPLY_RESIST_WARRIOR,
	APPLY_RESIST_ASSASSIN,
	APPLY_RESIST_SURA,
	APPLY_RESIST_SHAMAN,
	APPLY_ENERGY,
	APPLY_DEF_GRADE,
	APPLY_COSTUME_ATTR_BONUS,
	APPLY_MAGIC_ATTBONUS_PER,
	APPLY_MELEE_MAGIC_ATTBONUS_PER,

	APPLY_RESIST_ICE,
	APPLY_RESIST_EARTH,
	APPLY_RESIST_DARK,

	APPLY_ANTI_CRITICAL_PCT,
	APPLY_ANTI_PENETRATE_PCT,

#ifdef ENABLE_WOLFMAN_CHARACTER
	APPLY_BLEEDING_REDUCE			= 92,		//92
	APPLY_BLEEDING_PCT				= 93,		//93
	APPLY_ATTBONUS_WOLFMAN			= 94,
	APPLY_RESIST_WOLFMAN			= 95,
	APPLY_RESIST_CLAW				= 96,
#endif

#ifdef ENABLE_ACCE_SYSTEM
	APPLY_ACCEDRAIN_RATE			= 97,			//97
#endif

#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	APPLY_RESIST_MAGIC_REDUCTION	= 98,	//98
#endif
#ifdef __ELEMENT_SYSTEM__
	APPLY_ENCHANT_ELEC,					// 99
	APPLY_ENCHANT_FIRE,					// 100
	APPLY_ENCHANT_ICE,					// 101
	APPLY_ENCHANT_WIND,					// 102
	APPLY_ENCHANT_EARTH,				// 103
	APPLY_ENCHANT_DARK,					// 104
	APPLY_ATTBONUS_CZ,					// 105
	APPLY_ATTBONUS_INSECT,				// 106
	APPLY_ATTBONUS_DESERT,				// 107

	APPLY_RESIST_SWORD_REDUCTION,		// 108
	APPLY_RESIST_TWOHAND_REDUCTION,		// 109
	APPLY_RESIST_DAGGER_REDUCTION,		// 110
	APPLY_RESIST_BELL_REDUCTION,		// 111
	APPLY_RESIST_FAN_REDUCTION,			// 112
	APPLY_RESIST_BOW_REDUCTION,			// 113
#ifdef ENABLE_WOLFMAN_CHARACTER
	APPLY_RESIST_CLAW_REDUCTION,		// 114
#endif
	APPLY_RESIST_HUMAN,					// 115
#endif
#ifdef __ATTR_METIN_BOSS__
	APPLY_ATTBONUS_METIN,				// 115
	APPLY_ATTBONUS_BOSS,				// 116
#endif

	MAX_APPLY_NUM,						//
};

enum EOnClickEvents
{
	ON_CLICK_NONE,
	ON_CLICK_SHOP,
	ON_CLICK_TALK,
	ON_CLICK_MAX_NUM
};

enum EOnIdleEvents
{
	ON_IDLE_NONE,
	ON_IDLE_GENERAL,
	ON_IDLE_MAX_NUM
};

enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY,
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
#ifdef __AURA_SYSTEM__
	AURA_REFINE,
#endif
#ifdef __SPECIAL_INVENTORY_SYSTEM__
	SKILL_BOOK_INVENTORY,
	UPGRADE_ITEMS_INVENTORY,
	STONE_INVENTORY,
	GIFT_BOX_INVENTORY,
#endif
	BELT_INVENTORY,
#ifdef ENABLE_SWITCHBOT
	SWITCHBOT,
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	BONUS_NEW_67,
#endif
	GROUND
};

enum EMobSizes
{
	MOBSIZE_RESERVED,
	MOBSIZE_SMALL,
	MOBSIZE_MEDIUM,
	MOBSIZE_BIG
};

enum EAIFlags
{
	AIFLAG_AGGRESSIVE	= (1 << 0),
	AIFLAG_NOMOVE	= (1 << 1),
	AIFLAG_COWARD	= (1 << 2),
	AIFLAG_NOATTACKSHINSU	= (1 << 3),
	AIFLAG_NOATTACKJINNO	= (1 << 4),
	AIFLAG_NOATTACKCHUNJO	= (1 << 5),
	AIFLAG_ATTACKMOB = (1 << 6 ),
	AIFLAG_BERSERK	= (1 << 7),
	AIFLAG_STONESKIN	= (1 << 8),
	AIFLAG_GODSPEED	= (1 << 9),
	AIFLAG_DEATHBLOW	= (1 << 10),
	AIFLAG_REVIVE		= (1 << 11),
};

enum EMobStatType
{
	MOB_STATTYPE_POWER,
	MOB_STATTYPE_TANKER,
	MOB_STATTYPE_SUPER_POWER,
	MOB_STATTYPE_SUPER_TANKER,
	MOB_STATTYPE_RANGE,
	MOB_STATTYPE_MAGIC,
	MOB_STATTYPE_MAX_NUM
};

enum EImmuneFlags
{
	IMMUNE_STUN		= (1 << 0),
	IMMUNE_SLOW		= (1 << 1),
	IMMUNE_FALL		= (1 << 2),
	IMMUNE_CURSE	= (1 << 3),
	IMMUNE_POISON	= (1 << 4),
	IMMUNE_TERROR	= (1 << 5),
	IMMUNE_REFLECT	= (1 << 6),
};

enum EMobEnchants
{
	MOB_ENCHANT_CURSE,
	MOB_ENCHANT_SLOW,
	MOB_ENCHANT_POISON,
	MOB_ENCHANT_STUN,
	MOB_ENCHANT_CRITICAL,
	MOB_ENCHANT_PENETRATE,
#if defined(ENABLE_WOLFMAN_CHARACTER) && !defined(USE_MOB_BLEEDING_AS_POISON)
	MOB_ENCHANT_BLEEDING,
#endif
	MOB_ENCHANTS_MAX_NUM
};

enum EMobResists
{
	MOB_RESIST_SWORD,
	MOB_RESIST_TWOHAND,
	MOB_RESIST_DAGGER,
	MOB_RESIST_BELL,
	MOB_RESIST_FAN,
	MOB_RESIST_BOW,
	MOB_RESIST_FIRE,
	MOB_RESIST_ELECT,
	MOB_RESIST_MAGIC,
	MOB_RESIST_WIND,
	MOB_RESIST_POISON,
#if defined(ENABLE_WOLFMAN_CHARACTER) && !defined(USE_MOB_CLAW_AS_DAGGER)
	MOB_RESIST_CLAW,
#endif
#if defined(ENABLE_WOLFMAN_CHARACTER) && !defined(USE_MOB_BLEEDING_AS_POISON)
	MOB_RESIST_BLEEDING,
#endif
	MOB_RESISTS_MAX_NUM
};

enum
{
	SKILL_ATTR_TYPE_NORMAL = 1,
	SKILL_ATTR_TYPE_MELEE,
	SKILL_ATTR_TYPE_RANGE,
	SKILL_ATTR_TYPE_MAGIC
		/*
		   SKILL_ATTR_TYPE_FIRE,
		   SKILL_ATTR_TYPE_ICE,
		   SKILL_ATTR_TYPE_ELEC,
		   SKILL_ATTR_TYPE_DARK,
		   */
};

enum
{
	SKILL_NORMAL,
	SKILL_MASTER,
	SKILL_GRAND_MASTER,
	SKILL_PERFECT_MASTER,
};

enum EGuildWarType
{
	GUILD_WAR_TYPE_FIELD,
	GUILD_WAR_TYPE_BATTLE,
	GUILD_WAR_TYPE_FLAG,
	GUILD_WAR_TYPE_MAX_NUM
};

enum EGuildWarState
{
	GUILD_WAR_NONE,
	GUILD_WAR_SEND_DECLARE,
	GUILD_WAR_REFUSE,
	GUILD_WAR_RECV_DECLARE,
	GUILD_WAR_WAIT_START,
	GUILD_WAR_CANCEL,
	GUILD_WAR_ON_WAR,
	GUILD_WAR_END,
	GUILD_WAR_OVER,
	GUILD_WAR_RESERVE,

	GUILD_WAR_DURATION = 30*60,
	GUILD_WAR_WIN_POINT = 1000,
	GUILD_WAR_LADDER_HALF_PENALTY_TIME = 12*60*60,
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
#ifdef __PENDANT_SYSTEM__
	ATTRIBUTE_SET_PENDANT,
#endif
#ifdef ENABLE_ITEM_ATTR_COSTUME
	ATTRIBUTE_SET_COSTUME_BODY,
	ATTRIBUTE_SET_COSTUME_HAIR,
#if defined(ENABLE_ITEM_ATTR_COSTUME) && defined(ENABLE_WEAPON_COSTUME_SYSTEM)
	ATTRIBUTE_SET_COSTUME_WEAPON,
#endif
#endif
	ATTRIBUTE_SET_MAX_NUM
};

enum EPrivType
{
	PRIV_NONE,
	PRIV_ITEM_DROP,
	PRIV_GOLD_DROP,
	PRIV_GOLD10_DROP,
	PRIV_EXP_PCT,
	MAX_PRIV_NUM,
};

enum EMoneyLogType
{
	MONEY_LOG_RESERVED,
	MONEY_LOG_MONSTER,
	MONEY_LOG_SHOP,
	MONEY_LOG_REFINE,
	MONEY_LOG_QUEST,
	MONEY_LOG_GUILD,
	MONEY_LOG_MISC,
	MONEY_LOG_MONSTER_KILL,
	MONEY_LOG_DROP,
	MONEY_LOG_TYPE_MAX_NUM,
};

enum EPremiumTypes
{
	PREMIUM_EXP,
	PREMIUM_ITEM,
	PREMIUM_SAFEBOX,
	PREMIUM_AUTOLOOT,
	PREMIUM_FISH_MIND,
	PREMIUM_MARRIAGE_FAST,
	PREMIUM_GOLD,
	PREMIUM_MAX_NUM = 9
};

enum SPECIAL_EFFECT
{
	SE_NONE,

	SE_HPUP_RED,
	SE_SPUP_BLUE,
	SE_SPEEDUP_GREEN,
	SE_DXUP_PURPLE,
	SE_CRITICAL,
	SE_PENETRATE,
	SE_BLOCK,
	SE_DODGE,
	SE_CHINA_FIREWORK,
	SE_SPIN_TOP,
	SE_SUCCESS,
	SE_FAIL,
	SE_FR_SUCCESS,
	SE_LEVELUP_ON_14_FOR_GERMANY,
	SE_LEVELUP_UNDER_15_FOR_GERMANY,
	SE_PERCENT_DAMAGE1,
	SE_PERCENT_DAMAGE2,
	SE_PERCENT_DAMAGE3,

	SE_AUTO_HPUP,
	SE_AUTO_SPUP,

	SE_EQUIP_RAMADAN_RING,
	SE_EQUIP_HALLOWEEN_CANDY,
	SE_EQUIP_HAPPINESS_RING,
	SE_EQUIP_LOVE_PENDANT,

#ifdef ENABLE_ACCE_SYSTEM
	SE_EFFECT_ACCE_SUCCEDED,
	SE_EFFECT_ACCE_EQUIP,
#endif

#ifdef __GROWTH_PET_SYSTEM__
	SE_GYEONGGONG_BOOM,
#endif

#ifdef ENABLE_EXTENDED_BATTLE_PASS
	SE_EFFECT_BP_NORMAL_MISSION_COMPLETED,
	SE_EFFECT_BP_PREMIUM_MISSION_COMPLETED,
	SE_EFFECT_BP_EVENT_MISSION_COMPLETED,
	SE_EFFECT_BP_NORMAL_BATTLEPASS_COMPLETED,
	SE_EFFECT_BP_PREMIUM_BATTLEPASS_COMPLETED,
	SE_EFFECT_BP_EVENT_BATTLEPASS_COMPLETED,
#endif
};

enum ETeenFlags
{
	TEENFLAG_NONE = 0,
	TEENFLAG_1HOUR,
	TEENFLAG_2HOUR,
	TEENFLAG_3HOUR,
	TEENFLAG_4HOUR,
	TEENFLAG_5HOUR,
};

#include "item_length.h"






enum EDragonSoulRefineWindowSize
{
	DRAGON_SOUL_REFINE_GRID_MAX = 15,
};

enum EMisc2
{
	DRAGON_SOUL_EQUIP_SLOT_START = INVENTORY_MAX_NUM + WEAR_MAX_NUM,
	DRAGON_SOUL_EQUIP_SLOT_END = DRAGON_SOUL_EQUIP_SLOT_START + (DS_SLOT_MAX * DRAGON_SOUL_DECK_MAX_NUM),
	DRAGON_SOUL_EQUIP_RESERVED_SLOT_END = DRAGON_SOUL_EQUIP_SLOT_END + (DS_SLOT_MAX * DRAGON_SOUL_DECK_RESERVED_MAX_NUM),

	BELT_INVENTORY_SLOT_START = DRAGON_SOUL_EQUIP_RESERVED_SLOT_END,
	BELT_INVENTORY_SLOT_END = BELT_INVENTORY_SLOT_START + BELT_INVENTORY_SLOT_COUNT,

#ifdef __SPECIAL_INVENTORY_SYSTEM__
	SKILL_BOOK_INVENTORY_SLOT_START = BELT_INVENTORY_SLOT_END,
	SKILL_BOOK_INVENTORY_SLOT_END = SKILL_BOOK_INVENTORY_SLOT_START + SKILL_BOOK_INVENTORY_MAX_NUM,

	UPGRADE_ITEMS_INVENTORY_SLOT_START = SKILL_BOOK_INVENTORY_SLOT_END,
	UPGRADE_ITEMS_INVENTORY_SLOT_END = UPGRADE_ITEMS_INVENTORY_SLOT_START + UPGRADE_ITEMS_INVENTORY_MAX_NUM,

	STONE_INVENTORY_SLOT_START = UPGRADE_ITEMS_INVENTORY_SLOT_END,
	STONE_INVENTORY_SLOT_END = STONE_INVENTORY_SLOT_START + STONE_INVENTORY_MAX_NUM,

	GIFT_BOX_INVENTORY_SLOT_START = STONE_INVENTORY_SLOT_END,
	GIFT_BOX_INVENTORY_SLOT_END = GIFT_BOX_INVENTORY_SLOT_START + GIFT_BOX_INVENTORY_MAX_NUM,
#endif

#ifdef __SPECIAL_INVENTORY_SYSTEM__
	INVENTORY_AND_EQUIP_SLOT_MAX = GIFT_BOX_INVENTORY_SLOT_END,
#else
	INVENTORY_AND_EQUIP_SLOT_MAX = BELT_INVENTORY_SLOT_END,
#endif
};

#pragma pack(push, 1)

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
	SItemPos ()
	{
		window_type = INVENTORY;
		cell = WORD_MAX;
	}

	SItemPos (BYTE _window_type, WORD _cell)
	{
		window_type = _window_type;
		cell = _cell;
	}

	bool IsValidItemPosition() const
	{
		switch (window_type)
		{
		case RESERVED_WINDOW:
			return false;
		case INVENTORY:
		case EQUIPMENT:
		case BELT_INVENTORY:
			return cell < INVENTORY_AND_EQUIP_SLOT_MAX;
		case DRAGON_SOUL_INVENTORY:
			return cell < (DRAGON_SOUL_INVENTORY_MAX_NUM);
#ifdef __SPECIAL_INVENTORY_SYSTEM__
		case SKILL_BOOK_INVENTORY:
		case UPGRADE_ITEMS_INVENTORY:
		case STONE_INVENTORY:
		case GIFT_BOX_INVENTORY:
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
		case BONUS_NEW_67:
			return cell < BONUS_67_SLOT_MAX;
#endif
		case SAFEBOX:
		case MALL:
			return false;
#ifdef ENABLE_SWITCHBOT
		case SWITCHBOT:
			return cell < SWITCHBOT_SLOT_COUNT;
#endif
		default:
			return false;
		}
		return false;
	}

	bool IsEquipPosition() const
	{
		return ((INVENTORY == window_type || EQUIPMENT == window_type) && cell >= INVENTORY_MAX_NUM && cell < INVENTORY_MAX_NUM + WEAR_MAX_NUM)
			|| IsDragonSoulEquipPosition();
	}

	bool IsDragonSoulEquipPosition() const
	{
		return (DRAGON_SOUL_EQUIP_SLOT_START <= cell) && (DRAGON_SOUL_EQUIP_SLOT_END > cell);
	}

	bool IsBeltInventoryPosition() const
	{
		return (BELT_INVENTORY_SLOT_START <= cell) && (BELT_INVENTORY_SLOT_END > cell);
	}

	bool IsDefaultInventoryPosition() const
	{
		return INVENTORY == window_type && cell < INVENTORY_MAX_NUM;
	}

#ifdef ENABLE_SWITCHBOT
	bool IsSwitchbotPosition() const
	{
		return SWITCHBOT == window_type && cell < SWITCHBOT_SLOT_COUNT;
	}
#endif

#ifdef __SPECIAL_INVENTORY_SYSTEM__
	bool IsSkillBookInventoryPosition() const
	{
		return (INVENTORY == window_type && (cell >= SKILL_BOOK_INVENTORY_SLOT_START && cell < SKILL_BOOK_INVENTORY_SLOT_END));
	}

	bool IsUpgradeItemsInventoryPosition() const
	{
		return (INVENTORY == window_type && (cell >= UPGRADE_ITEMS_INVENTORY_SLOT_START && cell < UPGRADE_ITEMS_INVENTORY_SLOT_END));
	}

	bool IsStoneInventoryPosition() const
	{
		return (INVENTORY == window_type && (cell >= STONE_INVENTORY_SLOT_START && cell < STONE_INVENTORY_SLOT_END));
	}

	bool IsGiftBoxInventoryPosition() const
	{
		return (INVENTORY == window_type && (cell >= GIFT_BOX_INVENTORY_SLOT_START && cell < GIFT_BOX_INVENTORY_SLOT_END));
	}
#endif

	bool operator==(const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}
	bool operator<(const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
	bool operator!=(const struct SItemPos& rhs) const
	{
		return (window_type != rhs.window_type) || (cell != rhs.cell);
	}
} TItemPos;

const TItemPos NPOS (RESERVED_WINDOW, WORD_MAX);

typedef struct SItemPosEx
{
	TItemPos pos;
	int id{ 0 };
} TItemPosEx;

typedef enum
{
	SHOP_COIN_TYPE_GOLD, // DEFAULT VALUE
	SHOP_COIN_TYPE_SECONDARY_COIN,
} EShopCoinType;

#ifdef __SKILL_COLOR_SYSTEM__
namespace ESkillColorLength
{
	enum ESkillColorLength
	{
		MAX_SKILL_COUNT = 6,
		MAX_EFFECT_COUNT = 5,
#ifdef ENABLE_WOLFMAN_CHARACTER
		MAX_BUFF_COUNT = 6,
#else
		MAX_BUFF_COUNT = 5,
#endif
		BUFF_BEGIN = MAX_SKILL_COUNT,
	};
}
#endif

#ifdef __AURA_SYSTEM__
typedef struct SAuraRefineInfo
{
	BYTE bAuraRefineInfoLevel;
	BYTE bAuraRefineInfoExpPercent;
} TAuraRefineInfo;

enum EAuraMisc
{
	AURA_MAX_LEVEL = 250,
	AURA_REFINE_MAX_DISTANCE = 1000,
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

enum EAuraGradeType
{
	AURA_GRADE_NONE,
	AURA_GRADE_ORDINARY,
	AURA_GRADE_SIMPLE,
	AURA_GRADE_NOBLE,
	AURA_GRADE_SPARKLING,
	AURA_GRADE_MAGNIFICENT,
	AURA_GRADE_RADIANT,
	AURA_GRADE_MAX_NUM,
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

enum ERefineInfoType
{
	AURA_REFINE_INFO_SLOT_CURRENT,
	AURA_REFINE_INFO_SLOT_NEXT,
	AURA_REFINE_INFO_SLOT_EVOLVED,
	AURA_REFINE_INFO_SLOT_MAX
};
#endif

#ifdef __MULTI_LANGUAGE_SYSTEM__
enum EMultiLocale
{
	MAX_QUEST_NOTICE_ARGS = 5
};

enum ELocale
{
	LOCALE_YMIR, // Korea
	LOCALE_EN, // United Kingdom
	LOCALE_PT, // Portugal
	LOCALE_ES, // Spain
	LOCALE_FR, // France
	LOCALE_DE, // Germany
	LOCALE_RO, // Romania
	LOCALE_TR, // Turkey
	LOCALE_MAX_NUM,
	LOCALE_DEFAULT = LOCALE_EN,
};
#endif

#ifdef ENABLE_EXTENDED_BATTLE_PASS
enum EExtBattlePassType
{
	BATTLEPASS_NORMAL,
	BATTLEPASS_PREMIUM,
	BATTLEPASS_EVENT,

	BATTLEPASS_TYPE_MAX,
};

enum EExtBattleMissionPassType
{
	MISSION_TYPE_NONE,

	KILL_MONSTER,
	KILL_PLAYER,

	DAMAGE_MONSTER,
	DAMAGE_PLAYER,

	BP_ITEM_USE,
	BP_ITEM_SELL,
	BP_ITEM_CRAFT,
	BP_ITEM_REFINE,
	BP_ITEM_DESTROY,
	BP_ITEM_COLLECT,

	FISH_FISHING,
	FISH_GRILL,
	FISH_CATCH,
	
	EXP_COLLECT,
	YANG_COLLECT,
	
	GUILD_PLAY_GUILDWAR,
	GUILD_SPENT_EXP,
	
	GAYA_CRAFT_GAYA,
	GAYA_BUY_ITEM_GAYA_COST,
	
	PET_ENCHANT,

	COMPLETE_DUNGEON,
	COMPLETE_MINIGAME,

	MISSION_TYPE_MAX,
};
#endif

#pragma pack(pop)

#endif


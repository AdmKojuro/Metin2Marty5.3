#pragma once

#ifdef USE_AHNLAB_HACKSHIELD
#include "Hackshield.h"
#include METIN2HS_INCLUDE_ANTICPXSVR
#else
#pragma pack(push)
#pragma pack(8)

#define ANTICPX_TRANS_BUFFER_MAX			400

typedef struct _AHNHS_TRANS_BUFFER
{
	unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX];
	unsigned short nLength;
} AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;

#pragma pack(pop)
#endif /* !USE_AHNLAB_HACKSHIELD */

#include "Locale.h"
#include "../gamelib/RaceData.h"
#include "../gamelib/ItemData.h"

typedef BYTE TPacketHeader;

enum
{
	/////////////////////////////////////////////////
	// To Server
	// HEADER_BLANK is the not use(for future use)
	HEADER_CG_LOGIN								= 1,
	HEADER_CG_ATTACK							= 2,
	HEADER_CG_CHAT								= 3,
	HEADER_CG_PLAYER_CREATE						= 4,
	HEADER_CG_PLAYER_DESTROY					= 5,
	HEADER_CG_PLAYER_SELECT						= 6,
	HEADER_CG_CHARACTER_MOVE					= 7,
	HEADER_CG_SYNC_POSITION  					= 8,
	HEADER_CG_DIRECT_ENTER						= 9,
	HEADER_CG_ENTERGAME							= 10,
	HEADER_CG_ITEM_USE							= 11,
	HEADER_CG_ITEM_DROP							= 12,
	HEADER_CG_ITEM_MOVE							= 13,
	HEADER_CG_ITEM_PICKUP						= 15,
	HEADER_CG_QUICKSLOT_ADD						= 16,
	HEADER_CG_QUICKSLOT_DEL						= 17,
	HEADER_CG_QUICKSLOT_SWAP					= 18,
	HEADER_CG_WHISPER							= 19,
	HEADER_CG_ITEM_DROP2						= 20,
#ifdef ENABLE_NEW_DROP_DIALOG
	HEADER_CG_ITEM_DESTROY						= 21,
#endif
	//HEADER_BLANK21							= 21,
	//HEADER_BLANK22							= 22,
	//HEADER_BLANK22							= 23,
	//HEADER_BLANK24							= 24,
	//HEADER_BLANK25							= 25,
	HEADER_CG_ON_CLICK							= 26,
	HEADER_CG_EXCHANGE							= 27,
	HEADER_CG_CHARACTER_POSITION				= 28,
	HEADER_CG_SCRIPT_ANSWER						= 29,
	HEADER_CG_QUEST_INPUT_STRING				= 30,
	HEADER_CG_QUEST_CONFIRM						= 31,
	HEADER_CG_REQUEST_EVENT_QUEST				= 32,
	//HEADER_BLANK33							= 33,
	//HEADER_BLANK34							= 34,
	//HEADER_BLANK35							= 35,
	//HEADER_BLANK36							= 36,
	//HEADER_BLANK37							= 37,
	//HEADER_BLANK38							= 38,
	//HEADER_BLANK39							= 39,
	//HEADER_BLANK40							= 40,
	HEADER_CG_PVP								= 41,
	//HEADER_BLANK42							= 42,
	//HEADER_BLANK43							= 43,
	//HEADER_BLANK44							= 44,
	//HEADER_BLANK45							= 45,
	//HEADER_BLANK46							= 46,
	//HEADER_BLANK47							= 47,
	//HEADER_BLANK48							= 48,
	//HEADER_BLANK49							= 49,
	HEADER_CG_SHOP								= 50,
	HEADER_CG_FLY_TARGETING						= 51,
	HEADER_CG_USE_SKILL							= 52,
	HEADER_CG_ADD_FLY_TARGETING					= 53,
	HEADER_CG_SHOOT								= 54,
	HEADER_CG_MYSHOP							= 55,
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	HEADER_CG_SKILL_COLOR						= 56,
#endif
#ifdef ENABLE_SEND_TARGET_INFO
	HEADER_CG_TARGET_INFO_LOAD					= 59,
#endif

	//HEADER_BLANK56							= 56,
	//HEADER_BLANK57							= 57,
	//HEADER_BLANK58							= 58,
	//HEADER_BLANK59							= 59,
	HEADER_CG_ITEM_USE_TO_ITEM					= 60,
	HEADER_CG_TARGET							= 61,
	//HEADER_BLANK62							= 62,
	//HEADER_BLANK63							= 63,
	//HEADER_BLANK64							= 64,
	HEADER_CG_WARP								= 65,
	HEADER_CG_SCRIPT_BUTTON						= 66,
	HEADER_CG_MESSENGER							= 67,
	//HEADER_BLANK68							= 68,
	HEADER_CG_MALL_CHECKOUT						= 69,
	HEADER_CG_SAFEBOX_CHECKIN					= 70,
	HEADER_CG_SAFEBOX_CHECKOUT					= 71,
	HEADER_CG_PARTY_INVITE						= 72,
	HEADER_CG_PARTY_INVITE_ANSWER				= 73,
	HEADER_CG_PARTY_REMOVE						= 74,
	HEADER_CG_PARTY_SET_STATE					= 75,
	HEADER_CG_PARTY_USE_SKILL					= 76,
	HEADER_CG_SAFEBOX_ITEM_MOVE					= 77,
	HEADER_CG_PARTY_PARAMETER					= 78,
	//HEADER_BLANK68							= 79,
	HEADER_CG_GUILD								= 80,
	HEADER_CG_ANSWER_MAKE_GUILD					= 81,
	HEADER_CG_FISHING							= 82,
	HEADER_CG_GIVE_ITEM							= 83,
	//HEADER_BLANK84							= 84,
	//HEADER_BLANK85							= 85,
	//HEADER_BLANK86							= 86,
	//HEADER_BLANK87							= 87,
	//HEADER_BLANK88							= 88,
	//HEADER_BLANK89							= 89,
	HEADER_CG_EMPIRE							= 90,
	//HEADER_BLANK91							= 91,
	//HEADER_BLANK92							= 92,
	//HEADER_BLANK93							= 93,
	//HEADER_BLANK94							= 94,
	//HEADER_BLANK95							= 95,
	HEADER_CG_REFINE							= 96,
	//HEADER_BLANK97							= 97,
	//HEADER_BLANK98							= 98,
	//HEADER_BLANK99							= 99,

	HEADER_CG_MARK_LOGIN						= 100,
	HEADER_CG_MARK_CRCLIST						= 101,
	HEADER_CG_MARK_UPLOAD						= 102,
	HEADER_CG_MARK_IDXLIST						= 104,

	HEADER_CG_CRC_REPORT						= 103,

	HEADER_CG_HACK								= 105,
	HEADER_CG_CHANGE_NAME						= 106,
	HEADER_CG_SMS								= 107,
	HEADER_CG_CHINA_MATRIX_CARD					= 108,
	HEADER_CG_LOGIN2							= 109,
	HEADER_CG_DUNGEON							= 110,
	HEADER_CG_LOGIN3							= 111,
	HEADER_CG_GUILD_SYMBOL_UPLOAD				= 112,
	HEADER_CG_GUILD_SYMBOL_CRC					= 113,
	HEADER_CG_SCRIPT_SELECT_ITEM				= 114,
	HEADER_CG_LOGIN4							= 115,
#ifdef ENABLE_SWITCHBOT
	HEADER_CG_SWITCHBOT							= 171,
#endif
	HEADER_CG_LOGIN5_OPENID						= 116,
#ifdef ENABLE_EVENT_MANAGER
	HEADER_CG_REQUEST_EVENT_DATA				= 117,
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	HEADER_CG_PET_HATCH							= 118,
	HEADER_CG_PET_WINDOW_TYPE					= 119,
	HEADER_CG_PET_WINDOW						= 120,
	HEADER_CG_PET_NAME_CHANGE					= 121,
	HEADER_CG_PET_FEED							= 122,
	HEADER_CG_PET_DETERMINE						= 123,
	HEADER_CG_PET_ATTR_CHANGE					= 124,
	HEADER_CG_PET_REVIVE						= 125,
	HEADER_CG_PET_LEARN_SKILL					= 126,
	HEADER_CG_PET_SKILL_UPGRADE					= 127,
	HEADER_CG_PET_DELETE_SKILL					= 128,
	HEADER_CG_PET_DELETE_ALL_SKILL				= 129,
#endif
#ifdef ENABLE_EXTENDED_WHISPER_DETAILS
	HEADER_CG_WHISPER_DETAILS = 130,
#endif
	HEADER_CG_RUNUP_MATRIX_ANSWER				= 201,
	HEADER_CG_NEWCIBN_PASSPOD_ANSWER			= 202,

	HEADER_CG_HS_ACK							= 203,
	HEADER_CG_XTRAP_ACK							= 204,

	HEADER_CG_DRAGON_SOUL_REFINE				= 205,
	HEADER_CG_STATE_CHECKER						= 206,

#ifdef ENABLE_GAYA_SYSTEM
	HEADER_CG_GAYA_SYSTEM 						= 210,
#endif
#ifdef ENABLE_EXTENDED_BATTLE_PASS
	HEADER_CG_EXT_BATTLE_PASS_ACTION			= 213,
	HEADER_CG_EXT_SEND_BP_PREMIUM_ITEM			= 214,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_CG_CUBE_RENEWAL 						= 216,
#endif
#ifdef ENABLE_AURA_SYSTEM
		HEADER_CG_AURA 							= 217,
#endif
#ifdef ENABLE_REFINE_ELEMENT
	HEADER_CG_REFINE_ELEMENT 					= 172,
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	HEADER_CG_67_BONUS_NEW 						= 220,
#endif
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	HEADER_CG_CHANGE_LANGUAGE = 221,
#endif
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
	HEADER_CG_PRIVATE_SHOP_SEARCH = 223,
	HEADER_CG_PRIVATE_SHOP_SEARCH_CLOSE = 224,
	HEADER_CG_PRIVATE_SHOP_SEARCH_BUY_ITEM = 225,
#endif
	HEADER_CG_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_CG_TIME_SYNC							= 0xfc,
	HEADER_CG_CLIENT_VERSION					= 0xfd,
	HEADER_CG_CLIENT_VERSION2					= 0xf1,
	HEADER_CG_PONG								= 0xfe,
	HEADER_CG_HANDSHAKE							= 0xff,
	/////////////////////////////////////////////////
	// From Server


	HEADER_GC_CHARACTER_ADD						= 1,
	HEADER_GC_CHARACTER_DEL						= 2,
	HEADER_GC_CHARACTER_MOVE					= 3,
	HEADER_GC_CHAT								= 4,
	HEADER_GC_SYNC_POSITION 					= 5,
	HEADER_GC_LOGIN_SUCCESS3					= 6,
	HEADER_GC_LOGIN_FAILURE						= 7,
	HEADER_GC_PLAYER_CREATE_SUCCESS				= 8,
	HEADER_GC_PLAYER_CREATE_FAILURE				= 9,
	HEADER_GC_PLAYER_DELETE_SUCCESS				= 10,
	HEADER_GC_PLAYER_DELETE_WRONG_SOCIAL_ID		= 11,
	// 12
	HEADER_GC_STUN								= 13,
	HEADER_GC_DEAD								= 14,

	HEADER_GC_MAIN_CHARACTER					= 15,
	HEADER_GC_PLAYER_POINTS						= 16,
	HEADER_GC_PLAYER_POINT_CHANGE				= 17,
	HEADER_GC_CHANGE_SPEED						= 18,
	HEADER_GC_CHARACTER_UPDATE					= 19,
	HEADER_GC_ITEM_SET							= 20,
	HEADER_GC_ITEM_SET2							= 21,
	HEADER_GC_ITEM_USE							= 22,
	HEADER_GC_ITEM_DROP							= 23,
	HEADER_GC_ITEM_UPDATE						= 25,
	HEADER_GC_ITEM_GROUND_ADD					= 26,
	HEADER_GC_ITEM_GROUND_DEL					= 27,
	HEADER_GC_QUICKSLOT_ADD						= 28,
	HEADER_GC_QUICKSLOT_DEL						= 29,
	HEADER_GC_QUICKSLOT_SWAP					= 30,
	HEADER_GC_ITEM_OWNERSHIP					= 31,
	HEADER_GC_LOGIN_SUCCESS4					= 32,
	HEADER_GC_ITEM_UNBIND_TIME					= 33,
	HEADER_GC_WHISPER							= 34,
	HEADER_GC_ALERT								= 35,

	HEADER_GC_MOTION							= 36,

	HEADER_GC_SHOP								= 38,
	HEADER_GC_SHOP_SIGN							= 39,

	// 39 ~ 41 Balnk
	HEADER_GC_DUEL_START						= 40,
	HEADER_GC_PVP								= 41,
	HEADER_GC_EXCHANGE							= 42,
	HEADER_GC_CHARACTER_POSITION				= 43,

	HEADER_GC_PING								= 44,

	HEADER_GC_SCRIPT							= 45,
	HEADER_GC_QUEST_CONFIRM						= 46,
#ifdef ENABLE_SEND_TARGET_INFO
	HEADER_GC_TARGET_INFO						= 58,
#endif
	HEADER_GC_MOUNT								= 61,
	HEADER_GC_OWNERSHIP							= 62,
	HEADER_GC_TARGET							= 63,


	HEADER_GC_WARP								= 65,
	HEADER_GC_ADD_FLY_TARGETING					= 69,

	HEADER_GC_CREATE_FLY						= 70,
	HEADER_GC_FLY_TARGETING						= 71,
	HEADER_GC_SKILL_LEVEL						= 72,
	HEADER_GC_SKILL_COOLTIME_END				= 73,
	HEADER_GC_MESSENGER							= 74,
	HEADER_GC_GUILD								= 75,
	HEADER_GC_SKILL_LEVEL_NEW					= 76,

	HEADER_GC_PARTY_INVITE						= 77,
	HEADER_GC_PARTY_ADD							= 78,
	HEADER_GC_PARTY_UPDATE						= 79,
	HEADER_GC_PARTY_REMOVE						= 80,

	HEADER_GC_QUEST_INFO						= 81,
	HEADER_GC_REQUEST_MAKE_GUILD				= 82,
	HEADER_GC_PARTY_PARAMETER					= 83,

	HEADER_GC_SAFEBOX_MONEY_CHANGE				= 84,
	HEADER_GC_SAFEBOX_SET						= 85,
	HEADER_GC_SAFEBOX_DEL						= 86,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD			= 87,
	HEADER_GC_SAFEBOX_SIZE						= 88,

	HEADER_GC_FISHING							= 89,

	HEADER_GC_EMPIRE							= 90,

	HEADER_GC_PARTY_LINK						= 91,
	HEADER_GC_PARTY_UNLINK						= 92,

	HEADER_GC_REFINE_INFORMATION				= 95,

	HEADER_GC_OBSERVER_ADD						= 96,
	HEADER_GC_OBSERVER_REMOVE					= 97,
	HEADER_GC_OBSERVER_MOVE						= 98,
	HEADER_GC_VIEW_EQUIP						= 99,

	HEADER_GC_MARK_BLOCK						= 100,
	HEADER_GC_MARK_DIFF_DATA					= 101,
	HEADER_GC_MARK_IDXLIST						= 102,

	//HEADER_GC_SLOW_TIMER						= 105,
    HEADER_GC_TIME								= 106,
    HEADER_GC_CHANGE_NAME						= 107,

	HEADER_GC_DUNGEON							= 110,
	HEADER_GC_WALK_MODE							= 111,
	HEADER_GC_CHANGE_SKILL_GROUP				= 112,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER2_EMPIRE			= 113,
	// END_OF_SUPPORT_BGM

	HEADER_GC_SEPCIAL_EFFECT					= 114,
	HEADER_GC_NPC_POSITION						= 115,

	HEADER_GC_CHINA_MATRIX_CARD					= 116,
	HEADER_GC_CHARACTER_UPDATE2					= 117,
	HEADER_GC_LOGIN_KEY							= 118,
	HEADER_GC_REFINE_INFORMATION_NEW			= 119,
	HEADER_GC_CHARACTER_ADD2					= 120,
	HEADER_GC_CHANNEL							= 121,

	HEADER_GC_MALL_OPEN							= 122,
	HEADER_GC_TARGET_UPDATE						= 123,
	HEADER_GC_TARGET_DELETE						= 124,
	HEADER_GC_TARGET_CREATE_NEW					= 125,

	HEADER_GC_AFFECT_ADD						= 126,
	HEADER_GC_AFFECT_REMOVE						= 127,

	HEADER_GC_MALL_SET							= 128,
	HEADER_GC_MALL_DEL							= 129,
	HEADER_GC_LAND_LIST							= 130,
	HEADER_GC_LOVER_INFO						= 131,
	HEADER_GC_LOVE_POINT_UPDATE					= 132,
	HEADER_GC_GUILD_SYMBOL_DATA					= 133,
	HEADER_GC_DIG_MOTION						= 134,

	HEADER_GC_DAMAGE_INFO						= 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO				= 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM				= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL			= 138,
	// END_OF_SUPPORT_BGM

#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
	HEADER_CG_DROP_ITEM_SYSTEM					= 148,
#endif

	HEADER_GC_AUTH_SUCCESS						= 150,
#ifdef ENABLE_SWITCHBOT
	HEADER_GC_SWITCHBOT							= 171,
#endif
	HEADER_GC_PANAMA_PACK						= 151,

	//HYBRID CRYPT
	HEADER_GC_HYBRIDCRYPT_KEYS					= 152,
	HEADER_GC_HYBRIDCRYPT_SDB					= 153, // SDB means Supplmentary Data Blocks
	//HYBRID CRYPT

	HEADER_GC_AUTH_SUCCESS_OPENID				= 154,

#ifdef ENABLE_EVENT_MANAGER
	HEADER_GC_EVENT_INFO						= 155,
	HEADER_GC_EVENT_RELOAD						= 156,
	HEADER_GC_EVENT_KW_SCORE					= 157,
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	HEADER_GC_PET								= 158,
	HEADER_GC_PET_SET							= 159,
	HEADER_GC_PET_SET_EXCHANGE					= 160,
	HEADER_GC_PET_DEL							= 161,
	HEADER_GC_PET_SUMMON						= 162,
	HEADER_GC_PET_POINT_CHANGE					= 163,
	HEADER_GC_PET_NAME_CHANGE_RESULT			= 164,
	HEADER_GC_PET_DETERMINE_RESULT				= 165,
	HEADER_GC_PET_ATTR_CHANGE_RESULT			= 166,
	HEADER_GC_PET_SKILL_UPDATE					= 167,
	HEADER_GC_PET_SKILL_COOLTIME				= 168,
#endif
#ifdef ENABLE_EXTENDED_WHISPER_DETAILS
	HEADER_GC_WHISPER_DETAILS = 169,
#endif
#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
	HEADER_GC_DROP_ITEM_SYSTEM					= 170,
#endif
	HEADER_GC_RUNUP_MATRIX_QUIZ					= 201,
	HEADER_GC_NEWCIBN_PASSPOD_REQUEST			= 202,
	HEADER_GC_NEWCIBN_PASSPOD_FAILURE			= 203,

	HEADER_GC_SPECIFIC_EFFECT					= 208,
	HEADER_GC_DRAGON_SOUL_REFINE				= 209,
	HEADER_GC_RESPOND_CHANNELSTATUS				= 210,
#ifdef ENABLE_DS_SET
	HEADER_GC_DS_TABLE							= 212,
#endif
	// @fixme007
	HEADER_GC_UNK_213							= 213,
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	HEADER_GC_REQUEST_CHANGE_LANGUAGE = 216,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_GC_CUBE_RENEWAL 						= 217,
#endif
#ifdef ENABLE_AURA_SYSTEM
		HEADER_GC_AURA = 218,
#endif
#ifdef ENABLE_REFINE_ELEMENT
	HEADER_GC_REFINE_ELEMENT 					= 172,
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	HEADER_GC_67_BONUS_NEW 						= 220,
#endif
#ifdef ENABLE_ATLAS_BOSS
	HEADER_GC_BOSS_POSITION = 222,
#endif
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
	HEADER_GC_PRIVATE_SHOP_SEARCH = 221,
	HEADER_GC_PRIVATE_SHOP_SEARCH_OPEN = 222,
#endif
#if defined(__BL_KILL_BAR__)
	HEADER_GC_KILLBAR 							= 224,
#endif
#ifdef ENABLE_EXTENDED_BATTLE_PASS
	HEADER_GC_EXT_BATTLE_PASS_OPEN 				= 235,
	HEADER_GC_EXT_BATTLE_PASS_GENERAL_INFO 		= 236,
	HEADER_GC_EXT_BATTLE_PASS_MISSION_INFO 		= 237,
	HEADER_GC_EXT_BATTLE_PASS_MISSION_UPDATE 	= 238,
	HEADER_GC_EXT_BATTLE_PASS_SEND_RANKING 		= 239,
#endif
	HEADER_GC_KEY_AGREEMENT_COMPLETED			= 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT						= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_HANDSHAKE_OK						= 0xfc, // 252
	HEADER_GC_PHASE								= 0xfd,	// 253
    HEADER_GC_BINDUDP                           = 0xfe, // 254
    HEADER_GC_HANDSHAKE                         = 0xff, // 255
	/////////////////////////////////////////////////
	// Client To Client for UDP
	/*
	HEADER_CC_STATE_WAITING						= 1,
	HEADER_CC_STATE_WALKING						= 2,
	HEADER_CC_STATE_GOING						= 3,
	HEADER_CC_EVENT_NORMAL_ATTACKING			= 4,
	HEADER_CC_EVENT_COMBO_ATTACKING				= 5,
	HEADER_CC_EVENT_HIT							= 6,
	*/
};

enum
{
	ID_MAX_NUM = 30,
	PASS_MAX_NUM = 16,
	CHAT_MAX_NUM = 128,
	PATH_NODE_MAX_NUM = 64,
	SHOP_SIGN_MAX_LEN = 32,

	PLAYER_PER_ACCOUNT3 = 3,
#ifndef ENABLE_PLAYER_PER_ACCOUNT5
	PLAYER_PER_ACCOUNT4 = 4,
#else
	PLAYER_PER_ACCOUNT4 = 5,
	PLAYER_PER_ACCOUNT5 = 5,
#endif

	PLAYER_ITEM_SLOT_MAX_NUM = 20,

	QUICKSLOT_MAX_LINE = 4,
	QUICKSLOT_MAX_COUNT_PER_LINE = 8,
	QUICKSLOT_MAX_COUNT = QUICKSLOT_MAX_LINE * QUICKSLOT_MAX_COUNT_PER_LINE,

	QUICKSLOT_MAX_NUM = 36,

	SHOP_HOST_ITEM_MAX_NUM = 100,

	METIN_SOCKET_COUNT = 6,

	PARTY_AFFECT_SLOT_MAX_NUM = 7,

	GUILD_GRADE_NAME_MAX_LEN = 8,
	GUILD_NAME_MAX_LEN = 12,
	GUILD_GRADE_COUNT = 15,
	GULID_COMMENT_MAX_LEN = 50,

	MARK_CRC_NUM = 8*8,
	MARK_DATA_SIZE = 16*12,
	SYMBOL_DATA_SIZE = 128*256,
	QUEST_INPUT_STRING_MAX_NUM = 64,
	QUEST_NAME_MAX_NUM = 64,

	PRIVATE_CODE_LENGTH = 8,

	REFINE_MATERIAL_MAX_NUM = 5,

	CHINA_MATRIX_ANSWER_MAX_LEN	= 8,

	RUNUP_MATRIX_QUIZ_MAX_LEN	= 8,
	RUNUP_MATRIX_ANSWER_MAX_LEN = 4,
	NEWCIBN_PASSPOD_ANSWER_MAX_LEN = 8,
	NEWCIBN_PASSPOD_FAILURE_MAX_LEN = 128,

	WEAR_MAX_NUM = CItemData::WEAR_MAX_NUM,

	OPENID_AUTHKEY_LEN = 32,

	SHOP_TAB_NAME_MAX = 32,
	SHOP_TAB_COUNT_MAX = 3,
};

#pragma pack(push)
#pragma pack(1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mark
typedef struct command_mark_login
{
    BYTE    header;
    DWORD   handle;
    DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
    BYTE    header;
    DWORD   gid;
    BYTE    image[16*12*4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
    BYTE    header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
    BYTE    header;
    BYTE    imgIdx;
    DWORD   crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
    BYTE    header;
	DWORD	bufSize;
    WORD    count;
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
    BYTE    header;
    DWORD   bufSize;
	BYTE	imgIdx;
    DWORD   count;
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	handle;
} TPacketCGSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE	header;
	DWORD	dwGuildID;
	DWORD	dwCRC;
	DWORD	dwSize;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
    BYTE header;
    WORD size;
    DWORD guild_id;
} TPacketGCGuildSymbolData;

//
//
//
typedef struct packet_observer_add
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverAdd;

typedef struct packet_observer_move
{
	BYTE	header;
	DWORD	vid;
	WORD	x;
	WORD	y;
} TPacketGCObserverMove;


typedef struct packet_observer_remove
{
	BYTE	header;
	DWORD	vid;
} TPacketGCObserverRemove;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// To Server

typedef struct command_checkin
{
	BYTE header;
	char name[ID_MAX_NUM+1];
	char pwd[PASS_MAX_NUM+1];
} TPacketCGCheckin;

typedef struct command_login
{
    BYTE header;
    char name[ID_MAX_NUM + 1];
    char pwd[PASS_MAX_NUM + 1];
} TPacketCGLogin;

typedef struct command_login2
{
	BYTE	header;
	char	name[ID_MAX_NUM + 1];
	DWORD	login_key;
    DWORD	adwClientKey[4];
} TPacketCGLogin2;

typedef struct command_login3
{
    BYTE	header;
    char	name[ID_MAX_NUM + 1];
    char	pwd[PASS_MAX_NUM + 1];
    DWORD	adwClientKey[4];
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	DWORD bLanguage;
#endif
} TPacketCGLogin3;

typedef struct command_login5
{
    BYTE	header;
    char	authKey[OPENID_AUTHKEY_LEN + 1];
    DWORD	adwClientKey[4];
} TPacketCGLogin5;

#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
enum
{
	DROP_ITEM_SUB_HEADER_OPEN,
	DROP_ITEM_SUB_HEADER_SEARCH,
	DROP_ITEM_SUB_HEADER_OPEN_RECEIVE,
	DROP_ITEM_SUB_HEADER_DATES_RECEIVE,
	DROP_ITEM_SUB_HEADER_DATES_CLEAR_RECEIVE,
	DROP_ITEM_SUB_HEADER_DATES_LOADING,
};

typedef struct packet_send_drop_item_system
{
	uint8_t header;
	uint8_t subheader;
	int vnum_item;

}TPacketCGDropItemSend;

typedef struct info_drop_item_system
{
	char name_mob[100];
	uint32_t id_mob;
	uint32_t prob_item;
	int count_item;
	
}TInfoDropItemSystem;

typedef struct packet_receive_drop_item_system
{
	packet_receive_drop_item_system(): header(HEADER_GC_DROP_ITEM_SYSTEM)
	{}
	uint8_t header;
	uint8_t subheader;
	TInfoDropItemSystem info_drop;

}TPacketGCDropItemReceive;
#endif

typedef struct command_direct_enter
{
    BYTE        bHeader;
    char        login[ID_MAX_NUM + 1];
    char        passwd[PASS_MAX_NUM + 1];
    BYTE        index;
} TPacketCGDirectEnter;

typedef struct command_player_select
{
	BYTE	header;
	BYTE	player_index;
} TPacketCGSelectCharacter;

typedef struct command_attack
{
	BYTE	header;
	BYTE	bType;
	DWORD	dwVictimVID;
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

typedef struct command_chat
{
	BYTE	header;
	WORD	length;
	BYTE	type;
} TPacketCGChat;

typedef struct command_whisper
{
    BYTE        bHeader;
    WORD        wSize;
    char        szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_sms
{
    BYTE        bHeader;
    WORD        wSize;
    char        szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGSMS;

enum EBattleMode
{
	BATTLEMODE_ATTACK = 0,
	BATTLEMODE_DEFENSE = 1,
};

typedef struct command_EnterFrontGame
{
	BYTE header;
} TPacketCGEnterFrontGame;

typedef struct command_item_use
{
	BYTE header;
	TItemPos pos;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE header;
	TItemPos source_pos;
	TItemPos target_pos;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE  header;
	TItemPos pos;
	long long elk;
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD cheque;
#endif 
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
    BYTE        header;
    TItemPos pos;
    long long       gold;
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD		cheque;
#endif 
    BYTE        count;
} TPacketCGItemDrop2;

#ifdef ENABLE_NEW_DROP_DIALOG
typedef struct command_item_destroy
{
	BYTE header;
	TItemPos pos;
} TPacketCGItemDestroy;
#endif

typedef struct command_item_move
{
	BYTE header;
	TItemPos pos;
	TItemPos change_pos;
	BYTE num;
} TPacketCGItemMove;

typedef struct command_item_pickup
{
	BYTE header;
	DWORD vid;
} TPacketCGItemPickUp;

typedef struct command_quickslot_add
{
    BYTE        header;
    BYTE        pos;
	TQuickSlot	slot;
}TPacketCGQuickSlotAdd;

typedef struct command_quickslot_del
{
    BYTE        header;
    BYTE        pos;
}TPacketCGQuickSlotDel;

typedef struct command_quickslot_swap
{
    BYTE        header;
    BYTE        pos;
    BYTE        change_pos;
}TPacketCGQuickSlotSwap;

typedef struct command_on_click
{
	BYTE		header;
	DWORD		vid;
} TPacketCGOnClick;


enum
{
    SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
};

typedef struct command_shop
{
	BYTE        header;
	BYTE		subheader;
} TPacketCGShop;

enum
{
	EXCHANGE_SUBHEADER_CG_START,			// arg1 == vid of target character
	EXCHANGE_SUBHEADER_CG_ITEM_ADD,		// arg1 == position of item
	EXCHANGE_SUBHEADER_CG_ITEM_DEL,		// arg1 == position of item
	EXCHANGE_SUBHEADER_CG_ELK_ADD,			// arg1 == amount of elk
	EXCHANGE_SUBHEADER_CG_ACCEPT,			// arg1 == not used
	EXCHANGE_SUBHEADER_CG_CANCEL,			// arg1 == not used
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD,
#endif 
};

typedef struct command_exchange
{
	BYTE		header;
	BYTE		subheader;
	long long		arg1;
	BYTE		arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
    BYTE        header;
    BYTE        position;
} TPacketCGPosition;

typedef struct command_script_answer
{
    BYTE        header;
	BYTE		answer;
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
    BYTE        header;
	unsigned int			idx;
} TPacketCGScriptButton;

typedef struct command_target
{
    BYTE        header;
    DWORD       dwVID;
} TPacketCGTarget;

typedef struct command_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	LONG		lX;
	LONG		lY;
	DWORD		dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
    DWORD       dwVID;
    long        lX;
    long        lY;
} TPacketCGSyncPositionElement;

typedef struct command_sync_position
{
    BYTE        bHeader;
	WORD		wSize;
} TPacketCGSyncPosition;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
    BYTE        bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		lX;
	long		lY;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
    BYTE		bHeader;
    BYTE		bType;
} TPacketCGShoot;

typedef struct command_warp
{
	BYTE			bHeader;
} TPacketCGWarp;

enum
{
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
	MESSENGER_SUBHEADER_GC_MOBILE,
};

typedef struct packet_messenger
{
    BYTE header;
    WORD size;
    BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_list_offline
{
    BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

enum
{
	MESSENGER_CONNECTED_STATE_OFFLINE,
	MESSENGER_CONNECTED_STATE_ONLINE,
	MESSENGER_CONNECTED_STATE_MOBILE,
};

typedef struct packet_messenger_list_online
{
    BYTE connected;
	BYTE length;
	//BYTE length_char_name;
} TPacketGCMessengerListOnline;

typedef struct packet_messenger_login
{
	//BYTE length_login;
	//BYTE length_char_name;
	BYTE length;
} TPacketGCMessengerLogin;

typedef struct packet_messenger_logout
{
	BYTE length;
} TPacketGCMessengerLogout;

enum
{
    MESSENGER_SUBHEADER_CG_ADD_BY_VID,
    MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
    MESSENGER_SUBHEADER_CG_REMOVE,
};

typedef struct command_messenger
{
    BYTE header;
    BYTE subheader;
} TPacketCGMessenger;

typedef struct command_messenger_remove
{
	BYTE length;
} TPacketCGMessengerRemove;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
    BYTE        bHeader;
    BYTE        bState;
    DWORD       dwMoney;
} TPacketCGSafeboxMoney;

typedef struct command_safebox_checkout
{
    BYTE        bHeader;
    BYTE        bSafePos;
    TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
    BYTE        bHeader;
    BYTE        bSafePos;
    TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

typedef struct command_mall_checkout
{
    BYTE        bHeader;
    BYTE        bMallPos;
    TItemPos	ItemPos;
} TPacketCGMallCheckout;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_use_skill
{
    BYTE                bHeader;
    DWORD               dwVnum;
	DWORD				dwTargetVID;
} TPacketCGUseSkill;

typedef struct command_party_invite
{
    BYTE header;
    DWORD vid;
} TPacketCGPartyInvite;

typedef struct command_party_invite_answer
{
    BYTE header;
    DWORD leader_pid;
    BYTE accept;
} TPacketCGPartyInviteAnswer;

typedef struct command_party_remove
{
    BYTE header;
    DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
    BYTE byHeader;
    DWORD dwVID;
	BYTE byState;
    BYTE byFlag;
} TPacketCGPartySetState;

typedef struct packet_party_link
{
    BYTE header;
    DWORD pid;
    DWORD vid;
} TPacketGCPartyLink;

typedef struct packet_party_unlink
{
    BYTE header;
    DWORD pid;
	DWORD vid;
} TPacketGCPartyUnlink;

typedef struct command_party_use_skill
{
    BYTE byHeader;
	BYTE bySkillIndex;
    DWORD dwTargetVID;
} TPacketCGPartyUseSkill;

enum
{
	GUILD_SUBHEADER_CG_ADD_MEMBER,
	GUILD_SUBHEADER_CG_REMOVE_MEMBER,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY,
	GUILD_SUBHEADER_CG_OFFER,
	GUILD_SUBHEADER_CG_POST_COMMENT,
	GUILD_SUBHEADER_CG_DELETE_COMMENT,
	GUILD_SUBHEADER_CG_REFRESH_COMMENT,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_CG_USE_SKILL,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER,
	GUILD_SUBHEADER_CG_CHARGE_GSP,
	GUILD_SUBHEADER_CG_DEPOSIT_MONEY,
	GUILD_SUBHEADER_CG_WITHDRAW_MONEY,
};

typedef struct command_guild
{
    BYTE byHeader;
	BYTE bySubHeader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild;

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	BYTE byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
	BYTE		bHeader;
	char		szBuf[255 + 1];
} TPacketCGHack;

typedef struct command_dungeon
{
	BYTE		bHeader;
	WORD		size;
} TPacketCGDungeon;

#include "Locale_inc.h"
// Private Shop
typedef struct SShopItemTable
{
	DWORD		vnum;
	BYTE		count;

	TItemPos	pos;
#ifdef ENABLE_FULL_YANG
	long long	price;
#else
	DWORD		price;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
	DWORD		cheque;
#endif 
	BYTE		display_pos;
#ifdef ENABLE_BUY_ITEMS_WORLDARD
	DWORD 		item_vnum_buy;
	BYTE 		item_count_buy;
#endif
} TShopItemTable;

typedef struct SPacketCGMyShop
{
	BYTE		bHeader;
	char		szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE		bCount;	// count of TShopItemTable, max 39
#ifdef ENABLE_OFFLINE_SHOP
	BYTE		days;
#endif
} TPacketCGMyShop;

typedef struct SPacketCGRefine
{
	BYTE		header;
	BYTE		pos;
	BYTE		type;
} TPacketCGRefine;

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

typedef struct command_client_version
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion;

typedef struct command_client_version2
{
	BYTE header;
	char filename[32+1];
	char timestamp[32+1];
} TPacketCGClientVersion2;

typedef struct command_crc_report
{
	BYTE header;
	BYTE byPackMode;
	DWORD dwBinaryCRC32;
	DWORD dwProcessCRC32;
	DWORD dwRootPackCRC32;
} TPacketCGCRCReport;

typedef struct command_china_matrix_card
{
	BYTE	bHeader;
	char	szAnswer[CHINA_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGChinaMatrixCard;

typedef struct command_runup_matrix_answer
{
	BYTE	bHeader;
	char	szAnswer[RUNUP_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGRunupMatrixAnswer;

typedef struct command_newcibn_passpod_answer
{
	BYTE	bHeader;
	char	szAnswer[NEWCIBN_PASSPOD_ANSWER_MAX_LEN + 1];
} TPacketCGNEWCIBNPasspodAnswer;

enum EPartyExpDistributionType
{
    PARTY_EXP_DISTRIBUTION_NON_PARITY,
    PARTY_EXP_DISTRIBUTION_PARITY,
};

typedef struct command_party_parameter
{
    BYTE        bHeader;
    BYTE        bDistributeMode;
} TPacketCGPartyParameter;

typedef struct command_quest_input_string
{
    BYTE        bHeader;
    char		szString[QUEST_INPUT_STRING_MAX_NUM+1];
} TPacketCGQuestInputString;

typedef struct command_quest_confirm
{
    BYTE header;
    BYTE answer;
    DWORD requestPID;
} TPacketCGQuestConfirm;

typedef struct command_script_select_item
{
    BYTE header;
    DWORD selection;
} TPacketCGScriptSelectItem;

#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
typedef struct command_privateshop_searchcg
{
	BYTE 	header;
	BYTE 	bJob;
	BYTE 	bMaskType;
	int 	iMaskSub;
	int 	iMinRefine;
	int 	iMaxRefine;
	int 	iMinLevel;
	int 	iMaxLevel;
	int 	iMinGold;
	int 	iMaxGold;
	char	szItemName[CItemData::ITEM_NAME_MAX_LEN + 1];
#if defined(ENABLE_CHEQUE_SYSTEM)
	int 	iMinCheque;
	int 	iMaxCheque;
#endif
} TPacketCGPrivateShopSearch;

typedef struct command_privateshopsearch_item
{
	packet_shop_item item;
	char	szSellerName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwShopPID;
} TPacketGCPrivateShopSearchItem;

typedef struct command_privateshop_searchgc
{
	BYTE		header;
	WORD		size;
} TPacketGCPrivateShopSearch;

typedef struct command_privateshop_searchopengc
{
	BYTE		header;
} TPacketGCPrivateShopSearchOpen;

typedef struct command_privateshop_closecg
{
	BYTE		header;
} TPacketCGPrivateShopSearchClose;
typedef struct command_privateshop_buy_item
{
	BYTE		header;
	BYTE		pos;
	DWORD		dwShopPID;
} TPacketCGPrivateShopSearchBuyItem;
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// From Server
enum EPhase
{
    PHASE_CLOSE,
    PHASE_HANDSHAKE,
    PHASE_LOGIN,
    PHASE_SELECT,
    PHASE_LOADING,
    PHASE_GAME,
    PHASE_DEAD,

	PHASE_DBCLIENT_CONNECTING,
    PHASE_DBCLIENT,
    PHASE_P2P,
    PHASE_AUTH,
};

typedef struct packet_phase
{
    BYTE        header;
    BYTE        phase;
} TPacketGCPhase;

typedef struct packet_blank
{
	BYTE		header;
} TPacketGCBlank;

typedef struct packet_blank_dynamic
{
	BYTE		header;
	WORD		size;
} TPacketGCBlankDynamic;

typedef struct packet_header_handshake
{
	BYTE		header;
	DWORD		dwHandshake;
	DWORD		dwTime;
	LONG		lDelta;
} TPacketGCHandshake;

typedef struct packet_header_bindudp
{
	BYTE		header;
	DWORD		addr;
	WORD		port;
} TPacketGCBindUDP;

typedef struct packet_header_dynamic_size
{
	BYTE		header;
	WORD		size;
} TDynamicSizePacketHeader;

typedef struct SSimplePlayerInformation
{
    DWORD               dwID;
    char                szName[CHARACTER_NAME_MAX_LEN + 1];
    BYTE                byJob;
    BYTE                byLevel;
    DWORD               dwPlayMinutes;
    BYTE                byST, byHT, byDX, byIQ;
//	WORD				wParts[CRaceData::PART_MAX_NUM];
    WORD                wMainPart;
    BYTE                bChangeName;
	WORD				wHairPart;
#ifdef ENABLE_ACCE_SYSTEM
	WORD				wAccePart;
#endif
    BYTE                bDummy[4];
	long				x, y;
	LONG				lAddr;
	WORD				wPort;
	BYTE				bySkillGroup;
#if defined(__BL_LOADING_TIP__)
	long				lMapIndex;
#endif
#if defined(WJ_SORT_LASTPLAYTIME)
	DWORD dwLastPlayTime;
#endif
} TSimplePlayerInformation;

typedef struct packet_login_success3
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT3];
    DWORD						guild_id[PLAYER_PER_ACCOUNT3];
    char						guild_name[PLAYER_PER_ACCOUNT3][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess3;

typedef struct packet_login_success4
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT4];
    DWORD						guild_id[PLAYER_PER_ACCOUNT4];
    char						guild_name[PLAYER_PER_ACCOUNT4][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess4;


#ifdef ENABLE_PLAYER_PER_ACCOUNT5
typedef struct packet_login_success5
{
	BYTE						header;
	TSimplePlayerInformation	akSimplePlayerInformation[PLAYER_PER_ACCOUNT5];
    DWORD						guild_id[PLAYER_PER_ACCOUNT5];
    char						guild_name[PLAYER_PER_ACCOUNT5][GUILD_NAME_MAX_LEN+1];
	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess5;
#endif

enum { LOGIN_STATUS_MAX_LEN = 8 };
typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[LOGIN_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE		shape;
	BYTE		CON;
	BYTE		INT;
	BYTE		STR;
	BYTE		DEX;
} TPacketCGCreateCharacter;

typedef struct command_player_create_success
{
    BYTE						header;
    BYTE						bAccountCharacterSlot;
    TSimplePlayerInformation	kSimplePlayerInfomation;
} TPacketGCPlayerCreateSuccess;

typedef struct command_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

typedef struct command_player_delete
{
	BYTE        header;
	BYTE        index;
	char		szPrivateCode[PRIVATE_CODE_LENGTH];
} TPacketCGDestroyCharacter;

typedef struct packet_player_delete_success
{
	BYTE        header;
	BYTE        account_index;
} TPacketGCDestroyCharacterSuccess;

#if defined(__BL_KILL_BAR__)
typedef struct command_kill_bar
{
	BYTE	bHeader;
	BYTE	bKillerRace;
	BYTE	bKillerWeaponType;
	BYTE	bVictimRace;
	char	szKiller[CHARACTER_NAME_MAX_LEN + 1];
	char	szVictim[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCKillBar;
#endif

enum
{
	ADD_CHARACTER_STATE_DEAD   = (1 << 0),
	ADD_CHARACTER_STATE_SPAWN  = (1 << 1),
	ADD_CHARACTER_STATE_GUNGON = (1 << 2),
	ADD_CHARACTER_STATE_KILLER = (1 << 3),
	ADD_CHARACTER_STATE_PARTY  = (1 << 4),
};

enum EPKModes
{
	PK_MODE_PEACE,
	PK_MODE_REVENGE,
	PK_MODE_FREE,
	PK_MODE_PROTECT,
	PK_MODE_GUILD,
	PK_MODE_MAX_NUM,
};

enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,

#ifdef ENABLE_ACCE_SYSTEM
	CHR_EQUIPPART_ACCE,
#endif
#ifdef ENABLE_AURA_SYSTEM
	CHR_EQUIPPART_AURA,
#endif
	CHR_EQUIPPART_NUM,
};

typedef struct packet_char_additional_info
{
	BYTE    header;
	DWORD   dwVID;
	char    name[CHARACTER_NAME_MAX_LEN + 1];
	WORD    awPart[CHR_EQUIPPART_NUM];
	BYTE	bEmpire;
	DWORD   dwGuildID;
	DWORD   dwLevel;
	short   sAlignment;
	BYTE    bPKMode;
	DWORD   dwMountVnum;
#ifdef ENABLE_REFINE_ELEMENT
	BYTE	bRefineElementType;
#endif
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	DWORD	dwSkillColor[ESkillColorLength::MAX_SKILL_COUNT + MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
#endif
#ifdef ENABLE_GROWTH_PET_SYSTEM
	BYTE	bCharacterSize;
#endif
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	BYTE bLanguage;
#endif
} TPacketGCCharacterAdditionalInfo;

typedef struct packet_add_char
{
    BYTE        header;

    DWORD       dwVID;

    //char        name[CHARACTER_NAME_MAX_LEN + 1];

    float       angle;
    long        x;
    long        y;
    long        z;

	BYTE		bType;
    WORD        wRaceNum;
    //WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
    BYTE        bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];        // ??
    //BYTE      bEmpire;
    //DWORD     dwGuild;
    //short     sAlignment;
	//BYTE		bPKMode;
	//DWORD		dwMountVnum;
} TPacketGCCharacterAdd;

typedef struct packet_add_char2
{
    BYTE        header;

    DWORD       dwVID;

    char        name[CHARACTER_NAME_MAX_LEN + 1];

    float       angle;
    long        x;
    long        y;
    long        z;

	BYTE		bType;
    WORD        wRaceNum;
    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
    BYTE        bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];        // ??
    BYTE        bEmpire;

    DWORD       dwGuild;
    short       sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
} TPacketGCCharacterAdd2;

typedef struct packet_update_char
{
    BYTE        header;
    DWORD       dwVID;

    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
	BYTE		bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];

	DWORD		dwGuildID;
    short       sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
#ifdef ENABLE_REFINE_ELEMENT
	BYTE		bRefineElementType;
#endif
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	DWORD		dwSkillColor[ESkillColorLength::MAX_SKILL_COUNT + MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
#endif
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	BYTE bLanguage;
#endif
} TPacketGCCharacterUpdate;

typedef struct packet_update_char2
{
    BYTE        header;
    DWORD       dwVID;

    WORD        awPart[CHR_EQUIPPART_NUM];
    BYTE        bMovingSpeed;
	BYTE		bAttackSpeed;

    BYTE        bStateFlag;
    DWORD       dwAffectFlag[2];

	DWORD		dwGuildID;
    short       sAlignment;
	BYTE		bPKMode;
	DWORD		dwMountVnum;
#ifdef ENABLE_REFINE_ELEMENT
	BYTE		bRefineElementType;
#endif
} TPacketGCCharacterUpdate2;

typedef struct packet_del_char
{
	BYTE	header;
    DWORD	dwVID;
} TPacketGCCharacterDelete;

typedef struct packet_GlobalTime
{
	BYTE	header;
	float	GlobalTime;
} TPacketGCGlobalTime;

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
	CHAT_TYPE_MAX_NUM,
};

typedef struct packet_chatting
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	dwVID;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper
{
    BYTE        bHeader;
    WORD        wSize;
    BYTE        bType;
    char        szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCWhisper;

typedef struct packet_stun
{
	BYTE		header;
	DWORD		vid;
} TPacketGCStun;

typedef struct packet_dead
{
	BYTE		header;
	DWORD		vid;
} TPacketGCDead;

typedef struct packet_main_character
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character2_empire
{
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter2_EMPIRE;

typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_MAX_LEN = 24,
	};
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char        szBGMName[MUSIC_NAME_MAX_LEN + 1];
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_MAX_LEN = 24,
	};
    BYTE        header;
    DWORD       dwVID;
	WORD		wRaceNum;
    char        szUserName[CHARACTER_NAME_MAX_LEN + 1];
	char        szBGMName[MUSIC_NAME_MAX_LEN + 1];
	float		fBGMVol;
    long        lX, lY, lZ;
	BYTE		byEmpire;
	BYTE		bySkillGroup;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM


enum EPointTypes
{
    POINT_NONE,                 // 0
    POINT_LEVEL,                // 1
    POINT_VOICE,                // 2
    POINT_EXP,                  // 3
    POINT_NEXT_EXP,             // 4
    POINT_HP,                   // 5
    POINT_MAX_HP,               // 6
    POINT_SP,                   // 7
    POINT_MAX_SP,               // 8
    POINT_STAMINA,              // 9  ¨ö¨¬A¡¿©öI©øE
    POINT_MAX_STAMINA,          // 10 AO¢¥e ¨ö¨¬A¡¿©öI©øE

    POINT_GOLD,                 // 11
    POINT_ST,                   // 12 ¡¾U¡¤A
    POINT_HT,                   // 13 A¨ù¡¤A
    POINT_DX,                   // 14 ©öIA¢¬¨ù¨¬
    POINT_IQ,                   // 15 A¢´¨öA¡¤A
    POINT_ATT_POWER,            // 16 ¡Æ©ª¡ÆY¡¤A
    POINT_ATT_SPEED,            // 17 ¡Æ©ª¡ÆY¨ùO¥ì¥ì
    POINT_EVADE_RATE,           // 18 E¢¬CCA©÷
    POINT_MOV_SPEED,            // 19 AI¥ì¢¯¨ùO¥ì¥ì
    POINT_DEF_GRADE,            // 20 ©ö©¡¨úi¥ìi¡¾¨­
	POINT_CASTING_SPEED,        // 21 AO©ö¢ç¨ùO¥ì¥ì (A©£¢¥U¢¯iA¢¬AO*100) / (100 + AI¡Æ¨£) = AOA¨ú A©£¢¥U¢¯i A¢¬AO
	POINT_MAGIC_ATT_GRADE,      // 22 ¢¬¢Ò©öy¡Æ©ª¡ÆY¡¤A
    POINT_MAGIC_DEF_GRADE,      // 23 ¢¬¢Ò©öy©ö©¡¨úi¡¤A
    POINT_EMPIRE_POINT,         // 24 A|¡¾©öA¢®¨ùo
    POINT_LEVEL_STEP,           // 25 CN ¡¤©ö¨¬¡×¢¯¢®¨ù¡©AC ¢¥U¡Æe.. (1 2 3 ¥ìE ¢Ò¡× ¨¬¢¬¡ío, 4 ¥ìC¢¬e ¡¤©ö¨¬¡× ¨ú¡À)
    POINT_STAT,                 // 26 ¢¥E¡¤AA¢® ¢¯A¢¬¡¾ ¨ùo AO¢¥A ¡Æ©ø¨ùo
	POINT_SUB_SKILL,            // 27 ¨¬¢¬A¢Ò ¨ö¨¬A©ø ¨¡¡ÀAI¨¡¢ç
	POINT_SKILL,                // 28 ¨ú¡¿¨¡¨ù¨¬e ¨ö¨¬A©ø ¨¡¡ÀAI¨¡¢ç
//    POINT_SKILL_PASV,           // 27 ¨¡¨¢¨öA¨¬e ¡¾a¨ùu ¢¯A¢¬¡¾ ¨ùo AO¢¥A ¡Æ©ø¨ùo
//    POINT_SKILL_ACTIVE,         // 28 ¨ú¡¿¨¡¨ù¨¬e ¨ö¨¬A©ø ¨¡¡ÀAI¨¡¢ç
	POINT_MIN_ATK,				// 29 AO¨ùO ¨¡A¡¾¡ì¡¤A
	POINT_MAX_ATK,				// 30 AO¢¥e ¨¡A¡¾¡ì¡¤A
    POINT_PLAYTIME,             // 31 CA¡¤©öAI¨öA¡Æ¡Ì
    POINT_HP_REGEN,             // 32 HP E¢¬¨¬©ö¡¤u
    POINT_SP_REGEN,             // 33 SP E¢¬¨¬©ö¡¤u

    POINT_BOW_DISTANCE,         // 34 E¡Æ ¡ícA¢´¡ÆA¢¬¢ç Ao¡Æ¢®A¢® (meter)

    POINT_HP_RECOVERY,          // 35 A¨ù¡¤A E¢¬¨¬©ö Ao¡Æ¢®¡¤¢ç
    POINT_SP_RECOVERY,          // 36 A¢´¨öA¡¤A E¢¬¨¬©ö Ao¡Æ¢®¡¤¢ç

    POINT_POISON_PCT,           // 37 ¥ì¢Ò E¢ç¡¤u
    POINT_STUN_PCT,             // 38 ¡¾aAy E¢ç¡¤u
    POINT_SLOW_PCT,             // 39 ¨ö¨ö¡¤I¢¯i E¢ç¡¤u
    POINT_CRITICAL_PCT,         // 40 A¨Ï¢¬¢ç¨¡¨ùAA E¢ç¡¤u
    POINT_PENETRATE_PCT,        // 41 ¡ÆuAeA¢¬¡ÆY E¢ç¡¤u
    POINT_CURSE_PCT,            // 42 AuAO E¢ç¡¤u

    POINT_ATTBONUS_HUMAN,       // 43 AI¡Æ¡Ì¢¯¢®¡ÆO ¡Æ¡©CO
    POINT_ATTBONUS_ANIMAL,      // 44 ¥ì¢¯©ö¡Æ¢¯¢®¡ÆO ¥ì¡Í©öIAo % Ao¡Æ¢®
    POINT_ATTBONUS_ORC,         // 45 ¢¯o¡¾I¢¯¢®¡ÆO ¥ì¡Í©öIAo % Ao¡Æ¢®
    POINT_ATTBONUS_MILGYO,      // 46 ©ö¨¢¡¾©ø¢¯¢®¡ÆO ¥ì¡Í©öIAo % Ao¡Æ¢®
    POINT_ATTBONUS_UNDEAD,      // 47 ¨öAA¨ù¢¯¢®¡ÆO ¥ì¡Í©öIAo % Ao¡Æ¢®
    POINT_ATTBONUS_DEVIL,       // 48 ¢¬¢Ò¡¾I(¨úC¢¬¢Ò)¢¯¢®¡ÆO ¥ì¡Í©öIAo % Ao¡Æ¢®
    POINT_ATTBONUS_INSECT,      // 49 ©öu¡¤©öA¡¤
    POINT_ATTBONUS_FIRE,        // 50 E¡©¢¯¡ÆA¡¤
    POINT_ATTBONUS_ICE,         // 51 ¨¬u¨ù©øA¡¤
    POINT_ATTBONUS_DESERT,      // 52 ¡íc¢¬¡¤A¡¤
	POINT_ATTBONUS_MONSTER,			// 53 ¢¬©£¥ìc ¢¬o¨ö¨¬AI¢¯¢®¡ÆO ¡Æ¡©CO
	POINT_ATTBONUS_WARRIOR,			// 54 ©ö¡ì¡íc¢¯¢®¡ÆO ¡Æ¡©CO
	POINT_ATTBONUS_ASSASSIN,		// 55 AU¡Æ¢¥¢¯¢®¡ÆO ¡Æ¡©CO
	POINT_ATTBONUS_SURA,			// 56 ¨ùo¢Òo¢¯¢®¡ÆO ¡Æ¡©CO
	POINT_ATTBONUS_SHAMAN,			// 57 ©ö¡ì¢¥c¢¯¢®¡ÆO ¡Æ¡©CO
	POINT_ATTBONUS_TREE,			// 58 ©ø¨£©ö¡ì¢¯¢®¡ÆO ¡Æ¡©CO 20050729.myevan UNUSED5
	POINT_RESIST_WARRIOR,			// 59 ©ö¡ì¡íc¢¯¢®¡ÆO AuC¡¿
	POINT_RESIST_ASSASSIN,			// 60 AU¡Æ¢¥¢¯¢®¡ÆO AuC¡¿
	POINT_RESIST_SURA,				// 61 ¨ùo¢Òo¢¯¢®¡ÆO AuC¡¿
	POINT_RESIST_SHAMAN,			// 62 ©ö¡ì¢¥c¢¯¢®¡ÆO AuC¡¿

    POINT_STEAL_HP,             // 63 ¡íy¢¬i¡¤A Ei¨ùo
    POINT_STEAL_SP,             // 64 A¢´¨öA¡¤A Ei¨ùo

    POINT_MANA_BURN_PCT,        // 65 ¢¬¢Ò©ø¨£ ©ö©ª

    /// CCC¨ª¨öA ¨¬¢¬©øE¨ö¨¬ ///

    POINT_DAMAGE_SP_RECOVER,    // 66 ¡Æ©ª¡ÆY¢¥cCO ¨öA A¢´¨öA¡¤A E¢¬¨¬©ö E¢ç¡¤u

    POINT_BLOCK,                // 67 ¨¬i¡¤¡ÆA©÷
    POINT_DODGE,                // 68 E¢¬CCA©÷

    POINT_RESIST_SWORD,         // 69
    POINT_RESIST_TWOHAND,       // 70
    POINT_RESIST_DAGGER,        // 71
    POINT_RESIST_BELL,          // 72
    POINT_RESIST_FAN,           // 73
    POINT_RESIST_BOW,           // 74  E¡©¡íi   AuC¡¿   : ¢¥e©öIAo ¡Æ¡§¨ùO
    POINT_RESIST_FIRE,          // 75  E¡©¢¯¡Æ   AuC¡¿   : E¡©¢¯¡Æ¡Æ©ª¡ÆY¢¯¢® ¢¥eCN ¢¥e©öIAo ¡Æ¡§¨ùO
    POINT_RESIST_ELEC,          // 76  Au¡¾a   AuC¡¿   : Au¡¾a¡Æ©ª¡ÆY¢¯¢® ¢¥eCN ¢¥e©öIAo ¡Æ¡§¨ùO
    POINT_RESIST_MAGIC,         // 77  ¨ùu©öy   AuC¡¿   : ¢¬©£¥ìc¨ùu©öy¢¯¢® ¢¥eCN ¢¥e©öIAo ¡Æ¡§¨ùO
    POINT_RESIST_WIND,          // 78  ©öU¢Ò¡À   AuC¡¿   : ©öU¢Ò¡À¡Æ©ª¡ÆY¢¯¢® ¢¥eCN ¢¥e©öIAo ¡Æ¡§¨ùO

    POINT_REFLECT_MELEE,        // 79 ¡Æ©ª¡ÆY ©öY¡íc

    /// ¨¡?¨ùo CCC¨ª¨öA ///
    POINT_REFLECT_CURSE,        // 80 AuAO ©öY¡íc
    POINT_POISON_REDUCE,        // 81 ¥ì¢Ò¥ì¡Í©öIAo ¡Æ¡§¨ùO

    /// Au ¨ùO¢¬e¨öA ///
    POINT_KILL_SP_RECOVER,      // 82 Au ¨ùO¢¬e¨öA MP E¢¬¨¬©ö
    POINT_EXP_DOUBLE_BONUS,     // 83
    POINT_GOLD_DOUBLE_BONUS,    // 84
    POINT_ITEM_DROP_BONUS,      // 85

    /// E¢¬¨¬©ö ¡Æu¡¤A ///
    POINT_POTION_BONUS,         // 86
    POINT_KILL_HP_RECOVERY,      // 87

    POINT_IMMUNE_STUN,          // 88
    POINT_IMMUNE_SLOW,          // 89
    POINT_IMMUNE_FALL,          // 90
    //////////////////

    POINT_PARTY_ATT_GRADE,      // 91
    POINT_PARTY_DEF_GRADE,      // 92

    POINT_ATT_BONUS,            // 93
    POINT_DEF_BONUS,            // 94

    POINT_ATT_GRADE_BONUS,			// 95
    POINT_DEF_GRADE_BONUS,			// 96
    POINT_MAGIC_ATT_GRADE_BONUS,	// 97
    POINT_MAGIC_DEF_GRADE_BONUS,	// 98

    POINT_RESIST_NORMAL_DAMAGE,		// 99

	POINT_STAT_RESET_COUNT = 112,
    POINT_HORSE_SKILL = 113,

	POINT_MALL_ATTBONUS,		// 114
	POINT_MALL_DEFBONUS,		// 115
	POINT_MALL_EXPBONUS,		// 116
	POINT_MALL_ITEMBONUS,		// 117
	POINT_MALL_GOLDBONUS,		// 118
    POINT_MAX_HP_PCT,			// 119
    POINT_MAX_SP_PCT,			// 120

	POINT_SKILL_DAMAGE_BONUS,       // 121
	POINT_NORMAL_HIT_DAMAGE_BONUS,  // 122

    POINT_SKILL_DEFEND_BONUS,       // 123
    POINT_NORMAL_HIT_DEFEND_BONUS,  // 124
    POINT_PC_BANG_EXP_BONUS,        // 125
	POINT_PC_BANG_DROP_BONUS,       // 126
	POINT_ENERGY = 128,				// 128
	POINT_ENERGY_END_TIME = 129,	// 129
	POINT_COSTUME_ATTR_BONUS = 130,			// 130
	POINT_MAGIC_ATT_BONUS_PER = 131,		// 131
	POINT_MELEE_MAGIC_ATT_BONUS_PER = 132,	// 132

	POINT_RESIST_ICE = 133,				// 133 
	POINT_RESIST_EARTH = 134,			// 134 
	POINT_RESIST_DARK = 135,			// 135 

	POINT_RESIST_CRITICAL = 136,		// 136 
	POINT_RESIST_PENETRATE = 137,		// 137 
#ifdef ENABLE_WOLFMAN_CHARACTER
	POINT_BLEEDING_REDUCE = 138,
	POINT_BLEEDING_PCT = 139,
	POINT_ATTBONUS_WOLFMAN = 140, // 140 ¨ùoAIA¡¤¢¯¢®¡ÆO ¡Æ¡©CO
	POINT_RESIST_WOLFMAN = 141, // 141 ¨ùoAIA¡¤¢¯¢®¡ÆO AuC¡¿
	POINT_RESIST_CLAW = 142, // 142 CLAW¢¯¢® AuC¡¿
#endif
#ifdef ENABLE_ACCE_SYSTEM
	POINT_ACCEDRAIN_RATE = 143,
#endif
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	POINT_RESIST_MAGIC_REDUCTION = 144,
#endif
#ifdef ENABLE_ELEMENT_ADD
	POINT_ENCHANT_ELEC,					// 145
	POINT_ENCHANT_FIRE,					// 146
	POINT_ENCHANT_ICE,					// 147
	POINT_ENCHANT_WIND,					// 148
	POINT_ENCHANT_EARTH,				// 149
	POINT_ENCHANT_DARK,					// 150
	POINT_ATTBONUS_CZ,					// 151
	POINT_RESIST_SWORD_REDUCTION,		// 152
	POINT_RESIST_TWOHAND_REDUCTION,		// 153
	POINT_RESIST_DAGGER_REDUCTION,		// 154
	POINT_RESIST_BELL_REDUCTION,		// 155
	POINT_RESIST_FAN_REDUCTION,			// 156
	POINT_RESIST_BOW_REDUCTION,			// 157
#ifdef ENABLE_WOLFMAN_CHARACTER
	POINT_RESIST_CLAW_REDUCTION,		// 158
#endif
	POINT_RESIST_HUMAN,					// 159
#endif
#ifdef ENABLE_ATTR_METIN_BOSS
	POINT_ATTBONUS_METIN,					// 160
	POINT_ATTBONUS_BOSS,					// 161
#endif
#ifdef ENABLE_GAYA_SYSTEM
	POINT_GAYA = 175,
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
	POINT_CHEQUE = 180,
#endif
	// A¡þ¢ÒoAI¨ú©£¨¡¢ç ¨¡¡ÀAI¨¡¢ç
#ifdef ENABLE_EXTENDED_BATTLE_PASS
	POINT_BATTLE_PASS_PREMIUM_ID = 163,
#endif
	POINT_MIN_WEP = 200,
	POINT_MAX_WEP,
	POINT_MIN_MAGIC_WEP,
	POINT_MAX_MAGIC_WEP,
	POINT_HIT_RATE,


    //POINT_MAX_NUM = 255,=>stdafx.h ¡¤I/
};

typedef struct packet_points
{
    BYTE        header;
    long long        points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_point_change
{
    int         header;

	DWORD		dwVID;
	BYTE		Type;

	long long        amount; // 바뀐 값
    long long        value;  // 현재 값
} TPacketGCPointChange;

typedef struct packet_motion
{
	BYTE		header;
	DWORD		vid;
	DWORD		victim_vid;
	WORD		motion;
} TPacketGCMotion;

typedef struct packet_set_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	BYTE		count;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD		dwRefineElement;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemSet;

typedef struct packet_set_item2
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		vnum;
	BYTE		count;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD		dwRefineElement;
#endif
	DWORD		flags;
	DWORD		anti_flags;
	bool		highlight;
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemSet2;

typedef struct packet_item_del
{
    BYTE        header;
    BYTE        pos;
} TPacketGCItemDel;

typedef struct packet_use_item
{
	BYTE		header;
	TItemPos	Cell;
	DWORD		ch_vid;
	DWORD		victim_vid;

	DWORD		vnum;
} TPacketGCItemUse;

typedef struct packet_update_item
{
	BYTE		header;
	TItemPos	Cell;
	BYTE		count;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD		dwRefineElement;
#endif
	long		alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
    TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TPacketGCItemUpdate;

typedef struct packet_ground_add_item
{
    BYTE        bHeader;
    long        lX;
	long		lY;
	long		lZ;

    DWORD       dwVID;
    DWORD       dwVnum;
} TPacketGCItemGroundAdd;

typedef struct packet_ground_del_item
{
	BYTE		header;
	DWORD		vid;
} TPacketGCItemGroundDel;

typedef struct packet_item_ownership
{
    BYTE        bHeader;
    DWORD       dwVID;
    char        szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_quickslot_add
{
    BYTE        header;
    BYTE        pos;
	TQuickSlot	slot;
} TPacketGCQuickSlotAdd;

typedef struct packet_quickslot_del
{
    BYTE        header;
    BYTE        pos;
} TPacketGCQuickSlotDel;

typedef struct packet_quickslot_swap
{
    BYTE        header;
    BYTE        pos;
    BYTE        change_pos;
} TPacketGCQuickSlotSwap;

typedef struct packet_shop_start
{
	struct packet_shop_item		items[SHOP_HOST_ITEM_MAX_NUM];
} TPacketGCShopStart;

typedef struct packet_shop_start_ex
{
	typedef struct sub_packet_shop_tab
	{
		char name[SHOP_TAB_NAME_MAX];
		BYTE coin_type;
		packet_shop_item items[SHOP_HOST_ITEM_MAX_NUM];
	} TSubPacketShopTab;
	DWORD owner_vid;
	BYTE shop_tab_count;
} TPacketGCShopStartEx;


typedef struct packet_shop_update_item
{
	BYTE						pos;
	struct packet_shop_item		item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	long long iElkAmount;
} TPacketGCShopUpdatePrice;

enum EPacketShopSubHeaders
{
	SHOP_SUBHEADER_GC_START,
    SHOP_SUBHEADER_GC_END,
	SHOP_SUBHEADER_GC_UPDATE_ITEM,
	SHOP_SUBHEADER_GC_UPDATE_PRICE,
    SHOP_SUBHEADER_GC_OK,
    SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY,
    SHOP_SUBHEADER_GC_SOLDOUT,
    SHOP_SUBHEADER_GC_INVENTORY_FULL,
    SHOP_SUBHEADER_GC_INVALID_POS,
	SHOP_SUBHEADER_GC_SOLD_OUT,
	SHOP_SUBHEADER_GC_START_EX,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX,
#ifdef ENABLE_CHEQUE_SYSTEM
	SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_CHEQUE,
#endif 
	SHOP_SUBHEADER_GC_NOT_ENOUGH_COUNT,
};

typedef struct packet_shop
{
	BYTE		header;
	WORD		size;
	BYTE		subheader;
} TPacketGCShop;

typedef struct packet_exchange
{
	BYTE		header;
	BYTE		subheader;
	BYTE		is_me;
	long long       arg1;
	TItemPos		arg2;
	DWORD		arg3;
#ifdef WJ_ENABLE_TRADABLE_ICON
	TItemPos		arg4;
#endif
	long		alValues[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_REFINE_ELEMENT
	DWORD		dwRefineElement;
#endif
} TPacketGCExchange;

enum
{
	EXCHANGE_SUBHEADER_GC_START,			// arg1 == vid
	EXCHANGE_SUBHEADER_GC_ITEM_ADD,		// arg1 == vnum  arg2 == pos  arg3 == count
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,		// arg1 == pos
	EXCHANGE_SUBHEADER_GC_ELK_ADD,			// arg1 == elk
	EXCHANGE_SUBHEADER_GC_ACCEPT,			// arg1 == accept
	EXCHANGE_SUBHEADER_GC_END,				// arg1 == not used
	EXCHANGE_SUBHEADER_GC_ALREADY,			// arg1 == not used
	EXCHANGE_SUBHEADER_GC_LESS_ELK,		// arg1 == not used
#ifdef ENABLE_CHEQUE_SYSTEM
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD,
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE,
#endif 
};

typedef struct packet_position
{
    BYTE        header;
	DWORD		vid;
    BYTE        position;
} TPacketGCPosition;

typedef struct packet_ping
{
	BYTE		header;
} TPacketGCPing;

typedef struct packet_pong
{
	BYTE		bHeader;
} TPacketCGPong;

typedef struct packet_script
{
    BYTE		header;
    WORD        size;
	BYTE		skin;
    WORD        src_size;
} TPacketGCScript;

typedef struct packet_target
{
    BYTE        header;
    DWORD       dwVID;
    BYTE        bHPPercent;
#ifdef ENABLE_VIEW_TARGET_DECIMAL_HP
	int			iMinHP;
	int			iMaxHP;
#endif
#ifdef ENABLE_ELEMENT_ADD
	BYTE	bElement;
#endif
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	char		dwCostumeHideID[255 + 1];
#endif
} TPacketGCTarget;

#ifdef ENABLE_SEND_TARGET_INFO
typedef struct packet_target_info
{
	BYTE	header;
	DWORD	dwVID;
	DWORD	race;
	DWORD	dwVnum;
	BYTE	count;
} TPacketGCTargetInfo;

typedef struct packet_target_info_load
{
	BYTE header;
	DWORD dwVID;
} TPacketCGTargetInfoLoad;
#endif


typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVID;
	BYTE flag;
	int  damage;
} TPacketGCDamageInfo;

typedef struct packet_mount
{
    BYTE        header;
    DWORD       vid;
    DWORD       mount_vid;
    BYTE        pos;
	DWORD		_x, _y;
} TPacketGCMount;

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

typedef struct packet_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	LONG		lX;
	LONG		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;

enum
{
	QUEST_SEND_IS_BEGIN			= 1 << 0,
	QUEST_SEND_TITLE			= 1 << 1,
	QUEST_SEND_CLOCK_NAME		= 1 << 2,
	QUEST_SEND_CLOCK_VALUE		= 1 << 3,
	QUEST_SEND_COUNTER_NAME		= 1 << 4,
	QUEST_SEND_COUNTER_VALUE	= 1 << 5,
	QUEST_SEND_ICON_FILE		= 1 << 6,
};

typedef struct packet_quest_info
{
	BYTE header;
	WORD size;
	WORD index;
#ifdef ENABLE_QUEST_RENEWAL
	WORD c_index;
#endif
	BYTE flag;
} TPacketGCQuestInfo;

typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[64+1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_attack
{
    BYTE        header;
    DWORD       dwVID;
    DWORD       dwVictimVID;
    BYTE        bType;
} TPacketGCAttack;

typedef struct packet_c2c
{
	BYTE		header;
	WORD		wSize;
} TPacketGCC2C;

typedef struct packetd_sync_position_element
{
    DWORD       dwVID;
    long        lX;
    long        lY;
} TPacketGCSyncPositionElement;

typedef struct packetd_sync_position
{
    BYTE        bHeader;
    WORD		wSize;
} TPacketGCSyncPosition;

typedef struct packet_ownership
{
    BYTE                bHeader;
    DWORD               dwOwnerVID;
    DWORD               dwVictimVID;
} TPacketGCOwnership;

#define	SKILL_MAX_NUM 255

typedef struct packet_skill_level
{
    BYTE        bHeader;
    BYTE        abSkillLevels[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct SPlayerSkill
{
	BYTE bMasterType;
	BYTE bLevel;
	time_t tNextRead;
} TPlayerSkill;

typedef struct packet_skill_level_new
{
	BYTE bHeader;
	TPlayerSkill skills[SKILL_MAX_NUM];
} TPacketGCSkillLevelNew;

// fly
typedef struct packet_fly
{
    BYTE        bHeader;
    BYTE        bType;
    DWORD       dwStartVID;
    DWORD       dwEndVID;
} TPacketGCCreateFly;

enum EPVPModes
{
	PVP_MODE_NONE,
    PVP_MODE_AGREE,
    PVP_MODE_FIGHT,
    PVP_MODE_REVENGE,
};

typedef struct packet_duel_start
{
    BYTE	header ;
    WORD	wSize ;
} TPacketGCDuelStart ;

typedef struct packet_pvp
{
	BYTE		header;
	DWORD		dwVIDSrc;
	DWORD		dwVIDDst;
	BYTE		bMode;
} TPacketGCPVP;

typedef struct packet_skill_cooltime_end
{
	BYTE		header;
	BYTE		bSkill;
} TPacketGCSkillCoolTimeEnd;

typedef struct packet_warp
{
	BYTE			bHeader;
	LONG			lX;
	LONG			lY;
	LONG			lAddr;
	WORD			wPort;
#if defined(__BL_LOADING_TIP__)
	long			l_MapIndex;
#endif
} TPacketGCWarp;

typedef struct packet_party_invite
{
    BYTE header;
    DWORD leader_pid;
} TPacketGCPartyInvite;

typedef struct packet_party_add
{
    BYTE header;
    DWORD pid;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCPartyAdd;

typedef struct packet_party_update
{
    BYTE header;
    DWORD pid;
    BYTE state;
    BYTE percent_hp;
    short affects[PARTY_AFFECT_SLOT_MAX_NUM];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
    BYTE header;
    DWORD pid;
} TPacketGCPartyRemove;

typedef TPacketCGSafeboxCheckout TPacketGCSafeboxCheckout;
typedef TPacketCGSafeboxCheckin TPacketGCSafeboxCheckin;

typedef struct packet_safebox_wrong_password
{
    BYTE        bHeader;
} TPacketGCSafeboxWrongPassword;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCSafeboxSize;

typedef struct packet_safebox_money_change
{
    BYTE bHeader;
    DWORD dwMoney;
} TPacketGCSafeboxMoneyChange;

typedef struct command_empire
{
    BYTE        bHeader;
    BYTE        bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
    BYTE        bHeader;
    BYTE        bEmpire;
} TPacketGCEmpire;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
    FISHING_SUBHEADER_GC_FISH,
};

typedef struct packet_fishing
{
    BYTE header;
    BYTE subheader;
    DWORD info;
    BYTE dir;
} TPacketGCFishing;

typedef struct paryt_parameter
{
    BYTE        bHeader;
    BYTE        bDistributeMode;
} TPacketGCPartyParameter;

//////////////////////////////////////////////////////////////////////////
// Guild

enum
{
    GUILD_SUBHEADER_GC_LOGIN,
	GUILD_SUBHEADER_GC_LOGOUT,
	GUILD_SUBHEADER_GC_LIST,
	GUILD_SUBHEADER_GC_GRADE,
	GUILD_SUBHEADER_GC_ADD,
	GUILD_SUBHEADER_GC_REMOVE,
	GUILD_SUBHEADER_GC_GRADE_NAME,
	GUILD_SUBHEADER_GC_GRADE_AUTH,
	GUILD_SUBHEADER_GC_INFO,
	GUILD_SUBHEADER_GC_COMMENTS,
    GUILD_SUBHEADER_GC_CHANGE_EXP,
    GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_GC_SKILL_INFO,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_GC_GUILD_INVITE,
    GUILD_SUBHEADER_GC_WAR,
    GUILD_SUBHEADER_GC_GUILD_NAME,
    GUILD_SUBHEADER_GC_GUILD_WAR_LIST,
    GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST,
    GUILD_SUBHEADER_GC_WAR_POINT,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

typedef struct packet_guild
{
    BYTE header;
    WORD size;
    BYTE subheader;
} TPacketGCGuild;

// SubHeader - Grade
enum
{
    GUILD_AUTH_ADD_MEMBER       = (1 << 0),
    GUILD_AUTH_REMOVE_MEMBER    = (1 << 1),
    GUILD_AUTH_NOTICE           = (1 << 2),
    GUILD_AUTH_SKILL            = (1 << 3),
};

typedef struct packet_guild_sub_grade
{
	char grade_name[GUILD_GRADE_NAME_MAX_LEN+1];
	BYTE auth_flag;
} TPacketGCGuildSubGrade;

typedef struct packet_guild_sub_member
{
	DWORD pid;
	BYTE byGrade;
	BYTE byIsGeneral;
	BYTE byJob;
	BYTE byLevel;
	DWORD dwOffer;
	BYTE byNameFlag;
// if NameFlag is TRUE, name is sent from server.
//	char szName[CHARACTER_ME_MAX_LEN+1];
} TPacketGCGuildSubMember;

typedef struct packet_guild_sub_info
{
    WORD member_count;
    WORD max_member_count;
	DWORD guild_id;
    DWORD master_pid;
    DWORD exp;
    BYTE level;
    char name[GUILD_NAME_MAX_LEN+1];
	DWORD gold;
	BYTE hasLand;
} TPacketGCGuildInfo;

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

    GUILD_WAR_DURATION = 2*60*60,
};

typedef struct packet_guild_war
{
    DWORD       dwGuildSelf;
    DWORD       dwGuildOpp;
    BYTE        bType;
    BYTE        bWarState;
} TPacketGCGuildWar;

typedef struct SPacketGuildWarPoint
{
    DWORD dwGainGuildID;
    DWORD dwOpponentGuildID;
    long lPoint;
} TPacketGuildWarPoint;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE		bHeader;
    WORD		size;
    BYTE		subheader;
} TPacketGCDungeon;

// Private Shop
typedef struct SPacketGCShopSign
{
    BYTE        bHeader;
    DWORD       dwVID;
    char        szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

typedef struct SPacketGCTime
{
    BYTE        bHeader;
    time_t      time;
} TPacketGCTime;

enum
{
    WALKMODE_RUN,
    WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
    BYTE        header;
    DWORD       vid;
    BYTE        mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
    BYTE        header;
    BYTE        skill_group;
} TPacketGCChangeSkillGroup;

struct TMaterial
{
    DWORD vnum;
    DWORD count;
};

typedef struct SRefineTable
{
    DWORD src_vnum;
    DWORD result_vnum;
    BYTE material_count;
    int cost;
    int prob;
    TMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TRefineTable;

typedef struct SPacketGCRefineInformation
{
	BYTE			header;
	BYTE			pos;
	TRefineTable	refine_table;
} TPacketGCRefineInformation;

typedef struct SPacketGCRefineInformationNew
{
	BYTE			header;
	BYTE			type;
	BYTE			pos;
	TRefineTable	refine_table;
} TPacketGCRefineInformationNew;

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

#ifdef ENABLE_GROWTH_PET_SYSTEM
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

typedef struct SPacketGCSpecialEffect
{
    BYTE header;
    BYTE type;
    DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketGCNPCPosition
{
    BYTE header;
	WORD size;
    WORD count;
} TPacketGCNPCPosition;

struct TNPCPosition
{
    BYTE bType;
    char name[CHARACTER_NAME_MAX_LEN+1];
    long x;
    long y;
};

typedef struct SPacketGCChangeName
{
    BYTE header;
    DWORD pid;
    char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;

enum EBlockAction
{
    BLOCK_EXCHANGE              = (1 << 0),
    BLOCK_PARTY_INVITE          = (1 << 1),
    BLOCK_GUILD_INVITE          = (1 << 2),
    BLOCK_WHISPER               = (1 << 3),
    BLOCK_MESSENGER_INVITE      = (1 << 4),
    BLOCK_PARTY_REQUEST         = (1 << 5),
};

typedef struct packet_china_matrixd_card
{
	BYTE	bHeader;
	DWORD	dwRows;
	DWORD	dwCols;
} TPacketGCChinaMatrixCard;

typedef struct packet_runup_matrixd_quiz
{
	BYTE	bHeader;
	char	szQuiz[RUNUP_MATRIX_QUIZ_MAX_LEN + 1];
} TPacketGCRunupMatrixQuiz;

typedef struct packet_newcibn_passpod_request
{
	BYTE	bHeader;
} TPacketGCNEWCIBNPasspodRequest;

typedef struct packet_newcibn_passpod_failure
{
	BYTE	bHeader;
	char	szMessage[NEWCIBN_PASSPOD_FAILURE_MAX_LEN + 1];
} TPacketGCNEWCIBNPasspodFailure;

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct packet_auth_success
{
    BYTE        bHeader;
    DWORD       dwLoginKey;
    BYTE        bResult;
} TPacketGCAuthSuccess;

typedef struct packet_auth_success_openid
{
    BYTE        bHeader;
    DWORD       dwLoginKey;
    BYTE        bResult;
	char		login[ID_MAX_NUM + 1];
} TPacketGCAuthSuccessOpenID;

typedef struct packet_channel
{
    BYTE header;
    BYTE channel;
#ifdef ENABLE_ANTI_EXP
	bool anti_exp;
#endif
} TPacketGCChannel;

typedef struct SEquipmentItemSet
{
	DWORD   vnum;
	BYTE    count;
	long    alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
    BYTE header;
	DWORD dwVID;
	TEquipmentItemSet equips[WEAR_MAX_NUM];
} TPacketGCViewEquip;

typedef struct
{
    DWORD       dwID;
    long        x, y;
    long        width, height;
    DWORD       dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
    BYTE        header;
    WORD        size;
} TPacketGCLandList;

typedef struct
{
    BYTE        bHeader;
    long        lID;
    char        szTargetName[32+1];
} TPacketGCTargetCreate;

enum
{
	CREATE_TARGET_TYPE_NONE,
	CREATE_TARGET_TYPE_LOCATION,
	CREATE_TARGET_TYPE_CHARACTER,
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
	CREATE_TARGET_TYPE_SHOP_SEARCH = 1 << 2,
#endif
};

typedef struct
{
	BYTE		bHeader;
	long		lID;
	char		szTargetName[32+1];
	DWORD		dwVID;
	BYTE		byType;
} TPacketGCTargetCreateNew;

typedef struct
{
    BYTE        bHeader;
    long        lID;
    long        lX, lY;
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
	bool		bIsShopSearch;
#endif
} TPacketGCTargetUpdate;

typedef struct
{
    BYTE        bHeader;
    long        lID;
} TPacketGCTargetDelete;

typedef struct
{
    DWORD       dwType;
    BYTE        bPointIdxApplyOn;
    long        lApplyValue;
    DWORD       dwFlag;
    long        lDuration;
    long        lSPCost;
} TPacketAffectElement;

typedef struct
{
    BYTE bHeader;
    TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct
{
    BYTE bHeader;
    DWORD dwType;
    BYTE bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_mall_open
{
	BYTE bHeader;
	BYTE bSize;
} TPacketGCMallOpen;

typedef struct packet_lover_info
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE byLovePoint;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE bHeader;
	BYTE byLovePoint;
} TPacketGCLovePointUpdate;

typedef struct packet_dig_motion
{
    BYTE header;
    DWORD vid;
    DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;

typedef struct SPacketGCOnTime
{
    BYTE header;
    int ontime;     // sec
} TPacketGCOnTime;

typedef struct SPacketGCResetOnTime
{
    BYTE header;
} TPacketGCResetOnTime;

typedef struct SPacketGCPanamaPack
{
    BYTE    bHeader;
    char    szPackName[256];
    BYTE    abIV[32];
} TPacketGCPanamaPack;

typedef struct SPacketGCHybridCryptKeys
{
private:
	SPacketGCHybridCryptKeys() : m_pStream(NULL) {}

public:
	SPacketGCHybridCryptKeys(int iStreamSize) : iKeyStreamLen(iStreamSize)
	{
		m_pStream = new BYTE[iStreamSize];
	}
	~SPacketGCHybridCryptKeys()
	{
		if( m_pStream )
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}
	static int GetFixedHeaderSize()
	{
		return sizeof(BYTE)+sizeof(WORD)+sizeof(int);
	}

	BYTE	bHeader;
	WORD    wDynamicPacketSize;
	int		iKeyStreamLen;
	BYTE*	m_pStream;

} TPacketGCHybridCryptKeys;


typedef struct SPacketGCHybridSDB
{
private:
	SPacketGCHybridSDB() : m_pStream(NULL) {}

public:
	SPacketGCHybridSDB(int iStreamSize) : iSDBStreamLen(iStreamSize)
	{
		m_pStream = new BYTE[iStreamSize];
	}
	~SPacketGCHybridSDB()
	{
		delete[] m_pStream;
		m_pStream = NULL;
	}
	static int GetFixedHeaderSize()
	{
		return sizeof(BYTE)+sizeof(WORD)+sizeof(int);
	}

	BYTE	bHeader;
	WORD    wDynamicPacketSize;
	int		iSDBStreamLen;
	BYTE*	m_pStream;

} TPacketGCHybridSDB;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client To Client

typedef struct packet_state
{
	BYTE			bHeader;
	BYTE			bFunc;
	BYTE			bArg;
	BYTE			bRot;
	DWORD			dwVID;
	DWORD			dwTime;
	TPixelPosition	kPPos;
} TPacketCCState;

typedef struct packet_hs_check_req
{
	BYTE	bHeader;
	AHNHS_TRANS_BUFFER	Req;
} TPacketHSCheck;

typedef struct packet_xtrap_verify
{
	BYTE	bHeader;
	BYTE	bPacketData[128];

} TPacketXTrapCSVerify;
// AUTOBAN
typedef struct packet_autoban_quiz
{
    BYTE bHeader;
	BYTE bDuration;
    BYTE bCaptcha[64*32];
    char szQuiz[256];
} TPacketGCAutoBanQuiz;
// END_OF_AUTOBAN

#ifdef _IMPROVED_PACKET_ENCRYPTION_
struct TPacketKeyAgreement
{
	static const int MAX_DATA_LEN = 256;
	BYTE bHeader;
	WORD wAgreedLength;
	WORD wDataLength;
	BYTE data[MAX_DATA_LEN];
};

struct TPacketKeyAgreementCompleted
{
	BYTE bHeader;
	BYTE data[3]; // dummy (not used)
};
#endif // _IMPROVED_PACKET_ENCRYPTION_

typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[128];
} TPacketGCSpecificEffect;

enum EDragonSoulRefineWindowRefineType
{
	DragonSoulRefineWindow_UPGRADE,
	DragonSoulRefineWindow_IMPROVEMENT,
	DragonSoulRefineWindow_REFINE,
};

enum EPacketCGDragonSoulSubHeaderType
{
	DS_SUB_HEADER_OPEN,
	DS_SUB_HEADER_CLOSE,
	DS_SUB_HEADER_DO_UPGRADE,
	DS_SUB_HEADER_DO_IMPROVEMENT,
	DS_SUB_HEADER_DO_REFINE,
	DS_SUB_HEADER_REFINE_FAIL,
	DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE,
	DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL,
	DS_SUB_HEADER_REFINE_SUCCEED,
};

typedef struct SPacketCGDragonSoulRefine
{
	SPacketCGDragonSoulRefine() : header (HEADER_CG_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DS_REFINE_WINDOW_MAX_NUM];
} TPacketCGDragonSoulRefine;

#ifdef ENABLE_GAYA_SYSTEM
enum
{
	GAYA_SYSTEM_SUB_HEADER_CRAFT,
	GAYA_SYSTEM_SUB_HEADER_MARKET,
	GAYA_SYSTEM_SUB_HEADER_REFRESH,
};
typedef struct packet_gaya_system
{
	BYTE header;
	BYTE subheader;
	int  pos;

} TPacketCGGayaSystem;
#endif

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SChannelStatus
{
	short nPort;
	BYTE bStatus;
} TChannelStatus;

#ifdef ENABLE_DS_SET
typedef struct STPacketDSTable {
	BYTE	bHeader;
	int		iType, iApplyCount, iBasicApplyValue[255], iAditionalApplyValue[255];
	float	fWeight;
} TPacketDSTable;
#endif

#ifdef ENABLE_ACCE_SYSTEM
enum EAcceInfo
{
	ACCE_ABSORPTION_SOCKET = 0,
	ACCE_ABSORBED_SOCKET = 1,
	ACCE_CLEAN_ATTR_VALUE0 = 7,
	ACCE_WINDOW_MAX_MATERIALS = 2,
};

enum
{
	HEADER_CG_ACCE = 211,
	HEADER_GC_ACCE = 215,
	ACCE_SUBHEADER_GC_OPEN = 0,
	ACCE_SUBHEADER_GC_CLOSE,
	ACCE_SUBHEADER_GC_ADDED,
	ACCE_SUBHEADER_GC_REMOVED,
	ACCE_SUBHEADER_CG_REFINED,
	ACCE_SUBHEADER_CG_CLOSE = 0,
	ACCE_SUBHEADER_CG_ADD,
	ACCE_SUBHEADER_CG_REMOVE,
	ACCE_SUBHEADER_CG_REFINE,
};

typedef struct SPacketAcce
{
	BYTE	header;
	BYTE	subheader;
	bool	bWindow;
	DWORD	dwPrice;
	BYTE	bPos;
	TItemPos	tPos;
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TPacketAcce;

typedef struct SAcceMaterial
{
	BYTE	bHere;
	WORD	wCell;
} TAcceMaterial;

typedef struct SAcceResult
{
	DWORD	dwItemVnum;
	DWORD	dwMinAbs;
	DWORD	dwMaxAbs;
} TAcceResult;
#endif

// @fixme007 length 2
typedef struct packet_unk_213
{
	BYTE bHeader;
	BYTE bUnk2;
} TPacketGCUnk213;

#ifdef ENABLE_SKILL_COLOR_SYSTEM
typedef struct packet_skill_color
{
	BYTE		bheader;
	BYTE		skill;
	DWORD		col1;
	DWORD		col2;
	DWORD		col3;
	DWORD		col4;
	DWORD		col5;
}TPacketCGSkillColor;
#endif

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD

enum
{
	CUBE_RENEWAL_SUB_HEADER_OPEN_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_CLEAR_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_RECEIVE,
	CUBE_RENEWAL_SUB_HEADER_DATES_LOADING,

	CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM,
	CUBE_RENEWAL_SUB_HEADER_CLOSE,
};


typedef struct  packet_send_cube_renewal
{
	BYTE header;
	BYTE subheader;
	DWORD index_item;
	DWORD count_item;
	DWORD index_item_improve;
}TPacketCGCubeRenewalSend;



typedef struct dates_cube_renewal
{
	DWORD npc_vnum;
	DWORD index;

	DWORD	vnum_reward;
	int		count_reward;

	bool 	item_reward_stackable;

	DWORD	vnum_material_1;
	int		count_material_1;

	DWORD	vnum_material_2;
	int		count_material_2;

	DWORD	vnum_material_3;
	int		count_material_3;

	DWORD	vnum_material_4;
	int		count_material_4;

	DWORD	vnum_material_5;
	int		count_material_5;

	int 	gold;
	int 	percent;
#ifdef ENABLE_CUBE_RENEWAL_GEM_WORLDARD
	int 	gem;
#endif
#ifdef ENABLE_CUBE_RENEWAL_COPY_WORLDARD
	DWORD	allowCopy;
#endif
	char 	category[100];
}TInfoDateCubeRenewal;

typedef struct packet_receive_cube_renewal
{
	packet_receive_cube_renewal(): header(HEADER_GC_CUBE_RENEWAL)
	{}

	BYTE header;
	BYTE subheader;
	TInfoDateCubeRenewal	date_cube_renewal;
}TPacketGCCubeRenewalReceive;


#endif


#ifdef ENABLE_REFINE_ELEMENT
typedef struct SPacketCGRefineElement
{
	BYTE	bHeader;
	
	// 1. bArg is used for close window to clear the info
	// 2. bArg is used for change element type and it represent the new type
	BYTE	bArg;
} TPacketCGRefineElement;

typedef struct SPacketGCRefineElement
{
	BYTE	bHeader;
	WORD	wSrcCell;
	WORD	wDstCell;
	BYTE	bType;
} TPacketGCRefineElement;
#endif


#ifdef ENABLE_AURA_SYSTEM


enum EPacketGCAuraSubHeader
{
	AURA_SUBHEADER_GC_OPEN,
	AURA_SUBHEADER_GC_CLOSE,
	AURA_SUBHEADER_GC_SET_ITEM,
	AURA_SUBHEADER_GC_CLEAR_SLOT,
	AURA_SUBHEADER_GC_CLEAR_ALL,
	AURA_SUBHEADER_GC_CLEAR_RIGHT,
	AURA_SUBHEADER_GC_REFINE_INFO,
};

typedef struct SSubPacketGCAuraOpenClose
{
	BYTE	bAuraWindowType;
} TSubPacketGCAuraOpenClose;

typedef struct SSubPacketGCAuraSetItem
{
	TItemPos	Cell;
	TItemPos	AuraCell;
	TItemData2	pItem;
} TSubPacketGCAuraSetItem;

typedef struct SSubPacketGCAuraClearSlot
{
	BYTE	bAuraSlotPos;
} TSubPacketGCAuraClearSlot;

typedef struct SSubPacketGCAuraRefineInfo
{
	BYTE	bAuraRefineInfoType;
	BYTE	bAuraRefineInfoLevel;
	BYTE	bAuraRefineInfoExpPercent;
} TSubPacketGCAuraRefineInfo;

enum EPacketCGAuraSubHeader
{
	AURA_SUBHEADER_CG_REFINE_CHECKIN,
	AURA_SUBHEADER_CG_REFINE_CHECKOUT,
	AURA_SUBHEADER_CG_REFINE_ACCEPT,
	AURA_SUBHEADER_CG_REFINE_CANCEL,
};

typedef struct SSubPacketCGAuraRefineCheckIn
{
	TItemPos	ItemCell;
	TItemPos	AuraCell;
	BYTE		byAuraRefineWindowType;
} TSubPacketCGAuraRefineCheckIn;

typedef struct SSubPacketCGAuraRefineCheckOut
{
	TItemPos	AuraCell;
	BYTE		byAuraRefineWindowType;
} TSubPacketCGAuraRefineCheckOut;

typedef struct SSubPacketCGAuraRefineAccept
{
	BYTE		byAuraRefineWindowType;
} TSubPacketCGAuraRefineAccept;

typedef struct SPacketGCAura
{
	SPacketGCAura() : bHeader(HEADER_GC_AURA) {}
	BYTE bHeader;
	WORD wSize;
	BYTE bSubHeader;
} TPacketGCAura;

typedef struct SPacketCGAura
{
	SPacketCGAura() : bHeader(HEADER_CG_AURA) {}
	BYTE bHeader;
	WORD wSize;
	BYTE bSubHeader;
} TPacketCGAura;
#endif

#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM

enum
{
	BONUS_67_NEW_SUB_HEADER_CLOSE = 0,
	BONUS_67_NEW_SUB_HEADER_OPEN = 1,
	BONUS_67_NEW_SUB_HEADER_COMB_SKILLBOOK = 2,

	BONUS_67_NEW_SUB_HEADER_GET_FRAGMENT,
	BONUS_67_NEW_SUB_HEADER_ADD,

	BONUS_67_NEW_SUB_HEADER_FRAGMENT_RECEIVE,
};

typedef struct packet_send_67_bonus_system
{
	BYTE header;
	BYTE subheader;
	int  cell[c_skillbook_slot_max];
	int  regist_slot;
	int  count_fragment;
	int  cell_additive;
	int  count_additive;


} TPacketCG67BonusSend;

typedef struct packet_receive_67_bonus_new
{
	packet_receive_67_bonus_new(): header(HEADER_GC_67_BONUS_NEW)
	{}

	BYTE header;
	BYTE subheader;
	DWORD vnum_fragment;

}TPacketGC67BonusReceive;

#endif

#ifdef ENABLE_EVENT_MANAGER
typedef struct SPacketCGRequestEventData
{
	BYTE bHeader;
	BYTE bMonth;
} TPacketCGRequestEventData;

typedef struct SPacketGCEventInfo
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketGCEventInfo;

typedef struct SPacketGCEventReload
{
	BYTE	bHeader;
} TPacketGCEventReload;

typedef struct SPacketGCEventKWScore
{
	BYTE	bHeader;
	WORD	wKingdomScores[3];
} TPacketGCEventKWScore;

typedef struct SPacketEventData
{
	DWORD	dwID;
	BYTE	bType;
	long	startTime;
	long	endTime;
	int		iValue0;
	int		iValue1;
	bool	bCompleted;
} TPacketEventData;
#endif

typedef struct command_request_event_quest
{
	BYTE		bHeader;
	char		szName[QUEST_NAME_MAX_NUM + 1];
} TPacketCGRequestEventQuest;

#ifdef ENABLE_GROWTH_PET_SYSTEM
enum EGrowthPetPoints
{
	POINT_UPBRINGING_PET_LEVEL,
	POINT_UPBRINGING_PET_EXP,
	POINT_UPBRINGING_PET_ITEM_EXP,
	POINT_UPBRINGING_PET_NEXT_EXP,
	POINT_UPBRINGING_PET_EVOL_LEVEL,
	POINT_UPBRINGING_PET_HP,
	POINT_UPBRINGING_PET_DEF_GRADE,
	POINT_UPBRINGING_PET_SP,
	POINT_UPBRINGING_DURATION,
	POINT_UPBRINGING_MAX_DURATION,
	POINT_UPBRINGING_BIRTHDAY,

	POINT_UPBRINGING_MAX_NUM
};

enum EGrowthPetSubheader
{
	SUBHEADER_PET_EGG_USE_SUCCESS,
	SUBHEADER_PET_EGG_USE_FAILED_BECAUSE_NAME,
	SUBHEADER_PET_EGG_USE_FAILED_TIMEOVER,
	SUBHEADER_PET_UNSUMMON,
	SUBHEADER_PET_FEED_FAILED,
	SUBHEADER_PET_FEED_SUCCESS,
	SUBHEADER_PET_REVIVE_FAILED,
	SUBHEADER_PET_REVIVE_SUCCESS,
	SUBHEADER_PET_NAME_CHANGE_FAILED,
	SUBHEADER_PET_NAME_CHANGE_SUCCESS,
	SUBHEADER_PET_WINDOW_TYPE_INFO,
	SUBHEADER_PET_WINDOW_TYPE_ATTR_CHANGE,
	SUBHEADER_PET_WINDOW_TYPE_PREMIUM_FEED,
};

enum EGrowthPetWindow
{
	PET_WINDOW_HATCH,
	PET_WINDOW_NAME_CHANGE,
};

typedef struct SPacketGCPet
{
	BYTE	header;
	BYTE	subheader;
} TPacketGCPet;

typedef struct SPetSkillPacket
{
	bool    bLocked;
	BYTE    bSkill;
	BYTE    bLevel;
	DWORD    dwCooltime;
} TPetSkillPacket;

typedef struct SPacketGCPetSet
{
	BYTE		header;
	DWORD		dwID;
	DWORD		dwSummonItemVnum;
	char		szName[CItemData::PET_NAME_MAX_SIZE + 1];
	TPetSkillPacket	aSkill[PET_SKILL_COUNT_MAX];
	DWORD		dwPoints[POINT_UPBRINGING_MAX_NUM];
} TPacketGCPetSet;

typedef struct SPacketGCPetDelete
{
	BYTE		header;
	DWORD		dwID;
} TPacketGCPetDelete;

typedef struct SPacketGCPetPointUpdate
{
	BYTE	header;
	DWORD	dwID;
	BYTE	bPoint;
	DWORD	dwValue;
} TPacketGCPetPointUpdate;

typedef struct SPacketGCPetSummon
{
	BYTE		header;
	DWORD		dwID;
} TPacketGCPetSummon;

typedef struct SPacketGCPetDetermineResult
{
	BYTE		header;
	BYTE		type;
} TPacketGCPetDetermineResult;

typedef struct SPacketGCPetAttrChangeResult
{
	BYTE		header;
	BYTE		type;
	TItemPos	pos;
} TPacketGCPetAttrChangeResult;

typedef struct SPetSkillUpdatePacket
{
	bool	bLocked;
	BYTE	bSkill;
	BYTE	bLevel;
	DWORD	dwCooltime;
	BYTE	bSkillFormula1[PET_GROWTH_SKILL_LEVEL_MAX];
	WORD	wSkillFormula2[PET_GROWTH_SKILL_LEVEL_MAX];
	BYTE	bSkillFormula3[PET_GROWTH_SKILL_LEVEL_MAX];
} TPetSkillUpdatePacket;

typedef struct SPacketGCPetSkillCooltime
{
	BYTE		header;
	DWORD		dwID;
	BYTE		bSlot;
	DWORD		dwCooltime;
} TPacketGCPetSkillCooltime;

typedef struct SPacketGCPetSkillUpdate
{
	BYTE		header;
	DWORD		dwID;
	TPetSkillUpdatePacket	aSkill[PET_SKILL_COUNT_MAX];
} TPacketGCPetSkillUpdate;

typedef struct SPacketGCPetNameChangeResult
{
	BYTE		header;
	BYTE		subheader;
	DWORD		dwID;
	char		szName[CItemData::PET_NAME_MAX_SIZE + 1];
} TPacketGCPetNameChangeResult;

typedef struct SPacketCGPetHatch
{
	BYTE		header;
	char		name[CItemData::PET_NAME_MAX_SIZE + 1];
	TItemPos	eggPos;
} TPacketCGPetHatch;

typedef struct SPacketCGPetWindow
{
	BYTE	header;
	BYTE	window;
	bool	state;
} TPacketCGPetWindow;

typedef struct SPacketCGPetWindowType
{
	BYTE	header;
	BYTE	type;
} TPacketCGPetWindowType;

typedef struct SPacketCGPeFeed
{
	BYTE		header;
	BYTE		index;
	WORD		pos[PET_FEED_SLOT_MAX];
	WORD		count[PET_FEED_SLOT_MAX];
} TPacketCGPetFeed;

typedef struct SPacketCGPetDetermine
{
	BYTE		header;
	TItemPos	determinePos;
} TPacketCGPetDetermine;

typedef struct SPacketCGPetAttrChange
{
	BYTE		header;
	TItemPos	upBringingPos;
	TItemPos	attrChangePos;
} TPacketCGPetAttrChange;

typedef struct SPacketCGPetRevive
{
	BYTE		header;
	TItemPos	upBringingPos;
	WORD		pos[PET_REVIVE_MATERIAL_SLOT_MAX];
	WORD		count[PET_REVIVE_MATERIAL_SLOT_MAX];
} TPacketCGPetRevive;

typedef struct SPacketCGPetLearnSkill
{
	BYTE		header;
	BYTE		slotIndex;
	TItemPos	learnSkillPos;
} TPacketCGPetLearnSkill;

typedef struct SPacketCGPetUpgradeSkill
{
	BYTE		header;
	BYTE		slotIndex;
} TPacketCGPetSkillUpgrade;

typedef struct SPacketCGPetDeleteSkill
{
	BYTE		header;
	BYTE		slotIndex;
	TItemPos	deleteSkillPos;
} TPacketCGPetDeleteSkill;

typedef struct SPacketCGPetAllDeleteSkill
{
	BYTE		header;
	TItemPos	deleteAllSkillPos;
} TPacketCGPetDeleteAllSkill;

typedef struct SPacketCGPetNameChange
{
	BYTE		header;
	char		name[CItemData::PET_NAME_MAX_SIZE + 1];
	TItemPos	changeNamePos;
	TItemPos	upBringingPos;
} TPacketCGPetNameChange;
#endif

#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
typedef struct SPacketChangeLanguage
{
	BYTE bHeader;
	BYTE bLanguage;
} TPacketChangeLanguage;
#endif

#ifdef ENABLE_EXTENDED_WHISPER_DETAILS
typedef struct SPacketCGWhisperDetails
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisperDetails;

typedef struct SPacketGCWhisperDetails
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
#ifdef ENABLE_MULTI_LANGUAGE_SYSTEM
	BYTE bLanguage;
#endif
} TPacketGCWhisperDetails;
#endif

#ifdef ENABLE_SWITCHBOT
enum ECGSwitchbotSubheader
{
	SUBHEADER_CG_SWITCHBOT_START,
	SUBHEADER_CG_SWITCHBOT_STOP,
};

struct TPacketCGSwitchbot
{
	BYTE header;
	int size;
	BYTE subheader;
	BYTE slot;
};

enum EGCSwitchbotSubheader
{
	SUBHEADER_GC_SWITCHBOT_UPDATE,
	SUBHEADER_GC_SWITCHBOT_UPDATE_ITEM,
	SUBHEADER_GC_SWITCHBOT_SEND_ATTRIBUTE_INFORMATION,
};

struct TPacketGCSwitchbot
{
	BYTE header;
	int size;
	BYTE subheader;
	BYTE slot;
};

struct TSwitchbotUpdateItem
{
	BYTE	slot;
	BYTE	vnum;
	BYTE	count;
	long	alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
};
#endif

#ifdef ENABLE_SHOW_CHEST_DROP
enum
{
	HEADER_CG_CHEST_DROP_INFO = 215,
	HEADER_GC_CHEST_DROP_INFO = 219,
};

typedef struct SPacketCGChestDropInfo {
	BYTE	header;
	WORD	wInventoryCell;
} TPacketCGChestDropInfo;

typedef struct SChestDropInfoTable {
	BYTE	bPageIndex;
	BYTE	bSlotIndex;
	DWORD	dwItemVnum;
	BYTE	bItemCount;
} TChestDropInfoTable;

typedef struct SPacketGCChestDropInfo {
	BYTE	bHeader;
	WORD	wSize;
	DWORD	dwChestVnum;
} TPacketGCChestDropInfo;
#endif

#ifdef ENABLE_EXTENDED_BATTLE_PASS
typedef struct SPacketCGExtBattlePassAction
{
	BYTE bHeader;
	BYTE bAction;
} TPacketCGExtBattlePassAction;

typedef struct SPacketCGExtBattlePassSendPremiumItem
{
	BYTE bHeader;
	int iSlotIndex;
} TPacketCGExtBattlePassSendPremiumItem;

typedef struct SPacketGCExtBattlePassOpen
{
	BYTE bHeader;
} TPacketGCExtBattlePassOpen;

typedef struct SPacketGCExtBattlePassGeneralInfo
{
	BYTE bHeader;
	BYTE bBattlePassType;
	char	szSeasonName[64+1];
	DWORD dwBattlePassID;
	DWORD dwBattlePassStartTime;
	DWORD dwBattlePassEndTime;
} TPacketGCExtBattlePassGeneralInfo;

typedef struct SPacketGCExtBattlePassMissionInfo
{
	BYTE bHeader;
	WORD wSize;
	WORD wRewardSize;
	BYTE bBattlePassType;
	DWORD dwBattlePassID;
} TPacketGCExtBattlePassMissionInfo;

typedef struct SPacketGCExtBattlePassMissionUpdate
{
	BYTE bHeader;
	BYTE bBattlePassType;
	BYTE bMissionIndex;
	BYTE bMissionType;
	DWORD dwNewProgress;
} TPacketGCExtBattlePassMissionUpdate;

typedef struct SPacketGCExtBattlePassRanking
{
	BYTE bHeader;
	char	szPlayerName[CHARACTER_NAME_MAX_LEN + 1];
	BYTE bBattlePassType;
	BYTE	bBattlePassID;
	DWORD	dwStartTime;
	DWORD	dwEndTime;
} TPacketGCExtBattlePassRanking;
#endif

#ifdef ENABLE_ATLAS_BOSS
typedef struct SPacketGCBossPosition
{
	BYTE	bHeader;
	WORD	wSize;
	WORD	wCount;
} TPacketGCBossPosition;

struct TBossPosition
{
	BYTE	bType;

	char	szName[CHARACTER_NAME_MAX_LEN + 1];

	long	lX;
	long	lY;
	long	lTime;
};
#endif

#pragma pack(pop)


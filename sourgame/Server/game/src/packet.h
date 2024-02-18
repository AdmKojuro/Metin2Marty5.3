#ifndef __INC_PACKET_H__
#define __INC_PACKET_H__
#include "stdafx.h"
#include "../../common/CommonDefines.h"

enum
{
	HEADER_CG_HANDSHAKE				= 0xff,
	HEADER_CG_PONG					= 0xfe,
	HEADER_CG_TIME_SYNC				= 0xfc,
	HEADER_CG_KEY_AGREEMENT			= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_

	HEADER_CG_LOGIN					= 1,
	HEADER_CG_ATTACK				= 2,
	HEADER_CG_CHAT					= 3,
	HEADER_CG_CHARACTER_CREATE		= 4,
	HEADER_CG_CHARACTER_DELETE		= 5,
	HEADER_CG_CHARACTER_SELECT		= 6,
	HEADER_CG_MOVE					= 7,
	HEADER_CG_SYNC_POSITION			= 8,
	HEADER_CG_ENTERGAME				= 10,

	HEADER_CG_ITEM_USE				= 11,
	HEADER_CG_ITEM_DROP				= 12,
	HEADER_CG_ITEM_MOVE				= 13,
	HEADER_CG_ITEM_PICKUP			= 15,

	HEADER_CG_QUICKSLOT_ADD			= 16,
	HEADER_CG_QUICKSLOT_DEL			= 17,
	HEADER_CG_QUICKSLOT_SWAP		= 18,
	HEADER_CG_WHISPER				= 19,
	HEADER_CG_ITEM_DROP2			= 20,
#ifdef __NEW_DROP_DIALOG__
	HEADER_CG_ITEM_DESTROY			= 21,
#endif
	HEADER_CG_ON_CLICK				= 26,
	HEADER_CG_EXCHANGE				= 27,
	HEADER_CG_CHARACTER_POSITION	= 28,
	HEADER_CG_SCRIPT_ANSWER			= 29,
	HEADER_CG_QUEST_INPUT_STRING	= 30,
	HEADER_CG_QUEST_CONFIRM			= 31,
	HEADER_CG_REQUEST_EVENT_QUEST	= 32,

	HEADER_CG_SHOP					= 50,
	HEADER_CG_FLY_TARGETING			= 51,
	HEADER_CG_USE_SKILL				= 52,
	HEADER_CG_ADD_FLY_TARGETING		= 53,
	HEADER_CG_SHOOT					= 54,
	HEADER_CG_MYSHOP				= 55,

#ifdef __SKILL_COLOR_SYSTEM__
	HEADER_CG_SKILL_COLOR			= 56,
#endif
#ifdef __SEND_TARGET_INFO__
	HEADER_CG_TARGET_INFO_LOAD		= 59,
#endif
									
	HEADER_CG_ITEM_USE_TO_ITEM		= 60,
	HEADER_CG_TARGET			 	= 61,
									
	HEADER_CG_TEXT					= 64,
	HEADER_CG_WARP					= 65,
	HEADER_CG_SCRIPT_BUTTON			= 66,
	HEADER_CG_MESSENGER				= 67,
									
	HEADER_CG_MALL_CHECKOUT			= 69,
	HEADER_CG_SAFEBOX_CHECKIN		= 70,
	HEADER_CG_SAFEBOX_CHECKOUT		= 71,
									
	HEADER_CG_PARTY_INVITE			= 72,
	HEADER_CG_PARTY_INVITE_ANSWER	= 73,
	HEADER_CG_PARTY_REMOVE			= 74,
	HEADER_CG_PARTY_SET_STATE		= 75,
	HEADER_CG_PARTY_USE_SKILL		= 76,
	HEADER_CG_SAFEBOX_ITEM_MOVE		= 77,
	HEADER_CG_PARTY_PARAMETER		= 78,
									
	HEADER_CG_GUILD					= 80,
	HEADER_CG_ANSWER_MAKE_GUILD		= 81,

	HEADER_CG_FISHING				= 82,

	HEADER_CG_ITEM_GIVE				= 83,

	HEADER_CG_EMPIRE				= 90,

	HEADER_CG_REFINE				= 96,

	HEADER_CG_MARK_LOGIN			= 100,
	HEADER_CG_MARK_CRCLIST			= 101,
	HEADER_CG_MARK_UPLOAD			= 102,
	HEADER_CG_MARK_IDXLIST			= 104,

	HEADER_CG_HACK					= 105,
	HEADER_CG_CHANGE_NAME			= 106,
	HEADER_CG_LOGIN2				= 109,
	HEADER_CG_DUNGEON				= 110,
	HEADER_CG_LOGIN3				= 111,

	HEADER_CG_GUILD_SYMBOL_UPLOAD	= 112,
	HEADER_CG_SYMBOL_CRC			= 113,

	// SCRIPT_SELECT_ITEM
	HEADER_CG_SCRIPT_SELECT_ITEM	= 114,
	// END_OF_SCRIPT_SELECT_ITEM

#ifdef ENABLE_SWITCHBOT
	HEADER_CG_SWITCHBOT				= 171,
#endif

	HEADER_CG_LOGIN5_OPENID			= 116,
#ifdef __EVENT_MANAGER__
	HEADER_CG_REQUEST_EVENT_DATA	= 117,
#endif

#ifdef __GROWTH_PET_SYSTEM__
	HEADER_CG_PET_HATCH				= 118,
	HEADER_CG_PET_WINDOW_TYPE		= 119,
	HEADER_CG_PET_WINDOW			= 120,
	HEADER_CG_PET_NAME_CHANGE		= 121,
	HEADER_CG_PET_FEED				= 122,
	HEADER_CG_PET_DETERMINE			= 123,
	HEADER_CG_PET_ATTR_CHANGE		= 124,
	HEADER_CG_PET_REVIVE			= 125,
	HEADER_CG_PET_LEARN_SKILL		= 126,
	HEADER_CG_PET_SKILL_UPGRADE		= 127,
	HEADER_CG_PET_DELETE_SKILL		= 128,
	HEADER_CG_PET_DELETE_ALL_SKILL	= 129,
#endif
#ifdef __EXTENDED_WHISPER_DETAILS__
	HEADER_CG_WHISPER_DETAILS = 130,
#endif
#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
	HEADER_CG_DROP_ITEM_SYSTEM		= 148,
#endif
//	HEADER_CG_ROULETTE				= 200,
//	HEADER_CG_RUNUP_MATRIX_ANSWER	= 201,

	//
	HEADER_CG_PASSPOD_ANSWER		= 202,



	HEADER_CG_HS_ACK				= 203,
	HEADER_CG_XTRAP_ACK				= 204,

	HEADER_CG_DRAGON_SOUL_REFINE	= 205,
	HEADER_CG_STATE_CHECKER			= 206,
#ifdef ENABLE_GAYA_SYSTEM
	HEADER_CG_GAYA_SYSTEM 			= 210,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_CG_CUBE_RENEWAL 			= 216,
#endif
#ifdef __AURA_SYSTEM__
		HEADER_CG_AURA				= 217,
#endif
#ifdef ENABLE_REFINE_ELEMENT
	HEADER_CG_REFINE_ELEMENT 		= 172,
#endif
	HEADER_CG_CLIENT_VERSION		= 0xfd,
	HEADER_CG_CLIENT_VERSION2		= 0xf1,
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	HEADER_CG_67_BONUS_NEW 			= 220,
#endif
#ifdef __MULTI_LANGUAGE_SYSTEM__
	HEADER_CG_CHANGE_LANGUAGE = 221,
#endif

	/********************************************************/
	HEADER_GC_KEY_AGREEMENT_COMPLETED 		= 0xfa, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_KEY_AGREEMENT					= 0xfb, // _IMPROVED_PACKET_ENCRYPTION_
	HEADER_GC_TIME_SYNC						= 0xfc,
	HEADER_GC_PHASE							= 0xfd,
	HEADER_GC_BINDUDP						= 0xfe,
	HEADER_GC_HANDSHAKE						= 0xff,

	HEADER_GC_CHARACTER_ADD					= 1,
	HEADER_GC_CHARACTER_DEL					= 2,
	HEADER_GC_MOVE							= 3,
	HEADER_GC_CHAT							= 4,
	HEADER_GC_SYNC_POSITION					= 5,

	HEADER_GC_LOGIN_SUCCESS					= 6,
	HEADER_GC_LOGIN_SUCCESS_NEWSLOT			= 32,
	HEADER_GC_LOGIN_FAILURE					= 7,

	HEADER_GC_CHARACTER_CREATE_SUCCESS		= 8,
	HEADER_GC_CHARACTER_CREATE_FAILURE		= 9,
	HEADER_GC_CHARACTER_DELETE_SUCCESS		= 10,
	HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID	= 11,

	HEADER_GC_ATTACK						= 12,
	HEADER_GC_STUN							= 13,
	HEADER_GC_DEAD							= 14,

	HEADER_GC_MAIN_CHARACTER_OLD			= 15,
	HEADER_GC_CHARACTER_POINTS				= 16,
	HEADER_GC_CHARACTER_POINT_CHANGE		= 17,
	HEADER_GC_CHANGE_SPEED					= 18,
	HEADER_GC_CHARACTER_UPDATE				= 19,
	HEADER_GC_CHARACTER_UPDATE_NEW			= 24,

	HEADER_GC_ITEM_DEL						= 20,
	HEADER_GC_ITEM_SET						= 21,
	HEADER_GC_ITEM_USE						= 22,
	HEADER_GC_ITEM_DROP						= 23,
	HEADER_GC_ITEM_UPDATE					= 25,

	HEADER_GC_ITEM_GROUND_ADD				= 26,
	HEADER_GC_ITEM_GROUND_DEL				= 27,

	HEADER_GC_QUICKSLOT_ADD					= 28,
	HEADER_GC_QUICKSLOT_DEL					= 29,
	HEADER_GC_QUICKSLOT_SWAP				= 30,

	HEADER_GC_ITEM_OWNERSHIP				= 31,

	HEADER_GC_WHISPER						= 34,

	HEADER_GC_MOTION						= 36,
	HEADER_GC_PARTS							= 37,

	HEADER_GC_SHOP							= 38,
	HEADER_GC_SHOP_SIGN						= 39,

	HEADER_GC_DUEL_START					= 40,
	HEADER_GC_PVP							= 41,
	HEADER_GC_EXCHANGE						= 42,
	HEADER_GC_CHARACTER_POSITION			= 43,

	HEADER_GC_PING							= 44,
	HEADER_GC_SCRIPT						= 45,
	HEADER_GC_QUEST_CONFIRM					= 46,
#ifdef __SEND_TARGET_INFO__
	HEADER_GC_TARGET_INFO					= 58,
#endif
	HEADER_GC_MOUNT							= 61,
	HEADER_GC_OWNERSHIP						= 62,
	HEADER_GC_TARGET					 	= 63,

	HEADER_GC_WARP							= 65,

	HEADER_GC_ADD_FLY_TARGETING				= 69,
	HEADER_GC_CREATE_FLY					= 70,
	HEADER_GC_FLY_TARGETING					= 71,
	HEADER_GC_SKILL_LEVEL_OLD				= 72,
	HEADER_GC_SKILL_LEVEL					= 76,

	HEADER_GC_MESSENGER						= 74,
	HEADER_GC_GUILD							= 75,

	HEADER_GC_PARTY_INVITE					= 77,
	HEADER_GC_PARTY_ADD						= 78,
	HEADER_GC_PARTY_UPDATE					= 79,
	HEADER_GC_PARTY_REMOVE					= 80,
	HEADER_GC_QUEST_INFO					= 81,
	HEADER_GC_REQUEST_MAKE_GUILD			= 82,
	HEADER_GC_PARTY_PARAMETER				= 83,

	HEADER_GC_SAFEBOX_SET					= 85,
	HEADER_GC_SAFEBOX_DEL					= 86,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD		= 87,
	HEADER_GC_SAFEBOX_SIZE					= 88,

	HEADER_GC_FISHING						= 89,

	HEADER_GC_EMPIRE						= 90,

	HEADER_GC_PARTY_LINK					= 91,
	HEADER_GC_PARTY_UNLINK					= 92,

	HEADER_GC_REFINE_INFORMATION_OLD		= 95,

	HEADER_GC_VIEW_EQUIP					= 99,
		
	HEADER_GC_MARK_BLOCK					= 100,
	HEADER_GC_MARK_IDXLIST					= 102,
		
	HEADER_GC_TIME							= 106,
	HEADER_GC_CHANGE_NAME					= 107,
		
	HEADER_GC_DUNGEON						= 110,

	HEADER_GC_WALK_MODE						= 111,
	HEADER_GC_SKILL_GROUP					= 112,
	HEADER_GC_MAIN_CHARACTER				= 113,

	//	HEADER_GC_USE_POTION				= 114,
	HEADER_GC_SEPCIAL_EFFECT				= 114,
		
	HEADER_GC_NPC_POSITION					= 115,
		
	HEADER_GC_MATRIX_CARD					= 116,
	HEADER_GC_LOGIN_KEY						= 118,
	HEADER_GC_REFINE_INFORMATION			= 119,
	HEADER_GC_CHANNEL						= 121,

	// 122 HEADER_GC_MALL_OPEN
	HEADER_GC_TARGET_UPDATE					= 123,
	HEADER_GC_TARGET_DELETE					= 124,
	HEADER_GC_TARGET_CREATE					= 125,
		
	HEADER_GC_AFFECT_ADD					= 126,
	HEADER_GC_AFFECT_REMOVE					= 127,
		
	HEADER_GC_MALL_OPEN						= 122,
	HEADER_GC_MALL_SET						= 128,
	HEADER_GC_MALL_DEL						= 129,
		
	HEADER_GC_LAND_LIST						= 130,
	HEADER_GC_LOVER_INFO					= 131,
	HEADER_GC_LOVE_POINT_UPDATE				= 132,

	HEADER_GC_SYMBOL_DATA					= 133,
		
	// MINING		
	HEADER_GC_DIG_MOTION					= 134,
	// END_OF_MINING		
		
	HEADER_GC_DAMAGE_INFO					= 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO			= 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM			= 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL		= 138,
	// END_OF_SUPPORT_BGM

	HEADER_GC_AUTH_SUCCESS					= 150,
#ifdef ENABLE_SWITCHBOT
	HEADER_GC_SWITCHBOT						= 171,
#endif
	HEADER_GC_PANAMA_PACK					= 151,

	//HYBRID CRYPT
	HEADER_GC_HYBRIDCRYPT_KEYS				= 152,
	HEADER_GC_HYBRIDCRYPT_SDB				= 153, // SDB means Supplmentary Data Blocks
	//HYBRID CRYPT

	HEADER_GC_AUTH_SUCCESS_OPENID			= 154,
	
#ifdef __EVENT_MANAGER__
	HEADER_GC_EVENT_INFO					= 155,
	HEADER_GC_EVENT_RELOAD					= 156,
	HEADER_GC_EVENT_KW_SCORE				= 157,
#endif

#ifdef __GROWTH_PET_SYSTEM__
	HEADER_GC_PET							= 158,
	HEADER_GC_PET_SET						= 159,
	HEADER_GC_PET_SET_EXCHANGE				= 160,
	HEADER_GC_PET_DEL						= 161,
	HEADER_GC_PET_SUMMON					= 162,
	HEADER_GC_PET_POINT_CHANGE				= 163,
	HEADER_GC_PET_NAME_CHANGE_RESULT		= 164,
	HEADER_GC_PET_DETERMINE_RESULT			= 165,
	HEADER_GC_PET_ATTR_CHANGE_RESULT		= 166,
	HEADER_GC_PET_SKILL_UPDATE				= 167,
	HEADER_GC_PET_SKILL_COOLTIME			= 168,
#endif
#ifdef __EXTENDED_WHISPER_DETAILS__
	HEADER_GC_WHISPER_DETAILS = 169,
#endif
#ifdef ENABLE_SEARCH_ITEM_DROP_ON_MOB
	HEADER_GC_DROP_ITEM_SYSTEM		= 170,
#endif
	// ROULETTE
	HEADER_GC_ROULETTE						= 200,
	// END_ROULETTE
	//
	HEADER_GC_REQUEST_PASSPOD				= 202,
	HEADER_GC_REQUEST_PASSPOD_FAILED		= 203,

	HEADER_GC_HS_REQUEST					= 204,
	HEADER_GC_XTRAP_CS1_REQUEST				= 205,

	HEADER_GC_SPECIFIC_EFFECT				= 208,

	HEADER_GC_DRAGON_SOUL_REFINE			= 209,
	HEADER_GC_RESPOND_CHANNELSTATUS			= 210,
#ifdef ENABLE_DS_SET
	HEADER_GC_DS_TABLE						= 212,
#endif
#ifdef __MULTI_LANGUAGE_SYSTEM__
	HEADER_GC_REQUEST_CHANGE_LANGUAGE = 216,
#endif
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	HEADER_GC_CUBE_RENEWAL 					= 217,
#endif
#ifdef __AURA_SYSTEM__
		HEADER_GC_AURA						= 218,
#endif
#ifdef ENABLE_REFINE_ELEMENT
	HEADER_GC_REFINE_ELEMENT				= 172,
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	HEADER_GC_67_BONUS_NEW 					= 220,
#endif

	/////////////////////////////////////////////////////////////////////////////

	HEADER_GG_LOGIN				= 1,
	HEADER_GG_LOGOUT				= 2,
	HEADER_GG_RELAY				= 3,
	HEADER_GG_NOTICE				= 4,
	HEADER_GG_SHUTDOWN				= 5,
	HEADER_GG_GUILD				= 6,
	HEADER_GG_DISCONNECT			= 7,
	HEADER_GG_SHOUT				= 8,
	HEADER_GG_SETUP				= 9,
	HEADER_GG_MESSENGER_ADD                     = 10,
	HEADER_GG_MESSENGER_REMOVE                  = 11,
	HEADER_GG_FIND_POSITION			= 12,
	HEADER_GG_WARP_CHARACTER			= 13,
	HEADER_GG_MESSENGER_MOBILE			= 14,
	HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX		= 15,
	HEADER_GG_TRANSFER				= 16,
	HEADER_GG_XMAS_WARP_SANTA			= 17,
	HEADER_GG_XMAS_WARP_SANTA_REPLY		= 18,
	HEADER_GG_RELOAD_CRC_LIST			= 19,
	HEADER_GG_LOGIN_PING			= 20,
	HEADER_GG_CHECK_CLIENT_VERSION		= 21,
	HEADER_GG_BLOCK_CHAT			= 22,

	HEADER_GG_BLOCK_EXCEPTION		= 24,
	HEADER_GG_SIEGE					= 25,
	HEADER_GG_MONARCH_NOTICE		= 26,
	HEADER_GG_MONARCH_TRANSFER		= 27,
	HEADER_GG_PCBANG_UPDATE			= 28,

	HEADER_GG_CHECK_AWAKENESS		= 29,
#ifdef ENABLE_SWITCHBOT
	HEADER_GG_SWITCHBOT					= 35,
#endif
#ifdef ENABLE_FULL_NOTICE
	HEADER_GG_BIG_NOTICE			= 30,
#endif
#ifdef __EVENT_MANAGER__
	HEADER_GG_EVENT_RELOAD			= 31,
	HEADER_GG_EVENT					= 32,
	HEADER_GG_EVENT_HIDE_AND_SEEK	= 33,
#endif
#ifdef __MULTI_LANGUAGE_SYSTEM__
	HEADER_GG_LOCALE_NOTICE = 34,
#endif
};

#pragma pack(1)
typedef struct SPacketGGSetup
{
	BYTE	bHeader;
	WORD	wPort;
	BYTE	bChannel;
} TPacketGGSetup;

typedef struct SPacketGGLogin
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD	dwPID;
	BYTE	bEmpire;
	long	lMapIndex;
	BYTE	bChannel;
#ifdef __MULTI_LANGUAGE_SYSTEM__
	BYTE bLanguage;
#endif
} TPacketGGLogin;

typedef struct SPacketGGLogout
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGLogout;

typedef struct SPacketGGRelay
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lSize;
} TPacketGGRelay;

typedef struct SPacketGGNotice
{
	BYTE	bHeader;
	long	lSize;
} TPacketGGNotice;

typedef struct SPacketGGMonarchNotice
{
	BYTE	bHeader;
	BYTE	bEmpire;
	long	lSize;
} TPacketGGMonarchNotice;

//FORKED_ROAD
typedef struct SPacketGGForkedMapInfo
{
	BYTE	bHeader;
	BYTE	bPass;
	BYTE	bSungzi;
} TPacketGGForkedMapInfo;
//END_FORKED_ROAD
typedef struct SPacketGGShutdown
{
	BYTE	bHeader;
} TPacketGGShutdown;

typedef struct SPacketGGGuild
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
} TPacketGGGuild;

enum
{
	GUILD_SUBHEADER_GG_CHAT,
	GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS,
};

typedef struct SPacketGGGuildChat
{
	BYTE	bHeader;
	BYTE	bSubHeader;
	DWORD	dwGuild;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGGuildChat;

typedef struct SPacketGGParty
{
	BYTE	header;
	BYTE	subheader;
	DWORD	pid;
	DWORD	leaderpid;
} TPacketGGParty;

enum
{
	PARTY_SUBHEADER_GG_CREATE,
	PARTY_SUBHEADER_GG_DESTROY,
	PARTY_SUBHEADER_GG_JOIN,
	PARTY_SUBHEADER_GG_QUIT,
};

typedef struct SPacketGGDisconnect
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGDisconnect;

typedef struct SPacketGGShout
{
	BYTE	bHeader;
	BYTE	bEmpire;
	char	szText[CHAT_MAX_LEN + 1];
} TPacketGGShout;

typedef struct SPacketGGXmasWarpSanta
{
	BYTE	bHeader;
	BYTE	bChannel;
	long	lMapIndex;
} TPacketGGXmasWarpSanta;

typedef struct SPacketGGXmasWarpSantaReply
{
	BYTE	bHeader;
	BYTE	bChannel;
} TPacketGGXmasWarpSantaReply;

typedef struct SMessengerData
{
	char        szMobile[MOBILE_MAX_LEN + 1];
} TMessengerData;

typedef struct SPacketGGMessenger
{
	BYTE        bHeader;
	char        szAccount[CHARACTER_NAME_MAX_LEN + 1];
	char        szCompanion[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGMessenger;

typedef struct SPacketGGMessengerMobile
{
	BYTE        bHeader;
	char        szName[CHARACTER_NAME_MAX_LEN + 1];
	char        szMobile[MOBILE_MAX_LEN + 1];
} TPacketGGMessengerMobile;

typedef struct SPacketGGFindPosition
{
	BYTE header;
	DWORD dwFromPID;
	DWORD dwTargetPID;
} TPacketGGFindPosition;

typedef struct SPacketGGWarpCharacter
{
	BYTE header;
	DWORD pid;
	long x;
	long y;
#ifdef ENABLE_CMD_WARP_IN_DUNGEON
	int mapIndex;
#endif
} TPacketGGWarpCharacter;

//  HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX	    = 15,

typedef struct SPacketGGGuildWarMapIndex
{
	BYTE bHeader;
	DWORD dwGuildID1;
	DWORD dwGuildID2;
	long lMapIndex;
} TPacketGGGuildWarMapIndex;

typedef struct SPacketGGTransfer
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lX, lY;
} TPacketGGTransfer;

typedef struct SPacketGGLoginPing
{
	BYTE	bHeader;
	char	szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGLoginPing;

typedef struct SPacketGGBlockChat
{
	BYTE	bHeader;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lBlockDuration;
} TPacketGGBlockChat;



typedef struct command_text
{
	BYTE	bHeader;
} TPacketCGText;


typedef struct command_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketCGHandshake;

typedef struct command_login
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
} TPacketCGLogin;

typedef struct command_login2
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	DWORD	dwLoginKey;
	DWORD	adwClientKey[4];
} TPacketCGLogin2;

typedef struct command_login3
{
	BYTE	header;
	char	login[LOGIN_MAX_LEN + 1];
	char	passwd[PASSWD_MAX_LEN + 1];
	DWORD	adwClientKey[4];
#ifdef __MULTI_LANGUAGE_SYSTEM__
	BYTE bLanguage;
#endif
} TPacketCGLogin3;

typedef struct command_login5
{
	BYTE	header;
	char	authKey[OPENID_AUTHKEY_LEN + 1];
	DWORD	adwClientKey[4];
} TPacketCGLogin5;

typedef struct command_matrix_card
{
	BYTE	bHeader;
	char	szAnswer[MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGMatrixCard;

typedef struct packet_matrix_card
{
	BYTE	bHeader;
	DWORD	dwRows;
	DWORD	dwCols;
} TPacketGCMatrixCard;

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
	BYTE header;
	BYTE subheader;
	int vnum_item;

}TPacketCGDropItemSend;

typedef struct info_drop_item_system
{
	char name_mob[100];
	DWORD id_mob;
	DWORD prob_item;
	int count_item;
}TInfoDropItemSystem;

typedef struct packet_receive_drop_item_system
{
	packet_receive_drop_item_system(): header(HEADER_GC_DROP_ITEM_SYSTEM)
	{}
	BYTE header;
	BYTE subheader;
	TInfoDropItemSystem info_drop;

}TPacketGCDropItemReceive;
#endif

typedef struct packet_login_key
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
} TPacketGCLoginKey;

typedef struct command_player_select
{
	BYTE	header;
	BYTE	index;
} TPacketCGPlayerSelect;

typedef struct command_player_delete
{
	BYTE	header;
	BYTE	index;
	char	private_code[8];
} TPacketCGPlayerDelete;

typedef struct command_player_create
{
	BYTE        header;
	BYTE        index;
	char        name[CHARACTER_NAME_MAX_LEN + 1];
	WORD        job;
	BYTE	shape;
	BYTE	Con;
	BYTE	Int;
	BYTE	Str;
	BYTE	Dex;
} TPacketCGPlayerCreate;

typedef struct command_player_create_success
{
	BYTE		header;
	BYTE		bAccountCharacterIndex;
	TSimplePlayer	player;
} TPacketGCPlayerCreateSuccess;


typedef struct command_attack
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwVID;
	BYTE	bCRCMagicCubeProcPiece;
	BYTE	bCRCMagicCubeFilePiece;
} TPacketCGAttack;

enum EMoveFuncType
{
	FUNC_WAIT,
	FUNC_MOVE,
	FUNC_ATTACK,
	FUNC_COMBO,
	FUNC_MOB_SKILL,
	_FUNC_SKILL,
	FUNC_MAX_NUM,
	FUNC_SKILL = 0x80,
};


typedef struct command_move
{
	BYTE	bHeader;
	BYTE	bFunc;
	BYTE	bArg;
	BYTE	bRot;
	long	lX;
	long	lY;
	DWORD	dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketCGSyncPositionElement;


typedef struct command_sync_position
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketCGSyncPosition;


typedef struct command_chat
{
	BYTE	header;
	WORD	size;
	BYTE	type;
} TPacketCGChat;


typedef struct command_whisper
{
	BYTE	bHeader;
	WORD	wSize;
	char 	szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_entergame
{
	BYTE	header;
} TPacketCGEnterGame;

typedef struct command_item_use
{
	BYTE 	header;
	TItemPos 	Cell;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE	header;
	TItemPos	Cell;
	TItemPos	TargetCell;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
#ifdef __ENABLE_CHEQUE_SYSTEM__
	DWORD	cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
	BYTE 	header;
	TItemPos 	Cell;
	DWORD	gold;
#ifdef __ENABLE_CHEQUE_SYSTEM__
	DWORD	cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
	BYTE	count;
} TPacketCGItemDrop2;

#ifdef __NEW_DROP_DIALOG__
typedef struct command_item_destroy
{
	BYTE header;
	TItemPos Cell;
} TPacketCGItemDestroy;
#endif

typedef struct command_item_move
{
	BYTE 	header;
	TItemPos	Cell;
	TItemPos	CellTo;
	BYTE	count;
} TPacketCGItemMove;

typedef struct command_item_pickup
{
	BYTE 	header;
	DWORD	vid;
} TPacketCGItemPickup;

typedef struct command_quickslot_add
{
	BYTE	header;
	BYTE	pos;
	TQuickslot	slot;
} TPacketCGQuickslotAdd;

typedef struct command_quickslot_del
{
	BYTE	header;
	BYTE	pos;
} TPacketCGQuickslotDel;

typedef struct command_quickslot_swap
{
	BYTE	header;
	BYTE	pos;
	BYTE	change_pos;
} TPacketCGQuickslotSwap;

enum
{
	SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2
};

typedef struct command_shop_buy
{
	BYTE	count;
} TPacketCGShopBuy;

typedef struct command_shop_sell
{
	BYTE	pos;
	BYTE	count;
} TPacketCGShopSell;

typedef struct command_shop
{
	BYTE	header;
	BYTE	subheader;
} TPacketCGShop;

typedef struct command_on_click
{
	BYTE	header;
	DWORD	vid;
} TPacketCGOnClick;

enum
{
	EXCHANGE_SUBHEADER_CG_START,	/* arg1 == vid of target character */
	EXCHANGE_SUBHEADER_CG_ITEM_ADD,	/* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ITEM_DEL,	/* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ELK_ADD,	/* arg1 == amount of gold */
	EXCHANGE_SUBHEADER_CG_ACCEPT,	/* arg1 == not used */
	EXCHANGE_SUBHEADER_CG_CANCEL,	/* arg1 == not used */
#ifdef __ENABLE_CHEQUE_SYSTEM__
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD,
#endif // __ENABLE_CHEQUE_SYSTEM__
};

typedef struct command_exchange
{
	BYTE	header;
	BYTE	sub_header;
	DWORD	arg1;
	BYTE	arg2;
	TItemPos	Pos;
} TPacketCGExchange;

typedef struct command_position
{
	BYTE	header;
	BYTE	position;
} TPacketCGPosition;

typedef struct command_script_answer
{
	BYTE	header;
	BYTE	answer;
	//char	file[32 + 1];
	//BYTE	answer[16 + 1];
} TPacketCGScriptAnswer;


typedef struct command_script_button
{
	BYTE        header;
	unsigned int	idx;
} TPacketCGScriptButton;

typedef struct command_quest_input_string
{
	BYTE header;
	char msg[64+1];
} TPacketCGQuestInputString;

typedef struct command_quest_confirm
{
	BYTE header;
	BYTE answer;
	DWORD requestPID;
} TPacketCGQuestConfirm;


typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[64+1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_handshake
{
	BYTE	bHeader;
	DWORD	dwHandshake;
	DWORD	dwTime;
	long	lDelta;
} TPacketGCHandshake;

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
	PHASE_LOGIN,
	PHASE_SELECT,
	PHASE_LOADING,
	PHASE_GAME,
	PHASE_DEAD,

	PHASE_CLIENT_CONNECTING,
	PHASE_DBCLIENT,
	PHASE_P2P,
	PHASE_AUTH,
	PHASE_TEEN,
	PHASE_PASSPOD,
};

typedef struct packet_phase
{
	BYTE	header;
	BYTE	phase;
} TPacketGCPhase;

typedef struct packet_bindudp
{
	BYTE	header;
	DWORD	addr;
	WORD	port;
} TPacketGCBindUDP;

enum
{
	LOGIN_FAILURE_ALREADY	= 1,
	LOGIN_FAILURE_ID_NOT_EXIST	= 2,
	LOGIN_FAILURE_WRONG_PASS	= 3,
	LOGIN_FAILURE_FALSE		= 4,
	LOGIN_FAILURE_NOT_TESTOR	= 5,
	LOGIN_FAILURE_NOT_TEST_TIME	= 6,
	LOGIN_FAILURE_FULL		= 7
};

typedef struct packet_login_success
{
	BYTE				bHeader;
	TSimplePlayer		players[PLAYER_PER_ACCOUNT];
	DWORD				guild_id[PLAYER_PER_ACCOUNT];
	char				guild_name[PLAYER_PER_ACCOUNT][GUILD_NAME_MAX_LEN+1];

	DWORD				handle;
	DWORD				random_key;
} TPacketGCLoginSuccess;

typedef struct packet_auth_success
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
	BYTE	bResult;
} TPacketGCAuthSuccess;

typedef struct packet_auth_success_openid
{
	BYTE	bHeader;
	DWORD	dwLoginKey;
	BYTE	bResult;
	char	login[LOGIN_MAX_LEN + 1];
} TPacketGCAuthSuccessOpenID;

typedef struct packet_login_failure
{
	BYTE	header;
	char	szStatus[ACCOUNT_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct packet_create_failure
{
	BYTE	header;
	BYTE	bType;
} TPacketGCCreateFailure;

enum
{
	ADD_CHARACTER_STATE_DEAD		= (1 << 0),
	ADD_CHARACTER_STATE_SPAWN		= (1 << 1),
	ADD_CHARACTER_STATE_GUNGON		= (1 << 2),
	ADD_CHARACTER_STATE_KILLER		= (1 << 3),
	ADD_CHARACTER_STATE_PARTY		= (1 << 4),
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
#ifdef __AURA_SYSTEM__
	CHR_EQUIPPART_AURA,
#endif
	CHR_EQUIPPART_NUM,
};

typedef struct packet_add_char
{
	BYTE	header;
	DWORD	dwVID;

	float	angle;
	long	x;
	long	y;
	long	z;

	BYTE	bType;
	WORD	wRaceNum;
	BYTE	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD	dwAffectFlag[2];
} TPacketGCCharacterAdd;

typedef struct packet_char_additional_info
{
	BYTE    header;
	DWORD   dwVID;
	char    name[CHARACTER_NAME_MAX_LEN + 1];
	WORD    awPart[CHR_EQUIPPART_NUM];
	BYTE	bEmpire;
	DWORD   dwGuildID;
	DWORD   dwLevel;
	short	sAlignment;
	BYTE	bPKMode;
	DWORD	dwMountVnum;
#ifdef ENABLE_REFINE_ELEMENT
	BYTE	bRefineElementType;
#endif
#ifdef __SKILL_COLOR_SYSTEM__
	DWORD	dwSkillColor[ESkillColorLength::MAX_SKILL_COUNT + ESkillColorLength::MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
#endif
#ifdef __GROWTH_PET_SYSTEM__
	BYTE	bCharacterSize;
#endif
#ifdef __MULTI_LANGUAGE_SYSTEM__
	BYTE bLanguage;
#endif
} TPacketGCCharacterAdditionalInfo;

/*
   typedef struct packet_update_char_old
   {
   BYTE	header;
   DWORD	dwVID;

   WORD        awPart[CHR_EQUIPPART_NUM];
   BYTE	bMovingSpeed;
   BYTE	bAttackSpeed;

   BYTE	bStateFlag;
   DWORD	dwAffectFlag[2];

   DWORD	dwGuildID;
   short	sAlignment;
   BYTE	bPKMode;
   DWORD	dwMountVnum;
   } TPacketGCCharacterUpdateOld;
 */

typedef struct packet_update_char
{
	BYTE	header;
	DWORD	dwVID;

	WORD        awPart[CHR_EQUIPPART_NUM];
	BYTE	bMovingSpeed;
	BYTE	bAttackSpeed;

	BYTE	bStateFlag;
	DWORD	dwAffectFlag[2];

	DWORD	dwGuildID;
	short	sAlignment;
	BYTE	bPKMode;
	DWORD	dwMountVnum;
	//WORD	wRaceNum;
#ifdef ENABLE_REFINE_ELEMENT
	BYTE	bRefineElementType;
#endif
#ifdef __SKILL_COLOR_SYSTEM__
	DWORD	dwSkillColor[ESkillColorLength::MAX_SKILL_COUNT + ESkillColorLength::MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
#endif
#ifdef __MULTI_LANGUAGE_SYSTEM__
	BYTE bLanguage;
#endif
} TPacketGCCharacterUpdate;

typedef struct packet_del_char
{
	BYTE	header;
	DWORD	id;
} TPacketGCCharacterDelete;

typedef struct packet_chat
{
	BYTE	header;
	WORD	size;
	BYTE	type;
	DWORD	id;
	BYTE	bEmpire;
} TPacketGCChat;

typedef struct packet_whisper
{
	BYTE	bHeader;
	WORD	wSize;
	BYTE	bType;
	char	szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCWhisper;

typedef struct packet_main_character
{
	BYTE        header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE    header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[MUSIC_NAME_LEN + 1];
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE    header;
	DWORD	dwVID;
	WORD	wRaceNum;
	char	szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char	szBGMName[MUSIC_NAME_LEN + 1];
	float	fBGMVol;
	long	lx, ly, lz;
	BYTE	empire;
	BYTE	skill_group;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

typedef struct packet_points
{
	BYTE	header;
	INT		points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_skill_level
{
	BYTE		bHeader;
	TPlayerSkill	skills[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct packet_point_change
{
	int		header;
	DWORD	dwVID;
	BYTE	type;
	long	amount;
	long	value;
} TPacketGCPointChange;

typedef struct packet_stun
{
	BYTE	header;
	DWORD	vid;
} TPacketGCStun;

typedef struct packet_dead
{
	BYTE	header;
	DWORD	vid;
} TPacketGCDead;

struct TPacketGCItemDelDeprecated
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
	BYTE	count;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD	dwRefineElement;
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};

typedef struct packet_item_set
{
	BYTE	header;
	TItemPos Cell;
	DWORD	vnum;
	BYTE	count;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD	dwRefineElement;
#endif
	DWORD	flags;
	DWORD	anti_flags;
	bool	highlight;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TPacketGCItemSet;

typedef struct packet_item_del
{
	BYTE	header;
	BYTE	pos;
} TPacketGCItemDel;

struct packet_item_use
{
	BYTE	header;
	TItemPos Cell;
	DWORD	ch_vid;
	DWORD	victim_vid;
	DWORD	vnum;
};

struct packet_item_move
{
	BYTE	header;
	TItemPos Cell;
	TItemPos CellTo;
};

typedef struct packet_item_update
{
	BYTE	header;
	TItemPos Cell;
	BYTE	count;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD	dwRefineElement;
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TPacketGCItemUpdate;

typedef struct packet_item_ground_add
{
	BYTE	bHeader;
	long 	x, y, z;
	DWORD	dwVID;
	DWORD	dwVnum;
} TPacketGCItemGroundAdd;

typedef struct packet_item_ownership
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_item_ground_del
{
	BYTE	bHeader;
	DWORD	dwVID;
} TPacketGCItemGroundDel;

struct packet_quickslot_add
{
	BYTE	header;
	BYTE	pos;
	TQuickslot	slot;
};

struct packet_quickslot_del
{
	BYTE	header;
	BYTE	pos;
};

struct packet_quickslot_swap
{
	BYTE	header;
	BYTE	pos;
	BYTE	pos_to;
};

struct packet_motion
{
	BYTE	header;
	DWORD	vid;
	DWORD	victim_vid;
	WORD	motion;
};

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
#ifdef __ENABLE_CHEQUE_SYSTEM__
	SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_CHEQUE,
#endif // __ENABLE_CHEQUE_SYSTEM__
#ifdef ENABLE_BUY_ITEMS_WORLDARD
	SHOP_SUBHEADER_GC_NOT_ENOUGH_COUNT,
#endif
};

struct packet_shop_item
{
	DWORD		vnum;
#ifdef FULL_YANG
	long long	price;
#else
	DWORD		price;
#endif
#ifdef __ENABLE_CHEQUE_SYSTEM__
	DWORD       cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
	BYTE		count;
	BYTE		display_pos;
#ifdef ENABLE_REFINE_ELEMENT
	DWORD		dwRefineElement;
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_BUY_ITEMS_WORLDARD
	DWORD 		item_vnum_buy;
	BYTE 		item_count_buy;
#endif
};

typedef struct packet_shop_start
{
	DWORD   owner_vid;
	struct packet_shop_item	items[SHOP_HOST_ITEM_MAX_NUM];
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
	BYTE			pos;
	struct packet_shop_item	item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	int				iPrice;
} TPacketGCShopUpdatePrice;

typedef struct packet_shop
{
	BYTE		header;
	WORD	size;
	BYTE		subheader;
} TPacketGCShop;

struct packet_exchange
{
	BYTE	header;
	BYTE	sub_header;
	BYTE	is_me;
	DWORD	arg1;	// vnum
	TItemPos	arg2;	// cell
	DWORD	arg3;	// count
#ifdef WJ_ENABLE_TRADABLE_ICON
	TItemPos	arg4;	// srccell
#endif
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#ifdef ENABLE_REFINE_ELEMENT
	DWORD	dwRefineElement;
#endif
};

enum EPacketTradeSubHeaders
{
	EXCHANGE_SUBHEADER_GC_START,	/* arg1 == vid */
	EXCHANGE_SUBHEADER_GC_ITEM_ADD,	/* arg1 == vnum  arg2 == pos  arg3 == count */
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,
	EXCHANGE_SUBHEADER_GC_GOLD_ADD,	/* arg1 == gold */
	EXCHANGE_SUBHEADER_GC_ACCEPT,	/* arg1 == accept */
	EXCHANGE_SUBHEADER_GC_END,		/* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_ALREADY,	/* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_LESS_GOLD,	/* arg1 == not used */
#ifdef __ENABLE_CHEQUE_SYSTEM__
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD,
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE,
#endif // __ENABLE_CHEQUE_SYSTEM__
};

struct packet_position
{
	BYTE	header;
	DWORD	vid;
	BYTE	position;
};

typedef struct packet_ping
{
	BYTE	header;
} TPacketGCPing;

struct packet_script
{
	BYTE	header;
	WORD	size;
	BYTE	skin;
	WORD	src_size;
};

typedef struct packet_change_speed
{
	BYTE		header;
	DWORD		vid;
	WORD		moving_speed;
} TPacketGCChangeSpeed;

struct packet_mount
{
	BYTE	header;
	DWORD	vid;
	DWORD	mount_vid;
	BYTE	pos;
	DWORD	x, y;
};

typedef struct packet_move
{
	BYTE		bHeader;
	BYTE		bFunc;
	BYTE		bArg;
	BYTE		bRot;
	DWORD		dwVID;
	long		lX;
	long		lY;
	DWORD		dwTime;
	DWORD		dwDuration;
} TPacketGCMove;


typedef struct packet_ownership
{
	BYTE		bHeader;
	DWORD		dwOwnerVID;
	DWORD		dwVictimVID;
} TPacketGCOwnership;


typedef struct packet_sync_position_element
{
	DWORD	dwVID;
	long	lX;
	long	lY;
} TPacketGCSyncPositionElement;


typedef struct packet_sync_position
{
	BYTE	bHeader;
	WORD	wSize;
} TPacketGCSyncPosition;

typedef struct packet_fly
{
	BYTE	bHeader;
	BYTE	bType;
	DWORD	dwStartVID;
	DWORD	dwEndVID;
} TPacketGCCreateFly;

typedef struct command_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
	BYTE		bHeader;
	DWORD		dwShooterVID;
	DWORD		dwTargetVID;
	long		x, y;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
	BYTE		bHeader;
	BYTE		bType;
} TPacketCGShoot;

typedef struct packet_duel_start
{
	BYTE	header;
	WORD	wSize;
} TPacketGCDuelStart;

enum EPVPModes
{
	PVP_MODE_NONE,
	PVP_MODE_AGREE,
	PVP_MODE_FIGHT,
	PVP_MODE_REVENGE
};

typedef struct packet_pvp
{
	BYTE        bHeader;
	DWORD       dwVIDSrc;
	DWORD       dwVIDDst;
	BYTE        bMode;
} TPacketGCPVP;

typedef struct command_use_skill
{
	BYTE	bHeader;
	DWORD	dwVnum;
	DWORD	dwVID;
} TPacketCGUseSkill;

typedef struct command_target
{
	BYTE	header;
	DWORD	dwVID;
} TPacketCGTarget;

typedef struct packet_target
{
	BYTE	header;
	DWORD	dwVID;
	BYTE	bHPPercent;
#ifdef __VIEW_TARGET_DECIMAL_HP__
	int		iMinHP;
	int		iMaxHP;
#endif
#ifdef __SEND_TARGET_ELEMENT__
	BYTE	bElement;
#endif
#ifdef ENABLE_HIDE_COSTUME_SYSTEM
	char		dwCostumeHideID[255 + 1];
#endif
} TPacketGCTarget;

#ifdef __SEND_TARGET_INFO__
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


typedef struct packet_warp
{
	BYTE	bHeader;
	long	lX;
	long	lY;
	long	lAddr;
	WORD	wPort;
#if defined(__BL_LOADING_TIP__)
	long	l_MapIndex;
#endif
} TPacketGCWarp;

typedef struct command_warp
{
	BYTE	bHeader;
} TPacketCGWarp;

struct packet_quest_info
{
	BYTE header;
	WORD size;
	WORD index;
#ifdef __QUEST_RENEWAL__
	WORD c_index;
#endif
	BYTE flag;
};

enum
{
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
	MESSENGER_SUBHEADER_GC_MOBILE
};

typedef struct packet_messenger
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_guild_list
{
	BYTE connected;
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildList;

typedef struct packet_messenger_guild_login
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogin;

typedef struct packet_messenger_guild_logout
{
	BYTE length;

	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogout;

typedef struct packet_messenger_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

typedef struct packet_messenger_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerListOnline;

enum
{
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_REMOVE,
	MESSENGER_SUBHEADER_CG_INVITE_ANSWER,
};

typedef struct command_messenger
{
	BYTE header;
	BYTE subheader;
} TPacketCGMessenger;

typedef struct command_messenger_add_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddByVID;

typedef struct command_messenger_add_by_name
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketCGMessengerAddByName;

typedef struct command_messenger_remove
{
	char login[LOGIN_MAX_LEN+1];
	//DWORD account;
} TPacketCGMessengerRemove;

typedef struct command_safebox_checkout
{
	BYTE	bHeader;
	BYTE	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
	BYTE	bHeader;
	BYTE	bSafePos;
	TItemPos	ItemPos;
} TPacketCGSafeboxCheckin;

///////////////////////////////////////////////////////////////////////////////////
// Party

typedef struct command_party_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketCGPartyParameter;

typedef struct paryt_parameter
{
	BYTE	bHeader;
	BYTE	bDistributeMode;
} TPacketGCPartyParameter;

typedef struct packet_party_add
{
	BYTE	header;
	DWORD	pid;
	char	name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCPartyAdd;

typedef struct command_party_invite
{
	BYTE	header;
	DWORD	vid;
} TPacketCGPartyInvite;

typedef struct packet_party_invite
{
	BYTE	header;
	DWORD	leader_vid;
} TPacketGCPartyInvite;

typedef struct command_party_invite_answer
{
	BYTE	header;
	DWORD	leader_vid;
	BYTE	accept;
} TPacketCGPartyInviteAnswer;

typedef struct packet_party_update
{
	BYTE	header;
	DWORD	pid;
	BYTE	role;
	BYTE	percent_hp;
	short	affects[7];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketGCPartyRemove;

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

typedef struct command_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
	BYTE header;
	DWORD pid;
	BYTE byRole;
	BYTE flag;
} TPacketCGPartySetState;

enum
{
	PARTY_SKILL_HEAL = 1,
	PARTY_SKILL_WARP = 2
};

typedef struct command_party_use_skill
{
	BYTE header;
	BYTE bySkillIndex;
	DWORD vid;
} TPacketCGPartyUseSkill;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketCGSafeboxSize;

typedef struct packet_safebox_wrong_password
{
	BYTE	bHeader;
} TPacketCGSafeboxWrongPassword;

typedef struct command_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
	BYTE	bHeader;
	BYTE	bEmpire;
} TPacketGCEmpire;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
	BYTE        bHeader;
	BYTE        bState;
	long	lMoney;
} TPacketCGSafeboxMoney;

typedef struct packet_safebox_money_change
{
	BYTE	bHeader;
	long	lMoney;
} TPacketGCSafeboxMoneyChange;

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
	GUILD_SUBHEADER_GC_WAR_SCORE,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

enum GUILD_SUBHEADER_CG
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

typedef struct packet_guild
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCGuild;

typedef struct packet_guild_name_t
{
	BYTE header;
	WORD size;
	BYTE subheader;
	DWORD	guildID;
	char	guildName[GUILD_NAME_MAX_LEN];
} TPacketGCGuildName;

typedef struct packet_guild_war
{
	DWORD	dwGuildSelf;
	DWORD	dwGuildOpp;
	BYTE	bType;
	BYTE 	bWarState;
} TPacketGCGuildWar;

typedef struct command_guild
{
	BYTE header;
	BYTE subheader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN+1];
} TPacketCGAnswerMakeGuild;

typedef struct command_guild_use_skill
{
	DWORD	dwVnum;
	DWORD	dwPID;
} TPacketCGGuildUseSkill;

// Guild Mark
typedef struct command_mark_login
{
	BYTE    header;
	DWORD   handle;
	DWORD   random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
	BYTE	header;
	DWORD	gid;
	BYTE	image[16*12*4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
	BYTE	header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
	BYTE	header;
	BYTE	imgIdx;
	DWORD	crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
	BYTE    header;
	DWORD	bufSize;
	WORD	count;

} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
	BYTE	header;
	DWORD	bufSize;
	BYTE	imgIdx;
	DWORD	count;

} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE	header;
	WORD	size;
	DWORD	guild_id;
} TPacketCGGuildSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE header;
	DWORD guild_id;
	DWORD crc;
	DWORD size;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketGCGuildSymbolData;

// Fishing

typedef struct command_fishing
{
	BYTE header;
	BYTE dir;
} TPacketCGFishing;

typedef struct packet_fishing
{
	BYTE header;
	BYTE subheader;
	DWORD info;
	BYTE dir;
} TPacketGCFishing;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
	FISHING_SUBHEADER_GC_FISH,
};

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	BYTE byItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
	BYTE	bHeader;
	char	szBuf[255 + 1];
} TPacketCGHack;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE bHeader;
	WORD size;
	BYTE subheader;
} TPacketGCDungeon;

typedef struct packet_dungeon_dest_position
{
	long x;
	long y;
} TPacketGCDungeonDestPosition;

typedef struct SPacketGCShopSign
{
	BYTE	bHeader;
	DWORD	dwVID;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
} TPacketGCShopSign;

typedef struct SPacketCGMyShop
{
	BYTE	bHeader;
	char	szSign[SHOP_SIGN_MAX_LEN + 1];
	BYTE	bCount;
#ifdef OFFLINE_SHOP
	BYTE	days;
#endif
} TPacketCGMyShop;

typedef struct SPacketGCTime
{
	BYTE	bHeader;
	time_t	time;
} TPacketGCTime;

enum
{
	WALKMODE_RUN,
	WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
	BYTE	header;
	DWORD	vid;
	BYTE	mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
	BYTE        header;
	BYTE        skill_group;
} TPacketGCChangeSkillGroup;

typedef struct SPacketCGRefine
{
	BYTE	header;
	BYTE	pos;
	BYTE	type;
} TPacketCGRefine;

typedef struct SPacketCGRequestRefineInfo
{
	BYTE	header;
	BYTE	pos;
} TPacketCGRequestRefineInfo;

typedef struct SPacketGCRefineInformaion
{
	BYTE	header;
	BYTE	type;
	BYTE	pos;
	DWORD	src_vnum;
	DWORD	result_vnum;
	BYTE	material_count;
	int		cost;
	int		prob;
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];
} TPacketGCRefineInformation;

struct TNPCPosition
{
	BYTE bType;
	char name[CHARACTER_NAME_MAX_LEN+1];
	long x;
	long y;
};

typedef struct SPacketGCNPCPosition
{
	BYTE header;
	WORD size;
	WORD count;

	// array of TNPCPosition
} TPacketGCNPCPosition;

typedef struct SPacketGCSpecialEffect
{
	BYTE header;
	BYTE type;
	DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketCGChangeName;

typedef struct SPacketGCChangeName
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN+1];
} TPacketGCChangeName;


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

typedef struct packet_channel
{
	BYTE header;
	BYTE channel;
} TPacketGCChannel;

typedef struct SEquipmentItemSet
{
	DWORD   vnum;
	BYTE    count;
	long    alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TEquipmentItemSet;

typedef struct pakcet_view_equip
{
	BYTE  header;
	DWORD vid;
	TEquipmentItemSet equips[WEAR_MAX_NUM];
} TPacketViewEquip;

typedef struct
{
	DWORD	dwID;
	long	x, y;
	long	width, height;
	DWORD	dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
	BYTE	header;
	WORD	size;
} TPacketGCLandList;

typedef struct
{
	BYTE	bHeader;
	long	lID;
	char	szName[32+1];
	DWORD	dwVID;
	BYTE	bType;
} TPacketGCTargetCreate;

typedef struct
{
	BYTE	bHeader;
	long	lID;
	long	lX, lY;
} TPacketGCTargetUpdate;

typedef struct
{
	BYTE	bHeader;
	long	lID;
} TPacketGCTargetDelete;

typedef struct
{
	BYTE		bHeader;
	TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct
{
	BYTE	bHeader;
	DWORD	dwType;
	BYTE	bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_lover_info
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE love_point;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE header;
	BYTE love_point;
} TPacketGCLovePointUpdate;

// MINING
typedef struct packet_dig_motion
{
	BYTE header;
	DWORD vid;
	DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;
// END_OF_MINING

// SCRIPT_SELECT_ITEM
typedef struct command_script_select_item
{
	BYTE header;
	DWORD selection;
} TPacketCGScriptSelectItem;
// END_OF_SCRIPT_SELECT_ITEM

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVID;
	BYTE flag;
	int damage;
} TPacketGCDamageInfo;

enum
{
	RUNUP_MATRIX_ANSWER_MAX_LEN = 4,
	RUNUP_MATRIX_QUIZ_MAX_LEN = 8 ,

};

typedef struct packet_runup_matrix_quiz
{
	BYTE    bHeader;
	char    szQuiz[RUNUP_MATRIX_QUIZ_MAX_LEN + 1];
} TPacketGCRunupMatrixQuiz;

typedef struct command_runup_matrix_answer
{
	BYTE    bHeader;
	char    szAnswer[RUNUP_MATRIX_ANSWER_MAX_LEN + 1];
} TPacketCGRunupMatrixAnswer;

typedef struct packet_passpod
{
	BYTE    bHeader;
	char    szAnswer[8+1];
} TPacketCGPasspod;

typedef struct packet_passpod_failed
{
	BYTE    bHeader;
	char    szMessage[128];
} TPacketCGPasspodFailed;

typedef struct tag_GGSiege
{
	BYTE	bHeader;
	BYTE	bEmpire;
	BYTE	bTowerCount;
} TPacketGGSiege;

typedef struct SPacketGGMonarchTransfer
{
	BYTE	bHeader;
	DWORD	dwTargetPID;
	long	x;
	long	y;
} TPacketMonarchGGTransfer;

typedef struct SPacketGGPCBangUpdate
{
	BYTE bHeader;
	unsigned long ulPCBangID;
} TPacketPCBangUpdate;

typedef struct SPacketGGCheckAwakeness
{
	BYTE bHeader;
} TPacketGGCheckAwakeness;

typedef struct SPacketGCPanamaPack
{
	BYTE	bHeader;
	char	szPackName[256];
	BYTE	abIV[32];
} TPacketGCPanamaPack;


typedef struct SPacketGCHybridCryptKeys
{
	SPacketGCHybridCryptKeys() : m_pStream(NULL) {}
	~SPacketGCHybridCryptKeys()
	{

		if( m_pStream )
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}

	DWORD GetStreamSize()
	{
		return sizeof(bHeader) + sizeof(WORD) + sizeof(int) + KeyStreamLen;
	}

	BYTE* GetStreamData()
	{
		if( m_pStream )
			delete[] m_pStream;

		uDynamicPacketSize = (WORD)GetStreamSize();

		m_pStream = new BYTE[ uDynamicPacketSize ];

		memcpy( m_pStream, &bHeader, 1 );
		memcpy( m_pStream+1, &uDynamicPacketSize, 2 );
		memcpy( m_pStream+3, &KeyStreamLen, 4 );

		if( KeyStreamLen > 0 )
			memcpy( m_pStream+7, pDataKeyStream, KeyStreamLen );

		return m_pStream;
	}

	BYTE	bHeader;
	WORD    uDynamicPacketSize;
	int		KeyStreamLen;
	BYTE*   pDataKeyStream;

private:
	BYTE* m_pStream;


} TPacketGCHybridCryptKeys;


typedef struct SPacketGCPackageSDB
{
	SPacketGCPackageSDB() : m_pDataSDBStream(NULL), m_pStream(NULL) {}
	~SPacketGCPackageSDB()
	{
		if( m_pStream )
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}

	DWORD GetStreamSize()
	{
		return sizeof(bHeader) + sizeof(WORD) + sizeof(int) + iStreamLen;
	}

	BYTE* GetStreamData()
	{
		if( m_pStream )
			delete[] m_pStream;

		uDynamicPacketSize =  GetStreamSize();

		m_pStream = new BYTE[ uDynamicPacketSize ];

		memcpy( m_pStream, &bHeader, 1 );
		memcpy( m_pStream+1, &uDynamicPacketSize, 2 );
		memcpy( m_pStream+3, &iStreamLen, 4 );

		if( iStreamLen > 0 )
			memcpy( m_pStream+7, m_pDataSDBStream, iStreamLen );

		return m_pStream;
	}

	BYTE	bHeader;
	WORD    uDynamicPacketSize;
	int		iStreamLen;
	BYTE*   m_pDataSDBStream;

private:
	BYTE* m_pStream;


} TPacketGCPackageSDB;

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

#define MAX_EFFECT_FILE_NAME 128
typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[MAX_EFFECT_FILE_NAME];
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
	DS_SUB_HEADER_DO_REFINE_GRADE,
	DS_SUB_HEADER_DO_REFINE_STEP,
	DS_SUB_HEADER_DO_REFINE_STRENGTH,
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
	TItemPos ItemGrid[DRAGON_SOUL_REFINE_GRID_SIZE];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

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

typedef struct SPacketCGStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
} TPacketCGStateCheck;

typedef struct SPacketGCStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
	unsigned char state;
} TPacketGCStateCheck;

#ifdef ENABLE_DS_SET
typedef struct STPacketDSTable {
	BYTE	bHeader;
	int		iType, iApplyCount, iBasicApplyValue[255], iAditionalApplyValue[255];
	float	fWeight;
} TPacketDSTable;
#endif

#ifdef ENABLE_ACCE_SYSTEM
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
#endif

#ifdef __SKILL_COLOR_SYSTEM__
typedef struct packet_skill_color
{
	BYTE		bheader;
	BYTE		skill;
	DWORD		col1;
	DWORD		col2;
	DWORD		col3;
	DWORD		col4;
	DWORD		col5;
} TPacketCGSkillColor;
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
	DWORD	allowCopyAttr;
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

typedef struct SItemData
{
	DWORD	vnum;
	BYTE	count;
	DWORD	flags;
	DWORD	anti_flags;
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
} TItemData;

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

#ifdef __AURA_SYSTEM__
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
	TItemData	pItem;
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

#ifdef __EVENT_MANAGER__
typedef struct SPacketGGReloadEvent
{
	BYTE bHeader;
} TPacketGGReloadEvent;

typedef struct SPacketGGEvent
{
	BYTE			bHeader;
	TEventTable		table;
	bool			bState;
} TPacketGGEvent;

typedef struct SPacketGGEventHideAndSeek
{
	BYTE			bHeader;
	int				iPosition;
	int				iRound;
} TPacketGGEventHideAndSeek;

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

#ifdef __GROWTH_PET_SYSTEM__
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

typedef struct SPacketGCPetSet
{
	BYTE		header;
	DWORD		dwID;
	DWORD		dwSummonItemVnum;
	char		szName[PET_NAME_MAX_SIZE + 1];
	TPetSkill	aSkill[PET_SKILL_COUNT_MAX];
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
	char		szName[PET_NAME_MAX_SIZE + 1];
} TPacketGCPetNameChangeResult;

typedef struct SPacketCGPetHatch
{
	BYTE		header;
	char		name[PET_NAME_MAX_SIZE + 1];
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
	char		name[PET_NAME_MAX_SIZE + 1];
	TItemPos	changeNamePos;
	TItemPos	upBringingPos;
} TPacketCGPetNameChange;
#endif

#ifdef __MULTI_LANGUAGE_SYSTEM__
typedef struct SPacketGGLocaleNotice
{
	BYTE bHeader;
	bool bBigFont;
	char szNotice[MAX_QUEST_NOTICE_ARGS + 1][CHAT_MAX_LEN + 1];
} TPacketGGLocaleNotice;

typedef struct SPacketChangeLanguage
{
	BYTE bHeader;
	BYTE bLanguage;
} TPacketChangeLanguage;
#endif

#ifdef __EXTENDED_WHISPER_DETAILS__
typedef struct SPacketCGWhisperDetails
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisperDetails;

typedef struct SPacketGCWhisperDetails
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
#ifdef __MULTI_LANGUAGE_SYSTEM__
	BYTE bLanguage;
#endif
} TPacketGCWhisperDetails;
#endif

#ifdef ENABLE_SWITCHBOT
struct TPacketGGSwitchbot
{
	BYTE bHeader;
	WORD wPort;
	TSwitchbotTable table;

	TPacketGGSwitchbot() : bHeader(HEADER_GG_SWITCHBOT), wPort(0)
	{
		table = {};
	}
};

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
	long	alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
};
#endif

// 1. Go at the end of the file and add:
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

#pragma pack()
#endif


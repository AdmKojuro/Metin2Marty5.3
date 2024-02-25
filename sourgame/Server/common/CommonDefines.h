#ifndef __INC_METIN2_COMMON_DEFINES_H__
#define __INC_METIN2_COMMON_DEFINES_H__

enum eCommonDefines {
	MAP_ALLOW_LIMIT = 32, // 32 default
};
/* General */
#define ENABLE_D_NJGUILD
#define ENABLE_FULL_NOTICE
#define ENABLE_NEWSTUFF
#define ENABLE_PORT_SECURITY
#define ENABLE_BELT_INVENTORY_EX
#define ENABLE_CMD_WARP_IN_DUNGEON
#define ENABLE_ANTI_PACKET_FLOOD
// #define ENABLE_ITEM_ATTR_COSTUME
// #define ENABLE_SEQUENCE_SYSTEM
/* General */
#define ENABLE_SWITCHBOT
#define ENABLE_SEARCH_ITEM_DROP_ON_MOB
#define ENABLE_SHOW_CHEST_DROP
#define ENABLE_BUY_ITEMS_WORLDARD // comprar objetos en tienda con otros objetos
#define ENABLE_EXTENDED_BATTLE_PASS 	// Extended Battlepass-System by Aslan
#ifdef ENABLE_EXTENDED_BATTLE_PASS
	#define RESTRICT_COMMAND_GET_INFO					GM_LOW_WIZARD
	#define RESTRICT_COMMAND_SET_MISSION				GM_IMPLEMENTOR
	#define RESTRICT_COMMAND_PREMIUM_ACTIVATE	GM_IMPLEMENTOR	
#endif
#define ENABLE_BIYOLOG
//#define BL_PRIVATESHOP_SEARCH_SYSTEM--------------------------- en reparacion
#define ENABLE_ATLAS_BOSS
/* Wolfman */
#define ENABLE_WOLFMAN_CHARACTER
#ifdef ENABLE_WOLFMAN_CHARACTER
#define USE_MOB_BLEEDING_AS_POISON
#define USE_MOB_CLAW_AS_DAGGER
// #define USE_ITEM_BLEEDING_AS_POISON
// #define USE_ITEM_CLAW_AS_DAGGER
#define USE_WOLFMAN_STONES
#define USE_WOLFMAN_BOOKS
#endif
#define ENABLE_PLAYER_PER_ACCOUNT5
/* Wolfman */

/* Costume Enable */
#define ENABLE_MOUNT_COSTUME_SYSTEM
#define ENABLE_WEAPON_COSTUME_SYSTEM
#define ENABLE_ACCE_SYSTEM	//fixed version
#define __AURA_SYSTEM__		//System auras
/* Costume Enable */

/* WJ */
#define __WJ_PICKUP_ITEM_EFFECT__ //if you want to see highlighted a new item when dropped or when exchanged
#define __WJ_SORT_LASTPLAYTIME__ // Last play time
/* WJ */

/* System Enable */
#define ENABLE_DICE_SYSTEM
#define ENABLE_EXTEND_INVEN_SYSTEM
//#define ENABLE_MAGIC_REDUCTION_SYSTEM
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
	//#define USE_MAGIC_REDUCTION_STONES
#endif
// if ENABLE_GLOBAL_GIFT is defined, the GMs can use the quest global_gift_management.quest to set a global gift
//#define ENABLE_GLOBAL_GIFT
//#define DISABLE_STOP_RIDING_WHEN_DIE //	if DISABLE_TOP_RIDING_WHEN_DIE is defined , the player does not lose the horse after his death
#define __ENABLE_KILL_EVENT_FIX__ //if you want to fix the 0 exp problem about the when kill lua event (recommended)
#define ENABLE_HIDE_COSTUME_SYSTEM
#define ENABLE_HIDE_COSTUME_SYSTEM_ACCE
#define ENABLE_HIDE_COSTUME_SYSTEM_WEAPON_COSTUME
//#define ENABLE_HIDE_COSTUME_SYSTEM_TRANSMUTACION
/*OX*/
#define OFFLINE_SHOP // Offline shops system
#define GIFT_SYSTEM // gift system enable
//#define FULL_YANG // Enable support for yang type long long
#define SHOP_TIME_REFRESH 1*60 // time for cycle checking older shops
#define SHOP_BLOCK_GAME99 //Blocking create shops on channel 99
#define SHOP_ONLY_M1_M2
//#define SHOP_DISTANCE // Show shops in pos distance like WoM2
#define SHOP_AUTO_CLOSE //Enable auto closing shop after sell last item
//#define SHOP_ONLY_ALLOWED_INDEX //Enable limiting for other map index which is not defined in player.shop_limit
//#define SHOP_HIDE_NAME // Enable hidding shop npc names like "Player's shop"
#define SHOP_GM_PRIVILEGES GM_IMPLEMENTOR //Minimum GM privileges to using Shop GM Panel
#define ENABLE_OFFSHOP_DECO
#define BLOCK_MOUNT_COSTUME_NO_OX		//Bloqueia montaria no Mapa OX.
#define __SEND_TARGET_INFO__
#define __VIEW_TARGET_PLAYER_HP__
#define __VIEW_TARGET_DECIMAL_HP__
#define __SKILL_COLOR_SYSTEM__ //		Skill color system
#define ENABLE_GAYA_SYSTEM //			Gaya system
#define ENABLE_EXPRESSING_EMOTION //	Emotions system
#define __NEW_EXCHANGE_WINDOW__ //		Syste new exchange
#define ENABLE_CUBE_RENEWAL_WORLDARD
#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
	#define ENABLE_CUBE_RENEWAL_COPY_WORLDARD
	#define ENABLE_CUBE_RENEWAL_GEM_WORLDARD
#endif
#define __SEND_TARGET_ELEMENT__ // Target element
#define __ELEMENT_SYSTEM__ // Elements of monsters and pendants
#define __PENDANT_SYSTEM__ // Talismans of elements
#define __SOUL_SYSTEM__ // Soul system
#define __DS_GRADE_MYTH__ // < Dragon soul mythical grade
#define ENABLE_DS_SET
#define __QUEST_RENEWAL__ // Quest page renewal
#ifdef __QUEST_RENEWAL__
	#define _QR_MS_ // Marty Sama
#endif
#define __ENABLE_CHEQUE_SYSTEM__
#define __ENABLE_CHEQUE_OFFLINE_SHOP__
#define ENABLE_REFINE_ELEMENT
#define __NEW_DROP_DIALOG__ // New drop dialog with remove item option
#define __ATTR_METIN_BOSS__ // New attr metin & boss
#define __EVENT_MANAGER__
#define __GROWTH_PET_SYSTEM__
#define __MULTI_LANGUAGE_SYSTEM__ // Multi language system
#define __EXTENDED_WHISPER_DETAILS__ // Extended whisper details
#define __FLAG_IMAGE_SHOUT_LINE__
#define __BL_LOADING_TIP__
#define __DUNGEON_INFO_SYSTEM__
#define __SPECIAL_INVENTORY_SYSTEM__
#define __SORT_INVENTORY_ITEMS__
/* System Enable */

/* Modification & Correction */
#define ENABLE_EXIT_FAST
#define ENABLE_MOUNT_SYSTEM_MAP_BLOCK
/* Modification & Correction */

/* Enable for debug mod packet */
//#define ENABLE_SYSLOG_PACKET_SENT
/* Enable for debug mod packet */

#define __NEW_EVENT_HANDLER__

//#define ENABLE_6_7_BONUS_NEW_SYSTEM
#define WJ_ENABLE_TRADABLE_ICON	//Items en color rojo al comerciar/vender

#endif
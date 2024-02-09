import app
import uiScriptLocale
if app.ENABLE_SEND_TARGET_INFO:
	MONSTER_INFO_DATA = {}

ENABLE_FIX_TARGET_BOARD = True

if app.ENABLE_OFFICAL_CHARACTER_SCREEN:
	WOLF_MAN = "DISABLED"	# ENABLED/DISABLED
	WOLF_WOMEN = "DISABLED"	# ENABLED/DISABLED
	RESELECT_EMPIRE = 0

if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
	MULTI_LANGUAGE_NEED_RESTART_CLIENT = True # Enable the requirement to restart client after changing language in game

if app.ENABLE_GOOGLE_TRANSLATE_INGAME:
	AVAILABLE_LANGUAGES = {
		#'de': {
		#	'name': uiScriptLocale.LANGUAGE_DE,
		#	'encoding':'cp1252'
		#},
		'es': {
			'name': uiScriptLocale.LANGUAGE_ES,
			'encoding':'cp1252'
		},
		#'fr': {
		#	'name': uiScriptLocale.LANGUAGE_FR,
		#	'encoding':'cp1252'
		#},
		#'en': {
		#	'name': uiScriptLocale.LANGUAGE_EN,
		#	'encoding':'cp1252'
		#},
		# 'it': {
			# 'name': uiScriptLocale.LANGUAGE_IT,
			# 'encoding':'cp1252'
		# },
		#'pt': {
		#	'name': uiScriptLocale.LANGUAGE_PT,
		#	'encoding':'cp1252'
		#},
		# 'pt': {
			# 'name':'portuguese', 
			# 'encoding':'cp1252'
		# },
		#'tr': {
		#	'name': uiScriptLocale.LANGUAGE_TR, 
		#	'encoding':'cp1253'
		#},
		#'ro': {
		#	'name': uiScriptLocale.LANGUAGE_RO, 
		#	'encoding':'cp1252'
		#},
		# 'ru': {
			# 'name':'russian', 
			# 'encoding':'cp1250'
		# },
		# 'hu': {
			# 'name':'hungarian', 
			# 'encoding':'cp1250'
		# },
	}

# EXTRA BEGIN
# loads 5 (B,M,G,P,F) skills .mse
ENABLE_NEW_LEVELSKILL_SYSTEM = 0
# don't set a random channel when you open the client
ENABLE_RANDOM_CHANNEL_SEL = 0
# don't remove id&pass if the login attempt fails
ENABLE_CLEAN_DATA_IF_FAIL_LOGIN = 0
# ctrl+v will now work
ENABLE_PASTE_FEATURE = 0
# display all the bonuses added by a stone instead of the first one
ENABLE_FULLSTONE_DETAILS = 1
# enable successfulness % in the refine dialog
ENABLE_REFINE_PCT = 0
# extra ui features
EXTRA_UI_FEATURE = 1
#
NEW_678TH_SKILL_ENABLE = 1
# EXTRA END

INPUT_IGNORE = 0

# OFFLINE_SHOPS
shop_cost=[]
gift_items={}
MyShops=[]
# The range to see the shop name, previous this will be calculated in base of range bar.
SHOPNAMES_RANGE = 5000
#Decoration Shop
if app.ENABLE_OFFSHOP_DECO:
	decorationshop = ""
	decoration_w = 0
#Decoration Shop

# option
IN_GAME_SHOP_ENABLE = 1

DISABLE_MODEL_PREVIEW = 0

MINIMAP_CLOCKTWIX_ENABLE = 1

CONSOLE_ENABLE = 0

PVPMODE_ENABLE = 1
PVPMODE_TEST_ENABLE = 0
PVPMODE_ACCELKEY_ENABLE = 1
PVPMODE_ACCELKEY_DELAY = 0.5
PVPMODE_PROTECTED_LEVEL = 15

FOG_LEVEL0 = 4800.0
FOG_LEVEL1 = 9600.0
FOG_LEVEL2 = 12800.0
FOG_LEVEL = FOG_LEVEL0
FOG_LEVEL_LIST=[FOG_LEVEL0, FOG_LEVEL1, FOG_LEVEL2]

CAMERA_MAX_DISTANCE_SHORT = 2500.0
CAMERA_MAX_DISTANCE_LONG = 3500.0
CAMERA_MAX_DISTANCE_LIST=[CAMERA_MAX_DISTANCE_SHORT, CAMERA_MAX_DISTANCE_LONG]
CAMERA_MAX_DISTANCE = CAMERA_MAX_DISTANCE_SHORT

CHRNAME_COLOR_INDEX = 0

ENVIRONMENT_NIGHT="d:/ymir work/environment/moonlight04.msenv"

# constant
HIGH_PRICE = 500000
MIDDLE_PRICE = 50000
ERROR_METIN_STONE = 28960
SUB2_LOADING_ENABLE = 1
EXPANDED_COMBO_ENABLE = 1
CONVERT_EMPIRE_LANGUAGE_ENABLE = 0
USE_ITEM_WEAPON_TABLE_ATTACK_BONUS = 0
ADD_DEF_BONUS_ENABLE = 0
LOGIN_COUNT_LIMIT_ENABLE = 0

USE_SKILL_EFFECT_UPGRADE_ENABLE = 1

VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD = 1
GUILD_MONEY_PER_GSP = 100
GUILD_WAR_TYPE_SELECT_ENABLE = 1
TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE = 10

HAIR_COLOR_ENABLE = 1
ARMOR_SPECULAR_ENABLE = 1
WEAPON_SPECULAR_ENABLE = 1
SEQUENCE_PACKET_ENABLE = 1
KEEP_ACCOUNT_CONNETION_ENABLE = 1
MINIMAP_POSITIONINFO_ENABLE = 0

isItemQuestionDialog = 0

def GET_ITEM_QUESTION_DIALOG_STATUS():
	global isItemQuestionDialog
	return isItemQuestionDialog

def SET_ITEM_QUESTION_DIALOG_STATUS(flag):
	global isItemQuestionDialog
	isItemQuestionDialog = flag

import app
import net

########################

def SET_DEFAULT_FOG_LEVEL():
	global FOG_LEVEL
	app.SetMinFog(FOG_LEVEL)

def SET_FOG_LEVEL_INDEX(index):
	global FOG_LEVEL
	global FOG_LEVEL_LIST
	try:
		FOG_LEVEL=FOG_LEVEL_LIST[index]
	except IndexError:
		FOG_LEVEL=FOG_LEVEL_LIST[0]
	app.SetMinFog(FOG_LEVEL)

def GET_FOG_LEVEL_INDEX():
	global FOG_LEVEL
	global FOG_LEVEL_LIST
	return FOG_LEVEL_LIST.index(FOG_LEVEL)

########################

def SET_DEFAULT_CAMERA_MAX_DISTANCE():
	global CAMERA_MAX_DISTANCE
	app.SetCameraMaxDistance(CAMERA_MAX_DISTANCE)

def SET_CAMERA_MAX_DISTANCE_INDEX(index):
	global CAMERA_MAX_DISTANCE
	global CAMERA_MAX_DISTANCE_LIST
	try:
		CAMERA_MAX_DISTANCE=CAMERA_MAX_DISTANCE_LIST[index]
	except:
		CAMERA_MAX_DISTANCE=CAMERA_MAX_DISTANCE_LIST[0]

	app.SetCameraMaxDistance(CAMERA_MAX_DISTANCE)

def GET_CAMERA_MAX_DISTANCE_INDEX():
	global CAMERA_MAX_DISTANCE
	global CAMERA_MAX_DISTANCE_LIST
	return CAMERA_MAX_DISTANCE_LIST.index(CAMERA_MAX_DISTANCE)

########################

import chrmgr
import player
import app

def SET_DEFAULT_CHRNAME_COLOR():
	global CHRNAME_COLOR_INDEX
	chrmgr.SetEmpireNameMode(CHRNAME_COLOR_INDEX)

def SET_CHRNAME_COLOR_INDEX(index):
	global CHRNAME_COLOR_INDEX
	CHRNAME_COLOR_INDEX=index
	chrmgr.SetEmpireNameMode(index)

def GET_CHRNAME_COLOR_INDEX():
	global CHRNAME_COLOR_INDEX
	return CHRNAME_COLOR_INDEX

def SET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD(index):
	global VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD
	VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD = index

def GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD():
	global VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD
	return VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD

def SET_DEFAULT_CONVERT_EMPIRE_LANGUAGE_ENABLE():
	global CONVERT_EMPIRE_LANGUAGE_ENABLE
	net.SetEmpireLanguageMode(CONVERT_EMPIRE_LANGUAGE_ENABLE)

def SET_DEFAULT_USE_ITEM_WEAPON_TABLE_ATTACK_BONUS():
	global USE_ITEM_WEAPON_TABLE_ATTACK_BONUS
	player.SetWeaponAttackBonusFlag(USE_ITEM_WEAPON_TABLE_ATTACK_BONUS)

def SET_DEFAULT_USE_SKILL_EFFECT_ENABLE():
	global USE_SKILL_EFFECT_UPGRADE_ENABLE
	app.SetSkillEffectUpgradeEnable(USE_SKILL_EFFECT_UPGRADE_ENABLE)

def SET_TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE():
	global TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE
	app.SetTwoHandedWeaponAttSpeedDecreaseValue(TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE)

########################
import item

ACCESSORY_MATERIAL_LIST = [50623, 50624, 50625, 50626, 50627, 50628, 50629, 50630, 50631, 50632, 50633, 50634, 50635, 50636, 50637, 50638, 50639]
JewelAccessoryInfos = [
		# jewel		wrist	neck	ear
		[ 50634,	14420,	16220,	17220 ],
		[ 50635,	14500,	16500,	17500 ],
		[ 50636,	14520,	16520,	17520 ],
		[ 50637,	14540,	16540,	17540 ],
		[ 50638,	14560,	16560,	17560 ],
		[ 50639,	14570,	16570,	17570 ],
	]
def GET_ACCESSORY_MATERIAL_VNUM(vnum, subType):
	ret = vnum
	item_base = (vnum / 10) * 10
	for info in JewelAccessoryInfos:
		if item.ARMOR_WRIST == subType:
			if info[1] == item_base:
				return info[0]
		elif item.ARMOR_NECK == subType:
			if info[2] == item_base:
				return info[0]
		elif item.ARMOR_EAR == subType:
			if info[3] == item_base:
				return info[0]

	if vnum >= 16210 and vnum <= 16219:
		return 50625

	if item.ARMOR_WRIST == subType:
		WRIST_ITEM_VNUM_BASE = 14000
		ret -= WRIST_ITEM_VNUM_BASE
	elif item.ARMOR_NECK == subType:
		NECK_ITEM_VNUM_BASE = 16000
		ret -= NECK_ITEM_VNUM_BASE
	elif item.ARMOR_EAR == subType:
		EAR_ITEM_VNUM_BASE = 17000
		ret -= EAR_ITEM_VNUM_BASE

	type = ret/20

	if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
		type = (ret-170) / 20
		if type<0 or type>=len(ACCESSORY_MATERIAL_LIST):
			return 0

	return ACCESSORY_MATERIAL_LIST[type]

##################################################################
## ���� �߰��� '��Ʈ' ������ Ÿ�԰�, ��Ʈ�� ���Ͽ� ���� ������ ����..
## ��Ʈ�� ���Ͻý����� �Ǽ������� �����ϱ� ������, �� �Ǽ����� ���� �ϵ��ڵ�ó�� �̷������� �� ���ۿ� ����..

def GET_BELT_MATERIAL_VNUM(vnum, subType = 0):
	# ����� ��� ��Ʈ���� �ϳ��� ������(#18900)�� ���� ����
	return 18900

##################################################################
## �ڵ����� (HP: #72723 ~ #72726, SP: #72727 ~ #72730)

# �ش� vnum�� �ڵ������ΰ�?
def IS_AUTO_POTION(itemVnum):
	return IS_AUTO_POTION_HP(itemVnum) or IS_AUTO_POTION_SP(itemVnum)

# �ش� vnum�� HP �ڵ������ΰ�?
def IS_AUTO_POTION_HP(itemVnum):
	if 72723 <= itemVnum and 72726 >= itemVnum:
		return 1
	elif itemVnum >= 76021 and itemVnum <= 76022:		## ���� �� ������ ȭ���� �ູ
		return 1
	elif itemVnum == 79012:
		return 1

	return 0

# �ش� vnum�� SP �ڵ������ΰ�?
def IS_AUTO_POTION_SP(itemVnum):
	if 72727 <= itemVnum and 72730 >= itemVnum:
		return 1
	elif itemVnum >= 76004 and itemVnum <= 76005:		## ���� �� ������ ������ �ູ
		return 1
	elif itemVnum == 79013:
		return 1

	return 0

if app.ENABLE_GROWTH_PET_SYSTEM:
	def IS_PET_ITEM(itemVnum):
		if itemVnum == 0:
			return 0
			
		item.SelectItem(itemVnum)
		itemType = item.GetItemType()
		itemSubType = item.GetItemSubType()
		
		if itemType == item.ITEM_TYPE_PET and itemSubType == item.PET_UPBRINGING:
			return 1
			
		return 0

if app.ENABLE_MULTI_LANGUAGE_SYSTEM:
	def TRANSFORM_LANG(lang):
		langDict = { 1 : "en", 2 : "pt", 3 : "es", 4 : "fr", 5 : "de", 6 : "ro", 7 : "tr" }
		
		for key, value in langDict.iteritems():
			if key == lang:
				return value
			elif value == lang:
				return key
				
		return "en"
		
if app.ENABLE_GRAPHIC_ON_OFF:
	def IsShowSalesRange(vid):
		if not vid:
			return False

		try:
			VIEW_RANGE = 0

			(xShop, yShop, zShop) = chr.GetPixelPosition(vid)
			(x, y, z) = player.GetMainCharacterPosition()

			if systemSetting.GetPrivateShopLevel() == 1: VIEW_RANGE = 5000
			elif systemSetting.GetPrivateShopLevel() == 2: VIEW_RANGE = 2500
			elif systemSetting.GetPrivateShopLevel() == 3: VIEW_RANGE = 1000
			elif systemSetting.GetPrivateShopLevel() == 4: VIEW_RANGE = 500

			if (abs(x - xShop) < VIEW_RANGE and abs(y - yShop) < VIEW_RANGE) or systemSetting.GetPrivateShopLevel() < 1:
				return True
			else:
				return False
		except:
			return False
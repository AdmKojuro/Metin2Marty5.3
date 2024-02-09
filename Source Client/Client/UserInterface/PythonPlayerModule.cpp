#include "StdAfx.h"
#include "PythonPlayer.h"
#include "PythonApplication.h"
#include "../GameLib/GameLibDefines.h"

extern const DWORD c_iSkillIndex_Tongsol	= 121;
extern const DWORD c_iSkillIndex_Combo		= 122;
extern const DWORD c_iSkillIndex_Fishing	= 123;
extern const DWORD c_iSkillIndex_Mining		= 124;
extern const DWORD c_iSkillIndex_Making		= 125;
extern const DWORD c_iSkillIndex_Language1	= 126;
extern const DWORD c_iSkillIndex_Language2	= 127;
extern const DWORD c_iSkillIndex_Language3	= 128;
extern const DWORD c_iSkillIndex_Polymorph	= 129;
extern const DWORD c_iSkillIndex_Riding		= 130;
extern const DWORD c_iSkillIndex_Summon		= 131;

enum
{
	EMOTION_CLAP		= 1,
	EMOTION_CHEERS_1,
	EMOTION_CHEERS_2,
	EMOTION_DANCE_1,
	EMOTION_DANCE_2,
	EMOTION_DANCE_3,
	EMOTION_DANCE_4,
	EMOTION_DANCE_5,
	EMOTION_DANCE_6,
	EMOTION_CONGRATULATION,
	EMOTION_FORGIVE,
	EMOTION_ANGRY,

	EMOTION_ATTRACTIVE,
	EMOTION_SAD,
	EMOTION_SHY,
	EMOTION_CHEERUP,
	EMOTION_BANTER,
	EMOTION_JOY,

	EMOTION_KISS		= 51,
	EMOTION_FRENCH_KISS,
	EMOTION_SLAP,
#ifdef ENABLE_EXPRESSING_EMOTION
	EMOTION_PUSHUP 			= 60,
	EMOTION_DANCE_7			= 61,
	EMOTION_EXERCISE		= 62,
	EMOTION_DOZE			= 63,
	EMOTION_SELFIE			= 64,
#endif
};

std::map<int, CGraphicImage *> m_kMap_iEmotionIndex_pkIconImage;

extern int TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;


#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
class CBeltInventoryHelper
{
public:
	typedef BYTE	TGradeUnit;

	static TGradeUnit GetBeltGradeByRefineLevel(int refineLevel)
	{
		static TGradeUnit beltGradeByLevelTable[] =
		{
			0,
			1,			// +1
			1,			// +2
			2,			// +3
			2,			// +4,
			3,			// +5
			4,			// +6,
			5,			// +7,
			6,			// +8,
			7,			// +9
		};

		return beltGradeByLevelTable[refineLevel];
	}

	static const TGradeUnit* GetAvailableRuleTableByGrade()
	{


		static TGradeUnit availableRuleByGrade[c_Belt_Inventory_Slot_Count] = {
			1, 2, 4, 6,
			3, 3, 4, 6,
			5, 5, 5, 6,
			7, 7, 7, 7
		};

		return availableRuleByGrade;
	}

	static bool IsAvailableCell(WORD cell, int beltGrade /*int beltLevel*/)
	{
		//const TGradeUnit beltGrade = GetBeltGradeByRefineLevel(beltLevel);
		const TGradeUnit* ruleTable = GetAvailableRuleTableByGrade();

		return ruleTable[cell] <= beltGrade;
	}

};
#endif

PyObject * playerPickCloseItem(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().PickCloseItem();
	return Py_BuildNone();
}


PyObject * playerSetGameWindow(PyObject* poSelf, PyObject* poArgs)
{
	PyObject * pyHandle;
	if (!PyTuple_GetObject(poArgs, 0, &pyHandle))
		return Py_BadArgument();

	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetGameWindow(pyHandle);
	return Py_BuildNone();
}


PyObject * playerSetQuickCameraMode(PyObject* poSelf, PyObject* poArgs)
{
	int nIsEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nIsEnable))
		return Py_BadArgument();

	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetQuickCameraMode(nIsEnable ? true : false);

	return Py_BuildNone();
}

// Test Code
PyObject * playerSetMainCharacterIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CPythonPlayer::Instance().SetMainCharacterIndex(iVID);
	CPythonCharacterManager::Instance().SetMainInstance(iVID);

	return Py_BuildNone();
}
// Test Code

PyObject * playerGetMainCharacterIndex(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetMainCharacterIndex());
}

PyObject * playerGetMainCharacterName(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", CPythonPlayer::Instance().GetName());
}

PyObject * playerGetMainCharacterPosition(PyObject* poSelf, PyObject* poArgs)
{
	TPixelPosition kPPosMainActor;
	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_GetMainActorPosition(&kPPosMainActor);
	return Py_BuildValue("fff", kPPosMainActor.x, kPPosMainActor.y, kPPosMainActor.z);
}

PyObject * playerIsMainCharacterIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsMainCharacterIndex(iVID));
}

PyObject * playerCanAttackInstance(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Py_BuildValue("i", 0);
	if (!pTargetInstance)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pMainInstance->IsAttackableInstance(*pTargetInstance));
}

PyObject * playerIsActingEmotion(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pMainInstance)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pMainInstance->IsActingEmotion());
}

PyObject * playerIsPVPInstance(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Py_BuildValue("i", 0);
	if (!pTargetInstance)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pMainInstance->IsPVPInstance(*pTargetInstance));
}

PyObject * playerIsAntiFlagBySlot(PyObject * poSelf, PyObject * poArgs)
{
	TItemPos Cell;
	int antiflag;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &antiflag))
			return Py_BadArgument();
		break;

	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 2, &antiflag))
			return Py_BadArgument();
		break;
	default:
		return Py_BuildException();
	}

	DWORD itemAntiFlags = CPythonPlayer::Instance().GetItemAntiFlags(Cell);
	return Py_BuildValue("i", IS_SET(itemAntiFlags, antiflag) != 0 ? TRUE : FALSE);
}

PyObject * playerGetItemTypeBySlot(PyObject * poSelf, PyObject * poArgs)
{
	TItemPos Cell;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BadArgument();
		break;
	case 2:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BadArgument();
		break;
	default:
		return Py_BuildException();
	}

	return Py_BuildValue("i", CPythonPlayer::Instance().GetItemTypeBySlot(Cell));
}

PyObject * playerGetItemSubTypeBySlot(PyObject * poSelf, PyObject * poArgs)
{
	TItemPos Cell;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BadArgument();
		break;
	case 2:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BadArgument();
		break;
	default:
		return Py_BuildException();
	}

	return Py_BuildValue("i", CPythonPlayer::Instance().GetItemSubTypeBySlot(Cell));
}

PyObject * playerIsSameItemVnum(PyObject * poSelf, PyObject * poArgs)
{
	TItemPos Cell;
	int iItemVNum;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iItemVNum))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BadArgument();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iItemVNum))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 2, &Cell.cell))
			return Py_BadArgument();
		break;
	default:
		return Py_BuildException();
	}

	return Py_BuildValue("i", (iItemVNum == CPythonPlayer::Instance().GetItemIndex(Cell)) ? TRUE : FALSE);
}
PyObject * playerIsSameEmpire(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Py_BuildValue("i", FALSE);
	if (!pTargetInstance)
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", pMainInstance->IsSameEmpire(*pTargetInstance));
}

PyObject * playerIsChallengeInstance(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsChallengeInstance(iVID));
}

PyObject * playerIsRevengeInstance(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsRevengeInstance(iVID));
}

PyObject * playerIsCantFightInstance(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsCantFightInstance(iVID));
}

PyObject * playerGetCharacterDistance(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	CInstanceBase * pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);
	if (!pMainInstance)
		return Py_BuildValue("f", -1.0f);
	if (!pTargetInstance)
		return Py_BuildValue("f", -1.0f);

	return Py_BuildValue("f", pMainInstance->GetDistance(pTargetInstance));
}

PyObject * playerIsInSafeArea(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pMainInstance)
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", pMainInstance->IsInSafe());
}

PyObject * playerIsMountingHorse(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pMainInstance)
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", pMainInstance->IsMountingHorse());
}

PyObject * playerIsObserverMode(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	return Py_BuildValue("i", rkPlayer.IsObserverMode());
}

PyObject * playerActEmotion(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.ActEmotion(iVID);
	return Py_BuildNone();
}

PyObject * playerShowPlayer(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (pMainInstance)
		pMainInstance->GetGraphicThingInstanceRef().Show();
	return Py_BuildNone();
}

PyObject * playerHidePlayer(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (pMainInstance)
		pMainInstance->GetGraphicThingInstanceRef().Hide();
	return Py_BuildNone();
}


PyObject * playerComboAttack(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().NEW_Attack();
	return Py_BuildNone();
}


PyObject * playerRegisterEffect(PyObject* poSelf, PyObject* poArgs)
{
	int iEft;
	if (!PyTuple_GetInteger(poArgs, 0, &iEft))
		return Py_BadArgument();

	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	if (!rkPlayer.RegisterEffect(iEft, szFileName, false))
		return Py_BuildException("CPythonPlayer::RegisterEffect(eEft=%d, szFileName=%s", iEft, szFileName);

	return Py_BuildNone();
}

PyObject * playerRegisterCacheEffect(PyObject* poSelf, PyObject* poArgs)
{
	int iEft;
	if (!PyTuple_GetInteger(poArgs, 0, &iEft))
		return Py_BadArgument();

	char* szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BadArgument();

	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	if (!rkPlayer.RegisterEffect(iEft, szFileName, true))
		return Py_BuildException("CPythonPlayer::RegisterEffect(eEft=%d, szFileName=%s", iEft, szFileName);

	return Py_BuildNone();
}

PyObject * playerSetAttackKeyState(PyObject* poSelf, PyObject* poArgs)
{
	int isPressed;
	if (!PyTuple_GetInteger(poArgs, 0, &isPressed))
		return Py_BuildException("playerSetAttackKeyState(isPressed) - There is no first argument");

	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetAttackKeyState(isPressed ? true : false);
	return Py_BuildNone();
}

PyObject * playerSetSingleDIKKeyState(PyObject* poSelf, PyObject* poArgs)
{
	int eDIK;
	if (!PyTuple_GetInteger(poArgs, 0, &eDIK))
		return Py_BuildException("playerSetSingleDIKKeyState(eDIK, isPressed) - There is no first argument");

	int isPressed;
	if (!PyTuple_GetInteger(poArgs, 1, &isPressed))
		return Py_BuildException("playerSetSingleDIKKeyState(eDIK, isPressed) - There is no second argument");

	CPythonPlayer & rkPlayer = CPythonPlayer::Instance();
	rkPlayer.NEW_SetSingleDIKKeyState(eDIK, isPressed ? true : false);
	return Py_BuildNone();
}

PyObject * playerEndKeyWalkingImmediately(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().NEW_Stop();
	return Py_BuildNone();
}


PyObject * playerStartMouseWalking(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildNone();
}

PyObject * playerEndMouseWalking(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildNone();
}

PyObject * playerResetCameraRotation(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().NEW_ResetCameraRotation();
	return Py_BuildNone();
}

PyObject* playerSetAutoCameraRotationSpeed(PyObject* poSelf, PyObject* poArgs)
{
	float fCmrRotSpd;
	if (!PyTuple_GetFloat(poArgs, 0, &fCmrRotSpd))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.NEW_SetAutoCameraRotationSpeed(fCmrRotSpd);
	return Py_BuildNone();
}

PyObject* playerSetMouseState(PyObject* poSelf, PyObject* poArgs)
{
	int eMBT;
	if (!PyTuple_GetInteger(poArgs, 0, &eMBT))
		return Py_BuildException();

	int eMBS;
	if (!PyTuple_GetInteger(poArgs, 1, &eMBS))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.NEW_SetMouseState(eMBT, eMBS);

	return Py_BuildNone();
}

PyObject* playerSetMouseFunc(PyObject* poSelf, PyObject* poArgs)
{
	int eMBT;
	if (!PyTuple_GetInteger(poArgs, 0, &eMBT))
		return Py_BuildException();

	int eMBS;
	if (!PyTuple_GetInteger(poArgs, 1, &eMBS))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.NEW_SetMouseFunc(eMBT, eMBS);

	return Py_BuildNone();
}

PyObject* playerGetMouseFunc(PyObject* poSelf, PyObject* poArgs)
{
	int eMBT;
	if (!PyTuple_GetInteger(poArgs, 0, &eMBT))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	return Py_BuildValue("i", rkPlayer.NEW_GetMouseFunc(eMBT));
}

PyObject* playerSetMouseMiddleButtonState(PyObject* poSelf, PyObject* poArgs)
{
	int eMBS;
	if (!PyTuple_GetInteger(poArgs, 0, &eMBS))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.NEW_SetMouseMiddleButtonState(eMBS);

	return Py_BuildNone();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PyObject * playerGetName(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", CPythonPlayer::Instance().GetName());
}

PyObject * playerGetRace(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetRace());
}

#ifdef ENABLE_RENDER_TARGET
PyObject * playerGetArmor(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetArmor());
}

PyObject * playerGetWeapon(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetWeapon());
}

PyObject * playerGetHair(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetHair());
}

PyObject * playerGetAcce(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetAcce());
}

PyObject * playerGetAura(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetAura());
}

#endif

PyObject * playerGetJob(PyObject* poSelf, PyObject* poArgs)
{
	int race = CPythonPlayer::Instance().GetRace();
	int job = RaceToJob(race);
	return Py_BuildValue("i", job);
}

PyObject * playerGetPlayTime(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetPlayTime());
}

PyObject * playerSetPlayTime(PyObject* poSelf, PyObject* poArgs)
{
	int iTime;
	if (!PyTuple_GetInteger(poArgs, 0, &iTime))
		return Py_BuildException();

	CPythonPlayer::Instance().SetPlayTime(iTime);
	return Py_BuildNone();
}

PyObject * playerIsSkillCoolTime(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsSkillCoolTime(iSlotIndex));
}

PyObject * playerGetSkillCoolTime(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	float fCoolTime = CPythonPlayer::Instance().GetSkillCoolTime(iSlotIndex);
	float fElapsedCoolTime = CPythonPlayer::Instance().GetSkillElapsedCoolTime(iSlotIndex);
	return Py_BuildValue("ff", fCoolTime, fElapsedCoolTime);
}

PyObject * playerIsSkillActive(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsSkillActive(iSlotIndex));
}

PyObject * playerUseGuildSkill(PyObject* poSelf, PyObject* poArgs)
{
	int iSkillSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillSlotIndex))
		return Py_BuildException();

	CPythonPlayer::Instance().UseGuildSkill(iSkillSlotIndex);
	return Py_BuildNone();
}

PyObject * playerAffectIndexToSkillIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iAffectIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iAffectIndex))
		return Py_BuildException();

	DWORD dwSkillIndex;
	if (!CPythonPlayer::Instance().AffectIndexToSkillIndex(iAffectIndex, &dwSkillIndex))
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", dwSkillIndex);
}

PyObject * playerGetEXP(PyObject* poSelf, PyObject* poArgs)
{
	DWORD dwEXP = CPythonPlayer::Instance().GetStatus(POINT_EXP);
	return Py_BuildValue("l", dwEXP);
}

PyObject* playerGetLevel(PyObject* poSelf, PyObject* poArgs)
{
	DWORD dwLevel = CPythonPlayer::Instance().GetStatus(POINT_LEVEL);
	return Py_BuildValue("l", dwLevel);
}

PyObject * playerGetStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iType;
	if (!PyTuple_GetInteger(poArgs, 0, &iType))
		return Py_BuildException();

	long iValue = CPythonPlayer::Instance().GetStatus(iType);

	if (POINT_ATT_SPEED == iType)
	{
		CInstanceBase * pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
		if (pInstance && (CItemData::WEAPON_TWO_HANDED == pInstance->GetWeaponType()))
		{
			iValue -= TWOHANDED_WEWAPON_ATT_SPEED_DECREASE_VALUE;
		}
	}

	return Py_BuildValue("i", iValue);
}

PyObject * playerSetStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iType;
	if (!PyTuple_GetInteger(poArgs, 0, &iType))
		return Py_BuildException();

	int iValue;
	if (!PyTuple_GetInteger(poArgs, 1, &iValue))
		return Py_BuildException();

	CPythonPlayer::Instance().SetStatus(iType, iValue);
	return Py_BuildNone();
}

PyObject * playerGetElk(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetStatus(POINT_GOLD));
}

#ifdef ENABLE_CHEQUE_SYSTEM
PyObject * playerGetCheque(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetStatus(POINT_CHEQUE));
}
#endif

#ifdef ENABLE_GAYA_SYSTEM
PyObject * playerGetGaya(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetStatus(POINT_GAYA));
}
#endif

PyObject * playerGetGuildID(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (!pInstance)
		return Py_BuildValue("i", 0);
	return Py_BuildValue("i", pInstance->GetGuildID());
}

PyObject * playerGetGuildName(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	if (pInstance)
	{
		DWORD dwID = pInstance->GetGuildID();
		std::string strName;
		if (CPythonGuild::Instance().GetGuildName(dwID, &strName))
			return Py_BuildValue("s", strName.c_str());
	}
	return Py_BuildValue("s", "");
}

PyObject * playerGetAlignmentData(PyObject* poSelf, PyObject* poArgs)
{
	CInstanceBase * pInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	int iAlignmentPoint = 0;
	int iAlignmentGrade = 4;
	if (pInstance)
	{
		iAlignmentPoint = pInstance->GetAlignment();
		iAlignmentGrade = pInstance->GetAlignmentGrade();
	}
	return Py_BuildValue("ii", iAlignmentPoint, iAlignmentGrade);
}

PyObject * playerSetSkill(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSkillIndex))
		return Py_BuildException();

	CPythonPlayer::Instance().SetSkill(iSlotIndex, iSkillIndex);
	return Py_BuildNone();
}

PyObject * playerGetSkillIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().GetSkillIndex(iSlotIndex));
}

PyObject * playerGetSkillSlotIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Py_BuildException();

	DWORD dwSlotIndex;
	if (!CPythonPlayer::Instance().GetSkillSlotIndex(iSkillIndex, &dwSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", dwSlotIndex);
}

PyObject * playerGetSkillGrade(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().GetSkillGrade(iSlotIndex));
}

PyObject * playerGetSkillLevel(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().GetSkillLevel(iSlotIndex));
}

PyObject * playerGetSkillCurrentEfficientPercentage(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("f", CPythonPlayer::Instance().GetSkillCurrentEfficientPercentage(iSlotIndex));
}
PyObject * playerGetSkillNextEfficientPercentage(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	return Py_BuildValue("f", CPythonPlayer::Instance().GetSkillNextEfficientPercentage(iSlotIndex));
}

PyObject * playerClickSkillSlot(PyObject * poSelf, PyObject * poArgs)
{
	int iSkillSlot;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillSlot))
		return Py_BadArgument();

	CPythonPlayer::Instance().ClickSkillSlot(iSkillSlot);

	return Py_BuildNone();
}

PyObject * playerChangeCurrentSkillNumberOnly(PyObject * poSelf, PyObject * poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BadArgument();

	CPythonPlayer::Instance().ChangeCurrentSkillNumberOnly(iSlotIndex);

	return Py_BuildNone();
}

PyObject * playerClearSkillDict(PyObject * poSelf, PyObject * poArgs)
{
	CPythonPlayer::Instance().ClearSkillDict();
	return Py_BuildNone();
}

PyObject * playerMoveItem(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos srcCell;
	TItemPos dstCell;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		// int iSourceSlotIndex;
		if (!PyTuple_GetInteger(poArgs, 0, &srcCell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &dstCell.cell))
			return Py_BuildException();
		break;
	case 4:
		if (!PyTuple_GetByte(poArgs, 0, &srcCell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &srcCell.cell))
			return Py_BuildException();
		if (!PyTuple_GetByte(poArgs, 2, &dstCell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 3, &dstCell.cell))
			return Py_BuildException();
	default:
		return Py_BuildException();
	}

	CPythonPlayer::Instance().MoveItemData(srcCell, dstCell);
	return Py_BuildNone();
}

PyObject * playerSendClickItemPacket(PyObject* poSelf, PyObject* poArgs)
{
	int ivid;
	if (!PyTuple_GetInteger(poArgs, 0, &ivid))
		return Py_BuildException();

	CPythonPlayer::Instance().SendClickItemPacket(ivid);
	return Py_BuildNone();
}

PyObject * playerGetItemIndex(PyObject* poSelf, PyObject* poArgs)
{
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		{
			int iSlotIndex;
			if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Py_BuildException();

			int ItemIndex = CPythonPlayer::Instance().GetItemIndex(TItemPos (INVENTORY, iSlotIndex));
			return Py_BuildValue("i", ItemIndex);
		}
	case 2:
		{
			TItemPos Cell;
			if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Py_BuildException();
			if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Py_BuildException();

			int ItemIndex = CPythonPlayer::Instance().GetItemIndex(Cell);
			return Py_BuildValue("i", ItemIndex);
		}
	default:
		return Py_BuildException();

	}
}

PyObject * playerGetItemFlags(PyObject* poSelf, PyObject* poArgs)
{
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		{
			int iSlotIndex;
			if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Py_BuildException();

			DWORD flags = CPythonPlayer::Instance().GetItemFlags(TItemPos(INVENTORY, iSlotIndex));
			return Py_BuildValue("i", flags);
		}
	case 2:
		{
			TItemPos Cell;
			if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Py_BuildException();

			if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Py_BuildException();

			DWORD flags = CPythonPlayer::Instance().GetItemFlags(Cell);
			return Py_BuildValue("i", flags);
		}
	default:
		return Py_BuildException();
	}
}


PyObject * playerGetItemCount(PyObject* poSelf, PyObject* poArgs)
{
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		{
			int iSlotIndex;
			if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Py_BuildException();

			int ItemNum = CPythonPlayer::Instance().GetItemCount(TItemPos (INVENTORY, iSlotIndex));
			return Py_BuildValue("i", ItemNum);
		}
	case 2:
		{
			TItemPos Cell;
			if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Py_BuildException();

			if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Py_BuildException();

			int ItemNum = CPythonPlayer::Instance().GetItemCount(Cell);

			return Py_BuildValue("i", ItemNum);
		}
	default:
		return Py_BuildException();

	}
}

PyObject * playerSetItemCount(PyObject* poSelf, PyObject* poArgs)
{
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		{
			int iSlotIndex;
			if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Py_BuildException();

			BYTE bCount;
			if (!PyTuple_GetInteger(poArgs, 1, &bCount))
				return Py_BuildException();

			if (0 == bCount)
				return Py_BuildException();

			CPythonPlayer::Instance().SetItemCount(TItemPos (INVENTORY, iSlotIndex), bCount);
			return Py_BuildNone();
		}
	case 3:
		{
			TItemPos Cell;
			if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Py_BuildException();

			if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Py_BuildException();

			BYTE bCount;
			if (!PyTuple_GetInteger(poArgs, 2, &bCount))
				return Py_BuildException();

			CPythonPlayer::Instance().SetItemCount(Cell, bCount);

			return Py_BuildNone();
		}
	default:
		return Py_BuildException();

	}
}

PyObject * playerGetItemCountByVnum(PyObject* poSelf, PyObject* poArgs)
{
	int ivnum;
	if (!PyTuple_GetInteger(poArgs, 0, &ivnum))
		return Py_BuildException();

	int ItemNum = CPythonPlayer::Instance().GetItemCountByVnum(ivnum);
	return Py_BuildValue("i", ItemNum);
}

PyObject * playerGetItemMetinSocket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	int iMetinSocketIndex;

	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
			return Py_BuildException();

		break;
	case 3:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &iMetinSocketIndex))
			return Py_BuildException();

		break;

	default:
		return Py_BuildException();

	}
	int nMetinSocketValue = CPythonPlayer::Instance().GetItemMetinSocket(Cell, iMetinSocketIndex);
	return Py_BuildValue("i", nMetinSocketValue);
}

PyObject * playerGetItemAttribute(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	// int iSlotPos;
	int iAttributeSlotIndex;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &iAttributeSlotIndex))
			return Py_BuildException();

		break;
	case 3:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 2, &iAttributeSlotIndex))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}
	BYTE byType;
	short sValue;
	CPythonPlayer::Instance().GetItemAttribute(Cell, iAttributeSlotIndex, &byType, &sValue);

	return Py_BuildValue("ii", byType, sValue);
}

PyObject * playerGetItemLink(PyObject * poSelf, PyObject * poArgs)
{
	TItemPos Cell;

	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();
		break;
	case 2:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}
	const TItemData * pPlayerItem = CPythonPlayer::Instance().GetItemData(Cell);
	CItemData * pItemData = NULL;
	char buf[1024];

	if (pPlayerItem && CItemManager::Instance().GetItemDataPointer(pPlayerItem->vnum, &pItemData))
	{
		char itemlink[256 + 12];
		int len;
		bool isAttr = false;

#ifdef ENABLE_GROWTH_PET_SYSTEM
		if (pItemData->GetType() == CItemData::ITEM_TYPE_PET &&
			(pItemData->GetSubType() == CItemData::PET_UPBRINGING
				|| pItemData->GetSubType() == CItemData::PET_BAG))
		{
			CPythonPlayer::TPetData* pPet = nullptr;
			CPythonPlayer::Instance().GetPetInfo(pPlayerItem->alSockets[2], &pPet);

			len = snprintf(itemlink, sizeof(itemlink), "petitem:%x:%x:%x:%x:%x:",
				pPlayerItem->vnum, pPlayerItem->flags,
				pPlayerItem->alSockets[0], pPlayerItem->alSockets[1], pPlayerItem->alSockets[2]);

			if (pPet)
			{
				len += snprintf(itemlink + len, sizeof(itemlink) - len, "%x:%x:%x:%x:%x:%x:%x:%x:%x:%x:%s:%x:%x:%x:%x:%x:%x",
					pPet->GetLevel(),
					pPet->GetBirthday(),
					pPet->GetEvolution(),
					pPet->GetHP(),
					pPet->GetDef(),
					pPet->GetSP(),
					pPet->GetDuration(),
					pPet->GetMaxDuration(),
					pPet->GetSkillCount(),
					pPet->GetSummonItemVnum(),
					pPet->GetName().c_str(),
					pPet->skillData.aSkillInfo[0].bSkill,
					pPet->skillData.aSkillInfo[0].bLevel,
					pPet->skillData.aSkillInfo[1].bSkill,
					pPet->skillData.aSkillInfo[1].bLevel,
					pPet->skillData.aSkillInfo[2].bSkill,
					pPet->skillData.aSkillInfo[2].bLevel
				);
			}

				if (GetDefaultCodePage() == CP_ARABIC)
					snprintf(buf, sizeof(buf), " |h|r[%s]|cffffc700|H%s|h", pItemData->GetName(), itemlink);
				else 
					snprintf(buf, sizeof(buf), "|cffffc700|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
		}
		else
		{
			len = snprintf(itemlink, sizeof(itemlink), "item:%x:%x:%x:%x:%x:%x",
					pPlayerItem->vnum, pPlayerItem->flags,
					pPlayerItem->alSockets[0], pPlayerItem->alSockets[1], pPlayerItem->alSockets[2], pPlayerItem->dwRefineElement);

			for (int i = 0; i < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++i)
			{
				// if (pPlayerItem->aAttr[i].bType != 0) // @fixme009 (this line must be commented)
				{
					len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%d",
							pPlayerItem->aAttr[i].bType, pPlayerItem->aAttr[i].sValue);
					isAttr = true;
				}
			}

			if( GetDefaultCodePage() == CP_ARABIC ) {
				if (isAttr)
					snprintf(buf, sizeof(buf), " |h|r[%s]|cffffc700|H%s|h", pItemData->GetName(), itemlink);
				else
					snprintf(buf, sizeof(buf), " |h|r[%s]|cfff1e6c0|H%s|h", pItemData->GetName(), itemlink);
			} else {
				if (isAttr)
					snprintf(buf, sizeof(buf), "|cffffc700|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
				else
					snprintf(buf, sizeof(buf), "|cfff1e6c0|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
			}
		}
#else
		len = snprintf(itemlink, sizeof(itemlink), "item:%x:%x:%x:%x:%x:%x",
				pPlayerItem->vnum, pPlayerItem->flags,
				pPlayerItem->alSockets[0], pPlayerItem->alSockets[1], pPlayerItem->alSockets[2], pPlayerItem->dwRefineElement);

		for (int i = 0; i < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++i)
		{
			// if (pPlayerItem->aAttr[i].bType != 0) // @fixme009 (this line must be commented)
			{
				len += snprintf(itemlink + len, sizeof(itemlink) - len, ":%x:%d",
						pPlayerItem->aAttr[i].bType, pPlayerItem->aAttr[i].sValue);
				isAttr = true;
			}
		}

		if( GetDefaultCodePage() == CP_ARABIC ) {
			if (isAttr)
				snprintf(buf, sizeof(buf), " |h|r[%s]|cffffc700|H%s|h", pItemData->GetName(), itemlink);
			else
				snprintf(buf, sizeof(buf), " |h|r[%s]|cfff1e6c0|H%s|h", pItemData->GetName(), itemlink);
		} else {
			if (isAttr)
				snprintf(buf, sizeof(buf), "|cffffc700|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
			else
				snprintf(buf, sizeof(buf), "|cfff1e6c0|H%s|h[%s]|h|r", itemlink, pItemData->GetName());
		}
#endif
	}
	else
		buf[0] = '\0';

	return Py_BuildValue("s", buf);
}

PyObject * playerGetISellItemPrice(PyObject * poSelf, PyObject * poArgs)
{
	TItemPos Cell;

	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();
		break;
	case 2:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}
	CItemData * pItemData;

	if (!CItemManager::Instance().GetItemDataPointer(CPythonPlayer::Instance().GetItemIndex(Cell), &pItemData))
		return Py_BuildValue("i", 0);

	int iPrice;

	if (pItemData->IsFlag(CItemData::ITEM_FLAG_COUNT_PER_1GOLD))
		iPrice = CPythonPlayer::Instance().GetItemCount(Cell) / pItemData->GetISellItemPrice();
	else
		iPrice = pItemData->GetISellItemPrice() * CPythonPlayer::Instance().GetItemCount(Cell);

	iPrice /= 5;
	return Py_BuildValue("i", iPrice);
}

PyObject * playerGetQuickPage(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetQuickPage());
}

PyObject * playerSetQuickPage(PyObject* poSelf, PyObject* poArgs)
{
	int iPageIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iPageIndex))
		return Py_BuildException();

	CPythonPlayer::Instance().SetQuickPage(iPageIndex);
	return Py_BuildNone();
}

PyObject * playerLocalQuickSlotIndexToGlobalQuickSlotIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iLocalSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iLocalSlotIndex))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	return Py_BuildValue("i", rkPlayer.LocalQuickSlotIndexToGlobalQuickSlotIndex(iLocalSlotIndex));
}


PyObject * playerGetLocalQuickSlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	DWORD dwWndType;
	DWORD dwWndItemPos;

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.GetLocalQuickSlotData(iSlotIndex, &dwWndType, &dwWndItemPos);

	return Py_BuildValue("ii", dwWndType, dwWndItemPos);
}

PyObject * playerGetGlobalQuickSlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	DWORD dwWndType;
	DWORD dwWndItemPos;

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.GetGlobalQuickSlotData(iSlotIndex, &dwWndType, &dwWndItemPos);

	return Py_BuildValue("ii", dwWndType, dwWndItemPos);
}


PyObject * playerRequestAddLocalQuickSlot(PyObject * poSelf, PyObject * poArgs)
{
	int nSlotIndex;
	int nWndType;
	int nWndItemPos;

	if (!PyTuple_GetInteger(poArgs, 0, &nSlotIndex))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &nWndType))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 2, &nWndItemPos))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.RequestAddLocalQuickSlot(nSlotIndex, nWndType, nWndItemPos);

	return Py_BuildNone();
}

PyObject * playerRequestAddToEmptyLocalQuickSlot(PyObject* poSelf, PyObject* poArgs)
{
	int nWndType;
	if (!PyTuple_GetInteger(poArgs, 0, &nWndType))
		return Py_BuildException();

	int nWndItemPos;
	if (!PyTuple_GetInteger(poArgs, 1, &nWndItemPos))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.RequestAddToEmptyLocalQuickSlot(nWndType, nWndItemPos);

	return Py_BuildNone();
}

PyObject * playerRequestDeleteGlobalQuickSlot(PyObject * poSelf, PyObject * poArgs)
{
	int nGlobalSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &nGlobalSlotIndex))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.RequestDeleteGlobalQuickSlot(nGlobalSlotIndex);
	return Py_BuildNone();
}

PyObject * playerRequestMoveGlobalQuickSlotToLocalQuickSlot(PyObject * poSelf, PyObject * poArgs)
{
	int nGlobalSrcSlotIndex;
	int nLocalDstSlotIndex;

	if (!PyTuple_GetInteger(poArgs, 0, &nGlobalSrcSlotIndex))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &nLocalDstSlotIndex))
		return Py_BuildException();

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.RequestMoveGlobalQuickSlotToLocalQuickSlot(nGlobalSrcSlotIndex, nLocalDstSlotIndex);
	return Py_BuildNone();
}

PyObject * playerRequestUseLocalQuickSlot(PyObject* poSelf, PyObject* poArgs)
{
	int iLocalPosition;
	if (!PyTuple_GetInteger(poArgs, 0, &iLocalPosition))
		return Py_BuildException();

	CPythonPlayer::Instance().RequestUseLocalQuickSlot(iLocalPosition);

	return Py_BuildNone();
}

PyObject * playerRemoveQuickSlotByValue(PyObject* poSelf, PyObject* poArgs)
{
	int iType;
	if (!PyTuple_GetInteger(poArgs, 0, &iType))
		return Py_BuildException();

	int iPosition;
	if (!PyTuple_GetInteger(poArgs, 1, &iPosition))
		return Py_BuildException();

	CPythonPlayer::Instance().RemoveQuickSlotByValue(iType, iPosition);

	return Py_BuildNone();
}

PyObject * playerisItem(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	char Flag = CPythonPlayer::Instance().IsItem(TItemPos(INVENTORY, iSlotIndex));

	return Py_BuildValue("i", Flag);
}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
PyObject * playerIsBeltInventorySlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	char Flag = CPythonPlayer::Instance().IsBeltInventorySlot(TItemPos(INVENTORY, iSlotIndex));

	return Py_BuildValue("i", Flag);
}
#endif

PyObject * playerIsEquipmentSlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	if (iSlotIndex >= c_Equipment_Start)
	if (iSlotIndex <= c_DragonSoul_Equip_End)
		return Py_BuildValue("i", 1);

	return Py_BuildValue("i", 0);
}

PyObject * playerIsDSEquipmentSlot(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bWindowType))
		return Py_BuildException();
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSlotIndex))
		return Py_BuildException();

	if (INVENTORY == bWindowType)
	if (iSlotIndex >= c_DragonSoul_Equip_Start)
	if (iSlotIndex <= c_DragonSoul_Equip_End)
		return Py_BuildValue("i", 1);

	return Py_BuildValue("i", 0);

}

PyObject * playerIsCostumeSlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

#ifdef ENABLE_COSTUME_SYSTEM
	if (iSlotIndex >= c_Costume_Slot_Start)
	if (iSlotIndex <= c_Costume_Slot_End)
		return Py_BuildValue("i", 1);
#endif

	return Py_BuildValue("i", 0);
}

PyObject * playerIsOpenPrivateShop(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().IsOpenPrivateShop());
}

PyObject * playerIsValuableItem(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos SlotIndex;

	switch (PyTuple_Size (poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &SlotIndex.cell))
			return Py_BuildException();
		break;
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &SlotIndex.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &SlotIndex.cell)) // @fixme013 arg idx from 0 to 1
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}

	DWORD dwItemIndex = CPythonPlayer::Instance().GetItemIndex(SlotIndex);
	CItemManager::Instance().SelectItemData(dwItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find item data");

	BOOL hasMetinSocket = FALSE;
	BOOL isHighPrice = FALSE;

	for (int i = 0; i < METIN_SOCKET_COUNT; ++i)
		if (CPythonPlayer::METIN_SOCKET_TYPE_NONE != CPythonPlayer::Instance().GetItemMetinSocket(SlotIndex, i))
			hasMetinSocket = TRUE;

	DWORD dwValue = pItemData->GetISellItemPrice();
	if (dwValue > 5000)
		isHighPrice = TRUE;

	return Py_BuildValue("i", hasMetinSocket || isHighPrice);
}

int GetItemGrade(const char * c_szItemName)
{
	std::string strName = c_szItemName;
	if (strName.empty())
		return 0;

	char chGrade = strName[strName.length() - 1];
	if (chGrade < '0' || chGrade > '9')
		chGrade = '0';

	int iGrade = chGrade - '0';
	return iGrade;
}

PyObject * playerGetItemGrade(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos SlotIndex;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &SlotIndex.cell))
			return Py_BuildException();
		break;
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &SlotIndex.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &SlotIndex.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}

	int iItemIndex = CPythonPlayer::Instance().GetItemIndex(SlotIndex);
	CItemManager::Instance().SelectItemData(iItemIndex);
	CItemData * pItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pItemData)
		return Py_BuildException("Can't find item data");

	return Py_BuildValue("i", GetItemGrade(pItemData->GetName()));
}

enum
{
	REFINE_SCROLL_TYPE_MAKE_SOCKET = 1,
	REFINE_SCROLL_TYPE_UP_GRADE = 2,
};

enum
{
	REFINE_CANT,
	REFINE_OK,
	REFINE_ALREADY_MAX_SOCKET_COUNT,
	REFINE_NEED_MORE_GOOD_SCROLL,
	REFINE_CANT_MAKE_SOCKET_ITEM,
	REFINE_NOT_NEXT_GRADE_ITEM,
	REFINE_CANT_REFINE_METIN_TO_EQUIPMENT,
	REFINE_CANT_REFINE_ROD,
#ifdef ENABLE_AURA_SYSTEM
	REFINE_CANT_REFINE_AURA_ITEM,
#endif
};

PyObject * playerCanRefine(PyObject * poSelf, PyObject * poArgs)
{
	int iScrollItemIndex;
	TItemPos TargetSlotIndex;

	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
			return Py_BadArgument();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
			return Py_BadArgument();
		break;
	default:
		return Py_BadArgument();
	}

	if (CPythonPlayer::Instance().IsEquipmentSlot(TargetSlotIndex))
	{
		return Py_BuildValue("i", REFINE_CANT_REFINE_METIN_TO_EQUIPMENT);
	}

	// Scroll
	CItemManager::Instance().SelectItemData(iScrollItemIndex);
	CItemData * pScrollItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pScrollItemData)
		return Py_BuildValue("i", REFINE_CANT);
	int iScrollType = pScrollItemData->GetType();
	int iScrollSubType = pScrollItemData->GetSubType();
	if (iScrollType != CItemData::ITEM_TYPE_USE)
		return Py_BuildValue("i", REFINE_CANT);
	if (iScrollSubType != CItemData::USE_TUNING)
		return Py_BuildValue("i", REFINE_CANT);

	// Target Item
	int iTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemManager::Instance().SelectItemData(iTargetItemIndex);
	CItemData * pTargetItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pTargetItemData)
		return Py_BuildValue("i", REFINE_CANT);
	int iTargetType = pTargetItemData->GetType();
	//int iTargetSubType = pTargetItemData->GetSubType();
	if (CItemData::ITEM_TYPE_ROD == iTargetType)
		return Py_BuildValue("i", REFINE_CANT_REFINE_ROD);
#ifdef ENABLE_AURA_SYSTEM
	if (pTargetItemData->GetType() == CItemData::ITEM_TYPE_COSTUME && pTargetItemData->GetSubType() == CItemData::COSTUME_AURA)
		return Py_BuildValue("i", REFINE_CANT_REFINE_AURA_ITEM);
#endif
	if (pTargetItemData->HasNextGrade())
	{
		return Py_BuildValue("i", REFINE_OK);
	}
	else
	{
		return Py_BuildValue("i", REFINE_NOT_NEXT_GRADE_ITEM);
	}

	return Py_BuildValue("i", REFINE_CANT);
}

enum
{
	ATTACH_METIN_CANT,
	ATTACH_METIN_OK,
	ATTACH_METIN_NOT_MATCHABLE_ITEM,
	ATTACH_METIN_NO_MATCHABLE_SOCKET,
	ATTACH_METIN_NOT_EXIST_GOLD_SOCKET,
	ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT,
};

PyObject * playerCanAttachMetin(PyObject* poSelf, PyObject* poArgs)
{
	int iMetinItemID;
	TItemPos TargetSlotIndex;

	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iMetinItemID))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
			return Py_BuildException();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iMetinItemID))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}
	if (CPythonPlayer::Instance().IsEquipmentSlot(TargetSlotIndex))
	{
		return Py_BuildValue("i", ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT);
	}

	CItemData * pMetinItemData;
	if (!CItemManager::Instance().GetItemDataPointer(iMetinItemID, &pMetinItemData))
		return Py_BuildException("can't find item data");

	DWORD dwTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemData * pTargetItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwTargetItemIndex, &pTargetItemData))
		return Py_BuildException("can't find item data");

	DWORD dwMetinWearFlags = pMetinItemData->GetWearFlags();
	DWORD dwTargetWearFlags = pTargetItemData->GetWearFlags();
	if (0 == (dwMetinWearFlags & dwTargetWearFlags))
		return Py_BuildValue("i", ATTACH_METIN_NOT_MATCHABLE_ITEM);
	if (CItemData::ITEM_TYPE_ROD == pTargetItemData->GetType())
		return Py_BuildValue("i", ATTACH_METIN_CANT);

	BOOL bNotExistGoldSocket = FALSE;

	int iSubType = pMetinItemData->GetSubType();
	for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
	{
		DWORD dwSocketType = CPythonPlayer::Instance().GetItemMetinSocket(TargetSlotIndex, i);
		if (CItemData::METIN_NORMAL == iSubType)
		{
			if (CPythonPlayer::METIN_SOCKET_TYPE_SILVER == dwSocketType ||
				CPythonPlayer::METIN_SOCKET_TYPE_GOLD == dwSocketType)
			{
				return Py_BuildValue("i", ATTACH_METIN_OK);
			}
		}
		else if (CItemData::METIN_GOLD == iSubType)
		{
			if (CPythonPlayer::METIN_SOCKET_TYPE_GOLD == dwSocketType)
			{
				return Py_BuildValue("i", ATTACH_METIN_OK);
			}
			else if (CPythonPlayer::METIN_SOCKET_TYPE_SILVER == dwSocketType)
			{
				bNotExistGoldSocket = TRUE;
			}
		}
	}

	if (bNotExistGoldSocket)
	{
		return Py_BuildValue("i", ATTACH_METIN_NOT_EXIST_GOLD_SOCKET);
	}

	return Py_BuildValue("i", ATTACH_METIN_NO_MATCHABLE_SOCKET);
}

enum
{
	DETACH_METIN_CANT,
	DETACH_METIN_OK,
};

PyObject * playerCanDetach(PyObject * poSelf, PyObject * poArgs)
{
	int iScrollItemIndex;
	TItemPos TargetSlotIndex;
	switch (PyTuple_Size (poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
			return Py_BadArgument();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iScrollItemIndex))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
			return Py_BadArgument();
		break;
	default:
		return Py_BadArgument();
	}

	// Scroll
	CItemManager::Instance().SelectItemData(iScrollItemIndex);
	CItemData * pScrollItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pScrollItemData)
		return Py_BuildException("Can't find item data");

	int iScrollType = pScrollItemData->GetType();
	int iScrollSubType = pScrollItemData->GetSubType();
	if (iScrollType != CItemData::ITEM_TYPE_USE)
		return Py_BuildValue("i", DETACH_METIN_CANT);
	if (iScrollSubType != CItemData::USE_DETACHMENT)
		return Py_BuildValue("i", DETACH_METIN_CANT);

	// Target Item
	int iTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemManager::Instance().SelectItemData(iTargetItemIndex);
	CItemData * pTargetItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pTargetItemData)
		return Py_BuildException("Can't find item data");


#ifdef ENABLE_ACCE_SYSTEM
	if (pScrollItemData->GetValue(0) == ACCE_CLEAN_ATTR_VALUE0)
	{
		if ((pTargetItemData->GetType() != CItemData::ITEM_TYPE_COSTUME) || (pTargetItemData->GetSubType() != CItemData::COSTUME_ACCE))
			return Py_BuildValue("i", DETACH_METIN_CANT);

		const TItemData * pPlayerItem = CPythonPlayer::Instance().GetItemData(TargetSlotIndex);
		if (pPlayerItem)
		{
			if (pPlayerItem->alSockets[ACCE_ABSORBED_SOCKET] > 0)
				return Py_BuildValue("i", DETACH_METIN_OK);
			else
				return Py_BuildValue("i", DETACH_METIN_CANT);
		}
		else
			return Py_BuildValue("i", DETACH_METIN_CANT);
	}
#endif

	//int iTargetType = pTargetItemData->GetType();
	//int iTargetSubType = pTargetItemData->GetSubType();

	if (pTargetItemData->IsFlag(CItemData::ITEM_FLAG_REFINEABLE))
	{
		for (int iSlotCount = 0; iSlotCount < METIN_SOCKET_COUNT; ++iSlotCount)
			if (CPythonPlayer::Instance().GetItemMetinSocket(TargetSlotIndex, iSlotCount) > 2)
			{
				return Py_BuildValue("i", DETACH_METIN_OK);
			}
	}

	return Py_BuildValue("i", DETACH_METIN_CANT);
}

PyObject * playerCanUnlock(PyObject * poSelf, PyObject * poArgs)
{
	int iKeyItemIndex;
	TItemPos TargetSlotIndex;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iKeyItemIndex))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.cell))
			return Py_BadArgument();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iKeyItemIndex))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 1, &TargetSlotIndex.window_type))
			return Py_BadArgument();
		if (!PyTuple_GetInteger(poArgs, 2, &TargetSlotIndex.cell))
			return Py_BadArgument();
		break;
	default:
		return Py_BadArgument();
	}

	// Key
	CItemManager::Instance().SelectItemData(iKeyItemIndex);
	CItemData * pKeyItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pKeyItemData)
		return Py_BuildException("Can't find item data");
	int iKeyType = pKeyItemData->GetType();
	if (iKeyType != CItemData::ITEM_TYPE_TREASURE_KEY)
		return Py_BuildValue("i", FALSE);

	// Target Item
	int iTargetItemIndex = CPythonPlayer::Instance().GetItemIndex(TargetSlotIndex);
	CItemManager::Instance().SelectItemData(iTargetItemIndex);
	CItemData * pTargetItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pTargetItemData)
		return Py_BuildException("Can't find item data");
	int iTargetType = pTargetItemData->GetType();
	if (iTargetType != CItemData::ITEM_TYPE_TREASURE_BOX)
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", TRUE);
}

PyObject * playerIsRefineGradeScroll(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos ScrollSlotIndex;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &ScrollSlotIndex.cell))
			return Py_BuildException();
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &ScrollSlotIndex.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &ScrollSlotIndex.cell))
			return Py_BuildException();
	default:
		return Py_BuildException();
	}

	int iScrollItemIndex = CPythonPlayer::Instance().GetItemIndex(ScrollSlotIndex);
	CItemManager::Instance().SelectItemData(iScrollItemIndex);
	CItemData * pScrollItemData = CItemManager::Instance().GetSelectedItemDataPointer();
	if (!pScrollItemData)
		return Py_BuildException("Can't find item data");

	return Py_BuildValue("i", REFINE_SCROLL_TYPE_UP_GRADE == pScrollItemData->GetValue(0));
}

PyObject * playerUpdate(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().Update();
	return Py_BuildNone();
}

PyObject * playerRender(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildNone();
}

PyObject * playerClear(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().Clear();
	return Py_BuildNone();
}

PyObject * playerClearTarget(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().SetTarget(0);
	return Py_BuildNone();
}

PyObject * playerSetTarget(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CPythonPlayer::Instance().SetTarget(iVID);
	return Py_BuildNone();
}

PyObject * playerOpenCharacterMenu(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CPythonPlayer::Instance().OpenCharacterMenu(iVID);
	return Py_BuildNone();
}

PyObject * playerIsPartyMember(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().IsPartyMemberByVID(iVID));
}

PyObject * playerIsPartyLeader(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	DWORD dwPID;
	if (!CPythonPlayer::Instance().PartyMemberVIDToPID(iVID, &dwPID))
		return Py_BuildValue("i", FALSE);

	CPythonPlayer::TPartyMemberInfo * pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(dwPID, &pPartyMemberInfo))
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", CPythonPlayer::PARTY_ROLE_LEADER == pPartyMemberInfo->byState);
}

PyObject * playerIsPartyLeaderByPID(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();

	CPythonPlayer::TPartyMemberInfo * pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", CPythonPlayer::PARTY_ROLE_LEADER == pPartyMemberInfo->byState);
}

PyObject * playerGetPartyMemberHPPercentage(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();

	CPythonPlayer::TPartyMemberInfo * pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", pPartyMemberInfo->byHPPercentage);
}

PyObject * playerGetPartyMemberState(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();

	CPythonPlayer::TPartyMemberInfo * pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("i", pPartyMemberInfo->byState);
}

PyObject * playerGetPartyMemberAffects(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();

	CPythonPlayer::TPartyMemberInfo * pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(iPID, &pPartyMemberInfo))
		return Py_BuildValue("i", FALSE);

	return Py_BuildValue("iiiiiii",	pPartyMemberInfo->sAffects[0],
									pPartyMemberInfo->sAffects[1],
									pPartyMemberInfo->sAffects[2],
									pPartyMemberInfo->sAffects[3],
									pPartyMemberInfo->sAffects[4],
									pPartyMemberInfo->sAffects[5],
									pPartyMemberInfo->sAffects[6]);
}

PyObject * playerRemovePartyMember(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();

	CPythonPlayer::Instance().RemovePartyMember(iPID);
	return Py_BuildNone();
}

PyObject * playerExitParty(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::Instance().ExitParty();
	return Py_BuildNone();
}

PyObject * playerGetPKMode(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetPKMode());
}

PyObject * playerHasMobilePhoneNumber(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().HasMobilePhoneNumber());
}

PyObject * playerSetWeaponAttackBonusFlag(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	return Py_BuildNone();
}

PyObject * playerToggleCoolTime(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().__ToggleCoolTime());
}

PyObject * playerToggleLevelLimit(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().__ToggleLevelLimit());
}

PyObject * playerGetTargetVID(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetTargetVID());
}

PyObject * playerRegisterEmotionIcon(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	char * szFileName;
	if (!PyTuple_GetString(poArgs, 1, &szFileName))
		return Py_BuildException();

	CGraphicImage * pImage = (CGraphicImage *)CResourceManager::Instance().GetResourcePointer(szFileName);
	m_kMap_iEmotionIndex_pkIconImage.insert(make_pair(iIndex, pImage));

	return Py_BuildNone();
}

PyObject * playerGetEmotionIconImage(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	if (m_kMap_iEmotionIndex_pkIconImage.end() == m_kMap_iEmotionIndex_pkIconImage.find(iIndex))
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", m_kMap_iEmotionIndex_pkIconImage[iIndex]);
}

PyObject * playerSetItemData(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 1, &iVirtualID))
		return Py_BuildException();

	int iNum;
	if (!PyTuple_GetInteger(poArgs, 2, &iNum))
		return Py_BuildException();

	TItemData kItemInst;
	ZeroMemory(&kItemInst, sizeof(kItemInst));
	kItemInst.vnum=iVirtualID;
	kItemInst.count=iNum;
	CPythonPlayer::Instance().SetItemData(TItemPos(INVENTORY, iSlotIndex), kItemInst);
	return Py_BuildNone();
}

PyObject * playerSetItemMetinSocket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos ItemPos;
	int iMetinSocketNumber;
	int iNum;

	switch (PyTuple_Size(poArgs))
	{
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &ItemPos.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketNumber))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 2, &iNum))
			return Py_BuildException();

		break;
	case 4:
		if (!PyTuple_GetInteger(poArgs, 0, &ItemPos.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &ItemPos.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &iMetinSocketNumber))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 3, &iNum))
			return Py_BuildException();

		break;
	default:
		return Py_BuildException();
	}

	CPythonPlayer::Instance().SetItemMetinSocket(ItemPos, iMetinSocketNumber, iNum);
	return Py_BuildNone();
}

PyObject * playerSetItemAttribute(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos ItemPos;
	int iAttributeSlotIndex;
	int iAttributeType;
	int iAttributeValue;

	switch (PyTuple_Size(poArgs))
	{
	case 4:
		if (!PyTuple_GetInteger(poArgs, 0, &ItemPos.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &iAttributeSlotIndex))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 2, &iAttributeType))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 3, &iAttributeValue))
			return Py_BuildException();
		break;
	case 5:
		if (!PyTuple_GetInteger(poArgs, 0, &ItemPos.window_type))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &ItemPos.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 2, &iAttributeSlotIndex))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 3, &iAttributeType))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 4, &iAttributeValue))
			return Py_BuildException();
		break;
	}
	CPythonPlayer::Instance().SetItemAttribute(ItemPos, iAttributeSlotIndex, iAttributeType, iAttributeValue);
	return Py_BuildNone();
}

PyObject * playerSetAutoPotionInfo(PyObject* poSelf, PyObject* poArgs)
{
	int potionType = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &potionType))
		return Py_BadArgument();

	CPythonPlayer* player = CPythonPlayer::InstancePtr();

	CPythonPlayer::SAutoPotionInfo& potionInfo = player->GetAutoPotionInfo(potionType);

	if (!PyTuple_GetBoolean(poArgs, 1, &potionInfo.bActivated))
		return Py_BadArgument();

	if (!PyTuple_GetLong(poArgs, 2, &potionInfo.currentAmount))
		return Py_BadArgument();

	if (!PyTuple_GetLong(poArgs, 3, &potionInfo.totalAmount))
		return Py_BadArgument();

	if (!PyTuple_GetLong(poArgs, 4, &potionInfo.inventorySlotIndex))
		return Py_BadArgument();

	return Py_BuildNone();
}

PyObject * playerGetAutoPotionInfo(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer* player = CPythonPlayer::InstancePtr();

	int potionType = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &potionType))
		return Py_BadArgument();

	CPythonPlayer::SAutoPotionInfo& potionInfo = player->GetAutoPotionInfo(potionType);

	return Py_BuildValue("biii", potionInfo.bActivated, int(potionInfo.currentAmount), int(potionInfo.totalAmount), int(potionInfo.inventorySlotIndex));
}

PyObject * playerSlotTypeToInvenType(PyObject* poSelf, PyObject* poArgs)
{
	int slotType = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &slotType))
		return Py_BadArgument();

	return Py_BuildValue("i", SlotTypeToInvenType((BYTE)slotType));
}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
PyObject * playerIsEquippingBelt(PyObject* poSelf, PyObject* poArgs)
{
	const CPythonPlayer* player = CPythonPlayer::InstancePtr();
	bool bEquipping = false;

	const TItemData* data = player->GetItemData(TItemPos(EQUIPMENT, c_Equipment_Belt));

	if (NULL != data)
		bEquipping = 0 < data->count;

	return Py_BuildValue("b", bEquipping);

}

PyObject * playerIsAvailableBeltInventoryCell(PyObject* poSelf, PyObject* poArgs)
{
	const CPythonPlayer* player = CPythonPlayer::InstancePtr();
	const TItemData* pData = player->GetItemData(TItemPos(EQUIPMENT, c_Equipment_Belt));

	if (NULL == pData || 0 == pData->count)
		return Py_BuildValue("b", false);

	CItemManager::Instance().SelectItemData(pData->vnum);
	CItemData * pItem = CItemManager::Instance().GetSelectedItemDataPointer();

	long beltGrade = pItem->GetValue(0);

	int pos = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &pos))
		return Py_BadArgument();

	//return Py_BuildValue("b", CBeltInventoryHelper::IsAvailableCell(pos - c_Belt_Inventory_Slot_Start, GetItemGrade(pItem->GetName())));
	return Py_BuildValue("b", CBeltInventoryHelper::IsAvailableCell(pos - c_Belt_Inventory_Slot_Start, beltGrade));
}
#endif


PyObject* playerSendDragonSoulRefine(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bSubHeader;
	PyObject* pDic;
	TItemPos RefineItemPoses[DS_REFINE_WINDOW_MAX_NUM];
	if (!PyTuple_GetByte(poArgs, 0, &bSubHeader))
		return Py_BuildException();
	switch (bSubHeader)
	{
	case DS_SUB_HEADER_CLOSE:
		break;
	case DS_SUB_HEADER_DO_UPGRADE:
	case DS_SUB_HEADER_DO_IMPROVEMENT:
	case DS_SUB_HEADER_DO_REFINE:

		{
			if (!PyTuple_GetObject(poArgs, 1, &pDic))
				return Py_BuildException();
			int pos = 0;
			PyObject* key, *value;
			int size = PyDict_Size(pDic);

			while (PyDict_Next(pDic, &pos, &key, &value))
			{
				int i = PyInt_AsLong(key);
				if (i > DS_REFINE_WINDOW_MAX_NUM)
					return Py_BuildException();

				if (!PyTuple_GetByte(value, 0, &RefineItemPoses[i].window_type)
					|| !PyTuple_GetInteger(value, 1, &RefineItemPoses[i].cell))
				{
					return Py_BuildException();
				}
			}
		}
		break;
	}

	CPythonNetworkStream& rns=CPythonNetworkStream::Instance();
	rns.SendDragonSoulRefinePacket(bSubHeader, RefineItemPoses);

	return Py_BuildNone();
}

#ifdef ENABLE_SKILL_COLOR_SYSTEM
PyObject* playerSetSkillColor(PyObject* poSelf, PyObject* poArgs)
{
	int iSkillSlot;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillSlot))
		return Py_BadArgument();

	int iColor1;
	if (!PyTuple_GetInteger(poArgs, 1, &iColor1))
		return Py_BadArgument();

	int iColor2;
	if (!PyTuple_GetInteger(poArgs, 2, &iColor2))
		return Py_BadArgument();

	int iColor3;
	if (!PyTuple_GetInteger(poArgs, 3, &iColor3))
		return Py_BadArgument();

	int iColor4;
	if (!PyTuple_GetInteger(poArgs, 4, &iColor4))
		return Py_BadArgument();

	int iColor5;
	if (!PyTuple_GetInteger(poArgs, 5, &iColor5))
		return Py_BadArgument();

	if (iSkillSlot >= 255 || iSkillSlot < 0 || iColor1 < 0 || iColor2 < 0 || iColor3 < 0 || iColor4 < 0 || iColor5 < 0)
		return Py_BadArgument();

	CPythonNetworkStream& nstr = CPythonNetworkStream::Instance();
	nstr.SendSkillColorPacket(iSkillSlot, iColor1, iColor2, iColor3, iColor4, iColor5);

	return Py_BuildNone();
}

PyObject* playerGetSkillColor(PyObject* poSelf, PyObject* poArgs)
{
	int iSkillSlot;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillSlot))
		return Py_BadArgument();

	if (iSkillSlot >= 255 || iSkillSlot < 0)
		return Py_BadArgument();

	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();
	DWORD* dwSkillColor = pMainInstance->GetSkillColor(iSkillSlot);
	if (!dwSkillColor)
		return Py_BuildValue("iiiii", 0, 0, 0, 0, 0);

	return Py_BuildValue("iiiii", dwSkillColor[0], dwSkillColor[1], dwSkillColor[2], dwSkillColor[3], dwSkillColor[4]);
}
#endif

// PyObject* playerGetItemTypeBySlot(PyObject* poSelf, PyObject* poArgs)
// {
	// TItemPos Cell;
	// switch (PyTuple_Size(poArgs))
	// {
	// case 1:
		// if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			// return Py_BadArgument();
		// break;
	// case 2:
		// if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			// return Py_BadArgument();
		// if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			// return Py_BadArgument();
		// break;
	// default:
		// return Py_BuildException();
	// }

	// return Py_BuildValue("i", CPythonPlayer::Instance().GetItemTypeBySlot(Cell));
// }

// PyObject* playerGetItemSubTypeBySlot(PyObject* poSelf, PyObject* poArgs)
// {
	// TItemPos Cell;
	// switch (PyTuple_Size(poArgs))
	// {
	// case 1:
		// if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			// return Py_BadArgument();
		// break;
	// case 2:
		// if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			// return Py_BadArgument();
		// if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			// return Py_BadArgument();
		// break;
	// default:
		// return Py_BuildException();
	// }

	// return Py_BuildValue("i", CPythonPlayer::Instance().GetItemSubTypeBySlot(Cell));
// }

#ifdef ENABLE_REFINE_ELEMENT
PyObject * playerGetItemRefineElement(PyObject* poSelf, PyObject* poArgs)
{
	switch (PyTuple_Size(poArgs))
	{
		case 1:
		{
			int iSlotIndex;
			if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
				return Py_BuildException();
			
			DWORD dwRefineElement = CPythonPlayer::Instance().GetItemRefineElement(TItemPos(INVENTORY, iSlotIndex));
			return Py_BuildValue("i", dwRefineElement);
		}
		case 2:
		{
			TItemPos Cell;
			if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
				return Py_BuildException();
			
			if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
				return Py_BuildException();
			
			DWORD dwRefineElement = CPythonPlayer::Instance().GetItemRefineElement(Cell);
			return Py_BuildValue("i", dwRefineElement);
		}
		default:
			return Py_BuildException();
	}
}
#endif

#ifdef ENABLE_AURA_SYSTEM
PyObject* playerGetAuraItemID(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotPos))
		return Py_BadArgument();

	TItemData2* pAuraItemInstance;
	if (!CPythonPlayer::Instance().GetAuraItemDataPtr(iSlotPos, &pAuraItemInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pAuraItemInstance->vnum);
}

PyObject* playerGetAuraItemCount(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotPos))
		return Py_BadArgument();

	TItemData2* pAuraItemInstance;
	if (!CPythonPlayer::Instance().GetAuraItemDataPtr(iSlotPos, &pAuraItemInstance))
		return Py_BuildException();

	return Py_BuildValue("i", pAuraItemInstance->count);
}

PyObject* playerGetAuraItemMetinSocket(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotPos))
		return Py_BadArgument();
	int iMetinSocketIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iMetinSocketIndex))
		return Py_BadArgument();

	TItemData2* pAuraItemInstance;
	if (!CPythonPlayer::Instance().GetAuraItemDataPtr(iSlotPos, &pAuraItemInstance))
		return Py_BuildException();

	if (iMetinSocketIndex >= ITEM_SOCKET_SLOT_MAX_NUM || iMetinSocketIndex < 0)
		return Py_BuildException();

	return Py_BuildValue("i", pAuraItemInstance->alSockets[iMetinSocketIndex]);
}

PyObject* playerGetAuraItemAttribute(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotPos))
		return Py_BadArgument();
	int iAttributeSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iAttributeSlotIndex))
		return Py_BadArgument();

	TItemData2* pAuraItemInstance;
	if (!CPythonPlayer::Instance().GetAuraItemDataPtr(iSlotPos, &pAuraItemInstance) || (iAttributeSlotIndex >= ITEM_ATTRIBUTE_SLOT_MAX_NUM || iAttributeSlotIndex < 0))
		return Py_BuildValue("ii", 0, 0);

	TPlayerItemAttribute kAttr = pAuraItemInstance->aAttr[iAttributeSlotIndex];
	return Py_BuildValue("ii", kAttr.bType, kAttr.sValue);
}

PyObject* playerIsAuraRefineWindowEmpty(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().IsAuraRefineWindowEmpty());
}

PyObject* playerGetAuraCurrentItemSlotCount(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetAuraCurrentItemSlotCount());
}

PyObject* playerIsAuraRefineWindowOpen(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().IsAuraRefineWindowOpen());
}

PyObject* playerGetAuraRefineWindowType(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetAuraRefineWindowType());
}

PyObject* playerFineMoveAuraItemSlot(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().FineMoveAuraItemSlot());
}

PyObject* playerSetAuraActivatedItemSlot(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bAuraSlotPos;
	if (!PyTuple_GetByte(poArgs, 0, &bAuraSlotPos))
		return Py_BuildException();

	TItemPos ItemCell;
	if (!PyTuple_GetByte(poArgs, 1, &ItemCell.window_type))
		return Py_BuildException();
	if (!PyTuple_GetInteger(poArgs, 2, &ItemCell.cell))
		return Py_BuildException();

	CPythonPlayer::Instance().SetActivatedAuraSlot(bAuraSlotPos, ItemCell);
	return Py_BuildNone();
}

PyObject* playerFindActivatedAuraSlot(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos ItemCell;
	if (!PyTuple_GetByte(poArgs, 0, &ItemCell.window_type))
		return Py_BuildException();
	if (!PyTuple_GetInteger(poArgs, 1, &ItemCell.cell))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().FindActivatedAuraSlot(ItemCell));
}

PyObject* playerFindUsingAuraSlot(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bAuraSlotPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bAuraSlotPos))
		return Py_BuildException();

	TItemPos ItemCell = CPythonPlayer::Instance().FindUsingAuraSlot(bAuraSlotPos);
	return Py_BuildValue("(ii)", ItemCell.window_type, ItemCell.cell);
}

PyObject* playerGetAuraRefineInfo(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bLevel;
	if (!PyTuple_GetByte(poArgs, 0, &bLevel))
		return Py_BuildException();

	BYTE bRefineInfoIndex;
	if (!PyTuple_GetByte(poArgs, 1, &bRefineInfoIndex))
		return Py_BuildException();

	const int* info = GetAuraRefineInfo(bLevel);
	if (!info || bRefineInfoIndex < AURA_REFINE_INFO_STEP || bRefineInfoIndex >= AURA_REFINE_INFO_MAX)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", info[bRefineInfoIndex]);
}

PyObject* playerGetAuraRefineInfoExpPer(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bAuraRefineInfoSlot;
	if (!PyTuple_GetInteger(poArgs, 0, &bAuraRefineInfoSlot))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().GetAuraRefineInfoExpPct(bAuraRefineInfoSlot));
}

PyObject* playerGetAuraRefineInfoLevel(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bAuraRefineInfoSlot;
	if (!PyTuple_GetInteger(poArgs, 0, &bAuraRefineInfoSlot))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonPlayer::Instance().GetAuraRefineInfoLevel(bAuraRefineInfoSlot));
}
#endif

PyObject* playerNPOS(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("(ii)", NPOS.window_type, NPOS.cell);
}

PyObject* playerGetItemSlotIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	int slotIndex = CPythonPlayer::Instance().GetItemSlotIndex(iVnum);
	return Py_BuildValue("i", slotIndex);
}

#ifdef ENABLE_GROWTH_PET_SYSTEM
PyObject* playerSetOpenPetHatchingWindow(PyObject* poSelf, PyObject* poArgs)
{
	bool isOpen;
	if (!PyTuple_GetBoolean(poArgs, 0, &isOpen))
		return Py_BadArgument();

	CPythonPlayer::Instance().SetOpenPetHatchingWindow(isOpen);
	return Py_BuildNone();
}

PyObject* playerSetOpenPetNameChangeWindow(PyObject* poSelf, PyObject* poArgs)
{
	bool isOpen;
	if (!PyTuple_GetBoolean(poArgs, 0, &isOpen))
		return Py_BadArgument();

	CPythonPlayer::Instance().SetOpenPetNameChangeWindow(isOpen);
	return Py_BuildNone();
}

PyObject* playerSetOpenPetFeedWindow(PyObject* poSelf, PyObject* poArgs)
{
	bool isOpen;
	if (!PyTuple_GetBoolean(poArgs, 0, &isOpen))
		return Py_BadArgument();

	CPythonPlayer::Instance().SetOpenPetFeedWindow(isOpen);
	return Py_BuildNone();
}

PyObject* playerIsOpenPetHatchingWindow(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("b", CPythonPlayer::Instance().IsOpenPetHatchingWindow());
}

PyObject* playerIsOpenPetNameChangeWindow(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("b", CPythonPlayer::Instance().IsOpenPetNameChangeWindow());
}

PyObject* playerIsOpenPetFeedWindow(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("b", CPythonPlayer::Instance().IsOpenPetFeedWindow());
}

PyObject* playerSetOpenMall(PyObject* poSelf, PyObject* poArgs)
{
	bool isOpen;
	if (!PyTuple_GetBoolean(poArgs, 0, &isOpen))
		return Py_BadArgument();

	CPythonPlayer::Instance().SetOpenMall(isOpen);
	return Py_BuildNone();
}

PyObject* playerSetOpenSafeBox(PyObject* poSelf, PyObject* poArgs)
{
	bool isOpen;
	if (!PyTuple_GetBoolean(poArgs, 0, &isOpen))
		return Py_BadArgument();

	CPythonPlayer::Instance().SetOpenSafeBox(isOpen);
	return Py_BuildNone();
}

PyObject* playerIsOpenMall(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("b", CPythonPlayer::Instance().IsOpenMall());
}

PyObject* playerGetActivePetItemId(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonPlayer::Instance().GetActivePet());
}

PyObject* playerGetPetItemVNumInBag(PyObject* poSelf, PyObject* poArgs)
{
	int iPetID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPetID))
		return Py_BadArgument();

	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(iPetID, &pPet);

	if (!pPet)
		return Py_BuildValue("i", 0);

	return Py_BuildValue("i", pPet->GetSummonItemVnum());
}

PyObject* playerGetPetSkill(PyObject* poSelf, PyObject* poArgs)
{
	int iPetID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPetID))
		return Py_BadArgument();

	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(iPetID, &pPet);

	if (!pPet)
		return Py_BuildValue("iiiiiiiiii", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	return Py_BuildValue("iiiiiiiiii", 
		pPet->skillData.bSkillCount,
		pPet->skillData.aSkillInfo[0].bSkill,
		pPet->skillData.aSkillInfo[0].bLevel,
		pPet->skillData.aSkillInfo[0].dwCooltime,
		pPet->skillData.aSkillInfo[1].bSkill,
		pPet->skillData.aSkillInfo[1].bLevel,
		pPet->skillData.aSkillInfo[1].dwCooltime,
		pPet->skillData.aSkillInfo[2].bSkill,
		pPet->skillData.aSkillInfo[2].bLevel,
		pPet->skillData.aSkillInfo[2].dwCooltime);
}

PyObject* playerGetPetSkillByIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iPetID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPetID))
		return Py_BuildException();

	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &iSkillIndex))
		return Py_BadArgument();

	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(iPetID, &pPet);

	if (!pPet)
		return Py_BuildValue("iiiiiiiii", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	BYTE dwSkillLevel = pPet->skillData.aSkillInfo[iSkillIndex].bLevel - 1;
	BYTE dwNextSkillLevel = dwSkillLevel + 1;

	if (dwNextSkillLevel > PET_GROWTH_SKILL_LEVEL_MAX)
		dwNextSkillLevel = PET_GROWTH_SKILL_LEVEL_MAX;

	BYTE bBonusValue = (pPet->skillData.aSkillInfo[iSkillIndex].bLevel == PET_GROWTH_SKILL_LEVEL_MAX) ? 0 : 1;

	return Py_BuildValue("iiiiiiiii", 
		pPet->skillData.aSkillInfo[iSkillIndex].bSkill,
		pPet->skillData.aSkillInfo[iSkillIndex].bLevel,
		pPet->skillData.aSkillInfo[iSkillIndex].dwFormula1[dwSkillLevel],
		pPet->skillData.aSkillInfo[iSkillIndex].dwFormula2[dwSkillLevel],
		pPet->skillData.aSkillInfo[iSkillIndex].dwFormula3[dwSkillLevel],
		pPet->skillData.aSkillInfo[iSkillIndex].dwFormula1[dwNextSkillLevel],
		pPet->skillData.aSkillInfo[iSkillIndex].dwFormula2[dwNextSkillLevel],
		pPet->skillData.aSkillInfo[iSkillIndex].dwFormula3[dwNextSkillLevel],
		bBonusValue);
}

PyObject* playerGetActivePetItemVNum(PyObject* poSelf, PyObject* poArgs)
{
	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(CPythonPlayer::Instance().GetActivePet(), &pPet);
	return Py_BuildValue("i", pPet->GetSummonItemVnum());
}

PyObject* playerGetPetExpPoints(PyObject* poSelf, PyObject* poArgs)
{
	int iPetID = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPetID))
		return Py_BadArgument();

	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(iPetID, &pPet);

	if (!pPet)
		return Py_BuildValue("iiii", 0, 0, 0, 0);

	DWORD itemMaxEXP = pPet->GetNextEXP() / 10;
	DWORD nextEXP = pPet->GetNextEXP() - itemMaxEXP;

	return Py_BuildValue("iiii", pPet->GetEXP(), nextEXP, pPet->GetItemEXP(), itemMaxEXP);
}

PyObject* playerGetPetItem(PyObject* poSelf, PyObject* poArgs)
{
	int iPetID = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPetID))
		return Py_BadArgument();

	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(iPetID, &pPet);

	if(!pPet)
		return Py_BuildValue("iiisfff", 0, 0, 0, 0, 0, 0, 0);

	return Py_BuildValue("iiisfff",
		pPet->GetLevel(),
		pPet->GetEvolution(),
		pPet->GetBirthday(),
		pPet->GetName().c_str(),
		float(pPet->GetHP()) / 10.0f,
		float(pPet->GetDef()) / 10.0f,
		float(pPet->GetSP()) / 10.0f
		);
}

PyObject* playerGetPetLifeTime(PyObject* poSelf, PyObject* poArgs)
{
	int iPetID = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPetID))
		return Py_BadArgument();

	CPythonPlayer::TPetData* pPet = nullptr;
	CPythonPlayer::Instance().GetPetInfo(iPetID, &pPet);

	if (!pPet)
		return Py_BuildValue("ii", 0, 0);

	return Py_BuildValue("ii", pPet->GetDuration(), pPet->GetMaxDuration());
}

PyObject* playerCanUseGrowthPetQuickSlot(PyObject* poSelf, PyObject* poArgs)
{
	int iLocalSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iLocalSlotIndex))
		return Py_BadArgument();

	return Py_BuildValue("i", CPythonPlayer::Instance().CanUseGrowthPetQuickSlot(iLocalSlotIndex));
}
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
PyObject* playerIsSkillBookInventorySlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	char Flag = CPythonPlayer::Instance().IsSkillBookInventorySlot(TItemPos(INVENTORY, iSlotIndex));

	return Py_BuildValue("i", Flag);
}

PyObject* playerIsUpgradeItemsInventorySlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	char Flag = CPythonPlayer::Instance().IsUpgradeItemsInventorySlot(TItemPos(INVENTORY, iSlotIndex));

	return Py_BuildValue("i", Flag);
}

PyObject* playerIsStoneInventorySlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	char Flag = CPythonPlayer::Instance().IsStoneInventorySlot(TItemPos(INVENTORY, iSlotIndex));

	return Py_BuildValue("i", Flag);
}

PyObject* playerIsGiftBoxInventorySlot(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	char Flag = CPythonPlayer::Instance().IsGiftBoxInventorySlot(TItemPos(INVENTORY, iSlotIndex));

	return Py_BuildValue("i", Flag);
}
#endif

void initPlayer()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetAutoPotionInfo",			playerGetAutoPotionInfo,			METH_VARARGS },
		{ "SetAutoPotionInfo",			playerSetAutoPotionInfo,			METH_VARARGS },

		{ "PickCloseItem",				playerPickCloseItem,				METH_VARARGS },
		{ "SetGameWindow",				playerSetGameWindow,				METH_VARARGS },
		{ "RegisterEffect",				playerRegisterEffect,				METH_VARARGS },
		{ "RegisterCacheEffect",		playerRegisterCacheEffect,			METH_VARARGS },
		{ "SetMouseState",				playerSetMouseState,				METH_VARARGS },
		{ "SetMouseFunc",				playerSetMouseFunc,					METH_VARARGS },
		{ "GetMouseFunc",				playerGetMouseFunc,					METH_VARARGS },
		{ "SetMouseMiddleButtonState",	playerSetMouseMiddleButtonState,	METH_VARARGS },
		{ "SetMainCharacterIndex",		playerSetMainCharacterIndex,		METH_VARARGS },
		{ "GetMainCharacterIndex",		playerGetMainCharacterIndex,		METH_VARARGS },
		{ "GetMainCharacterName",		playerGetMainCharacterName,			METH_VARARGS },
		{ "GetMainCharacterPosition",	playerGetMainCharacterPosition,		METH_VARARGS },
		{ "IsMainCharacterIndex",		playerIsMainCharacterIndex,			METH_VARARGS },
		{ "CanAttackInstance",			playerCanAttackInstance,			METH_VARARGS },
		{ "IsActingEmotion",			playerIsActingEmotion,				METH_VARARGS },
		{ "IsPVPInstance",				playerIsPVPInstance,				METH_VARARGS },
		{ "IsSameEmpire",				playerIsSameEmpire,					METH_VARARGS },
		{ "IsChallengeInstance",		playerIsChallengeInstance,			METH_VARARGS },
		{ "IsRevengeInstance",			playerIsRevengeInstance,			METH_VARARGS },
		{ "IsCantFightInstance",		playerIsCantFightInstance,			METH_VARARGS },
		{ "GetCharacterDistance",		playerGetCharacterDistance,			METH_VARARGS },
		{ "IsInSafeArea",				playerIsInSafeArea,					METH_VARARGS },
		{ "IsMountingHorse",			playerIsMountingHorse,				METH_VARARGS },
		{ "IsObserverMode",				playerIsObserverMode,				METH_VARARGS },
		{ "ActEmotion",					playerActEmotion,					METH_VARARGS },

		{ "ShowPlayer",					playerShowPlayer,					METH_VARARGS },
		{ "HidePlayer",					playerHidePlayer,					METH_VARARGS },

		{ "ComboAttack",				playerComboAttack,					METH_VARARGS },

		{ "SetAutoCameraRotationSpeed",	playerSetAutoCameraRotationSpeed,	METH_VARARGS },
		{ "SetAttackKeyState",			playerSetAttackKeyState,			METH_VARARGS },
		{ "SetSingleDIKKeyState",		playerSetSingleDIKKeyState,			METH_VARARGS },
		{ "EndKeyWalkingImmediately",	playerEndKeyWalkingImmediately,		METH_VARARGS },
		{ "StartMouseWalking",			playerStartMouseWalking,			METH_VARARGS },
		{ "EndMouseWalking",			playerEndMouseWalking,				METH_VARARGS },
		{ "ResetCameraRotation",		playerResetCameraRotation,			METH_VARARGS },
		{ "SetQuickCameraMode",			playerSetQuickCameraMode,			METH_VARARGS },

		///////////////////////////////////////////////////////////////////////////////////////////

		{ "SetSkill",							playerSetSkill,								METH_VARARGS },
		{ "GetSkillIndex",						playerGetSkillIndex,						METH_VARARGS },
		{ "GetSkillSlotIndex",					playerGetSkillSlotIndex,					METH_VARARGS },
		{ "GetSkillGrade",						playerGetSkillGrade,						METH_VARARGS },
		{ "GetSkillLevel",						playerGetSkillLevel,						METH_VARARGS },
		{ "GetSkillCurrentEfficientPercentage",	playerGetSkillCurrentEfficientPercentage,	METH_VARARGS },
		{ "GetSkillNextEfficientPercentage",	playerGetSkillNextEfficientPercentage,		METH_VARARGS },
		{ "ClickSkillSlot",						playerClickSkillSlot,						METH_VARARGS },
		{ "ChangeCurrentSkillNumberOnly",		playerChangeCurrentSkillNumberOnly,			METH_VARARGS },
		{ "ClearSkillDict",						playerClearSkillDict,						METH_VARARGS },

		{ "GetItemIndex",						playerGetItemIndex,							METH_VARARGS },
		{ "GetItemFlags",						playerGetItemFlags,							METH_VARARGS },
		{ "GetItemCount",						playerGetItemCount,							METH_VARARGS },
		{ "GetItemCountByVnum",					playerGetItemCountByVnum,					METH_VARARGS },
		{ "GetItemMetinSocket",					playerGetItemMetinSocket,					METH_VARARGS },
		{ "GetItemAttribute",					playerGetItemAttribute,						METH_VARARGS },
		{ "GetISellItemPrice",					playerGetISellItemPrice,					METH_VARARGS },
		{ "MoveItem",							playerMoveItem,								METH_VARARGS },
		{ "SendClickItemPacket",				playerSendClickItemPacket,					METH_VARARGS },

		///////////////////////////////////////////////////////////////////////////////////////////

		{ "GetName",					playerGetName,						METH_VARARGS },
		{ "GetJob",						playerGetJob,						METH_VARARGS },
		{ "GetRace",					playerGetRace,						METH_VARARGS },

		{ "GetPlayTime",				playerGetPlayTime,					METH_VARARGS },
		{ "SetPlayTime",				playerSetPlayTime,					METH_VARARGS },
#ifdef ENABLE_RENDER_TARGET
		{ "GetArmor",					playerGetArmor,						METH_VARARGS },
		{ "GetHair",					playerGetHair,						METH_VARARGS },
		{ "GetWeapon",					playerGetWeapon,					METH_VARARGS },
		{ "GetAcce",					playerGetAcce,						METH_VARARGS },
		{ "GetAura",					playerGetAura,						METH_VARARGS },
#endif
		{ "IsSkillCoolTime",			playerIsSkillCoolTime,				METH_VARARGS },
		{ "GetSkillCoolTime",			playerGetSkillCoolTime,				METH_VARARGS },
		{ "IsSkillActive",				playerIsSkillActive,				METH_VARARGS },
		{ "UseGuildSkill",				playerUseGuildSkill,				METH_VARARGS },
		{ "AffectIndexToSkillIndex",	playerAffectIndexToSkillIndex,		METH_VARARGS },
		{ "GetEXP",						playerGetEXP,						METH_VARARGS },
		{ "GetLevel", playerGetLevel, METH_VARARGS },
		{ "GetStatus",					playerGetStatus,					METH_VARARGS },
		{ "SetStatus",					playerSetStatus,					METH_VARARGS },
		{ "GetElk",						playerGetElk,						METH_VARARGS },
		{ "GetMoney",					playerGetElk,						METH_VARARGS },
#ifdef ENABLE_CHEQUE_SYSTEM
		{ "GetCheque",					playerGetCheque,					METH_VARARGS },
#endif
#ifdef ENABLE_GAYA_SYSTEM
		{ "GetGaya",					playerGetGaya,						METH_VARARGS },
#endif
		{ "GetGuildID",					playerGetGuildID,					METH_VARARGS },
		{ "GetGuildName",				playerGetGuildName,					METH_VARARGS },
		{ "GetAlignmentData",			playerGetAlignmentData,				METH_VARARGS },
		{ "RequestAddLocalQuickSlot",					playerRequestAddLocalQuickSlot,						METH_VARARGS },
		{ "RequestAddToEmptyLocalQuickSlot",			playerRequestAddToEmptyLocalQuickSlot,				METH_VARARGS },
		{ "RequestDeleteGlobalQuickSlot",				playerRequestDeleteGlobalQuickSlot,					METH_VARARGS },
		{ "RequestMoveGlobalQuickSlotToLocalQuickSlot",	playerRequestMoveGlobalQuickSlotToLocalQuickSlot,	METH_VARARGS },
		{ "RequestUseLocalQuickSlot",					playerRequestUseLocalQuickSlot,						METH_VARARGS },
		{ "LocalQuickSlotIndexToGlobalQuickSlotIndex",	playerLocalQuickSlotIndexToGlobalQuickSlotIndex,	METH_VARARGS },

		{ "GetQuickPage",				playerGetQuickPage,					METH_VARARGS },
		{ "SetQuickPage",				playerSetQuickPage,					METH_VARARGS },
		{ "GetLocalQuickSlot",			playerGetLocalQuickSlot,			METH_VARARGS },
		{ "GetGlobalQuickSlot",			playerGetGlobalQuickSlot,			METH_VARARGS },
		{ "RemoveQuickSlotByValue",		playerRemoveQuickSlotByValue,		METH_VARARGS },

		{ "isItem",						playerisItem,						METH_VARARGS },
		{ "IsEquipmentSlot",			playerIsEquipmentSlot,				METH_VARARGS },
		{ "IsDSEquipmentSlot",			playerIsDSEquipmentSlot,			METH_VARARGS },
		{ "IsCostumeSlot",				playerIsCostumeSlot,				METH_VARARGS },
		{ "IsValuableItem",				playerIsValuableItem,				METH_VARARGS },
		{ "IsOpenPrivateShop",			playerIsOpenPrivateShop,			METH_VARARGS },

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
		{ "IsBeltInventorySlot",			playerIsBeltInventorySlot,			METH_VARARGS },
		{ "IsEquippingBelt",				playerIsEquippingBelt,				METH_VARARGS },
		{ "IsAvailableBeltInventoryCell",	playerIsAvailableBeltInventoryCell,	METH_VARARGS },
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
		{ "IsSkillBookInventorySlot", playerIsSkillBookInventorySlot, METH_VARARGS },
		{ "IsUpgradeItemsInventorySlot", playerIsUpgradeItemsInventorySlot, METH_VARARGS },
		{ "IsStoneInventorySlot", playerIsStoneInventorySlot, METH_VARARGS },
		{ "IsGiftBoxInventorySlot", playerIsGiftBoxInventorySlot, METH_VARARGS },
#endif
		// Refine
		{ "GetItemGrade",				playerGetItemGrade,					METH_VARARGS },
		{ "CanRefine",					playerCanRefine,					METH_VARARGS },
		{ "CanDetach",					playerCanDetach,					METH_VARARGS },
		{ "CanUnlock",					playerCanUnlock,					METH_VARARGS },
		{ "CanAttachMetin",				playerCanAttachMetin,				METH_VARARGS },
		{ "IsRefineGradeScroll",		playerIsRefineGradeScroll,			METH_VARARGS },

		{ "ClearTarget",				playerClearTarget,					METH_VARARGS },
		{ "SetTarget",					playerSetTarget,					METH_VARARGS },
		{ "OpenCharacterMenu",			playerOpenCharacterMenu,			METH_VARARGS },

		{ "Update",						playerUpdate,						METH_VARARGS },
		{ "Render",						playerRender,						METH_VARARGS },
		{ "Clear",						playerClear,						METH_VARARGS },

		// Party
		{ "IsPartyMember",				playerIsPartyMember,				METH_VARARGS },
		{ "IsPartyLeader",				playerIsPartyLeader,				METH_VARARGS },
		{ "IsPartyLeaderByPID",			playerIsPartyLeaderByPID,			METH_VARARGS },
		{ "GetPartyMemberHPPercentage",	playerGetPartyMemberHPPercentage,	METH_VARARGS },
		{ "GetPartyMemberState",		playerGetPartyMemberState,			METH_VARARGS },
		{ "GetPartyMemberAffects",		playerGetPartyMemberAffects,		METH_VARARGS },
		{ "RemovePartyMember",			playerRemovePartyMember,			METH_VARARGS },
		{ "ExitParty",					playerExitParty,					METH_VARARGS },

		// PK Mode
		{ "GetPKMode",					playerGetPKMode,					METH_VARARGS },

		// Mobile
		{ "HasMobilePhoneNumber",		playerHasMobilePhoneNumber,			METH_VARARGS },

		// Emotion
		{ "RegisterEmotionIcon",		playerRegisterEmotionIcon,			METH_VARARGS },
		{ "GetEmotionIconImage",		playerGetEmotionIconImage,			METH_VARARGS },

		// For System
		{ "SetWeaponAttackBonusFlag",	playerSetWeaponAttackBonusFlag,		METH_VARARGS },
		{ "ToggleCoolTime",				playerToggleCoolTime,				METH_VARARGS },
		{ "ToggleLevelLimit",			playerToggleLevelLimit,				METH_VARARGS },
		{ "GetTargetVID",				playerGetTargetVID,					METH_VARARGS },

		{ "SetItemData",				playerSetItemData,					METH_VARARGS },
		{ "SetItemMetinSocket",			playerSetItemMetinSocket,			METH_VARARGS },
		{ "SetItemAttribute",			playerSetItemAttribute,				METH_VARARGS },
		{ "SetItemCount",				playerSetItemCount,					METH_VARARGS },

		{ "GetItemLink",				playerGetItemLink,					METH_VARARGS },
		{ "SlotTypeToInvenType",		playerSlotTypeToInvenType,			METH_VARARGS },
		{ "SendDragonSoulRefine",		playerSendDragonSoulRefine,			METH_VARARGS },

#ifdef ENABLE_SEALBIND_SYSTEM
		{ "CanSealItem",				playerCanSealItem,					METH_VARARGS },
		{ "GetItemSealDate",			playerGetItemSealDate,				METH_VARARGS },
		{ "GetItemUnSealLeftTime",		GetItemUnSealLeftTime,				METH_VARARGS },
#endif

#ifdef ENABLE_SKILL_COLOR_SYSTEM
		{ "SetSkillColor",				playerSetSkillColor,				METH_VARARGS },
		{ "GetSkillColor",				playerGetSkillColor,				METH_VARARGS },
#endif
		{ "GetItemTypeBySlot",			playerGetItemTypeBySlot,			METH_VARARGS },
		{ "GetItemSubTypeBySlot",		playerGetItemSubTypeBySlot,			METH_VARARGS },

#ifdef ENABLE_REFINE_ELEMENT
		{ "GetItemRefineElement",			playerGetItemRefineElement,			METH_VARARGS },
#endif
#ifdef ENABLE_AURA_SYSTEM
		{ "GetAuraItemID",							playerGetAuraItemID,						METH_VARARGS },
		{ "GetAuraItemCount",						playerGetAuraItemCount,						METH_VARARGS },
		{ "GetAuraItemMetinSocket",					playerGetAuraItemMetinSocket,				METH_VARARGS },
		{ "GetAuraItemAttribute",					playerGetAuraItemAttribute,					METH_VARARGS },

		{ "IsAuraRefineWindowEmpty",				playerIsAuraRefineWindowEmpty,				METH_VARARGS },
		{ "GetAuraCurrentItemSlotCount",			playerGetAuraCurrentItemSlotCount,			METH_VARARGS },
		{ "IsAuraRefineWindowOpen",					playerIsAuraRefineWindowOpen,				METH_VARARGS },
		{ "GetAuraRefineWindowType",				playerGetAuraRefineWindowType,				METH_VARARGS },
		{ "FineMoveAuraItemSlot",					playerFineMoveAuraItemSlot,					METH_VARARGS },
		{ "SetAuraActivatedItemSlot",				playerSetAuraActivatedItemSlot,				METH_VARARGS },
		{ "FindActivatedAuraSlot",					playerFindActivatedAuraSlot,				METH_VARARGS },
		{ "FindUsingAuraSlot",						playerFindUsingAuraSlot,					METH_VARARGS },

		{ "GetAuraRefineInfo",						playerGetAuraRefineInfo,					METH_VARARGS },
		{ "GetAuraRefineInfoExpPer",				playerGetAuraRefineInfoExpPer,				METH_VARARGS },
		{ "GetAuraRefineInfoLevel",					playerGetAuraRefineInfoLevel,				METH_VARARGS },
#endif
		{ "NPOS",									playerNPOS,									METH_VARARGS },
		{ "IsAntiFlagBySlot",			playerIsAntiFlagBySlot,				METH_VARARGS },
		// { "GetItemTypeBySlot",			playerGetItemTypeBySlot,			METH_VARARGS },
		// { "GetItemSubTypeBySlot",		playerGetItemSubTypeBySlot,			METH_VARARGS },
		{ "IsSameItemVnum",				playerIsSameItemVnum,				METH_VARARGS },
		
		{ "GetItemSlotIndex",					playerGetItemSlotIndex,						METH_VARARGS },
		
#ifdef ENABLE_GROWTH_PET_SYSTEM
		{ "SetOpenPetHatchingWindow",		playerSetOpenPetHatchingWindow,		METH_VARARGS },
		{ "SetOpenPetNameChangeWindow",		playerSetOpenPetNameChangeWindow,	METH_VARARGS },
		{ "SetOpenPetFeedWindow",			playerSetOpenPetFeedWindow,			METH_VARARGS },
		{ "IsOpenPetHatchingWindow",		playerIsOpenPetHatchingWindow,		METH_VARARGS },
		{ "IsOpenPetFeedWindow",			playerIsOpenPetFeedWindow,			METH_VARARGS },
		{ "IsOpenPetNameChangeWindow",		playerIsOpenPetNameChangeWindow,	METH_VARARGS },

		{ "SetOpenMall",					playerSetOpenMall,					METH_VARARGS },
		{ "SetOpenSafeBox",					playerSetOpenSafeBox,				METH_VARARGS },
		{ "IsOpenMall",						playerIsOpenMall,					METH_VARARGS },

		{ "GetActivePetItemId",				playerGetActivePetItemId,			METH_VARARGS },
		{ "GetPetItemVNumInBag",			playerGetPetItemVNumInBag,			METH_VARARGS },
		{ "GetPetSkill",					playerGetPetSkill,					METH_VARARGS },
		{ "GetPetSkillByIndex",				playerGetPetSkillByIndex,			METH_VARARGS },
		{ "GetActivePetItemVNum",			playerGetActivePetItemVNum,			METH_VARARGS },
		{ "GetPetExpPoints",				playerGetPetExpPoints,				METH_VARARGS },
		{ "GetPetItem",						playerGetPetItem,					METH_VARARGS },
		{ "GetPetLifeTime",					playerGetPetLifeTime,				METH_VARARGS },
		
		{ "CanUseGrowthPetQuickSlot",		playerCanUseGrowthPetQuickSlot,		METH_VARARGS },
#endif

		{ NULL,							NULL,								NULL },
	};

	PyObject* poModule = Py_InitModule("player", s_methods);
    PyModule_AddIntConstant(poModule, "LEVEL",					POINT_LEVEL);
    PyModule_AddIntConstant(poModule, "VOICE",					POINT_VOICE);
    PyModule_AddIntConstant(poModule, "EXP",					POINT_EXP);
    PyModule_AddIntConstant(poModule, "NEXT_EXP",				POINT_NEXT_EXP);
    PyModule_AddIntConstant(poModule, "HP",						POINT_HP);
    PyModule_AddIntConstant(poModule, "MAX_HP",					POINT_MAX_HP);
    PyModule_AddIntConstant(poModule, "SP",						POINT_SP);
    PyModule_AddIntConstant(poModule, "MAX_SP",					POINT_MAX_SP);
    PyModule_AddIntConstant(poModule, "STAMINA",				POINT_STAMINA);
    PyModule_AddIntConstant(poModule, "MAX_STAMINA",			POINT_MAX_STAMINA);
    PyModule_AddIntConstant(poModule, "ELK",					POINT_GOLD);
    PyModule_AddIntConstant(poModule, "ST",						POINT_ST);
    PyModule_AddIntConstant(poModule, "HT",						POINT_HT);
    PyModule_AddIntConstant(poModule, "DX",						POINT_DX);
    PyModule_AddIntConstant(poModule, "IQ",						POINT_IQ);
    PyModule_AddIntConstant(poModule, "ATT_POWER",				POINT_ATT_POWER);
	PyModule_AddIntConstant(poModule, "ATT_MIN",				POINT_MIN_ATK);
	PyModule_AddIntConstant(poModule, "ATT_MAX",				POINT_MAX_ATK);
	PyModule_AddIntConstant(poModule, "MIN_MAGIC_WEP",			POINT_MIN_MAGIC_WEP);
	PyModule_AddIntConstant(poModule, "MAX_MAGIC_WEP",			POINT_MAX_MAGIC_WEP);
    PyModule_AddIntConstant(poModule, "ATT_SPEED",				POINT_ATT_SPEED);
	PyModule_AddIntConstant(poModule, "ATT_BONUS",				POINT_ATT_GRADE_BONUS);
    PyModule_AddIntConstant(poModule, "EVADE_RATE",				POINT_EVADE_RATE);
    PyModule_AddIntConstant(poModule, "MOVING_SPEED",			POINT_MOV_SPEED);
    PyModule_AddIntConstant(poModule, "DEF_GRADE",				POINT_DEF_GRADE);
    PyModule_AddIntConstant(poModule, "DEF_BONUS",				POINT_DEF_GRADE_BONUS);
    PyModule_AddIntConstant(poModule, "CASTING_SPEED",			POINT_CASTING_SPEED);
    PyModule_AddIntConstant(poModule, "MAG_ATT",				POINT_MAGIC_ATT_GRADE);
    PyModule_AddIntConstant(poModule, "MAG_DEF",				POINT_MAGIC_DEF_GRADE);
    PyModule_AddIntConstant(poModule, "EMPIRE_POINT",			POINT_EMPIRE_POINT);
	PyModule_AddIntConstant(poModule, "STAT",					POINT_STAT);
	PyModule_AddIntConstant(poModule, "SKILL_PASSIVE",			POINT_SUB_SKILL);
	PyModule_AddIntConstant(poModule, "SKILL_SUPPORT",			POINT_SUB_SKILL);
	PyModule_AddIntConstant(poModule, "SKILL_ACTIVE",			POINT_SKILL);
	PyModule_AddIntConstant(poModule, "SKILL_HORSE",			POINT_HORSE_SKILL);
	PyModule_AddIntConstant(poModule, "PLAYTIME",				POINT_PLAYTIME);
	PyModule_AddIntConstant(poModule, "BOW_DISTANCE",			POINT_BOW_DISTANCE);
	PyModule_AddIntConstant(poModule, "HP_RECOVERY",			POINT_HP_RECOVERY);
	PyModule_AddIntConstant(poModule, "SP_RECOVERY",			POINT_SP_RECOVERY);
	PyModule_AddIntConstant(poModule, "ATTACKER_BONUS",			POINT_PARTY_ATT_GRADE);
    PyModule_AddIntConstant(poModule, "MAX_NUM",				POINT_MAX_NUM);
	////
	PyModule_AddIntConstant(poModule, "POINT_CRITICAL_PCT",		POINT_CRITICAL_PCT);
	PyModule_AddIntConstant(poModule, "POINT_PENETRATE_PCT",	POINT_PENETRATE_PCT);
	PyModule_AddIntConstant(poModule, "POINT_MALL_ATTBONUS",	POINT_MALL_ATTBONUS);
	PyModule_AddIntConstant(poModule, "POINT_MALL_DEFBONUS",	POINT_MALL_DEFBONUS);
	PyModule_AddIntConstant(poModule, "POINT_MALL_EXPBONUS",	POINT_MALL_EXPBONUS);
	PyModule_AddIntConstant(poModule, "POINT_MALL_ITEMBONUS",	POINT_MALL_ITEMBONUS);
	PyModule_AddIntConstant(poModule, "POINT_MALL_GOLDBONUS",	POINT_MALL_GOLDBONUS);
	PyModule_AddIntConstant(poModule, "POINT_MAX_HP_PCT",		POINT_MAX_HP_PCT);
	PyModule_AddIntConstant(poModule, "POINT_MAX_SP_PCT",		POINT_MAX_SP_PCT);

	PyModule_AddIntConstant(poModule, "POINT_SKILL_DAMAGE_BONUS",		POINT_SKILL_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "POINT_NORMAL_HIT_DAMAGE_BONUS",		POINT_NORMAL_HIT_DAMAGE_BONUS);
	PyModule_AddIntConstant(poModule, "POINT_SKILL_DEFEND_BONUS",		POINT_SKILL_DEFEND_BONUS);
	PyModule_AddIntConstant(poModule, "POINT_NORMAL_HIT_DEFEND_BONUS",		POINT_NORMAL_HIT_DEFEND_BONUS);
	PyModule_AddIntConstant(poModule, "POINT_PC_BANG_EXP_BONUS",		POINT_PC_BANG_EXP_BONUS);
	PyModule_AddIntConstant(poModule, "POINT_PC_BANG_DROP_BONUS",		POINT_PC_BANG_DROP_BONUS);

	PyModule_AddIntConstant(poModule, "ENERGY",		POINT_ENERGY);
	PyModule_AddIntConstant(poModule, "ENERGY_END_TIME",		POINT_ENERGY_END_TIME);
#ifdef ENABLE_CHEQUE_SYSTEM
	PyModule_AddIntConstant(poModule, "CHEQUE",			POINT_CHEQUE);
#endif 
    PyModule_AddIntConstant(poModule, "SKILL_GRADE_NORMAL",			CPythonPlayer::SKILL_NORMAL);
    PyModule_AddIntConstant(poModule, "SKILL_GRADE_MASTER",			CPythonPlayer::SKILL_MASTER);
    PyModule_AddIntConstant(poModule, "SKILL_GRADE_GRAND_MASTER",	CPythonPlayer::SKILL_GRAND_MASTER);
    PyModule_AddIntConstant(poModule, "SKILL_GRADE_PERFECT_MASTER",	CPythonPlayer::SKILL_PERFECT_MASTER);

	PyModule_AddIntConstant(poModule, "CATEGORY_ACTIVE",		CPythonPlayer::CATEGORY_ACTIVE);
	PyModule_AddIntConstant(poModule, "CATEGORY_PASSIVE",		CPythonPlayer::CATEGORY_PASSIVE);

	PyModule_AddIntConstant(poModule, "INVENTORY_PAGE_COLUMN",	c_Inventory_Page_Column);
	PyModule_AddIntConstant(poModule, "INVENTORY_PAGE_ROW",		c_Inventory_Page_Row);
	PyModule_AddIntConstant(poModule, "INVENTORY_PAGE_SIZE",	c_Inventory_Page_Size);
	PyModule_AddIntConstant(poModule, "INVENTORY_PAGE_COUNT",	c_Inventory_Page_Count);
	PyModule_AddIntConstant(poModule, "INVENTORY_SLOT_COUNT",	c_Inventory_Count);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_SLOT_START",	c_Equipment_Start);
	PyModule_AddIntConstant(poModule, "EQUIPMENT_PAGE_COUNT",	c_Equipment_Count);

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	PyModule_AddIntConstant(poModule, "NEW_EQUIPMENT_SLOT_START",	c_New_Equipment_Start);
	PyModule_AddIntConstant(poModule, "NEW_EQUIPMENT_SLOT_COUNT",	c_New_Equipment_Count);
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	PyModule_AddIntConstant(poModule, "SPECIAL_ITEM_SLOT_COUNT", c_Special_ItemSlot_Count);
	PyModule_AddIntConstant(poModule, "SPECIAL_INVENTORY_PAGE_SIZE", c_Special_Inventory_Page_Size);
	PyModule_AddIntConstant(poModule, "SPECIAL_INVENTORY_PAGE_COUNT", c_Special_Inventory_Page_Count);

	PyModule_AddIntConstant(poModule, "SKILL_BOOK_INVENTORY_SLOT_COUNT", c_Skill_Book_Inventory_Slot_Count);
	PyModule_AddIntConstant(poModule, "UPGRADE_ITEMS_INVENTORY_SLOT_COUNT", c_Upgrade_Items_Inventory_Slot_Count);
	PyModule_AddIntConstant(poModule, "STONE_INVENTORY_SLOT_COUNT", c_Stone_Inventory_Slot_Count);
	PyModule_AddIntConstant(poModule, "GIFT_BOX_INVENTORY_SLOT_COUNT", c_GiftBox_Inventory_Slot_Count);
#endif

	PyModule_AddIntConstant(poModule, "MBF_SKILL",	CPythonPlayer::MBF_SKILL);
	PyModule_AddIntConstant(poModule, "MBF_ATTACK",	CPythonPlayer::MBF_ATTACK);
	PyModule_AddIntConstant(poModule, "MBF_CAMERA",	CPythonPlayer::MBF_CAMERA);
	PyModule_AddIntConstant(poModule, "MBF_SMART",	CPythonPlayer::MBF_SMART);
	PyModule_AddIntConstant(poModule, "MBF_MOVE",	CPythonPlayer::MBF_MOVE);
	PyModule_AddIntConstant(poModule, "MBF_AUTO",	CPythonPlayer::MBF_AUTO);
	PyModule_AddIntConstant(poModule, "MBS_PRESS",	CPythonPlayer::MBS_PRESS);
	PyModule_AddIntConstant(poModule, "MBS_CLICK",	CPythonPlayer::MBS_CLICK);
	PyModule_AddIntConstant(poModule, "MBT_RIGHT",	CPythonPlayer::MBT_RIGHT);
	PyModule_AddIntConstant(poModule, "MBT_LEFT",	CPythonPlayer::MBT_LEFT);

	// Public code with server
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_NONE",						SLOT_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_INVENTORY",				SLOT_TYPE_INVENTORY);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_SKILL",					SLOT_TYPE_SKILL);
	// Special indecies for client
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_SHOP",						SLOT_TYPE_SHOP);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_EXCHANGE_OWNER",			SLOT_TYPE_EXCHANGE_OWNER);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_EXCHANGE_TARGET",			SLOT_TYPE_EXCHANGE_TARGET);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_QUICK_SLOT",				SLOT_TYPE_QUICK_SLOT);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_SAFEBOX",					SLOT_TYPE_SAFEBOX);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_PRIVATE_SHOP",				SLOT_TYPE_PRIVATE_SHOP);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_MALL",						SLOT_TYPE_MALL);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_EMOTION",					SLOT_TYPE_EMOTION);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_DRAGON_SOUL_INVENTORY",	SLOT_TYPE_DRAGON_SOUL_INVENTORY);
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_SKILL_BOOK_INVENTORY", SLOT_TYPE_SKILL_BOOK_INVENTORY);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_UPGRADE_ITEMS_INVENTORY", SLOT_TYPE_UPGRADE_ITEMS_INVENTORY);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_STONE_INVENTORY", SLOT_TYPE_STONE_INVENTORY);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_GIFT_BOX_INVENTORY", SLOT_TYPE_GIFT_BOX_INVENTORY);
#endif
	PyModule_AddIntConstant(poModule, "RESERVED_WINDOW",					RESERVED_WINDOW);
	PyModule_AddIntConstant(poModule, "INVENTORY",							INVENTORY);
	PyModule_AddIntConstant(poModule, "EQUIPMENT",							EQUIPMENT);
	PyModule_AddIntConstant(poModule, "SAFEBOX",							SAFEBOX);
	PyModule_AddIntConstant(poModule, "MALL",								MALL);
	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_INVENTORY",				DRAGON_SOUL_INVENTORY);
	PyModule_AddIntConstant(poModule, "GROUND",								GROUND);

	PyModule_AddIntConstant(poModule, "ITEM_MONEY",					-1);
#ifdef ENABLE_CHEQUE_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_CHEQUE",				-2);
#endif 
	PyModule_AddIntConstant(poModule, "SKILL_SLOT_COUNT",			SKILL_MAX_NUM);

	PyModule_AddIntConstant(poModule, "EFFECT_PICK",				CPythonPlayer::EFFECT_PICK);

	PyModule_AddIntConstant(poModule, "METIN_SOCKET_TYPE_NONE",					CPythonPlayer::METIN_SOCKET_TYPE_NONE);
	PyModule_AddIntConstant(poModule, "METIN_SOCKET_TYPE_SILVER",				CPythonPlayer::METIN_SOCKET_TYPE_SILVER);
	PyModule_AddIntConstant(poModule, "METIN_SOCKET_TYPE_GOLD",					CPythonPlayer::METIN_SOCKET_TYPE_GOLD);
	PyModule_AddIntConstant(poModule, "METIN_SOCKET_MAX_NUM",					ITEM_SOCKET_SLOT_MAX_NUM);
	// refactored attribute slot begin
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_NORM_NUM",					ITEM_ATTRIBUTE_SLOT_NORM_NUM);
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_RARE_NUM",					ITEM_ATTRIBUTE_SLOT_RARE_NUM);
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_NORM_START",					ITEM_ATTRIBUTE_SLOT_NORM_START);
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_NORM_END",					ITEM_ATTRIBUTE_SLOT_NORM_END);
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_RARE_START",					ITEM_ATTRIBUTE_SLOT_RARE_START);
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_RARE_END",					ITEM_ATTRIBUTE_SLOT_RARE_END);
	PyModule_AddIntConstant(poModule, "ATTRIBUTE_SLOT_MAX_NUM",						ITEM_ATTRIBUTE_SLOT_MAX_NUM);
	// refactored attribute slot end

	PyModule_AddIntConstant(poModule, "REFINE_CANT",							REFINE_CANT);
	PyModule_AddIntConstant(poModule, "REFINE_OK",								REFINE_OK);
	PyModule_AddIntConstant(poModule, "REFINE_ALREADY_MAX_SOCKET_COUNT",		REFINE_ALREADY_MAX_SOCKET_COUNT);
	PyModule_AddIntConstant(poModule, "REFINE_NEED_MORE_GOOD_SCROLL",			REFINE_NEED_MORE_GOOD_SCROLL);
	PyModule_AddIntConstant(poModule, "REFINE_CANT_MAKE_SOCKET_ITEM",			REFINE_CANT_MAKE_SOCKET_ITEM);
	PyModule_AddIntConstant(poModule, "REFINE_NOT_NEXT_GRADE_ITEM",				REFINE_NOT_NEXT_GRADE_ITEM);
	PyModule_AddIntConstant(poModule, "REFINE_CANT_REFINE_METIN_TO_EQUIPMENT",	REFINE_CANT_REFINE_METIN_TO_EQUIPMENT);
	PyModule_AddIntConstant(poModule, "REFINE_CANT_REFINE_ROD",					REFINE_CANT_REFINE_ROD);
	PyModule_AddIntConstant(poModule, "ATTACH_METIN_CANT",						ATTACH_METIN_CANT);
	PyModule_AddIntConstant(poModule, "ATTACH_METIN_OK",						ATTACH_METIN_OK);
	PyModule_AddIntConstant(poModule, "ATTACH_METIN_NOT_MATCHABLE_ITEM",		ATTACH_METIN_NOT_MATCHABLE_ITEM);
	PyModule_AddIntConstant(poModule, "ATTACH_METIN_NO_MATCHABLE_SOCKET",		ATTACH_METIN_NO_MATCHABLE_SOCKET);
	PyModule_AddIntConstant(poModule, "ATTACH_METIN_NOT_EXIST_GOLD_SOCKET",		ATTACH_METIN_NOT_EXIST_GOLD_SOCKET);
	PyModule_AddIntConstant(poModule, "ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT",	ATTACH_METIN_CANT_ATTACH_TO_EQUIPMENT);
	PyModule_AddIntConstant(poModule, "DETACH_METIN_CANT",						DETACH_METIN_CANT);
	PyModule_AddIntConstant(poModule, "DETACH_METIN_OK",						DETACH_METIN_OK);

	// Party
	PyModule_AddIntConstant(poModule, "PARTY_STATE_NORMAL",						CPythonPlayer::PARTY_ROLE_NORMAL);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_LEADER",						CPythonPlayer::PARTY_ROLE_LEADER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_ATTACKER",					CPythonPlayer::PARTY_ROLE_ATTACKER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_TANKER",						CPythonPlayer::PARTY_ROLE_TANKER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_BUFFER",						CPythonPlayer::PARTY_ROLE_BUFFER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_SKILL_MASTER",				CPythonPlayer::PARTY_ROLE_SKILL_MASTER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_BERSERKER",					CPythonPlayer::PARTY_ROLE_BERSERKER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_DEFENDER",					CPythonPlayer::PARTY_ROLE_DEFENDER);
	PyModule_AddIntConstant(poModule, "PARTY_STATE_MAX_NUM",					CPythonPlayer::PARTY_ROLE_MAX_NUM);

	// Skill Index
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_TONGSOL",		c_iSkillIndex_Tongsol);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_FISHING",		c_iSkillIndex_Fishing);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_MINING",			c_iSkillIndex_Mining);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_MAKING",			c_iSkillIndex_Making);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_COMBO",			c_iSkillIndex_Combo);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_LANGUAGE1",		c_iSkillIndex_Language1);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_LANGUAGE2",		c_iSkillIndex_Language2);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_LANGUAGE3",		c_iSkillIndex_Language3);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_POLYMORPH",		c_iSkillIndex_Polymorph);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_RIDING",			c_iSkillIndex_Riding);
	PyModule_AddIntConstant(poModule, "SKILL_INDEX_SUMMON",			c_iSkillIndex_Summon);

	// PK Mode
	PyModule_AddIntConstant(poModule, "PK_MODE_PEACE",				PK_MODE_PEACE);
	PyModule_AddIntConstant(poModule, "PK_MODE_REVENGE",			PK_MODE_REVENGE);
	PyModule_AddIntConstant(poModule, "PK_MODE_FREE",				PK_MODE_FREE);
	PyModule_AddIntConstant(poModule, "PK_MODE_PROTECT",			PK_MODE_PROTECT);
	PyModule_AddIntConstant(poModule, "PK_MODE_GUILD",				PK_MODE_GUILD);
	PyModule_AddIntConstant(poModule, "PK_MODE_MAX_NUM",			PK_MODE_MAX_NUM);

	// Block Mode
	PyModule_AddIntConstant(poModule, "BLOCK_EXCHANGE",				BLOCK_EXCHANGE);
	PyModule_AddIntConstant(poModule, "BLOCK_PARTY",				BLOCK_PARTY_INVITE);
	PyModule_AddIntConstant(poModule, "BLOCK_GUILD",				BLOCK_GUILD_INVITE);
	PyModule_AddIntConstant(poModule, "BLOCK_WHISPER",				BLOCK_WHISPER);
	PyModule_AddIntConstant(poModule, "BLOCK_FRIEND",				BLOCK_MESSENGER_INVITE);
	PyModule_AddIntConstant(poModule, "BLOCK_PARTY_REQUEST",		BLOCK_PARTY_REQUEST);

	// Party
	PyModule_AddIntConstant(poModule, "PARTY_EXP_NON_DISTRIBUTION",		PARTY_EXP_DISTRIBUTION_NON_PARITY);
	PyModule_AddIntConstant(poModule, "PARTY_EXP_DISTRIBUTION_PARITY",	PARTY_EXP_DISTRIBUTION_PARITY);

	// Emotion
	PyModule_AddIntConstant(poModule, "EMOTION_CLAP",			EMOTION_CLAP);
	PyModule_AddIntConstant(poModule, "EMOTION_CHEERS_1",		EMOTION_CHEERS_1);
	PyModule_AddIntConstant(poModule, "EMOTION_CHEERS_2",		EMOTION_CHEERS_2);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_1",		EMOTION_DANCE_1);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_2",		EMOTION_DANCE_2);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_3",		EMOTION_DANCE_3);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_4",		EMOTION_DANCE_4);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_5",		EMOTION_DANCE_5);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_6",		EMOTION_DANCE_6);
	PyModule_AddIntConstant(poModule, "EMOTION_CONGRATULATION",	EMOTION_CONGRATULATION);
	PyModule_AddIntConstant(poModule, "EMOTION_FORGIVE",		EMOTION_FORGIVE);
	PyModule_AddIntConstant(poModule, "EMOTION_ANGRY",			EMOTION_ANGRY);
	PyModule_AddIntConstant(poModule, "EMOTION_ATTRACTIVE",		EMOTION_ATTRACTIVE);
	PyModule_AddIntConstant(poModule, "EMOTION_SAD",			EMOTION_SAD);
	PyModule_AddIntConstant(poModule, "EMOTION_SHY",			EMOTION_SHY);
	PyModule_AddIntConstant(poModule, "EMOTION_CHEERUP",		EMOTION_CHEERUP);
	PyModule_AddIntConstant(poModule, "EMOTION_BANTER",			EMOTION_BANTER);
	PyModule_AddIntConstant(poModule, "EMOTION_JOY",			EMOTION_JOY);

	PyModule_AddIntConstant(poModule, "EMOTION_KISS",			EMOTION_KISS);
	PyModule_AddIntConstant(poModule, "EMOTION_FRENCH_KISS",	EMOTION_FRENCH_KISS);
	PyModule_AddIntConstant(poModule, "EMOTION_SLAP",			EMOTION_SLAP);
#ifdef ENABLE_EXPRESSING_EMOTION
	PyModule_AddIntConstant(poModule, "EMOTION_PUSHUP",			EMOTION_PUSHUP);
	PyModule_AddIntConstant(poModule, "EMOTION_DANCE_7",			EMOTION_DANCE_7);
	PyModule_AddIntConstant(poModule, "EMOTION_EXERCISE",			EMOTION_EXERCISE);
	PyModule_AddIntConstant(poModule, "EMOTION_DOZE",			EMOTION_DOZE);
	PyModule_AddIntConstant(poModule, "EMOTION_SELFIE",			EMOTION_SELFIE);
#endif
	PyModule_AddIntConstant(poModule, "AUTO_POTION_TYPE_HP",	CPythonPlayer::AUTO_POTION_TYPE_HP);
	PyModule_AddIntConstant(poModule, "AUTO_POTION_TYPE_SP",	CPythonPlayer::AUTO_POTION_TYPE_SP);

	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_PAGE_SIZE",	c_DragonSoul_Inventory_Box_Size);
	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_PAGE_COUNT",	DRAGON_SOUL_GRADE_MAX);
	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_SLOT_COUNT",	c_DragonSoul_Inventory_Count);
	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_EQUIPMENT_SLOT_START",	c_DragonSoul_Equip_Start);
	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_EQUIPMENT_PAGE_COUNT",	DS_DECK_MAX_NUM);
	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_EQUIPMENT_FIRST_SIZE",	c_DragonSoul_Equip_Slot_Max);

	PyModule_AddIntConstant(poModule, "DRAGON_SOUL_REFINE_CLOSE",	DS_SUB_HEADER_CLOSE);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_DO_UPGRADE",	DS_SUB_HEADER_DO_UPGRADE);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_DO_IMPROVEMENT",	DS_SUB_HEADER_DO_IMPROVEMENT);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_DO_REFINE",	DS_SUB_HEADER_DO_REFINE);
#ifdef WJ_ENABLE_TRADABLE_ICON
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_NONE",			ON_TOP_WND_NONE);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_SHOP",			ON_TOP_WND_SHOP);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_EXCHANGE",		ON_TOP_WND_EXCHANGE);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_SAFEBOX",			ON_TOP_WND_SAFEBOX);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_PRIVATE_SHOP",	ON_TOP_WND_PRIVATE_SHOP);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_ITEM_COMB",		ON_TOP_WND_ITEM_COMB);
	// PyModule_AddIntConstant(poModule, "ON_TOP_WND_PET_FEED",		ON_TOP_WND_PET_FEED);
#endif
#ifdef ENABLE_6_7_BONUS_NEW_SYSTEM
	PyModule_AddIntConstant(poModule, "SKILLBOOK_COMB_SLOT_MAX",	c_skillbook_slot_max);
#endif
#ifdef ENABLE_AURA_SYSTEM
	PyModule_AddIntConstant(poModule, "REFINE_CANT_REFINE_AURA_ITEM",		REFINE_CANT_REFINE_AURA_ITEM);
	PyModule_AddIntConstant(poModule, "SLOT_TYPE_AURA",						SLOT_TYPE_AURA);
	PyModule_AddIntConstant(poModule, "AURA_REFINE",						AURA_REFINE);
	PyModule_AddIntConstant(poModule, "AURA_MAX_LEVEL",						c_AuraMaxLevel);
	PyModule_AddIntConstant(poModule, "AURA_SLOT_MAIN",						AURA_SLOT_MAIN);
	PyModule_AddIntConstant(poModule, "AURA_SLOT_SUB",						AURA_SLOT_SUB);
	PyModule_AddIntConstant(poModule, "AURA_SLOT_RESULT",					AURA_SLOT_RESULT);
	PyModule_AddIntConstant(poModule, "AURA_SLOT_MAX",						AURA_SLOT_MAX);
	PyModule_AddIntConstant(poModule, "AURA_WINDOW_TYPE_ABSORB",			AURA_WINDOW_TYPE_ABSORB);
	PyModule_AddIntConstant(poModule, "AURA_WINDOW_TYPE_GROWTH",			AURA_WINDOW_TYPE_GROWTH);
	PyModule_AddIntConstant(poModule, "AURA_WINDOW_TYPE_EVOLVE",			AURA_WINDOW_TYPE_EVOLVE);
	PyModule_AddIntConstant(poModule, "AURA_WINDOW_TYPE_MAX",				AURA_WINDOW_TYPE_MAX);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_SLOT_CURRENT",		AURA_REFINE_INFO_SLOT_CURRENT);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_SLOT_NEXT",			AURA_REFINE_INFO_SLOT_NEXT);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_SLOT_EVOLVED",		AURA_REFINE_INFO_SLOT_EVOLVED);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_STEP",				AURA_REFINE_INFO_STEP);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_LEVEL_MIN",			AURA_REFINE_INFO_LEVEL_MIN);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_LEVEL_MAX",			AURA_REFINE_INFO_LEVEL_MAX);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_NEED_EXP",			AURA_REFINE_INFO_NEED_EXP);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_MATERIAL_VNUM",		AURA_REFINE_INFO_MATERIAL_VNUM);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_MATERIAL_COUNT",	AURA_REFINE_INFO_MATERIAL_COUNT);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_NEED_GOLD",			AURA_REFINE_INFO_NEED_GOLD);
	PyModule_AddIntConstant(poModule, "AURA_REFINE_INFO_EVOLVE_PCT",		AURA_REFINE_INFO_EVOLVE_PCT);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM",	CPythonPlayer::ITEM_SOCKET_AURA_DRAIN_ITEM_VNUM);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_AURA_CURRENT_LEVEL",		CPythonPlayer::ITEM_SOCKET_AURA_CURRENT_LEVEL);
	PyModule_AddIntConstant(poModule, "ITEM_SOCKET_AURA_BOOST",				CPythonPlayer::ITEM_SOCKET_AURA_BOOST);
	PyModule_AddIntConstant(poModule, "ITEM_VALUE_AURA_MATERIAL_EXP",		CPythonPlayer::ITEM_VALUE_AURA_MATERIAL_EXP);
	PyModule_AddIntConstant(poModule, "ITEM_VALUE_AURA_BOOST_PERCENT",		CPythonPlayer::ITEM_VALUE_AURA_BOOST_PERCENT);
	PyModule_AddIntConstant(poModule, "ITEM_VALUE_AURA_BOOST_TIME",			CPythonPlayer::ITEM_VALUE_AURA_BOOST_TIME);
	PyModule_AddIntConstant(poModule, "ITEM_VALUE_AURA_BOOST_UNLIMITED",	CPythonPlayer::ITEM_VALUE_AURA_BOOST_UNLIMITED);
#endif

#ifdef ENABLE_GROWTH_PET_SYSTEM
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_PET_FEED",		ON_TOP_WND_PET_FEED);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_PET_ATTR_CHANGE", ON_TOP_WND_PET_ATTR_CHANGE);
	PyModule_AddIntConstant(poModule, "ON_TOP_WND_PET_PRIMIUM_FEEDSTUFF", ON_TOP_WND_PET_PRIMIUM_FEEDSTUFF);

	PyModule_AddIntConstant(poModule, "PET_FEED",							PET_FEED);

	PyModule_AddIntConstant(poModule, "PET_FEED_SLOT_MAX",					PET_FEED_SLOT_MAX);
	PyModule_AddIntConstant(poModule, "PET_GROWTH_EVOL_MAX",				PET_GROWTH_EVOL_MAX);
	PyModule_AddIntConstant(poModule, "PET_GROWTH_SKILL_LEVEL_MAX",			PET_GROWTH_SKILL_LEVEL_MAX);
	PyModule_AddIntConstant(poModule, "PET_GROWTH_SKILL_OPEN_EVOL_LEVEL",	PET_GROWTH_SKILL_OPEN_EVOL_LEVEL);
	PyModule_AddIntConstant(poModule, "PET_REVIVE_MATERIAL_SLOT_MAX",		PET_REVIVE_MATERIAL_SLOT_MAX);
	PyModule_AddIntConstant(poModule, "PET_SKILL_COUNT_MAX",				PET_SKILL_COUNT_MAX);
	PyModule_AddIntConstant(poModule, "LIFE_TIME_FLASH_MIN_TIME",			LIFE_TIME_FLASH_MIN_TIME);
	PyModule_AddIntConstant(poModule, "ITEM_SLOT_COUNT",					ITEM_SLOT_COUNT);
	PyModule_AddIntConstant(poModule, "SPECIAL_EVOL_MIN_AGE",				SPECIAL_EVOL_MIN_AGE);

	PyModule_AddIntConstant(poModule, "FEED_LIFE_TIME_EVENT",				FEED_LIFE_TIME_EVENT);
	PyModule_AddIntConstant(poModule, "FEED_EXP_EVENT",						FEED_EXP_EVENT);
	PyModule_AddIntConstant(poModule, "FEED_EVOL_EVENT",					FEED_EVOL_EVENT);
	PyModule_AddIntConstant(poModule, "FEED_BUTTON_MAX",					FEED_BUTTON_MAX);

	PyModule_AddIntConstant(poModule, "PET_WINDOW_INFO",					PET_WINDOW_INFO);
	PyModule_AddIntConstant(poModule, "PET_WINDOW_ATTR_CHANGE",				PET_WINDOW_ATTR_CHANGE);
	PyModule_AddIntConstant(poModule, "PET_WINDOW_PRIMIUM_FEEDSTUFF",		PET_WINDOW_PRIMIUM_FEEDSTUFF);

	PyModule_AddIntConstant(poModule, "PET_WND_SLOT_ATTR_CHANGE",			PET_WND_SLOT_ATTR_CHANGE);
	PyModule_AddIntConstant(poModule, "PET_WND_SLOT_ATTR_CHANGE_ITEM",		PET_WND_SLOT_ATTR_CHANGE_ITEM);
	PyModule_AddIntConstant(poModule, "PET_WND_SLOT_ATTR_CHANGE_RESULT",	PET_WND_SLOT_ATTR_CHANGE_RESULT);
	PyModule_AddIntConstant(poModule, "PET_WND_SLOT_ATTR_CHANGE_MAX",		PET_WND_SLOT_ATTR_CHANGE_MAX);

	PyModule_AddIntConstant(poModule, "PET_ATTR_DETERMINE_ITEM",			PET_ATTR_DETERMINE_ITEM);
	PyModule_AddIntConstant(poModule, "PET_ATTR_CHANGE_ITEM",				PET_ATTR_CHANGE_ITEM);

	PyModule_AddIntConstant(poModule, "QUICK_SLOT_POS_ERROR",				QUICK_SLOT_POS_ERROR);
	PyModule_AddIntConstant(poModule, "QUICK_SLOT_ITEM_USE_SUCCESS",		QUICK_SLOT_ITEM_USE_SUCCESS);
	PyModule_AddIntConstant(poModule, "QUICK_SLOT_IS_NOT_ITEM",				QUICK_SLOT_IS_NOT_ITEM);
	PyModule_AddIntConstant(poModule, "QUICK_SLOT_PET_ITEM_USE_SUCCESS",	QUICK_SLOT_PET_ITEM_USE_SUCCESS);
	PyModule_AddIntConstant(poModule, "QUICK_SLOT_PET_ITEM_USE_FAILED",		QUICK_SLOT_PET_ITEM_USE_FAILED);
	PyModule_AddIntConstant(poModule, "QUICK_SLOT_CAN_NOT_USE_PET_ITEM",	QUICK_SLOT_CAN_NOT_USE_PET_ITEM);
#endif
}

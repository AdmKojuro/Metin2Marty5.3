/*
* Title: Dungeon Information System
* Author: Owsap
* Description: List of all available dungeons.
* Date: 2021.01.09
* Last Update: 2021.05.19
* Version 2.0.0.1
*
* Skype: owsap.
* Discord: Owsap#0905
*
* 0x0
*
* Web: https://owsap-productions.com/
* GitHub: https://github.com/Owsap
*/

#include "StdAfx.h"

#if defined(ENABLE_DUNGEON_INFO_SYSTEM)
#include "PythonDungeonInfo.h"
#include "PythonNetworkStream.h"

PyObject* dungeonInfoClose(PyObject* poSelf, PyObject* poArgs)
{
	CPythonDungeonInfo::Instance().Close();
	return Py_BuildNone();
}

PyObject* dungeonInfoOpen(PyObject* poSelf, PyObject* poArgs)
{
	CPythonDungeonInfo::Instance().Open();
	return Py_BuildNone();
}

PyObject* dungeonInfoWarp(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	CPythonDungeonInfo::Instance().Warp(byIndex);
	return Py_BuildNone();
}

PyObject* dungeonInfoRanking(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE byRankType;
	if (!PyTuple_GetByte(poArgs, 1, &byRankType))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendDungeonInfo(DungeonInfo::EAction::RANK, byIndex, byRankType);
	return Py_BuildNone();
}

PyObject* dungeonInfoGetCount(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetCount());
}

PyObject* dungeonInfoGetKey(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetKey(byIndex));
}

PyObject* dungeonInfoGetType(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetType(byIndex));
}

PyObject* dungeonInfoGetMapIndex(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetMapIndex(byIndex));
}

PyObject* dungeonInfoGetEntryMapIndex(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetEntryMapIndex(byIndex));
}

PyObject* dungeonInfoGetBossVnum(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetBossVnum(byIndex));
}

PyObject* dungeonInfoGetLevelLimit(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE byLimit;
	if (!PyTuple_GetByte(poArgs, 1, &byLimit))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetLevelLimit(byIndex, byLimit));
}

PyObject* dungeonInfoGetMemberLimit(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE byLimit;
	if (!PyTuple_GetByte(poArgs, 1, &byLimit))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetMemberLimit(byIndex, byLimit));
}

PyObject* dungeonInfoGetRequiredItemVnum(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE bySlot;
	if (!PyTuple_GetByte(poArgs, 1, &bySlot))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetRequiredItemVnum(byIndex, bySlot));
}

PyObject* dungeonInfoGetRequiredItemCount(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE bySlot;
	if (!PyTuple_GetByte(poArgs, 1, &bySlot))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetRequiredItemCount(byIndex, bySlot));
}

PyObject* dungeonInfoGetDuration(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetDuration(byIndex));
}

PyObject* dungeonInfoGetCooldown(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetCooldown(byIndex));
}

PyObject* dungeonInfoGetElement(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetElement(byIndex));
}

PyObject* dungeonInfoGetAttBonusCount(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetAttBonusCount(byIndex));
}

PyObject* dungeonInfoGetAttBonus(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE byType;
	if (!PyTuple_GetByte(poArgs, 1, &byType))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetAttBonus(byIndex, byType));
}

PyObject* dungeonInfoGetDefBonusCount(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetDefBonusCount(byIndex));
}

PyObject* dungeonInfoDefAttBonus(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE byType;
	if (!PyTuple_GetByte(poArgs, 1, &byType))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetDefBonus(byIndex, byType));
}

PyObject* dungeonInfoGetBossDropCount(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetBossDropCount(byIndex));
}

PyObject* dungeonInfoGetBossDropItemVnum(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE bySlot;
	if (!PyTuple_GetByte(poArgs, 1, &bySlot))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetBossDropItemVnum(byIndex, bySlot));
}

PyObject* dungeonInfoGetBossDropItemCount(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	BYTE bySlot;
	if (!PyTuple_GetByte(poArgs, 1, &bySlot))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetBossDropItemCount(byIndex, bySlot));
}

PyObject* dungeonInfoGetTotalFinished(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetFinish(byIndex));
}

PyObject* dungeonInfoGetFastestTime(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetFinishTime(byIndex));
}

PyObject* dungeonInfoGetHighestDamage(PyObject* poSelf, PyObject* poArgs)
{
	BYTE byIndex;
	if (!PyTuple_GetByte(poArgs, 0, &byIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetFinishDamage(byIndex));
}

//
// Ranking
//

PyObject* dungeonInfoGetRankingCount(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetRankingCount());
}

PyObject* dungeonInfoGetRankingByLine(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BadArgument();

	const auto c_aRankingData = CPythonDungeonInfo::Instance().GetRankingByLine(iIndex);
	if (!c_aRankingData)
		return Py_BuildException("Failed to find rank by index %d", iIndex);

	return Py_BuildValue("sii", c_aRankingData->strName.c_str(), c_aRankingData->iLevel, c_aRankingData->dwPoints);
}

PyObject* dungeonInfoGetMyRankingByLine(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonDungeonInfo::Instance().GetMyRankingLine());
}

PyObject* dungeonInfoClearRanking(PyObject* poSelf, PyObject* poArgs)
{
	CPythonDungeonInfo::Instance().ClearRankingData();
	return Py_BuildNone();
}

void initDungeonInfo()
{
	static PyMethodDef s_methods[] =
	{
		{ "Close", dungeonInfoClose, METH_VARARGS },
		{ "Open", dungeonInfoOpen, METH_VARARGS },
		{ "Warp", dungeonInfoWarp, METH_VARARGS },
		{ "Ranking", dungeonInfoRanking, METH_VARARGS },

		{ "GetKey", dungeonInfoGetKey, METH_VARARGS },
		{ "GetCount", dungeonInfoGetCount, METH_VARARGS },
		{ "GetType", dungeonInfoGetType, METH_VARARGS },
		{ "GetMapIndex", dungeonInfoGetMapIndex, METH_VARARGS },
		{ "GetEntryMapIndex", dungeonInfoGetEntryMapIndex, METH_VARARGS },
		{ "GetBossVnum", dungeonInfoGetBossVnum, METH_VARARGS },
		{ "GetLevelLimit", dungeonInfoGetLevelLimit, METH_VARARGS },
		{ "GetMemberLimit", dungeonInfoGetMemberLimit, METH_VARARGS },
		{ "GetRequiredItemVnum", dungeonInfoGetRequiredItemVnum, METH_VARARGS },
		{ "GetRequiredItemCount", dungeonInfoGetRequiredItemCount, METH_VARARGS },
		{ "GetDuration", dungeonInfoGetDuration, METH_VARARGS },
		{ "GetCooldown", dungeonInfoGetCooldown, METH_VARARGS },
		{ "GetElement", dungeonInfoGetElement, METH_VARARGS },
		{ "GetAttBonusCount", dungeonInfoGetAttBonusCount, METH_VARARGS },
		{ "GetAttBonus", dungeonInfoGetAttBonus, METH_VARARGS },
		{ "GetDefBonusCount", dungeonInfoGetDefBonusCount, METH_VARARGS },
		{ "GetDefBonus", dungeonInfoDefAttBonus, METH_VARARGS },
		{ "GetBossDropCount", dungeonInfoGetBossDropCount, METH_VARARGS },
		{ "GetBossDropItemVnum", dungeonInfoGetBossDropItemVnum, METH_VARARGS },
		{ "GetBossDropItemCount", dungeonInfoGetBossDropItemCount, METH_VARARGS },

		{ "GetFinish", dungeonInfoGetTotalFinished, METH_VARARGS },
		{ "GetFinishTime", dungeonInfoGetFastestTime, METH_VARARGS },
		{ "GetFinishDamage", dungeonInfoGetHighestDamage, METH_VARARGS },

		{ "GetRankingCount", dungeonInfoGetRankingCount, METH_VARARGS },
		{ "GetRankingByLine", dungeonInfoGetRankingByLine, METH_VARARGS },
		{ "GetMyRankingLine", dungeonInfoGetMyRankingByLine, METH_VARARGS },
		{ "ClearRanking", dungeonInfoClearRanking, METH_VARARGS },

		{ NULL, NULL, NULL },
	};

	PyObject* poModule = Py_InitModule("dungeonInfo", s_methods);

	PyModule_AddIntConstant(poModule, "MAX_DUNGEONS", DungeonInfo::EMisc::MAX_DUNGEONS);
	PyModule_AddIntConstant(poModule, "MAX_REQUIRED_ITEMS", DungeonInfo::EMisc::MAX_REQUIRED_ITEMS);
	PyModule_AddIntConstant(poModule, "MAX_BOSS_ITEM_SLOTS", DungeonInfo::EMisc::MAX_BOSS_ITEM_SLOTS);
	PyModule_AddIntConstant(poModule, "MAX_BOSS_DROP_ITEMS", DungeonInfo::EMisc::MAX_BOSS_DROP_ITEMS);
	PyModule_AddIntConstant(poModule, "MAX_DUNGEON_SCROLL", DungeonInfo::EMisc::MAX_DUNGEON_SCROLL);

	PyModule_AddIntConstant(poModule, "CLOSE", DungeonInfo::EAction::CLOSE);
	PyModule_AddIntConstant(poModule, "OPEN", DungeonInfo::EAction::OPEN);
	PyModule_AddIntConstant(poModule, "WARP", DungeonInfo::EAction::WARP);
	PyModule_AddIntConstant(poModule, "RANK", DungeonInfo::EAction::RANK);

	PyModule_AddIntConstant(poModule, "MAX_RANKING_LINES", DungeonInfo::ERank::MAX_RANKING_LINES);
	PyModule_AddIntConstant(poModule, "MAX_RANKING_COUNT", DungeonInfo::ERank::MAX_RANKING_COUNT);

	PyModule_AddIntConstant(poModule, "RANKING_TYPE_COMPLETED", DungeonInfo::ERankTypes::RANKING_TYPE_COMPLETED);
	PyModule_AddIntConstant(poModule, "RANKING_TYPE_TIME", DungeonInfo::ERankTypes::RANKING_TYPE_TIME);
	PyModule_AddIntConstant(poModule, "RANKING_TYPE_DAMAGE", DungeonInfo::ERankTypes::RANKING_TYPE_DAMAGE);
}

#endif

#include "StdAfx.h"
#include "PythonApplication.h"
#include "Locale_inc.h"

PyObject* renderTargetSelectModel(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SelectModel(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetChangeWeapon(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeWeapon(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetChangeArmor(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeArmor(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetChangeHair(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeHair(modelIndex);

	return Py_BuildNone();
}
PyObject* renderTargetChangeAcce(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeAcce(modelIndex - 85000);

	return Py_BuildNone();
}

#ifdef ENABLE_AURA_SYSTEM
PyObject* renderTargetChangeAura(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeAura(modelIndex);

	return Py_BuildNone();
}
#endif

#ifdef ENABLE_SHINING_ITEMS
PyObject* renderTargetChangeWeaponShining(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeWeaponShining(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetChangeArmorShining(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeArmorShining(modelIndex);

	return Py_BuildNone();
}

PyObject* renderTargetChangeSpecialShining(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	int modelIndex = 0;
	if (!PyTuple_GetInteger(poArgs, 1, &modelIndex))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeSpecialShining(modelIndex);

	return Py_BuildNone();
}
#endif

PyObject* renderTargetChangeEffect(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->ChangeEffect();

	return Py_BuildNone();
}

PyObject* renderTargetSetVisibility(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	bool isShow = false;
	if (!PyTuple_GetBoolean(poArgs, 1, &isShow))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SetVisibility(isShow);

	return Py_BuildNone();
}

PyObject* renderTargetSetBackground(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	char * szPathName;
	if (!PyTuple_GetString(poArgs, 1, &szPathName))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->CreateBackground(
		szPathName, CPythonApplication::Instance().GetWidth(),
		CPythonApplication::Instance().GetHeight());
	return Py_BuildNone();
}

PyObject* renderTargetSetZoom(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index = 0;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BadArgument();

	float zoom_value = 1500.0f;
	if (!PyTuple_GetFloat(poArgs, 1, &zoom_value))
		return Py_BadArgument();

	CRenderTargetManager::Instance().GetRenderTarget(index)->SetZoom(zoom_value);

	return Py_BuildNone();
}


void initRenderTarget() {
	static PyMethodDef s_methods[] =
	{
		{ "SelectModel", renderTargetSelectModel, METH_VARARGS },
		{ "SetVisibility", renderTargetSetVisibility, METH_VARARGS },
		{ "SetBackground", renderTargetSetBackground, METH_VARARGS },
		{ "ChangeArmor", renderTargetChangeArmor, METH_VARARGS },
		{ "ChangeWeapon", renderTargetChangeWeapon, METH_VARARGS },
		{ "ChangeHair", renderTargetChangeHair, METH_VARARGS },
		{ "ChangeAcce", renderTargetChangeAcce, METH_VARARGS },
#ifdef ENABLE_AURA_SYSTEM
		{ "ChangeAura", renderTargetChangeAura, METH_VARARGS },
#endif
		{ "ChangeEffect", renderTargetChangeEffect, METH_VARARGS },
		{ "SetZoom", renderTargetSetZoom, METH_VARARGS},

#ifdef ENABLE_SHINING_ITEMS
		{ "ChangeWeaponShining", renderTargetChangeWeaponShining, METH_VARARGS },
		{ "ChangeArmorShining", renderTargetChangeArmorShining, METH_VARARGS },
		{ "ChangeSpecialShining", renderTargetChangeSpecialShining, METH_VARARGS },
#endif

		{nullptr, nullptr, 0 },
	};

	PyObject* poModule = Py_InitModule("renderTarget", s_methods);

}
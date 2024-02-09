#include "StdAfx.h"
#include "Python67Bonus.h"
#include "PythonNetworkStream.h"
#include "Packet.h"

CPython67Bonus::CPython67Bonus()
{
}

CPython67Bonus::~CPython67Bonus()
{
}
 

PyObject* netSendSkillBookCombinationPacket(PyObject* poSelf, PyObject* poArgs)
{

	PyObject* cell;
	if (!PyTuple_GetObject(poArgs, 0, &cell))
		return Py_BuildException();

	int index_skillbook_combi;
	if (!PyTuple_GetInteger(poArgs, 1, &index_skillbook_combi))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendSkillBookCombinationPacket(cell,index_skillbook_combi);
	return Py_BuildNone();
}

PyObject* netSendRegistFragmentPacket(PyObject* poSelf, PyObject* poArgs)
{

	int cell;
	if (!PyTuple_GetInteger(poArgs, 0, &cell))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendRegistFragmentPacket(cell);
	return Py_BuildNone();
}

PyObject* netSendAddAttrCombPacket(PyObject* poSelf, PyObject* poArgs)
{
	int index_comb;
	if (!PyTuple_GetInteger(poArgs, 0, &index_comb))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendAddAttrCombPacket(index_comb);
	return Py_BuildNone();
}

PyObject* netSendAddAttrPacket(PyObject* poSelf, PyObject* poArgs)
{

	int cell;
	if (!PyTuple_GetInteger(poArgs, 0, &cell))
		return Py_BuildException();

	int count_fragment;
	if (!PyTuple_GetInteger(poArgs, 1, &count_fragment))
		return Py_BuildException();

	int cell_additive;
	if (!PyTuple_GetInteger(poArgs, 2, &cell_additive))
		return Py_BuildException();

	int count_additive;
	if (!PyTuple_GetInteger(poArgs, 3, &count_additive))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendAddAttrPacket(cell,count_fragment,cell_additive,count_additive);
	return Py_BuildNone();
}



void int67bonus()
{
	static PyMethodDef s_methods[] =
	{

		{"SendSkillBookCombinationPacket"	, netSendSkillBookCombinationPacket, 	METH_VARARGS},
		{"SendRegistFragmentPacket"			, netSendRegistFragmentPacket, 			METH_VARARGS},
		{"SendAddAttrPacket"				, netSendAddAttrPacket,					METH_VARARGS},
		{"SendAddAttrCombPacket"			, netSendAddAttrCombPacket,				METH_VARARGS},
		{ NULL,								NULL,								NULL },
	};

	PyObject * poModule = Py_InitModule("bonus67", s_methods);
}
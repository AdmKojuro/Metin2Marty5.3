#include "StdAfx.h"
#include "PythonDropItem.h"
#include "PythonNetworkStream.h"
#include "Packet.h"


CPythonDropItem::CPythonDropItem()
	: m_DropHandler(NULL)
{
	ClearList();
}

CPythonDropItem::~CPythonDropItem()
{
	ClearList();
}


void CPythonDropItem::LoadingList()
{
	if(m_DropHandler)
		PyCallClassMemberFunc(m_DropHandler, "BINARY_DROP_ITEM_LOADING", Py_BuildValue("()"));
}

void CPythonDropItem::ClearList()
{
	PageSelect(1);
	item_drop_list.clear();
}

void CPythonDropItem::ReceiveList(const TInfoDropItemSystem & TInfoDrop)
{
	TInfoDropItemSystem TInfoDropItem;

	memcpy (TInfoDropItem.name_mob, 	TInfoDrop.name_mob, sizeof(TInfoDropItem.name_mob));
	TInfoDropItem.id_mob 				= TInfoDrop.id_mob;
	TInfoDropItem.prob_item 			= TInfoDrop.prob_item;
	TInfoDropItem.count_item 			= TInfoDrop.count_item;

	item_drop_list.push_back(TInfoDropItem);
}

int CPythonDropItem::GetCountList()
{
	return item_drop_list.size();
}

const CPythonDropItem::TInfoStrucDropItem & CPythonDropItem::GetList()
{
	return item_drop_list;
}

PyObject* DropItemOpen(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendDropItemOpen();
	return Py_BuildNone();
}


PyObject* SearchDropItem(PyObject* poSelf, PyObject* poArgs)
{

	int vnum_item;
	if (!PyTuple_GetInteger(poArgs, 0, &vnum_item))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SearchDropItem(vnum_item);
	return Py_BuildNone();
}

PyObject * GetCountDropItemList(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonDropItem::Instance().GetCountList());
}

PyObject * GetCountPageView(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonDropItem::Instance().GetCountPageView());
}

PyObject * GetDropItemDates(PyObject * poSelf, PyObject * poArgs)
{
	int index;
	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	const CPythonDropItem::TInfoStrucDropItem & DropItemVector = CPythonDropItem::Instance().GetList();
	if (DWORD(index) >= DropItemVector.size())
		return Py_BuildValue("siii","None",0,0,0);


	const TInfoDropItemSystem & CDropItemVector = DropItemVector[index];
	return Py_BuildValue("siii",
		CDropItemVector.name_mob,
		CDropItemVector.id_mob,
		CDropItemVector.prob_item,
		CDropItemVector.count_item
		);
}

PyObject* SetPageSelect(PyObject * poSelf, PyObject * poArgs)
{
	int page_select;
	if (!PyTuple_GetInteger(poArgs, 0, &page_select))
		return Py_BuildException();

	CPythonDropItem::Instance().PageSelect(page_select);

	return Py_BuildNone();
}

PyObject* GetPageActual(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonDropItem::Instance().GetPageActual());
}

PyObject * SetDropItemHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject * poEventHandler;
	if (!PyTuple_GetObject(poArgs, 0, &poEventHandler))
		return Py_BuildException();

	CPythonDropItem::Instance().SetDropItemHandler(poEventHandler);
}


void intdropitem()
{
	static PyMethodDef s_methods[] =
	{
		{ "Open",						DropItemOpen,							METH_VARARGS },
		{ "SearchItem",					SearchDropItem,							METH_VARARGS },
		{ "CountDates",					GetCountDropItemList,					METH_VARARGS },
		{ "GetDates", 					GetDropItemDates, 						METH_VARARGS },
		{ "GetPageView",				GetCountPageView,						METH_VARARGS },
		{ "SetPageSelect",				SetPageSelect,							METH_VARARGS },
		{ "GetPageActual",				GetPageActual,							METH_VARARGS },
		{ "SetDropItemHandler",			SetDropItemHandler,						METH_VARARGS },
		

		{ NULL,								NULL,								NULL },
	};

	PyObject * poModule = Py_InitModule("drop_item", s_methods);
}
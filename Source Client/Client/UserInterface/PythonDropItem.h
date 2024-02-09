#pragma once
#include "Packet.h"

class CPythonDropItem : public CSingleton<CPythonDropItem>
{
	public:
		enum{

			PAGES_VIEW = 5,
		};

		typedef std::vector<TInfoDropItemSystem> TInfoStrucDropItem;

	public:
		CPythonDropItem();
		virtual ~CPythonDropItem();


		void ClearList();
		void LoadingList();
		void ReceiveList(const TInfoDropItemSystem & TInfoDrop);
		int  GetCountList();
		int  GetCountPageView(){return PAGES_VIEW;}
		
		void PageSelect(int _page_select){page_actual = _page_select;}
		int  GetPageActual(){return page_actual;}

		void SetDropItemHandler(PyObject* _DropHandler){m_DropHandler = _DropHandler;}


		const TInfoStrucDropItem & GetList();

	private:
		int page_actual;
		TInfoStrucDropItem item_drop_list;
		PyObject* m_DropHandler;

};
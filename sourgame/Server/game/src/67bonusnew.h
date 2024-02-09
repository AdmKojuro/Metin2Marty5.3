#include <boost/unordered_map.hpp>
#include "../../common/stl.h"
#include "../../common/tables.h"

enum
{
	TIME_WAIT_BONUS67 = 30, //Seconds
	MAX_FRAGMENTS 		= 10,
	MAX_SUPPORT			= 5,
	PERCENT_FRAGMENT 	= 2,

};

class C67BonusNew : public singleton<C67BonusNew>
{	

	public:
		C67BonusNew();

		void CombiStart(LPCHARACTER ch, int cell[c_skillbook_slot_max]);
		bool CheckCombiStart(LPCHARACTER ch, int cell[c_skillbook_slot_max]);
		void DeleteItemsCombi(LPCHARACTER, int cell[c_skillbook_slot_max]);
		void ChechFragment(LPCHARACTER ch, int cell);
		int GetVnumFragment(LPCHARACTER ch,LPITEM item);
		void SendDate67BonusNewPackets(LPCHARACTER ch, DWORD vnum);
		void AddAttr(LPCHARACTER ch, int cell, int count_fragment, int cell_additive, int count_additive);
		bool CheckItemAdd(LPITEM item);
		void SetTimeAddAttr(LPCHARACTER ch, int time);
		int GetTimeAddAttr(LPCHARACTER ch);
		bool GetPosGetItem(LPCHARACTER ch);
		int  GetItemAttr(LPCHARACTER ch);
		void SetPorcentTotalAttr(LPCHARACTER ch, int porcent);
		int GetPorcentAttr(LPCHARACTER ch);


};



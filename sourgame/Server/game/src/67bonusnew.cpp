#include "stdafx.h"
#include "67bonusnew.h"
#include "char.h"
#include "locale_service.h"
#include "packet.h"
#include "desc_client.h"
#include "item.h"
#include "item_manager.h"
#include "constants.h"
#include "utils.h"

C67BonusNew::C67BonusNew()
{
}

bool C67BonusNew::CheckItemAdd(LPITEM item)
{
	if((item->GetType() == ITEM_ARMOR || (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW)))
	{
		if (item->GetAttributeCount() >= 5 && item->GetAttributeCount() < 7)
		{
			return true;

		}else{

			return false;
		}
		
	}

	return false;

}

int C67BonusNew::GetVnumFragment(LPCHARACTER ch,LPITEM item)
{

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limit = item->GetProto()->aLimits[i].lValue;
		switch (item->GetProto()->aLimits[i].bType)
		{
			case LIMIT_LEVEL:{
				if (limit <= 29){
					return 39070;
				}else if (limit == 30){
					return 39078;
				}else if (limit >= 31 && limit <= 39){
					return 39071;
				}else if (limit >= 40 && limit <= 49){
					return 39072;
				}else if (limit >= 50 && limit <= 59){
					return 39073;
				}else if (limit >= 60 && limit <= 74){
					return 39074;
				}else if (limit == 75){
					return 39079;
				}else if (limit >= 76 && limit <= 89){
					return 39075;
				}else if (limit >= 90 && limit <= 104){
					return 39076;
				}else if (limit == 105){
					return 39080;
				}else if (limit > 105){
					return 39077;
				}
			}
			break;
		}
	}

	return 0;
}

void C67BonusNew::ChechFragment(LPCHARACTER ch, int cell)
{
	if (cell < 0)
	{
		return;
	}

	LPITEM item = ch->GetInventoryItem(cell);
	if(item)
	{
		if(CheckItemAdd(item) == false)
		{
			return;
		}

		int get_vnum_fragment = GetVnumFragment(ch,item);
		if(get_vnum_fragment == 0)
		{
			return;
		}

		ch->DateItemAdd(item);
		SendDate67BonusNewPackets(ch,get_vnum_fragment);
	}
}

bool C67BonusNew::CheckCombiStart(LPCHARACTER ch, int cell[c_skillbook_slot_max])
{

	int total_items = -1;

	for (int i = 0; i < c_skillbook_slot_max; ++i)
	{	
		LPITEM pItem = ch->GetInventoryItem(cell[i]);

		if(pItem)
		{
			if (pItem->GetType() == ITEM_SKILLBOOK)
			{
				total_items++;
			}
		}
	}

	if (total_items != (c_skillbook_slot_max-1)){
		return false;
	}

	if(ch->GetGold() < GOLD_COMB_SKILLBOOK){
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈이 부족하거나 아이템이 제자리에 없습니다."));	// 이 텍스트는 이미 널리 쓰이는거라 추가번역 필요 없음
		return false;
	}

	return true;
}

void C67BonusNew::DeleteItemsCombi(LPCHARACTER ch, int cell[c_skillbook_slot_max])
{
	for (int i = 0; i < c_skillbook_slot_max; ++i)
	{
		LPITEM pItem = ch->GetInventoryItem(cell[i]);
		pItem->SetCount(pItem->GetCount()-1);
	}

	ch->PointChange(POINT_GOLD, -GOLD_COMB_SKILLBOOK);
	ch->AutoGiveItem(39067);
}

void C67BonusNew::CombiStart(LPCHARACTER ch, int cell[c_skillbook_slot_max])
{

	bool count_check = CheckCombiStart(ch,cell);
	if(count_check == false){
		return;
	}

	DeleteItemsCombi(ch,cell);
	
}

void C67BonusNew::SetPorcentTotalAttr(LPCHARACTER ch, int porcent)
{
	ch->SetQuestFlag("worldard_bonus67.porcent_bonus67",porcent);
}

int C67BonusNew::GetPorcentAttr(LPCHARACTER ch)
{
	return ch->GetQuestFlag("worldard_bonus67.porcent_bonus67");
}

void C67BonusNew::SetTimeAddAttr(LPCHARACTER ch, int time)
{
	ch->SetQuestFlag("worldard_bonus67.time_wait_bonus67",time);
}

int C67BonusNew::GetTimeAddAttr(LPCHARACTER ch)
{
	return ch->GetQuestFlag("worldard_bonus67.time_wait_bonus67")-get_global_time();
}

void C67BonusNew::AddAttr(LPCHARACTER ch, int cell, int count_fragment, int cell_additive, int count_additive)
{

	if ((count_fragment <= 0 && count_additive <= 0) || cell < 0)
	{
		return;
	}

	LPITEM get_space = ch->GetBonus67NewItem();
	LPITEM get_item = ch->GetDateItemAdd();
	LPITEM item_send = ch->GetInventoryItem(cell);
	LPITEM item_additive = NULL;
	int porcent_total = 0;

	if(!get_item || !item_send || get_space)
	{
		return;
	}

	if((cell_additive >= 0 && count_additive <= 0) || (count_additive > 0 && cell_additive < 0) || count_additive > MAX_SUPPORT)
	{
		return;
	}

	if(get_item->GetID() != item_send->GetID())
	{
		return;
	}

	if(CheckItemAdd(item_send) == false || GetVnumFragment(ch,item_send) == 0)
	{
		return;
	}

	if (count_fragment != 0){
		if (ch->CountSpecifyItem(GetVnumFragment(ch,item_send)) < count_fragment || count_fragment > MAX_FRAGMENTS || count_fragment <= 0)
		{
			return;
		}
		ch->RemoveSpecifyItem(GetVnumFragment(ch,item_send), count_fragment);
		porcent_total += (count_fragment * PERCENT_FRAGMENT);
	}

	if(cell_additive >= 0 && count_additive > 0){
		item_additive = ch->GetInventoryItem(cell_additive);
		if(!item_additive)
		{
			return;

		}else{

			int itemVNum = item_additive->GetVnum();
			int itemCount = item_additive->GetCount();

			if(itemVNum >= 72064 && itemVNum <= 72067){
				if (itemCount < count_additive)
				{
					return;
				}

				porcent_total += count_additive * (item_additive->GetValue(1)/MAX_SUPPORT);
				item_additive->SetCount(item_additive->GetCount()-count_additive);
				
			}else{
				return;
			}
		}
	}


	ch->DateItemAdd(NULL);
	SetTimeAddAttr(ch,get_global_time()+TIME_WAIT_BONUS67);
	SetPorcentTotalAttr(ch,porcent_total);
	ch->ChatPacket(CHAT_TYPE_INFO,"[NEW BONUS] Item agregado exitosamente: %s.",item_send->GetName());
	item_send->RemoveFromCharacter();
	ch->SetItem(TItemPos(BONUS_NEW_67, 0), item_send);

}

bool C67BonusNew::GetPosGetItem(LPCHARACTER ch)
{
	LPITEM item = ch->GetBonus67NewItem();
	if(item)
	{
		int iEmptyPos = item->IsDragonSoul() ? ch->GetEmptyDragonSoulInventory(item) : ch->GetEmptyInventory(item->GetSize());
		if (iEmptyPos < 0)
		{
			return false;
		}

		return true;
	}

	return false;
}

int C67BonusNew::GetItemAttr(LPCHARACTER ch)
{
	LPITEM item = ch->GetBonus67NewItem();
	if(item)
	{
		int iEmptyPos = item->IsDragonSoul() ? ch->GetEmptyDragonSoulInventory(item) : ch->GetEmptyInventory(item->GetSize());
		item->RemoveFromCharacter();

		int porcent = number(1,100);

		// 1 = true attr - 2 = false attr

		if(porcent <= GetPorcentAttr(ch))
		{
			item->AddRareAttribute();
			ch->SetItem(TItemPos(item->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : INVENTORY, iEmptyPos), item);
			SetTimeAddAttr(ch,0);
			SetPorcentTotalAttr(ch,0);
			return 1;
		}
		else{

			ch->SetItem(TItemPos(item->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : INVENTORY, iEmptyPos), item);
			SetTimeAddAttr(ch,0);
			SetPorcentTotalAttr(ch,0);
			return 2;
		}
	
	}

	return 0;
}

void C67BonusNew::SendDate67BonusNewPackets(LPCHARACTER ch, DWORD vnum)
{

	TPacketGC67BonusReceive pack;
	pack.subheader = BONUS_67_NEW_SUB_HEADER_FRAGMENT_RECEIVE;
	pack.vnum_fragment = vnum;

	LPDESC d = ch->GetDesc();

	if (NULL == d)
	{
		sys_err ("User SendDate67BonusNewPackets (%s)'s DESC is NULL POINT.", ch->GetName());
		return ;
	}

	d->Packet(&pack, sizeof(pack));

}
#include "stdafx.h"
#include "../../libgame/include/grid.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "shop.h"
#include "desc.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "questmanager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "locale_service.h"

//#define ENABLE_SHOP_BLACKLIST
/* ------------------------------------------------------------------------------------ */
CShop::CShop()
	: m_dwVnum(0), m_dwNPCVnum(0), m_pkPC(NULL)
{
	m_pGrid = M2_NEW CGrid(10, 10);
#ifdef OFFLINE_SHOP
	SetLocked(false);
#endif
}

CShop::~CShop()
{
	TPacketGCShop pack;

	pack.header		= HEADER_GC_SHOP;
	pack.subheader	= SHOP_SUBHEADER_GC_END;
	pack.size		= sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

	GuestMapType::iterator it;

	it = m_map_guest.begin();

	while (it != m_map_guest.end())
	{
		LPCHARACTER ch = it->first;
		ch->SetShop(NULL);
		++it;
	}

	M2_DELETE(m_pGrid);
}

void CShop::SetPCShop(LPCHARACTER ch)
{
	m_pkPC = ch;
}

bool CShop::Create(DWORD dwVnum, DWORD dwNPCVnum, TShopItemTable * pTable)
{
	/*
	   if (NULL == CMobManager::instance().Get(dwNPCVnum))
	   {
	   sys_err("No such a npc by vnum %d", dwNPCVnum);
	   return false;
	   }
	 */
	sys_log(0, "SHOP #%d (Shopkeeper %d)", dwVnum, dwNPCVnum);

	m_dwVnum = dwVnum;
	m_dwNPCVnum = dwNPCVnum;

	BYTE bItemCount;

	for (bItemCount = 0; bItemCount < SHOP_HOST_ITEM_MAX_NUM; ++bItemCount)
		if (0 == (pTable + bItemCount)->vnum)
			break;

	SetShopItems(pTable, bItemCount);
	return true;
}

void CShop::SetShopItems(TShopItemTable * pTable, BYTE bItemCount)
{
	if (bItemCount > SHOP_HOST_ITEM_MAX_NUM)
		return;

	m_pGrid->Clear();

	m_itemVector.resize(SHOP_HOST_ITEM_MAX_NUM);
	msl::refill(m_itemVector);

	for (int i = 0; i < bItemCount; ++i)
	{
		LPITEM pkItem = NULL;
		const TItemTable * item_table;

		if (m_pkPC)
		{
			pkItem = m_pkPC->GetItem(pTable->pos);

			if (!pkItem)
			{
				sys_err("cannot find item on pos (%d, %d) (name: %s)", pTable->pos.window_type, pTable->pos.cell, m_pkPC->GetName());
				continue;
			}

			item_table = pkItem->GetProto();
		}
		else
		{
			if (!pTable->vnum)
				continue;

			item_table = ITEM_MANAGER::instance().GetTable(pTable->vnum);
		}

		if (!item_table)
		{
			sys_err("Shop: no item table by item vnum #%d", pTable->vnum);
			continue;
		}

		int iPos;

		if (IsPCShop())
		{
			sys_log(0, "MyShop: use position %d", pTable->display_pos);
			iPos = pTable->display_pos;
		}
		else
			iPos = m_pGrid->FindBlank(1, item_table->bSize);

		if (iPos < 0)
		{
			sys_err("not enough shop window");
			continue;
		}

		if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
		{
			if (IsPCShop())
			{
				sys_err("not empty position for pc shop %s[%d]", m_pkPC->GetName(), m_pkPC->GetPlayerID());
			}
			else
			{
				sys_err("not empty position for npc shop");
			}
			continue;
		}

		m_pGrid->Put(iPos, 1, item_table->bSize);

		SHOP_ITEM & item = m_itemVector[iPos];

		item.pkItem = pkItem;
		item.itemid = 0;

		if (item.pkItem)
		{
			item.vnum = pkItem->GetVnum();
			item.count = pkItem->GetCount();
			item.price = pTable->price;
			item.itemid	= pkItem->GetID();
#ifdef __ENABLE_CHEQUE_SYSTEM__
			item.cheque = pTable->cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
		}
		else
		{
			item.vnum = pTable->vnum;
			item.count = pTable->count;
			item.price = pTable->price;

#ifdef ENABLE_BUY_ITEMS_WORLDARD
			item.item_vnum_buy = pTable->item_vnum_buy;
			item.item_count_buy = pTable->item_count_buy;
#endif

			if (IS_SET(item_table->dwFlags, ITEM_FLAG_COUNT_PER_1GOLD))
			{
				if (item_table->dwGold == 0)
					item.price = item.count;
				else
					item.price = item.count / item_table->dwGold;
			}
			else
				item.price = item_table->dwGold * item.count;
#ifdef __ENABLE_CHEQUE_SYSTEM__
			item.cheque = 0;
#endif // __ENABLE_CHEQUE_SYSTEM__
		}

		char name[36];
		snprintf(name, sizeof(name), "%-20s(#%-5d) (x %d)", item_table->szName, (int) item.vnum, item.count);

		sys_log(0, "SHOP_ITEM: %-36s PRICE %-5d", name, item.price);
		++pTable;
	}
}

#ifdef OFFLINE_SHOP
#include "char.h"
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
long long CShop::Buy(LPCHARACTER ch, BYTE pos, bool bIsShopSearch)
#else
long long CShop::Buy(LPCHARACTER ch, BYTE pos)
#endif
{
	if (IsLocked())
		return SHOP_SUBHEADER_GC_END;
	
	if (pos >= m_itemVector.size())
	{
		sys_log(0, "Shop::Buy : invalid position %d : %s", pos, ch->GetName());
		return SHOP_SUBHEADER_GC_INVALID_POS;
	}

	sys_log(0, "Shop::Buy : name %s pos %d", ch->GetName(), pos);

	GuestMapType::iterator it = m_map_guest.find(ch);

#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
	if (bIsShopSearch == false && it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;
#else
	if (it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;
#endif

	SHOP_ITEM& r_item = m_itemVector[pos];

#ifdef ENABLE_BUY_ITEMS_WORLDARD
	if (r_item.price < 0 && r_item.item_vnum_buy == 0)
#else
	if (r_item.price < 0)	
#endif
	{
		LogManager::instance().HackLog("SHOP_BUY_GOLD_OVERFLOW", ch);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}

#ifdef __ENABLE_CHEQUE_SYSTEM__
	if (r_item.cheque<0){
		LogManager::instance().HackLog("SHOP_BUY_CHEQUE_OVERFLOW", ch);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE;
	}
#endif // __ENABLE_CHEQUE_SYSTEM__

	LPITEM pkSelectedItem = ITEM_MANAGER::instance().Find(r_item.itemid);

#ifdef ENABLE_BUY_ITEMS_WORLDARD
	DWORD 	item_vnum_buy  = r_item.item_vnum_buy;
	DWORD 	item_count_buy = r_item.item_count_buy;
#endif
#ifdef FULL_YANG
	long long dwPrice = r_item.price;
#else
	DWORD dwPrice = r_item.price;
#endif
	DWORD dwTax = 0;
#ifdef __ENABLE_CHEQUE_SYSTEM__
	DWORD dwCheque = r_item.cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__


	int iVal = quest::CQuestManager::instance().GetEventFlag ("personal_shop");

	if (0 < iVal)
	{
		if (iVal > 100)
		{
			iVal = 100;
		}

		dwTax = dwPrice * iVal / 100;
		dwPrice = dwPrice - dwTax;
	}
	else
	{
		iVal = 0;
		dwTax = 0;
	}

#ifdef __ENABLE_CHEQUE_SYSTEM__
	if ((int)dwPrice>0 && (int)dwCheque > 0) // Yang-Cheque
	{
		if (ch->GetGold() < (int)dwPrice || ch->GetCheque() < (int)dwCheque)
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_CHEQUE;
	}
	else if ((int)dwPrice>0 && (int)dwCheque <= 0) // Yang
	{
		if (ch->GetGold() < (int)dwPrice)
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}
	else if ((int)dwPrice<=0 && (int)dwCheque > 0) // cheque
	{
		if (ch->GetCheque() < (int)dwCheque)
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE;
	}
#else
	if (ch->GetGold() < r_item.price)
	{
		sys_log(1, "Shop::Buy : Not enough money : %s has %d, price %d", ch->GetName(), ch->GetGold(), dwPrice);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}
#endif
#ifdef ENABLE_BUY_ITEMS_WORLDARD
	if (item_vnum_buy == 0)
	{
		if (ch->GetGold() < r_item.price)
		{
			sys_log(1, "Shop::Buy : Not enough money : %s has %d, price %lld", ch->GetName(), ch->GetGold(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
		}
	}
	else
	{
		if (ch->CountSpecifyItemBuy(item_vnum_buy) < (int) item_count_buy){
			//ch->ChatPacket(CHAT_TYPE_INFO, "Shop::Buy : Not enough count : %s has %d, price %d", ch->GetName(), ch->CountSpecifyItemBuy(item_vnum_buy), item_count_buy);
			sys_log(1, "Shop::Buy : Not enough count : %s has %d, price %d", ch->GetName(), ch->CountSpecifyItemBuy(item_vnum_buy), item_count_buy);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_COUNT;
		}
	}
#else
	if (ch->GetGold() < r_item.price)
	{
		sys_log(1, "Shop::Buy : Not enough money : %s has %d, price %lld", ch->GetName(), ch->GetGold(), dwPrice);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}
#endif

	LPITEM item;

	if (m_pkPC) 
		item = r_item.pkItem;
	else {
		item = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);
	}
	if (!item)
		return SHOP_SUBHEADER_GC_SOLD_OUT;

	int iEmptyPos;
	if (item->IsDragonSoul())
	{
		iEmptyPos = ch->GetEmptyDragonSoulInventory(item);
	}
#ifdef __SPECIAL_INVENTORY_SYSTEM__
	else if (item->IsSkillBook())
	{
		iEmptyPos = ch->GetEmptySkillBookInventory(item->GetSize());
	}
	else if (item->IsUpgradeItem())
	{
		iEmptyPos = ch->GetEmptyUpgradeItemsInventory(item->GetSize());
	}
	else if (item->IsStone())
	{
		iEmptyPos = ch->GetEmptyStoneInventory(item->GetSize());
	}
	else if (item->IsGiftBox())
	{
		iEmptyPos = ch->GetEmptyGiftBoxInventory(item->GetSize());
	}
#endif

	else
	{
		iEmptyPos = ch->GetEmptyInventory(item->GetSize());
	}

	if (iEmptyPos < 0)
	{
		if (m_pkPC)
		{
			sys_log(1, "Shop::Buy at PC Shop : Inventory full : %s size %d", ch->GetName(), item->GetSize());
			return SHOP_SUBHEADER_GC_INVENTORY_FULL;
		}
		else
		{
			sys_log(1, "Shop::Buy : Inventory full : %s size %d", ch->GetName(), item->GetSize());
			M2_DESTROY_ITEM(item);
			return SHOP_SUBHEADER_GC_INVENTORY_FULL;
		}
	}
#ifdef ENABLE_BUY_ITEMS_WORLDARD
	if (item_vnum_buy == 0)
	{
#ifdef FULL_YANG_OWN
		ch->ChangeGold(-dwPrice);
#else
		ch->PointChange(POINT_GOLD, -dwPrice, false);
#endif
	}
	else
	{
		ch->RemoveSpecifyItem(item_vnum_buy, item_count_buy);
	}
#else
	ch->PointChange(POINT_GOLD, -dwPrice, false);
#endif
#ifdef __ENABLE_CHEQUE_SYSTEM__
	ch->PointChange(POINT_CHEQUE, -dwCheque, false);
#endif // __ENABLE_CHEQUE_SYSTEM__
	// ±?AO ?????? : ??±Y A??o
	if (m_pkPC)
	{
		if (!m_pkPC->IsPrivShop())
			m_pkPC->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 255);

		char buf[512];
		DWORD mpid = (m_pkPC->IsPrivShop() ? m_pkPC->GetPrivShopOwner() : m_pkPC->GetPlayerID());
		if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
		{
			snprintf(buf, sizeof(buf), "%s FROM: %u TO: %u PRICE: %lld", item->GetName(), ch->GetPlayerID(), mpid, dwPrice);
			LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), SHOP_BUY, buf);
			LogManager::instance().GoldBarLog(mpid, item->GetID(), SHOP_SELL, buf);
		}
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);

		if (pkNewItem)
		{
			pkNewItem->SetRefineElement(item->GetRefineElement());
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
			{
				pkNewItem->SetSocket(i, item->GetSocket(i));
			}
			// ?? ??? ??
			item->CopyAttributeTo(pkNewItem);

			if (pkNewItem->IsDragonSoul())
				pkNewItem->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));

			else
				pkNewItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);
			std::string szBuy("SHOP_BUY");
			std::string szSell("SHOP_SELL");
			if (m_pkPC->IsPrivShop())
			{
				std::string temp("OFFLINE_");
				szBuy = temp + szBuy;
				szSell = temp + szSell;
			}
#ifdef FULL_YANG
			snprintf(buf, sizeof(buf), "%s %u(%s) %lld %u", pkNewItem->GetName(), mpid, m_pkPC->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(ch, pkNewItem, szBuy.c_str(), buf);
			snprintf(buf, sizeof(buf), "%s %u(%s) %lld %u", pkNewItem->GetName(), ch->GetPlayerID(), ch->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(m_pkPC, pkNewItem, szSell.c_str(), buf);
#else
			snprintf(buf, sizeof(buf), "%s %u(%s) %u %u", pkNewItem->GetName(), mpid, m_pkPC->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(ch, pkNewItem, szBuy.c_str(), buf);
			snprintf(buf, sizeof(buf), "%s %u(%s) %u %u", pkNewItem->GetName(), ch->GetPlayerID(), ch->GetName(), dwPrice, pkNewItem->GetCount());
			LogManager::instance().ItemLog(m_pkPC, pkNewItem, szSell.c_str(), buf);
#endif



			r_item.pkItem = NULL;
			BroadcastUpdateItem(pos);
			if (m_pkPC->IsPrivShop()) {
				LPCHARACTER owner = CHARACTER_MANAGER::instance().FindByPID(m_pkPC->GetPrivShopOwner());
				

#ifdef FULL_YANG
					DBManager::instance().DirectQuery("UPDATE player_shop SET gold=gold + %lld WHERE id=%d", dwPrice, m_pkPC->GetPrivShop());
#else
					DBManager::instance().DirectQuery("UPDATE player_shop SET gold=gold + %d WHERE id=%d", dwPrice, m_pkPC->GetPrivShop());
#endif
#ifdef __ENABLE_CHEQUE_SYSTEM__
					DBManager::instance().DirectQuery("UPDATE player_shop SET cheque=cheque + %d WHERE id=%d", dwCheque, m_pkPC->GetPrivShop());
#endif // __ENABLE_CHEQUE_SYSTEM__
					
			
				DBManager::instance().DirectQuery("DELETE FROM player_shop_items WHERE shop_id=%d and id = %d", m_pkPC->GetPrivShop(), r_item.itemid);
				if (owner)
				{
					owner->LoadPrivShops();
#ifdef FULL_YANG
					if (owner->GetGiftPages() > 0)
						owner->ChatPacket(CHAT_TYPE_COMMAND, "gift_info %d", owner->GetGiftPages());
#else
					owner->RefreshGift();
#endif
				}
			}
			else{
#ifdef FULL_YANG_OWN
				m_pkPC->ChangeGold(dwPrice);
#else
				m_pkPC->PointChange(POINT_GOLD, dwPrice, false);
#endif
#ifdef __ENABLE_CHEQUE_SYSTEM__
				m_pkPC->PointChange(POINT_CHEQUE, dwCheque, false);
#endif // __ENABLE_CHEQUE_SYSTEM__
				if (iVal > 0)
				{
					m_pkPC->ChatPacket (CHAT_TYPE_INFO, LC_TEXT ("????? %d %% ? ???? ??????"), iVal);
				}

			}
			item = pkNewItem;
		}
		else {
			return SHOP_SUBHEADER_GC_INVALID_POS;
		}


	}
	else
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
		else
			item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);
		LogManager::instance().ItemLog(ch, item, "BUY", item->GetName());

		if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
		{
			LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), PERSONAL_SHOP_BUY, "");
		}

		//DBManager::instance().SendMoneyLog(MONEY_LOG_SHOP, item->GetVnum(), -dwPrice);
	}

	if (item)
		sys_log(0, "SHOP: BUY: name %s %s(x %d):%u price %lld", ch->GetName(), item->GetName(), item->GetCount(), item->GetID(), dwPrice);
	ch->Save();
#if defined(SHOP_AUTO_CLOSE) && defined(OFFLINE_SHOP)
	if (m_pkPC && m_pkPC->IsPrivShop())
	{
		if (GetItemCount()<=0)
			m_pkPC->DeleteMyShop();
	}
#endif
	return (SHOP_SUBHEADER_GC_OK);
}
void CShop::RemoveGuests(LPCHARACTER except)
{
	GuestMapType::iterator it = m_map_guest.begin();
	while (it != m_map_guest.end())
	{
		LPCHARACTER ch = it->first;
		if (except && ch == except)
		{
			it++;
			continue;
		}
		if (ch)
		{
			ch->SetShop(NULL);

			TPacketGCShop pack;

			pack.header = HEADER_GC_SHOP;
			pack.subheader = SHOP_SUBHEADER_GC_END;
			pack.size = sizeof(TPacketGCShop);

			ch->GetDesc()->Packet(&pack, sizeof(pack));
		}
		m_map_guest.erase(it++);
	}
}
void CShop::ClearItems()
{
	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		m_itemVector[i].pkItem = NULL;
		BroadcastUpdateItem(i);
	}
}
int CShop::GetItemCount()
{
	int count = 0;
	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		if (m_itemVector[i].pkItem)
			count++;
	}
	return count;
}
bool CShop::GetItems()
{
	if (!m_pkPC)
		return false;
	if (!m_pkPC)
		return false;
	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		if (!m_itemVector[i].pkItem)
			continue;
		char query[8192];
		sprintf(query, "INSERT INTO player_gift SET \
		owner_id=%d,vnum=%d,count=%d,refine_element=%d,reason='%s',`from`=replace(\"%s\",' ','_'),status='WAIT',date_add=NOW()",
		m_pkPC->GetPrivShopOwner(), m_itemVector[i].vnum, m_itemVector[i].count, m_itemVector[i].pkItem->GetRefineElement(), LC_TEXT("SHOP_REASON"), m_pkPC->GetName());

		for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
		{
			sprintf(query, "%s, socket%d='%ld'", query, s, m_itemVector[i].pkItem->GetSocket(s));

		}
		for (int ia = 0; ia < ITEM_ATTRIBUTE_MAX_NUM; ia++)
		{
			const TPlayerItemAttribute& attr = m_itemVector[i].pkItem->GetAttribute(ia);
			if (ia < 7)
			{
				sprintf(query, "%s, attrtype%d='%u'", query, ia, attr.bType);
				sprintf(query, "%s, attrvalue%d='%d'", query, ia, attr.sValue);
			}
			else
			{
				sprintf(query, "%s, applytype%d='%u'", query, ia-7, attr.bType);
				sprintf(query, "%s, applyvalue%d='%d'", query, ia-7, attr.sValue);
			}
		}
		DBManager::instance().DirectQuery(query);

		DBManager::instance().DirectQuery("delete from player_shop_items where id='%d'", m_itemVector[i].itemid);
		m_itemVector[i].pkItem->RemoveFromCharacter();
		m_itemVector[i].pkItem = NULL;
		BroadcastUpdateItem(i);

	}


	return true;
}
void CShop::SetPrivShopItems(std::vector<TShopItemTable *> map_shop)
{
	m_pGrid->Clear();

	m_itemVector.resize(SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_itemVector[0], 0, sizeof(SHOP_ITEM) * m_itemVector.size());

	for (DWORD count = 0;count < map_shop.size();count++)
	{
		TShopItemTable * pTable = map_shop[count];
			LPITEM pkItem = NULL;
			const TItemTable * item_table;

			if (!m_pkPC)
				return;
			pkItem = m_pkPC->GetItem(pTable->pos);

			if (!pkItem)
			{
				sys_err("cannot find item on pos (%d, %d) (name: %s)", pTable->pos.window_type, pTable->pos.cell, m_pkPC->GetName());
				continue;
			}

		item_table = pkItem->GetProto();


		if (!item_table)
		{
			sys_err("Shop: no item table by item vnum #%d", pTable->vnum);
			continue;
		}

		WORD iPos = pTable->display_pos;
		sys_log(0, "MyShop: use position %d", iPos);


		if (iPos < 0)
		{
			sys_err("not enough shop window");
			continue;
		}

		if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
		{
			sys_err("not empty position for pc shop %s[%d] fixing", m_pkPC->GetName(), m_pkPC->GetPlayerID());
			iPos = m_pGrid->FindBlank(1, item_table->bSize);
			if (!m_pGrid->IsEmpty(iPos, 1, item_table->bSize))
			{
				sys_err("not empty position for pc shop %s[%d]", m_pkPC->GetName(), m_pkPC->GetPlayerID());
				continue;
			}
		}

		m_pGrid->Put(iPos, 1, item_table->bSize);

		SHOP_ITEM & item = m_itemVector[iPos];

		item.pkItem = pkItem;
		item.itemid = 0;
#ifdef __ENABLE_CHEQUE_SYSTEM__
		item.cheque = 0;
#endif // __ENABLE_CHEQUE_SYSTEM__
		if (item.pkItem)
		{
			item.vnum = pkItem->GetVnum();
			item.count = pkItem->GetCount(); // PC ?? ?? ??? ??? ?? ???? ???? ??.
			item.price = pTable->price; // ??? ???? ????..
			item.itemid = pkItem->GetRealID();
#ifdef __ENABLE_CHEQUE_SYSTEM__
			item.cheque = pTable->cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
		}

		char name[36];
		snprintf(name, sizeof(name), "%-20s(#%-5d) (x %d)", item_table->szName, (int)item.vnum, item.count);

		sys_log(0, "PRIV_SHOP_ITEM: %-36s PRICE %-5d", name, item.price);
	}
}
#else
extern bool FN_check_item_socket(LPITEM item);
long long CShop::Buy(LPCHARACTER ch, BYTE pos)
{
	if (pos >= m_itemVector.size())
	{
		sys_log(0, "Shop::Buy : invalid position %d : %s", pos, ch->GetName());
		return SHOP_SUBHEADER_GC_INVALID_POS;
	}

	sys_log(0, "Shop::Buy : name %s pos %d", ch->GetName(), pos);

	GuestMapType::iterator it = m_map_guest.find(ch);

	if (it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;
	if (ch == m_pkPC)
		return SHOP_SUBHEADER_GC_END;
	SHOP_ITEM& r_item = m_itemVector[pos];

	if (r_item.price < 0)
	{
		LogManager::instance().HackLog("SHOP_BUY_GOLD_OVERFLOW", ch);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}

	LPITEM pkSelectedItem = ITEM_MANAGER::instance().Find(r_item.itemid);

	if (IsPCShop())
	{
		if (!pkSelectedItem)
		{
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
			if (bIsShopSearch == true)
				return SHOP_SUBHEADER_GC_SOLDOUT;
#endif
			sys_log(0, "Shop::Buy : Critical: This user seems to be a hacker : invalid pcshop item : BuyerPID:%d SellerPID:%d",
					ch->GetPlayerID(),
					m_pkPC->GetPlayerID());
			return false;
		}

		if ((pkSelectedItem->GetOwner() != m_pkPC))
		{
#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
			if (bIsShopSearch == true)
				return SHOP_SUBHEADER_GC_SOLDOUT;
#endif
			sys_log(0, "Shop::Buy : Critical: This user seems to be a hacker : invalid pcshop item : BuyerPID:%d SellerPID:%d",
					ch->GetPlayerID(),
					m_pkPC->GetPlayerID());

			return false;
		}
	}

	long long  dwPrice = r_item.price;


	if (ch->GetGold() < (long long) dwPrice)
	{
		sys_log(1, "Shop::Buy : Not enough money : %s has %lld, price %lld", ch->GetName(), ch->GetGold(), dwPrice);
		return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
	}

	LPITEM item;

	if (m_pkPC) // ??? ???? ?? ??? ?? ???? ?????? ??.
		item = r_item.pkItem;
	else
		item = ITEM_MANAGER::instance().CreateItem(r_item.vnum, r_item.count);

	if (!item)
		return SHOP_SUBHEADER_GC_SOLD_OUT;



	WORD iEmptyPos;
	if (item->IsDragonSoul())
	{
		iEmptyPos = ch->GetEmptyDragonSoulInventory(item);
	}
	else
	{
		iEmptyPos = ch->GetEmptyInventory(item->GetSize());
	}

	if (iEmptyPos < 0)
	{
		if (m_pkPC)
		{
			sys_log(1, "Shop::Buy at PC Shop : Inventory full : %s size %d", ch->GetName(), item->GetSize());
			return SHOP_SUBHEADER_GC_INVENTORY_FULL;
		}
		else
		{
			sys_log(1, "Shop::Buy : Inventory full : %s size %d", ch->GetName(), item->GetSize());
			M2_DESTROY_ITEM(item);
			return SHOP_SUBHEADER_GC_INVENTORY_FULL;
		}
	}
	ch->SetGold(ch->GetGold() - dwPrice);
	ch->SendGold();
	//ch->PointChange(POINT_GOLD, -dwPrice, false);
	/*TShopTable test;
	std::map<int, TShopTable *> map_shop;
	TShopTable * shop_table;
	
	shop_table->dwNPCVnum;

	
	ch->SetQuestNPCID('2');
	quest::CQuestManager::instance().OnBuy(ch->GetPlayerID(), item);
	*/

	

	// ?? ??? : ?? ??
	if (m_pkPC)
	{

		m_pkPC->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 255);

		char buf[512];

		if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
		{
			snprintf(buf, sizeof(buf), "%s FROM: %u TO: %u PRICE: %lld", item->GetName(), ch->GetPlayerID(), m_pkPC->GetPlayerID(), dwPrice);
			LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), SHOP_BUY, buf);
			LogManager::instance().GoldBarLog(m_pkPC->GetPlayerID(), item->GetID(), SHOP_SELL, buf);
		}
		
		item->RemoveFromCharacter();
		if (item->IsDragonSoul())
			item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
		else
			item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);
		

		snprintf(buf, sizeof(buf), "%s %u(%s) %lld %u", item->GetName(), m_pkPC->GetPlayerID(), m_pkPC->GetName(), dwPrice, item->GetCount());
		LogManager::instance().ItemLog(ch, item, "SHOP_BUY", buf);
		// BUY EVENT
		

		snprintf(buf, sizeof(buf), "%s %u(%s) %lld %u", item->GetName(), ch->GetPlayerID(), ch->GetName(), dwPrice, item->GetCount());
		LogManager::instance().ItemLog(m_pkPC, item, "SHOP_SELL", buf);
	

		r_item.pkItem = NULL;
		BroadcastUpdateItem(pos);
		
		m_pkPC->SetGold(m_pkPC->GetGold() + dwPrice);
		m_pkPC->SendGold();

		
		
	}
	else
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
		else{
			WORD bCount = item->GetCount();
			if ((item->GetFlag() == 4 || item->GetFlag() == 20))
			{

				for (WORD i = 0; i < INVENTORY_MAX_NUM; ++i)
				{
					LPITEM item2 = ch->GetInventoryItem(i);

					if (!item2)
						continue;

					if (item2->GetVnum() == item->GetVnum())
					{
						int j;

						for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
							if (item2->GetSocket(j) != item->GetSocket(j))
								break;

						if (j != ITEM_SOCKET_MAX_NUM)
							continue;

						WORD bCount2 = MIN(g_bItemCountLimit - item2->GetCount(), bCount);
						bCount -= bCount2;

						item2->SetCount(item2->GetCount() + bCount2);

						if (bCount == 0)
						{
							break;
						}
					}
				}

				item->SetCount(bCount);
			}
			if (bCount > 0)
			{
				item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
			}
			else
				M2_DESTROY_ITEM(item);
		}
		ITEM_MANAGER::instance().FlushDelayedSave(item);
		LogManager::instance().ItemLog(ch, item, "BUY", item->GetName());

		if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
		{
			LogManager::instance().GoldBarLog(ch->GetPlayerID(), item->GetID(), PERSONAL_SHOP_BUY, "");
		}

		//DBManager::instance().SendMoneyLog(MONEY_LOG_SHOP, item->GetVnum(), -dwPrice);
	}

	if (item)
		sys_log(0, "SHOP: BUY: name %s %s(x %d):%u price %lld", ch->GetName(), item->GetName(), item->GetCount(), item->GetID(), dwPrice);

	ch->Save();
	return (SHOP_SUBHEADER_GC_OK);
}
#endif

bool CShop::AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire)
{
	if (!ch)
		return false;

	if (ch->GetExchange())
		return false;

	if (ch->GetShop())
		return false;

	ch->SetShop(this);

	m_map_guest.insert(GuestMapType::value_type(ch, bOtherEmpire));

	TPacketGCShop pack;

	pack.header		= HEADER_GC_SHOP;
	pack.subheader	= SHOP_SUBHEADER_GC_START;

	TPacketGCShopStart pack2;

	memset(&pack2, 0, sizeof(pack2));
	pack2.owner_vid = owner_vid;

	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const SHOP_ITEM & item = m_itemVector[i];

#ifdef ENABLE_SHOP_BLACKLIST
		//HIVALUE_ITEM_EVENT
		if (quest::CQuestManager::instance().GetEventFlag("hivalue_item_sell") == 0)
		{

			if (item.vnum == 70024 || item.vnum == 70035)
			{
				continue;
			}
		}
#endif
		//END_HIVALUE_ITEM_EVENT
		if (m_pkPC && !item.pkItem)
			continue;

		pack2.items[i].vnum = item.vnum;

		// REMOVED_EMPIRE_PRICE_LIFT
#ifdef ENABLE_NEWSTUFF
		if (bOtherEmpire && !g_bEmpireShopPriceTripleDisable) // no empire price penalty for pc shop
#else
		if (bOtherEmpire) // no empire price penalty for pc shop
#endif
		{
			pack2.items[i].price = item.price * 3;
		}
		else
			pack2.items[i].price = item.price;
#ifdef __ENABLE_CHEQUE_SYSTEM__
		pack2.items[i].cheque = item.cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
		// END_REMOVED_EMPIRE_PRICE_LIFT

		pack2.items[i].count = item.count;

		if (item.pkItem)
		{
			thecore_memcpy(pack2.items[i].alSockets, item.pkItem->GetSockets(), sizeof(pack2.items[i].alSockets));
			thecore_memcpy(pack2.items[i].aAttr, item.pkItem->GetAttributes(), sizeof(pack2.items[i].aAttr));
#ifdef ENABLE_REFINE_ELEMENT
			pack2.items[i].dwRefineElement = item.pkItem->GetRefineElement();
#endif		
		}
#ifdef ENABLE_BUY_ITEMS_WORLDARD
		pack2.items[i].item_vnum_buy = item.item_vnum_buy;
		pack2.items[i].item_count_buy = item.item_count_buy;
#endif
	}

	pack.size = sizeof(pack) + sizeof(pack2);

	ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
	ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCShopStart));
	return true;
}

void CShop::RemoveGuest(LPCHARACTER ch)
{
	if (ch->GetShop() != this)
		return;

	m_map_guest.erase(ch);
	ch->SetShop(NULL);

	TPacketGCShop pack;

	pack.header		= HEADER_GC_SHOP;
	pack.subheader	= SHOP_SUBHEADER_GC_END;
	pack.size		= sizeof(TPacketGCShop);

	ch->GetDesc()->Packet(&pack, sizeof(pack));
}

void CShop::Broadcast(const void * data, int bytes)
{
	sys_log(1, "Shop::Broadcast %p %d", data, bytes);

	GuestMapType::iterator it;

	it = m_map_guest.begin();

	while (it != m_map_guest.end())
	{
		LPCHARACTER ch = it->first;

		if (ch->GetDesc())
			ch->GetDesc()->Packet(data, bytes);

		++it;
	}
}

void CShop::BroadcastUpdateItem(BYTE pos)
{
	TPacketGCShop pack;
	TPacketGCShopUpdateItem pack2;

	TEMP_BUFFER	buf;

	pack.header		= HEADER_GC_SHOP;
	pack.subheader	= SHOP_SUBHEADER_GC_UPDATE_ITEM;
	pack.size		= sizeof(pack) + sizeof(pack2);

	pack2.pos		= pos;

	if (m_pkPC && !m_itemVector[pos].pkItem)
		pack2.item.vnum = 0;
	else
	{
		pack2.item.vnum	= m_itemVector[pos].vnum;
		if (m_itemVector[pos].pkItem)
		{
			thecore_memcpy(pack2.item.alSockets, m_itemVector[pos].pkItem->GetSockets(), sizeof(pack2.item.alSockets));
			thecore_memcpy(pack2.item.aAttr, m_itemVector[pos].pkItem->GetAttributes(), sizeof(pack2.item.aAttr));
#ifdef ENABLE_REFINE_ELEMENT
			pack2.item.dwRefineElement = m_itemVector[pos].pkItem->GetRefineElement();
#endif	
		}
		else
		{
			memset(pack2.item.alSockets, 0, sizeof(pack2.item.alSockets));
			memset(pack2.item.aAttr, 0, sizeof(pack2.item.aAttr));
#ifdef ENABLE_REFINE_ELEMENT
			pack2.item.dwRefineElement = 0;
#endif	
		}
	}

	pack2.item.price	= m_itemVector[pos].price;
#ifdef __ENABLE_CHEQUE_SYSTEM__
	pack2.item.cheque = m_itemVector[pos].cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
	pack2.item.count	= m_itemVector[pos].count;

	buf.write(&pack, sizeof(pack));
	buf.write(&pack2, sizeof(pack2));

	Broadcast(buf.read_peek(), buf.size());
}

int CShop::GetNumberByVnum(DWORD dwVnum)
{
	int itemNumber = 0;

	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		const SHOP_ITEM & item = m_itemVector[i];

		if (item.vnum == dwVnum)
		{
			itemNumber += item.count;
		}
	}

	return itemNumber;
}

bool CShop::IsSellingItem(DWORD itemID)
{
	bool isSelling = false;

	for (DWORD i = 0; i < m_itemVector.size() && i < SHOP_HOST_ITEM_MAX_NUM; ++i)
	{
		if ((unsigned int)(m_itemVector[i].itemid) == itemID)
		{
			isSelling = true;
			break;
		}
	}

	return isSelling;

}


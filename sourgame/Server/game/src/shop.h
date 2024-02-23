#ifndef __INC_METIN_II_GAME_SHOP_H__
#define __INC_METIN_II_GAME_SHOP_H__

enum
{
	SHOP_MAX_DISTANCE = 1000
};

class CGrid;

/* ---------------------------------------------------------------------------------- */
class CShop
{
	public:
		typedef struct shop_item
		{
			DWORD	vnum;
#ifdef FULL_YANG
			long	long	price;
#else
			DWORD	price;
#endif
			BYTE	count;
			DWORD	m_dwRefineElement;
			LPITEM	pkItem;
			int		itemid;
#ifdef __ENABLE_CHEQUE_SYSTEM__
			int		cheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
#ifdef ENABLE_BUY_ITEMS_WORLDARD
			DWORD 	item_vnum_buy;
			DWORD 	item_count_buy;
#endif
			shop_item()
			{
				vnum = 0;
				price = 0;
				count = 0;
				m_dwRefineElement = 0;
				itemid = 0;
#ifdef __ENABLE_CHEQUE_SYSTEM__
				cheque = 0;
#endif // __ENABLE_CHEQUE_SYSTEM__
#ifdef ENABLE_BUY_ITEMS_WORLDARD
				item_vnum_buy = 0;
				item_count_buy = 0;
#endif
				pkItem = NULL;
			}
		} SHOP_ITEM;

		CShop();
		virtual ~CShop(); // @fixme139 (+virtual)

		bool	Create(DWORD dwVnum, DWORD dwNPCVnum, TShopItemTable * pItemTable);
		void	SetShopItems(TShopItemTable * pItemTable, BYTE bItemCount);

		virtual void	SetPCShop(LPCHARACTER ch);
		virtual bool	IsPCShop()	{ return m_pkPC ? true : false; }


		virtual bool	AddGuest(LPCHARACTER ch,DWORD owner_vid, bool bOtherEmpire);
		void	RemoveGuest(LPCHARACTER ch);


#if defined(BL_PRIVATESHOP_SEARCH_SYSTEM)
		const std::vector<SHOP_ITEM>& GetItemVector() const { return m_itemVector; }
		LPCHARACTER GetShopOwner() { return m_pkPC; }
		virtual long long	Buy(LPCHARACTER ch, BYTE pos, bool bIsShopSearch = false);
#else
		virtual long long	Buy(LPCHARACTER ch, BYTE pos);
#endif


		void	BroadcastUpdateItem(BYTE pos);


		int		GetNumberByVnum(DWORD dwVnum);


		virtual bool	IsSellingItem(DWORD itemID);

		DWORD	GetVnum() { return m_dwVnum; }
		DWORD	GetNPCVnum() { return m_dwNPCVnum; }

#ifdef OFFLINE_SHOP
	public:
		int		GetItemCount();
		bool	GetItems();
		void	SetLocked(bool val) { m_bLocked = val; }
		bool	IsLocked() { return m_bLocked; }
		void	RemoveGuests(LPCHARACTER except);
		void	ClearItems();
		void	SetPrivShopItems(std::vector<TShopItemTable *> map_shop);
		LPCHARACTER GetPC() { return m_pkPC; }
	protected:
		bool					m_bLocked;
#endif


	protected:
		void	Broadcast(const void * data, int bytes);

	protected:
		DWORD				m_dwVnum;
		DWORD				m_dwNPCVnum;

		CGrid *				m_pGrid;

		typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
		GuestMapType m_map_guest;
		std::vector<SHOP_ITEM>		m_itemVector;

		LPCHARACTER			m_pkPC;
};

#endif


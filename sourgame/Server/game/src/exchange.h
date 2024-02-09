#ifndef __INC_METIN_II_GAME_EXCHANGE_H__
#define __INC_METIN_II_GAME_EXCHANGE_H__

class CGrid;

enum EExchangeValues
{
#ifdef __NEW_EXCHANGE_WINDOW__
	EXCHANGE_ITEM_MAX_NUM	= 24,
#else
	EXCHANGE_ITEM_MAX_NUM	= 12,
#endif
	EXCHANGE_MAX_DISTANCE	= 1000
};

class CExchange
{
	public:
		CExchange(LPCHARACTER pOwner);
		~CExchange();

		bool		Accept(bool bIsAccept = true);
		void		Cancel();

		bool		AddGold(long lGold);
#ifdef __ENABLE_CHEQUE_SYSTEM__
		bool		AddCheque(long Cheque);
#endif // __ENABLE_CHEQUE_SYSTEM__
		bool		AddItem(TItemPos item_pos, BYTE display_pos);
		bool		RemoveItem(BYTE pos);

		LPCHARACTER	GetOwner()	{ return m_pOwner;	}
		CExchange *	GetCompany()	{ return m_pCompany;	}

		bool		GetAcceptStatus() { return m_bAccept; }

		void		SetCompany(CExchange * pExchange)	{ m_pCompany = pExchange; }

	private:
		bool		Done();
		bool		Check(int * piItemCount);
		bool		CheckSpace();

	private:
		CExchange *	m_pCompany;

		LPCHARACTER	m_pOwner;

		TItemPos		m_aItemPos[EXCHANGE_ITEM_MAX_NUM];
		LPITEM		m_apItems[EXCHANGE_ITEM_MAX_NUM];
		BYTE		m_abItemDisplayPos[EXCHANGE_ITEM_MAX_NUM];

		bool 		m_bAccept;
		long		m_lGold;
#ifdef __ENABLE_CHEQUE_SYSTEM__
		DWORD		m_lCheque;
#endif // __ENABLE_CHEQUE_SYSTEM__
		CGrid *		m_pGrid;

};

#endif


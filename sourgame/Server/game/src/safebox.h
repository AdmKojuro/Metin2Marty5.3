#ifndef __INC_METIN_II_GAME_SAFEBOX_H__
#define __INC_METIN_II_GAME_SAFEBOX_H__

class CHARACTER;
class CItem;
class CGrid;

#ifdef __GROWTH_PET_SYSTEM__
#include "growth_pet.h"
#endif

class CSafebox
{
	public:
		CSafebox(LPCHARACTER pkChrOwner, int iSize, DWORD dwGold);
		~CSafebox();

		bool		Add(DWORD dwPos, LPITEM pkItem);
		LPITEM		Get(DWORD dwPos);
		LPITEM		Remove(DWORD dwPos);
		void		ChangeSize(int iSize);

		bool		MoveItem(BYTE bCell, BYTE bDestCell, BYTE count);
		LPITEM		GetItem(BYTE bCell);

		void		Save();

		bool		IsEmpty(DWORD dwPos, BYTE bSize);
		bool		IsValidPosition(DWORD dwPos);

		void		SetWindowMode(BYTE bWindowMode);
		
#ifdef __GROWTH_PET_SYSTEM__
		void		LoadPet(DWORD dwCount, TGrowthPet* pPets);
		void		AddPet(LPGROWTH_PET pPet);
		bool		RemovePet(LPITEM pSummonItem);
#endif


	protected:
		void		__Destroy();

		LPCHARACTER	m_pkChrOwner;
		LPITEM		m_pkItems[SAFEBOX_MAX_NUM];
		CGrid *		m_pkGrid;
		int		m_iSize;
		long		m_lGold;

		BYTE		m_bWindowMode;
		
#ifdef __GROWTH_PET_SYSTEM__
		CGrowthPetManager::TGrowthPetMap	m_growthPetMap;
#endif
};

#endif


#pragma once

#include "ItemData.h"

class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_NUM,
		};

#ifdef ENABLE_ACCE_SYSTEM
		enum EItemScaleCol
		{
			ITEMSCALE_COL_VNUM,
			ITEMSCALE_COL_JOB,
			ITEMSCALE_COL_SEX,
			ITEMSCALE_COL_SCALE_X,
			ITEMSCALE_COL_SCALE_Y,
			ITEMSCALE_COL_SCALE_Z,
			ITEMSCALE_COL_PARTICLE_SCALE,
		};
#endif

#ifdef ENABLE_AURA_SYSTEM
	public:
		enum EAuraScaleCol
		{
			AURA_SCALE_COL_VNUM,
			AURA_SCALE_COL_JOB,
			AURA_SCALE_COL_SEX,
			AURA_SCALE_COL_MESH_SCALE_X,
			AURA_SCALE_COL_MESH_SCALE_Y,
			AURA_SCALE_COL_MESH_SCALE_Z,
			AURA_SCALE_COL_PARTICLE_SCALE,
			AURA_SCALE_COL_NUM,
		};

		bool	LoadAuraScale(const char* c_szFileName);
#endif

	public:
		typedef std::map<DWORD, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;

	public:
		CItemManager();
		virtual ~CItemManager();

		void			Destroy();

		TItemMap		GetItems() const { return m_ItemMap; }

		BOOL			SelectItemData(DWORD dwIndex);
		CItemData *		GetSelectedItemDataPointer();
#ifdef ENABLE_ITEM_EXTRA_PROTO
		CItemData::TItemExtraProto* GetSelectedExtraProto();
#endif
		BOOL			GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData);

		/////
		bool			LoadItemDesc(const char* c_szFileName);
		bool			LoadItemList(const char* c_szFileName);
		bool			LoadItemTable(const char* c_szFileName);
		CItemData *		MakeItemData(DWORD dwIndex);

#ifdef ENABLE_ACCE_SYSTEM
		bool			LoadItemScale(const char* c_szFileName);
#endif
#ifdef ENABLE_SHINING_SYSTEM
		bool			LoadShiningTable(const char* szShiningTable);
#endif
#ifdef ENABLE_ITEM_EXTRA_PROTO
		bool			LoadItemExtraProto(std::string filename);
		CItemData::TItemExtraProto* GetExtraProto(DWORD vnum);
#endif
	protected:
		TItemMap m_ItemMap;
		std::vector<CItemData*>  m_vec_ItemRange;
		CItemData * m_pSelectedItemData;
#ifdef ENABLE_ITEM_EXTRA_PROTO
		std::map<DWORD, CItemData::TItemExtraProto> m_map_extraProto;
		CItemData::TItemExtraProto* m_pSelectedExtraProto;
#endif
};


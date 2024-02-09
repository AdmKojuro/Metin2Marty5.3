#include <boost/unordered_map.hpp>
#include "../../common/stl.h"
#include "../../common/tables.h"

class CDropItem : public singleton<CDropItem>
{
	public:
		CDropItem();
		virtual ~CDropItem();

		struct WStrucInfoDropItem
		{

			DWORD id_mob;
			std::string name_mob;
			DWORD prob_item;
			int count_item;

			WStrucInfoDropItem(DWORD _id_mob, const char* _name_mob, int _count_item, DWORD _prob_item):
				id_mob(_id_mob),
				name_mob(_name_mob),
				count_item(_count_item),
				prob_item(_prob_item)
			{
			}
		};

		void	Open(LPCHARACTER ch);
		void	Search(LPCHARACTER ch, int vnum_item);
		void	SendDateDropItemPackets(LPCHARACTER ch, BYTE subheader);


		void	InsetListDi(int index, std::vector<int> get_mobs_item_group, int CountItem, DWORD ProbItem, int CountItemGroup);
		void	DropItemGroupDI(int index, int vnum_item, std::vector<int> get_mobs_item_group);
		void	MobItemGroupDI(int index, int vnum_item, std::vector<int> get_mobs_item_group);
		void	LevelItemGroupDI(int index, int vnum_item, std::vector<int> get_mobs_item_group);

	private:

		std::map<DWORD, WStrucInfoDropItem*> list_mobs;
};
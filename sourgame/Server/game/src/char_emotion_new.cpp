#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "db.h"


void CHARACTER::LoadingInfoEmotions()
{
	LoadInfoEmotions();
	LoadEmotions();
}

void CHARACTER::LoadInfoEmotions()
{
	char szQuery[1024];
	load_info_emotion.clear();

	snprintf(szQuery, sizeof(szQuery),
	"SELECT id_emotion, tiempo_emotion FROM %semotions_new WHERE player_id = %d", get_table_postfix(), GetPlayerID());
	std::auto_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows > 0){
		
		int id_emotion;
		DWORD tiempo_emotion;

		for (int i = 0; i < mysql_num_rows(pMsg->Get()->pSQLResult); ++i)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int col = 0;

			str_to_number(id_emotion, row[col++]);
			str_to_number(tiempo_emotion, row[col++]);

			save_info_emotion.id_emotion = id_emotion;
			save_info_emotion.tiempo_emotion = tiempo_emotion;

			memcpy (&copy_info_emotion, &save_info_emotion, sizeof(save_info_emotion));
			load_info_emotion.push_back(copy_info_emotion);
		}
	}
}

void CHARACTER::LoadEmotions()
{

	ChatPacket(CHAT_TYPE_COMMAND, "SERVER_EMOTIONS_CLEAR");

	if(CountEmotion() > 0){
		for (int i = 0; i<load_info_emotion.size(); ++i){
			ChatPacket(CHAT_TYPE_COMMAND, "SERVER_EMOTIONS_ADD %d %d",load_info_emotion[i].id_emotion,load_info_emotion[i].tiempo_emotion - get_global_time());
		}
	}

	ChatPacket(CHAT_TYPE_COMMAND, "SERVER_EMOTIONS_LOAD");
}

bool CHARACTER::CheckEmotionList(int emotion){
	for (int i = 0; i<load_info_emotion.size(); ++i){
		if (load_info_emotion[i].id_emotion == emotion){
			return true;
		}
	}
	return false;
}

int CHARACTER::EmotionsList(){

	std::vector<int> emotions_list;

	int SLOT_EMOTION_START = 60;

	int emotions_vnums[] = {
		SLOT_EMOTION_START,
		SLOT_EMOTION_START+1,
		SLOT_EMOTION_START+2,
		SLOT_EMOTION_START+3,
		SLOT_EMOTION_START+4,
		SLOT_EMOTION_START+5,
		SLOT_EMOTION_START+6,
		SLOT_EMOTION_START+7,
		SLOT_EMOTION_START+8,
		SLOT_EMOTION_START+9,
		SLOT_EMOTION_START+10,
		SLOT_EMOTION_START+11,
		SLOT_EMOTION_START+12,
		SLOT_EMOTION_START+13,
		SLOT_EMOTION_START+14,
		SLOT_EMOTION_START+15,
		SLOT_EMOTION_START+16,
		SLOT_EMOTION_START+17,
	};


	for (int b = 0; b<sizeof(emotions_vnums)/sizeof(*emotions_vnums);++b){
		if (CheckEmotionList(emotions_vnums[b]) == false){
			emotions_list.push_back(emotions_vnums[b]);
		}
	}

	int emotion_select;
	emotion_select = number(0, emotions_list.size()-1);

	return emotions_list[emotion_select];
}

void CHARACTER::InsertEmotion()
{
	int id_emotion = EmotionsList();

	int time_expire_emotion = 43200;

	DWORD time_emotion = get_global_time() + (time_expire_emotion);
	DBManager::instance().DirectQuery("INSERT INTO player.emotions_new(player_id, id_emotion, tiempo_emotion) VALUES(%d,%d,%d)",
			GetPlayerID(),
			id_emotion,
			time_emotion);
	LoadingInfoEmotions();
}

EVENTFUNC(check_time_emotion_event)
{

	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "check_time_emotion_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (NULL == ch || ch->IsNPC())
		return 0;

	if(ch->CountEmotion() > 0){
		for (int i = 0; i<ch->CountEmotion(); ++i){
			bool loading_info = false;
			
			if (get_global_time() > ch->get_time_emotion(i))
			{
				DBManager::instance().Query("DELETE FROM %semotions_new WHERE player_id=%d and id_emotion=%d", get_table_postfix(), ch->GetPlayerID(),ch->get_id_emotion(i));
				loading_info = true;
			}

			if(loading_info == true){
				ch->LoadingInfoEmotions();
				loading_info = false;
			}
		}
		
	}

	return PASSES_PER_SEC(2);

}

int CHARACTER::get_time_emotion(int value)
{
	return load_info_emotion[value].tiempo_emotion;
}

int CHARACTER::get_id_emotion(int value){
	return load_info_emotion[value].id_emotion;
}

void CHARACTER::StartCheckTimeEmotion()
{
	if (TimeEmotionUpdateTime)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	TimeEmotionUpdateTime = event_create(check_time_emotion_event, info, PASSES_PER_SEC(5));	// 1ºÐ
}

int CHARACTER::CountEmotion()
{
	return load_info_emotion.size();
}
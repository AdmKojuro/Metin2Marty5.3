#include "stdafx.h"
#ifdef __FreeBSD__
#include <md5.h>
#else
#include "../../libthecore/include/xmd5.h"
#endif

#include "utils.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "motion.h"
#include "packet.h"
#include "affect.h"
#include "pvp.h"
#include "start_position.h"
#include "party.h"
#include "guild_manager.h"
#include "p2p.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "war_map.h"
#include "questmanager.h"
#include "item_manager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "dev_log.h"
#include "item.h"
#include "arena.h"
#include "buffer_manager.h"
#include "unique_item.h"
#include "threeway_war.h"
#include "log.h"
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
#include "MountSystem.h"
#endif
#include "../../common/VnumHelper.h"
#include "fishing.h"

ACMD(do_user_horse_ride)
{
	if (ch->IsObserverMode())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHorseRiding() == false)
	{

		if (ch->GetMountVnum())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì Å»°ÍÀ» ÀÌ¿ëÁßÀÔ´Ï´Ù."));
			return;
		}

		if (ch->GetHorse() == NULL)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
			return;
		}

		ch->StartRiding();
	}
	else
	{
		ch->StopRiding();
	}
}

ACMD(do_user_horse_back)
{
	if (ch->GetHorse() != NULL)
	{
		ch->HorseSummon(false);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» µ¹·Áº¸³Â½À´Ï´Ù."));
	}
	else if (ch->IsHorseRiding() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡¼­ ¸ÕÀú ³»·Á¾ß ÇÕ´Ï´Ù."));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
	}
}

ACMD(do_user_horse_feed)
{

	if (ch->GetMyShop())
		return;

	if (ch->GetHorse() == NULL)
	{
		if (ch->IsHorseRiding() == false)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» Åº »óÅÂ¿¡¼­´Â ¸ÔÀÌ¸¦ ÁÙ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	DWORD dwFood = ch->GetHorseGrade() + 50054 - 1;

	if (ch->CountSpecifyItem(dwFood) > 0)
	{
		ch->RemoveSpecifyItem(dwFood, 1);
		ch->FeedHorse();
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡°Ô %s%s ÁÖ¾ú½À´Ï´Ù."),
				ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName,
				"");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¾ÆÀÌÅÛÀÌ ÇÊ¿äÇÕ´Ï´Ù"), ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName);
	}
}

#define MAX_REASON_LEN		128

EVENTINFO(TimedEventInfo)
{
	DynamicCharacterPtr ch;
	int		subcmd;
	int         	left_second;
	char		szReason[MAX_REASON_LEN];

	TimedEventInfo()
	: ch()
	, subcmd( 0 )
	, left_second( 0 )
	{
		::memset( szReason, 0, MAX_REASON_LEN );
	}
};

struct SendDisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter()->GetGMLevel() == GM_PLAYER)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "quit Shutdown(SendDisconnectFunc)");
		}
	}
};

struct DisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetType() == DESC_TYPE_CONNECTOR)
			return;

		if (d->IsPhase(PHASE_P2P))
			return;

		if (d->GetCharacter())
			d->GetCharacter()->Disconnect("Shutdown(DisconnectFunc)");

		d->SetPhase(PHASE_CLOSE);
	}
};

EVENTINFO(shutdown_event_data)
{
	int seconds;

	shutdown_event_data()
	: seconds( 0 )
	{
	}
};

EVENTFUNC(shutdown_event)
{
	shutdown_event_data* info = dynamic_cast<shutdown_event_data*>( event->info );

	if ( info == NULL )
	{
		sys_err( "shutdown_event> <Factor> Null pointer" );
		return 0;
	}

	int * pSec = & (info->seconds);

	if (*pSec < 0)
	{
		sys_log(0, "shutdown_event sec %d", *pSec);

		if (--*pSec == -10)
		{
			const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), DisconnectFunc());
			return passes_per_sec;
		}
		else if (*pSec < -10)
			return 0;

		return passes_per_sec;
	}
	else if (*pSec == 0)
	{
		const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_set_desc.begin(), c_set_desc.end(), SendDisconnectFunc());
		g_bNoMoreClient = true;
		--*pSec;
		return passes_per_sec;
	}
	else
	{
#ifdef __MULTI_LANGUAGE_SYSTEM__
		SendNotice(LC_TEXT("¼Ë´Ù¿îÀÌ %dÃÊ ³²¾Ò½À´Ï´Ù."), *pSec);
#else
		char buf[64];
		snprintf(buf, sizeof(buf), LC_TEXT("¼Ë´Ù¿îÀÌ %dÃÊ ³²¾Ò½À´Ï´Ù."), *pSec);
		SendNotice(buf);
#endif

		--*pSec;
		return passes_per_sec;
	}
}

void Shutdown(int iSec)
{
	if (g_bNoMoreClient)
	{
		thecore_shutdown();
		return;
	}

	CWarMapManager::instance().OnShutdown();

#ifdef __MULTI_LANGUAGE_SYSTEM__
	SendNotice(LC_TEXT("%dÃÊ ÈÄ °ÔÀÓÀÌ ¼Ë´Ù¿î µË´Ï´Ù."), iSec);
#else
	char buf[64];
	snprintf(buf, sizeof(buf), LC_TEXT("%dÃÊ ÈÄ °ÔÀÓÀÌ ¼Ë´Ù¿î µË´Ï´Ù."), iSec);

	SendNotice(buf);
#endif

	shutdown_event_data* info = AllocEventInfo<shutdown_event_data>();
	info->seconds = iSec;

	event_create(shutdown_event, info, 1);
}

ACMD(do_shutdown)
{
	if (NULL == ch)
	{
		sys_err("Accept shutdown command from %s.", ch->GetName());
	}
	TPacketGGShutdown p;
	p.bHeader = HEADER_GG_SHUTDOWN;
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShutdown));

	Shutdown(10);
}

EVENTFUNC(timed_event)
{
	TimedEventInfo * info = dynamic_cast<TimedEventInfo *>( event->info );

	if ( info == NULL )
	{
		sys_err( "timed_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}
	LPDESC d = ch->GetDesc();

	if (info->left_second <= 0)
	{
		ch->m_pkTimedEvent = NULL;

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
			case SCMD_QUIT:
			case SCMD_PHASE_SELECT:
				{
					TPacketNeedLoginLogInfo acc_info;
					acc_info.dwPlayerID = ch->GetDesc()->GetAccountTable().id;
#ifdef __MULTI_LANGUAGE_SYSTEM__
					acc_info.bLanguage = ch->GetDesc()->GetAccountTable().bLanguage;
#endif
					db_clientdesc->DBPacket( HEADER_GD_VALID_LOGOUT, 0, &acc_info, sizeof(acc_info) );

					LogManager::instance().DetailLoginLog( false, ch );
				}
				break;
		}

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
				if (d)
					d->SetPhase(PHASE_CLOSE);
				break;

			case SCMD_QUIT:
				ch->ChatPacket(CHAT_TYPE_COMMAND, "quit");
				break;

			case SCMD_PHASE_SELECT:
				{
					ch->Disconnect("timed_event - SCMD_PHASE_SELECT");

					if (d)
					{
						d->SetPhase(PHASE_SELECT);
					}
				}
				break;
		}

		return 0;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÃÊ ³²¾Ò½À´Ï´Ù."), info->left_second);
		--info->left_second;
	}

	return PASSES_PER_SEC(1);
}

ACMD(do_cmd)
{


	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãë¼Ò µÇ¾ú½À´Ï´Ù."));
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}

	switch (subcmd)
	{
		case SCMD_LOGOUT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("·Î±×ÀÎ È­¸éÀ¸·Î µ¹¾Æ °©´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
			break;

		case SCMD_QUIT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°ÔÀÓÀ» Á¾·á ÇÕ´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
			break;

		case SCMD_PHASE_SELECT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ä³¸¯ÅÍ¸¦ ÀüÈ¯ ÇÕ´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
			break;
	}

	int nExitLimitTime = 10;

	if (ch->IsHack(false, true, nExitLimitTime) &&
		false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()) &&
	   	(!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		return;
	}

	switch (subcmd)
	{
		case SCMD_LOGOUT:
		case SCMD_QUIT:
		case SCMD_PHASE_SELECT:
			{
				TimedEventInfo* info = AllocEventInfo<TimedEventInfo>();

				{
					if (ch->IsPosition(POS_FIGHTING))
						info->left_second = 10;
					else
						info->left_second = 3;
				}

				info->ch		= ch;
				info->subcmd		= subcmd;
				strlcpy(info->szReason, argument, sizeof(info->szReason));

				ch->m_pkTimedEvent	= event_create(timed_event, info, 1);
			}
			break;
	}
}

ACMD(do_mount)
{

}

ACMD(do_fishing)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	ch->SetRotation(atof(arg1));
	ch->fishing();
}

ACMD(do_console)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ConsoleEnable");
}

ACMD(do_restart)
{
	if (false == ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
		ch->StartRecoveryEvent();
		return;
	}

	if (NULL == ch->m_pkDeadEvent)
		return;

	int iTimeToDead = (event_time(ch->m_pkDeadEvent) / passes_per_sec);

	if (subcmd != SCMD_RESTART_TOWN && (!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		if (!test_server)
		{
			if (ch->IsHack())
			{

				if (false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - (180 - g_nPortalLimitTime));
					return;
				}
			}
#define eFRS_HERESEC	170
			if (iTimeToDead > eFRS_HERESEC)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - eFRS_HERESEC);
				return;
			}
		}
	}

	//PREVENT_HACK


	if (subcmd == SCMD_RESTART_TOWN)
	{
		if (ch->IsHack())
		{

			if ((!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG) ||
			   	false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - (180 - g_nPortalLimitTime));
				return;
			}
		}

#define eFRS_TOWNSEC	173
		if (iTimeToDead > eFRS_TOWNSEC)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ ¸¶À»¿¡¼­ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%d ÃÊ ³²À½)"), iTimeToDead - eFRS_TOWNSEC);
			return;
		}
	}
	//END_PREVENT_HACK

	ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");

	ch->GetDesc()->SetPhase(PHASE_GAME);
	ch->SetPosition(POS_STANDING);
	ch->StartRecoveryEvent();

	//FORKED_LOAD

	if (1 == quest::CQuestManager::instance().GetEventFlag("threeway_war"))
	{
		if (subcmd == SCMD_RESTART_TOWN || subcmd == SCMD_RESTART_HERE)
		{
			if (true == CThreeWayWar::instance().IsThreeWayWarMapIndex(ch->GetMapIndex()) &&
					false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));

				ch->ReviveInvisible(5);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());

				return;
			}


			if (true == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				if (CThreeWayWar::instance().GetReviveTokenForPlayer(ch->GetPlayerID()) <= 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ºÁö¿¡¼­ ºÎÈ° ±âÈ¸¸¦ ¸ðµÎ ÀÒ¾ú½À´Ï´Ù! ¸¶À»·Î ÀÌµ¿ÇÕ´Ï´Ù!"));
					ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
				}
				else
				{
					ch->Show(ch->GetMapIndex(), GetSungziStartX(ch->GetEmpire()), GetSungziStartY(ch->GetEmpire()));
				}

				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(5);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
				return;
			}
		}
	}
	//END_FORKED_LOAD

	if (ch->GetDungeon())
		ch->GetDungeon()->UseRevive(ch);

	if (ch->GetWarMap() && !ch->IsObserverMode())
	{
		CWarMap * pMap = ch->GetWarMap();
		DWORD dwGuildOpponent = pMap ? pMap->GetGuildOpponent(ch) : 0;

		if (dwGuildOpponent)
		{
			switch (subcmd)
			{
				case SCMD_RESTART_TOWN:
					sys_log(0, "do_restart: restart town");
					PIXEL_POSITION pos;

					if (CWarMapManager::instance().GetStartPosition(ch->GetMapIndex(), ch->GetGuild()->GetID() < dwGuildOpponent ? 0 : 1, pos))
						ch->Show(ch->GetMapIndex(), pos.x, pos.y);
					else
						ch->ExitToSavedLocation();

					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
					ch->ReviveInvisible(5);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
					ch->CheckMount();
#endif
					break;

				case SCMD_RESTART_HERE:
					sys_log(0, "do_restart: restart here");
					ch->RestartAtSamePos();
					//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
					ch->ReviveInvisible(5);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
					ch->CheckMount();
#endif
					break;
			}

			return;
		}
	}
	switch (subcmd)
	{
		case SCMD_RESTART_TOWN:
			sys_log(0, "do_restart: restart town");
			PIXEL_POSITION pos;

			if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos))
				ch->WarpSet(pos.x, pos.y);
			else
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(1);
			break;

		case SCMD_RESTART_HERE:
			sys_log(0, "do_restart: restart here");
			ch->RestartAtSamePos();
			//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(0);
			ch->ReviveInvisible(5);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
				ch->CheckMount();
#endif
			break;
	}
}

#define MAX_STAT g_iStatusPointSetMaxValue

ACMD(do_stat_reset)
{
	ch->PointChange(POINT_STAT_RESET_COUNT, 12 - ch->GetPoint(POINT_STAT_RESET_COUNT));
}

ACMD(do_stat_minus)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetPoint(POINT_STAT_RESET_COUNT) <= 0)
		return;

	if (!strcmp(arg1, "st"))
	{
		if (ch->GetRealPoint(POINT_ST) <= JobInitialPoints[ch->GetJob()].st)
			return;

		ch->SetRealPoint(POINT_ST, ch->GetRealPoint(POINT_ST) - 1);
		ch->SetPoint(POINT_ST, ch->GetPoint(POINT_ST) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_ST, 0);
	}
	else if (!strcmp(arg1, "dx"))
	{
		if (ch->GetRealPoint(POINT_DX) <= JobInitialPoints[ch->GetJob()].dx)
			return;

		ch->SetRealPoint(POINT_DX, ch->GetRealPoint(POINT_DX) - 1);
		ch->SetPoint(POINT_DX, ch->GetPoint(POINT_DX) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_DX, 0);
	}
	else if (!strcmp(arg1, "ht"))
	{
		if (ch->GetRealPoint(POINT_HT) <= JobInitialPoints[ch->GetJob()].ht)
			return;

		ch->SetRealPoint(POINT_HT, ch->GetRealPoint(POINT_HT) - 1);
		ch->SetPoint(POINT_HT, ch->GetPoint(POINT_HT) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_HT, 0);
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (!strcmp(arg1, "iq"))
	{
		if (ch->GetRealPoint(POINT_IQ) <= JobInitialPoints[ch->GetJob()].iq)
			return;

		ch->SetRealPoint(POINT_IQ, ch->GetRealPoint(POINT_IQ) - 1);
		ch->SetPoint(POINT_IQ, ch->GetPoint(POINT_IQ) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_IQ, 0);
		ch->PointChange(POINT_MAX_SP, 0);
	}
	else
		return;

	ch->PointChange(POINT_STAT, +1);
	ch->PointChange(POINT_STAT_RESET_COUNT, -1);
	ch->ComputePoints();
}

ACMD(do_stat)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;

	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	// ch->ChatPacket(CHAT_TYPE_INFO, "%s GRP(%d) idx(%u), MAX_STAT(%d), expr(%d)", __FUNCTION__, ch->GetRealPoint(idx), idx, MAX_STAT, ch->GetRealPoint(idx) >= MAX_STAT);
	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + 1);
	ch->SetPoint(idx, ch->GetPoint(idx) + 1);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
	{
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (idx == POINT_HT)
	{
		ch->PointChange(POINT_MAX_SP, 0);
	}

	ch->PointChange(POINT_STAT, -1);
	ch->ComputePoints();
}

ACMD(do_pvp)
{
	if (ch->GetArena() != NULL || CArenaManager::instance().IsArenaMap(ch->GetMapIndex()) == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(vid);

	if (!pkVictim)
		return;

	if (pkVictim->IsNPC())
		return;

	if (pkVictim->GetArena() != NULL)
	{
		pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ´ë·ÃÁßÀÔ´Ï´Ù."));
		return;
	}

	CPVPManager::instance().Insert(ch, pkVictim);
}

ACMD(do_guildskillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch->GetGuild())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿¡ ¼ÓÇØÀÖÁö ¾Ê½À´Ï´Ù."));
		return;
	}

	CGuild* g = ch->GetGuild();
	TGuildMember* gm = g->GetMember(ch->GetPlayerID());
	if (gm->grade == GUILD_LEADER_GRADE)
	{
		DWORD vnum = 0;
		str_to_number(vnum, arg1);
		g->SkillLevelUp(vnum);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå ½ºÅ³ ·¹º§À» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
	}
}

ACMD(do_skillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vnum = 0;
	str_to_number(vnum, arg1);

	if (true == ch->CanUseSkill(vnum))
	{
		ch->SkillLevelUp(vnum);
	}
	else
	{
		switch(vnum)
		{
			case SKILL_HORSE_WILDATTACK:
			case SKILL_HORSE_CHARGE:
			case SKILL_HORSE_ESCAPE:
			case SKILL_HORSE_WILDATTACK_RANGE:

			case SKILL_7_A_ANTI_TANHWAN:
			case SKILL_7_B_ANTI_AMSEOP:
			case SKILL_7_C_ANTI_SWAERYUNG:
			case SKILL_7_D_ANTI_YONGBI:

			case SKILL_8_A_ANTI_GIGONGCHAM:
			case SKILL_8_B_ANTI_YEONSA:
			case SKILL_8_C_ANTI_MAHWAN:
			case SKILL_8_D_ANTI_BYEURAK:

			case SKILL_ADD_HP:
			case SKILL_RESIST_PENETRATE:
				ch->SkillLevelUp(vnum);
				break;
		}
	}
}

//
//
ACMD(do_safebox_close)
{
	ch->CloseSafebox();
}

//
//
ACMD(do_safebox_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	ch->ReqSafeboxLoad(arg1);
}

ACMD(do_safebox_change_password)
{
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || strlen(arg1)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	if (!*arg2 || strlen(arg2)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	TSafeboxChangePasswordPacket p;

	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szOldPassword, arg1, sizeof(p.szOldPassword));
	strlcpy(p.szNewPassword, arg2, sizeof(p.szNewPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_PASSWORD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	int iPulse = thecore_pulse();

	if (ch->GetMall())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í°¡ ÀÌ¹Ì ¿­·ÁÀÖ½À´Ï´Ù."));
		return;
	}

	if (iPulse - ch->GetMallLoadTime() < passes_per_sec * 10)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í¸¦ ´ÝÀºÁö 10ÃÊ ¾È¿¡´Â ¿­ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	ch->SetMallLoadTime(iPulse);

	TSafeboxLoadPacket p;
	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, ch->GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, arg1, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_MALL_LOAD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_close)
{
	if (ch->GetMall())
	{
		ch->SetMallLoadTime(thecore_pulse());
		ch->CloseMall();
		ch->Save();
	}
}

ACMD(do_ungroup)
{
	if (!ch->GetParty())
		return;

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÀü ¾È¿¡¼­´Â ÆÄÆ¼¿¡¼­ ³ª°¥ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	LPPARTY pParty = ch->GetParty();

	if (pParty->GetMemberCount() == 2)
	{
		// party disband
		CPartyManager::instance().DeleteParty(pParty);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ ³ª°¡¼Ì½À´Ï´Ù."));
		//pParty->SendPartyRemoveOneToAll(ch);
		pParty->Quit(ch->GetPlayerID());
		//pParty->SendPartyRemoveAllToOne(ch);
	}
}

#ifdef GIFT_SYSTEM
#include "db.h"
#include <string>
#include <boost/algorithm/string.hpp>

ACMD(do_gift_show)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_show");
}
bool GetGift(LPCHARACTER ch, DWORD id,bool all=false)
{
	char szSockets[1024] = { '\0' };
	char *tempSockets = szSockets;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		tempSockets += sprintf(tempSockets, "socket%d", i);

		if (i<ITEM_SOCKET_MAX_NUM - 1)
			tempSockets += sprintf(tempSockets, ",");
	}
	char szAttrs[1024] = { '\0' };
	char *tempAttrs = szAttrs;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		if (i < 7)
			tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
		else
			tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
		if (i<ITEM_ATTRIBUTE_MAX_NUM - 1)
			tempAttrs += sprintf(tempAttrs, ",");
	}
	char query[8192];
	if (!all)
		snprintf(query, sizeof(query), "SELECT id,vnum,count,refine_element,%s,%s from player_gift where id='%d' and owner_id=%d and status='WAIT'", szSockets, szAttrs, id, ch->GetPlayerID());
	else
		snprintf(query, sizeof(query), "SELECT id,vnum,count,refine_element,%s,%s from player_gift where owner_id=%d and status='WAIT'", szSockets, szAttrs, ch->GetPlayerID());
	SQLMsg * pkMsg(DBManager::instance().DirectQuery(query));
	SQLResult * pRes = pkMsg->Get();
	if (pRes->uiNumRows > 0)
	{
		ch->SetQuestFlag("gift.time", get_global_time()+(1*pRes->uiNumRows)+2);
		MYSQL_ROW row;
		bool force = false;
		while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
		{
			DWORD vnum, socket[ITEM_SOCKET_MAX_NUM], attr[ITEM_ATTRIBUTE_MAX_NUM][2];
			int col = 0;
			long long count = 0;
			int m_dwRefineElement;
			str_to_number(id, row[col++]);
			str_to_number(vnum, row[col++]);
			str_to_number(count, row[col++]);
			str_to_number(m_dwRefineElement, row[col++]);
			if (vnum == 1)
			{
#ifndef FULL_YANG
				long long nTotalMoney = ch->GetGold()+count;

				if (GOLD_MAX <= nTotalMoney)
				{
					//sys_err("[OVERFLOW_GOLD] Overflow (GOLD_MAX) id %u name %s", ch->GetPlayerID(), ch->GetName());
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20ld l R?AE°ú?z?»ó?R?z­Lö°^ lr???"));
					return true;
				}
#endif
#ifdef FULL_YANG_OWN
				ch->ChangeGold(count);
#else
				ch->PointChange(POINT_GOLD, count, false);
#endif
			}
#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
			else if (vnum == 3)
			{
#ifndef FULL_YANG
				long long nTotalMoney = ch->GetCheque() + count;

				if (CHEQUE_MAX <= nTotalMoney)
				{
					sys_err("[OVERFLOW_CHEQUE] Overflow (CHEQUE_MAX) id %u name %s", ch->GetPlayerID(), ch->GetName());
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("No puedes sobrepasar el limite de 99 won."));
					return true;
				}
#endif
#ifdef FULL_YANG_OWN
				ch->ChangeGold(count);
#else
				ch->PointChange(POINT_CHEQUE, count, false);
#endif

			}
#endif
			else {
				if (force)
					continue;
				for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
					str_to_number(socket[s], row[col++]);

				for (int a = 0; a < ITEM_ATTRIBUTE_MAX_NUM; a++)
				{
					str_to_number(attr[a][0], row[col++]);
					str_to_number(attr[a][1], row[col++]);
				}
				LPITEM item = ITEM_MANAGER::instance().CreateItem(vnum, count, 0, true);
				if (item)
				{
					item->SetRefineElement(m_dwRefineElement);
					
					for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
						item->SetSocket(s, socket[s], false);
					item->ClearAttribute();
					for (int a = 0; a < ITEM_ATTRIBUTE_MAX_NUM; a++)
						item->SetForceAttribute(a, attr[a][0], attr[a][1]);


					int iEmptyPos;
					if (item->IsDragonSoul()) {
						iEmptyPos = ch->GetEmptyDragonSoulInventory(item); 
					}
#ifdef __SPECIAL_INVENTORY_SYSTEM__
					else if (item->IsSkillBook())
						iEmptyPos = ch->GetEmptySkillBookInventory(item->GetSize());
					else if (item->IsUpgradeItem())
						iEmptyPos = ch->GetEmptyUpgradeItemsInventory(item->GetSize());
					else if (item->IsStone())
						iEmptyPos = ch->GetEmptyStoneInventory(item->GetSize());
					else if (item->IsGiftBox())
						iEmptyPos = ch->GetEmptyGiftBoxInventory(item->GetSize()); 
#endif
					else {
						iEmptyPos = ch->GetEmptyInventory(item->GetSize());
					}
					if (item->IsDragonSoul())
					{
						if (iEmptyPos != -1)
						{
							item->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
						}
						else
						{
							M2_DESTROY_ITEM(item);
							if (!ch->DragonSoul_IsQualified())
							{
								ch->ChatPacket(CHAT_TYPE_INFO, "<Gift> %s #3", LC_TEXT("UNKNOW_ERROR"));
								force = true;
								continue;
							}
							else {
								ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
								force = true;
								continue;
							}
						}
					}
					else
					{
						if (iEmptyPos != -1)
						{
#ifdef __SPECIAL_INVENTORY_SYSTEM__
								if (item->IsSkillBook())
									item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
								else if (item->IsUpgradeItem())
									item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
								else if (item->IsStone())
									item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
								else if (item->IsGiftBox())
									item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
								else
#endif										
									item->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

								
						}
						else
						{
							M2_DESTROY_ITEM(item);
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INVENTORY_FULL_ERROR"));
							force = true;
							continue;
						}
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "<Gift> %s #4", LC_TEXT( "UNKNOW_ERROR"));
					force = true;
					continue;
				}

			}
			DBManager::instance().DirectQuery("UPDATE player_gift SET status='OK',date_get=NOW() where id=%d;", id);
		}
		if (force)
			return true;
		if (all)
			ch->ChatPacket(CHAT_TYPE_INFO,LC_TEXT("GIFT_ADD_ALL_SUCCESS"));
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT( "GIFT_ADD_SUCCESS"));
		ch->SetQuestFlag("gift.time", get_global_time()+2);
		return true;
	}
	return false;
}
ACMD(do_gift_get)
{
	if(ch->GetQuestFlag("gift.time") > get_global_time())
		return;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	bool full = !isdigit(*arg1);
	DWORD id;
	str_to_number(id, arg1);
	if (GetGift(ch, id,full))
	{
		ch->RefreshGift();
		ch->LoadGiftPage(ch->GetLastGiftPage());
	}

}
ACMD(do_gift_refresh)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
	{
		if (ch->GetGiftPages() > 0)
			ch->ChatPacket(CHAT_TYPE_COMMAND, "gift_info %d", ch->GetGiftPages());
	}
	else{
		int page;
		str_to_number(page, arg1);
		ch->LoadGiftPage(page);

	}


}
#endif

#ifdef OFFLINE_SHOP
#ifdef ENABLE_OFFSHOP_DECO
void DeleteShop(DWORD id)
{
	CharacterVectorInteractor i;
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30000, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30000 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30001, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30001 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30002, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30002 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30003, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30003 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30004, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30004 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30005, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30005 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30006, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30006 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30007, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30007 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}

	TPacketShopClose packet;
	packet.shop_id = id;
	packet.pid = 0;
	db_clientdesc->DBPacket(HEADER_GD_SHOP_CLOSE, 0, &packet, sizeof(packet));
}
#else
void DeleteShop(DWORD id)
{
	CharacterVectorInteractor i;
	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30000, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end()) {
			LPCHARACTER pc = *it++;
			if (pc)
				if (pc->GetRaceNum() == 30000 && pc->GetPrivShop() == id) {
					pc->DeleteMyShop();
					return;
				}


		}
	}
	TPacketShopClose packet;
	packet.shop_id = id;
	packet.pid = 0;
	db_clientdesc->DBPacket(HEADER_GD_SHOP_CLOSE, 0, &packet, sizeof(packet));
}
#endif

ACMD(do_close_shop)
{
#ifdef ACCOUNT_SHIELD
	if (ch->IsBlockAccount())
		return;
#endif
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
	{
		ch->CloseMyShop();
	}else{
		str_to_number(id, arg1);
		char pid[4096];
		sprintf(pid, "and player_id=%d", ch->GetPlayerID());
		if (id > 0)
		{
			SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT player_id,channel from player_shop WHERE id = %d %s", id, (ch->GetGMLevel() >= SHOP_GM_PRIVILEGES ? "" : pid)));
			SQLResult * pRes = pkMsg->Get();
			if (pRes->uiNumRows > 0)
				DeleteShop(id);
		}



	}
}
#include "banword.h"
ACMD(do_set_name_shop)
{
#ifdef ACCOUNT_SHIELD
	if (ch->IsBlockAccount())
		return;
#endif
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	DWORD id;
	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;
	str_to_number(id, arg1);
	char pid[4096];
	sprintf(pid, "and player_id=%d", ch->GetPlayerID());
	std::string m_stShopSign(arg2);
	boost::algorithm::replace_all(m_stShopSign, "\\", " ");
	boost::algorithm::replace_all(m_stShopSign, "%", "%%");
	if (m_stShopSign.length()>SHOP_SIGN_MAX_LEN)
		m_stShopSign.resize(SHOP_SIGN_MAX_LEN);
	if (m_stShopSign.length()<1) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("???O?i©©? ???i¡Æ¢§ ?¡ÀCO¥ì? ¡íoA¢§ ??¢¬¡×?¢¬¡¤I ¡íoA¢§?¡í ?¡© ?o ??¢©?¢¥?¢¥?."));
		return;
	}
#ifdef STRING_PROTECTION
	if (CBanwordManager::instance().CheckString(m_stShopSign.c_str(), m_stShopSign.length()) != "")
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("???O?i©©? ???i¡Æ¢§ ?¡ÀCO¥ì? ¡íoA¢§ ??¢¬¡×?¢¬¡¤I ¡íoA¢§?¡í ?¡© ?o ??¢©?¢¥?¢¥?."));
		return;
	}
#else

	if (CBanwordManager::instance().CheckString(m_stShopSign.c_str(), m_stShopSign.length()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("???O?i©©? ???i¡Æ¢§ ?¡ÀCO¥ì? ¡íoA¢§ ??¢¬¡×?¢¬¡¤I ¡íoA¢§?¡í ?¡© ?o ??¢©?¢¥?¢¥?."));
		return;
	}
#endif
	if (id>0)
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT id,player_id,channel from player_shop WHERE id = %d %s", id, (ch->GetGMLevel() >= SHOP_GM_PRIVILEGES ? "" : pid)));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows > 0)
		{
			char szName[256];
			DBManager::instance().EscapeString(szName, 256, m_stShopSign.c_str(), m_stShopSign.length());
			DBManager::Instance().DirectQuery("UPDATE player_shop SET name='%s' WHERE id=%d", szName, id);
			ch->LoadPrivShops();
#ifdef ENABLE_OFFSHOP_DECO
			std::auto_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT npc_decoration FROM player_shop WHERE id = %d %s", id, (ch->GetGMLevel() >= SHOP_GM_PRIVILEGES ? "" : pid)));
			MYSQL_ROW row1 = mysql_fetch_row(pMsg1->Get()->pSQLResult);
			std::string m_variable01 = row1[0];
			std::string m_variable03 = m_variable01+m_stShopSign;
#endif
			CharacterVectorInteractor i;
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30000, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
#ifdef ENABLE_OFFSHOP_DECO
							pc->SetShopSign(m_variable03.c_str());
#else
							pc->SetShopSign(m_stShopSign.c_str());
#endif
							return;
						}


				}
			}
#ifdef ENABLE_OFFSHOP_DECO
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30001, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30002, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30003, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30004, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30005, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30006, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30007, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetMyShop() && pc->GetPrivShop() == id) {
							pc->SetShopSign(m_variable03.c_str());
							return;
						}


				}
			}
#endif
			TPacketShopName packet;
			packet.shop_id = id;
#ifdef ENABLE_OFFSHOP_DECO
			strlcpy(packet.szSign, m_variable03.c_str(), sizeof(packet.szSign) - 1);
#else
			strlcpy(packet.szSign, m_stShopSign.c_str(), sizeof(packet.szSign) - 1);
#endif
			db_clientdesc->DBPacket(HEADER_GD_SHOP_NAME, 0, &packet, sizeof(packet));


		}
	}
}

ACMD(do_shop_refresh)
{
	ch->SendShops();
}

#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
ACMD(do_shop_cheque)
{
	if (ch->IsObserverMode() || ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetMyShop())
		return;
	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	str_to_number(id, arg1);
	if (*arg1)
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT cheque from player_shop WHERE id = %d", id));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
			{
				long long gold;
				str_to_number(gold, row[0]);
				if (gold >0)
				{
#ifndef FULL_YANG
					long long nTotalMoney = ch->GetCheque() + gold;

					if (CHEQUE_MAX <= nTotalMoney)
					{
						sys_err("[OVERFLOW_GOLD] Overflow (GOLD_MAX) id %u name %s", ch->GetPlayerID(), ch->GetName());
						return;
					}
#endif
#ifdef FULL_YANG_OWN
					ch->ChangeCheque(gold);
#else
					ch->PointChange(POINT_CHEQUE, gold, false);
#endif

					TPrivShop s = ch->GetPrivShopTable(id);
					s.gold = s.gold - gold;
					s.rest_count = s.item_count - 1;
					ch->UpdatePrivShopTable(s.shop_id, s);
					ch->SendShops();
					DBManager::instance().DirectQuery("UPDATE player_shop SET cheque=cheque - %lld WHERE id = %d", gold, id);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_YANG_ADD"));
				}
			}
		}
	}
}
#endif


ACMD(do_shop_yang)

{
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	str_to_number(id, arg1);
	if (*arg1)
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT gold from player_shop WHERE id = %d", id));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
			{
				long long gold;
				str_to_number(gold, row[0]);
				if (gold >0)
				{
#ifndef FULL_YANG
					long long nTotalMoney = ch->GetGold() + gold;

					if (GOLD_MAX <= nTotalMoney)
					{
						sys_err("[OVERFLOW_GOLD] Overflow (GOLD_MAX) id %u name %s", ch->GetPlayerID(), ch->GetName());
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20?? ©©E?¡í ??¡ÆuC??¨Ï ¡íoA¢§?¡í ?¡©?o¡Æ¢§ ??¢©?¢¥?¢¥?"));
						return;
					}
#endif
#ifdef FULL_YANG_OWN
					ch->ChangeGold(gold);
#else
					ch->PointChange(POINT_GOLD,gold, false);
#endif

					TPrivShop s = ch->GetPrivShopTable(id);
					s.gold = s.gold - gold;
					s.rest_count = s.item_count - 1;
					ch->UpdatePrivShopTable(s.shop_id, s);
					ch->SendShops();
					DBManager::instance().DirectQuery("UPDATE player_shop SET gold=gold - %lld WHERE id = %d", gold, id);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SHOP_YANG_ADD"));
				}
				}
			}
		}
	}
#include <boost/algorithm/string.hpp>
ACMD(do_shop_update_item)
{

	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	std::vector<std::string> args;
	boost::split(args, arg1, boost::is_any_of("|"));
	if (args.size()<3) {
		return;
	}
	DWORD shop_id;
	str_to_number(shop_id, args[1].c_str());
	SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT id from player_shop WHERE id = %d and player_id=%d", shop_id, ch->GetPlayerID()));
	SQLResult * pRes = pkMsg->Get();
	if (pRes->uiNumRows>0)
	{
		/*
		/update_shop_item price|%d|%d|%d)
		/update_shop_item remove|%d|%d
		/update_shop_item add|%d|%d|%d|%d|%s

		*/
#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
		if (args.size() == 5)
#else
		if (args.size() == 4)
#endif
		{
			DWORD item_id;
			str_to_number(item_id, args[2].c_str());
			long long price;
			str_to_number(price, args[3].c_str());
#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
			DWORD cheque;
			str_to_number(cheque, args[4].c_str());
#endif
			if (price <= 0 || item_id <= 0)
				return;
			{
				SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT id from player_shop_items WHERE id = %d and shop_id=%d", item_id, shop_id));
				SQLResult * pRes = pkMsg->Get();
				if (pRes->uiNumRows > 0)
					DBManager::instance().DirectQuery("UPDATE player_shop_items SET price=%lld WHERE id = %d and shop_id=%d", price, item_id, shop_id);

#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
				if (pRes->uiNumRows > 0)
					DBManager::instance().DirectQuery("UPDATE player_shop_items SET cheque=%d WHERE id = %d and shop_id=%d", cheque, item_id, shop_id);
#endif

			}
			}
		else if (args.size() == 3)
		{
			DWORD item_id;
			str_to_number(item_id, args[2].c_str());
			if (item_id <= 0)
				return;
			{

				std::string shop_name(LC_TEXT( "SHOP_NAME"));
				boost::replace_all(shop_name, "#PLAYER_NAME#", ch->GetName());
				boost::replace_all(shop_name, "#ID#", "");


				char szSockets[1024] = { '\0' };
				char *tempSockets = szSockets;
				for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
				{
					tempSockets += sprintf(tempSockets, "socket%d", i);

					if (i<ITEM_SOCKET_MAX_NUM - 1)
						tempSockets += sprintf(tempSockets, ",");
				}
				char szAttrs[1024] = { '\0' };
				char *tempAttrs = szAttrs;
				for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
				{
					if (i < 7)
						tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
					else
						tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
					if (i<ITEM_ATTRIBUTE_MAX_NUM - 1)
						tempAttrs += sprintf(tempAttrs, ",");
				}

				SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT vnum,count,refine_element,%s,%s from player_shop_items where shop_id='%d' and id=%d", szSockets,szAttrs,shop_id, item_id));
				SQLResult * pRes = pkMsg->Get();
				if (pRes->uiNumRows>0)
				{
					MYSQL_ROW row;
					while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
					{

						int col = 0;
						char query[8192];
						sprintf(query, "INSERT INTO player_gift SET owner_id=%d,reason='%s',`from`=replace(\"%s\",' ','_'),status='WAIT',date_add=NOW()", ch->GetPlayerID(), LC_TEXT( "SHOP_ITEM_REASON"), shop_name.c_str());
						sprintf(query, "%s, vnum='%s'", query, row[col++]);
						sprintf(query, "%s, count='%s'", query, row[col++]);
						sprintf(query, "%s, refine_element='%s'", query, row[col++]);
						for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
							sprintf(query, "%s, socket%d='%s'", query, s, row[col++]);
				
						for (int ia = 0; ia < ITEM_ATTRIBUTE_MAX_NUM; ia++)
						{
							if (ia < 7)
							{
								sprintf(query, "%s, attrtype%d='%s'", query, ia, row[col++]);
								sprintf(query, "%s, attrvalue%d='%s'", query, ia, row[col++]);
							}
							else
							{
								sprintf(query, "%s, applytype%d='%s'", query, ia-7,row[col++]);
								sprintf(query, "%s, applyvalue%d='%s'", query, ia-7,row[col++]);
							}
						}
						SQLMsg * pkMsg(DBManager::instance().DirectQuery(query));
						SQLResult * pRes = pkMsg->Get();
						DWORD gift_id = pRes->uiInsertID;
						if (gift_id > 0)
						{
							GetGift(ch, gift_id);
							DBManager::instance().DirectQuery("delete from player_shop_items where id='%d'", item_id);
							DBManager::instance().DirectQuery("UPDATE player_shop SET item_count=item_count-1 WHERE id = %d", shop_id);
						}
					}
				}

			}
		}

#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
		else if (args.size() == 7)
#else
		else if (args.size() == 6)
#endif
		{
			BYTE display_pos;
			str_to_number(display_pos, args[2].c_str());

			if (display_pos < 0)
				return;
			WORD pos;
			str_to_number(pos, args[3].c_str());
			if (pos < 0)
				return;

			BYTE window_type;
			str_to_number(window_type, args[4].c_str());
			if (window_type < 0)
				return;

			#ifdef ACCE_LENNT_FIX
			if(ch->IsAcceOpen())
				Acce_close(ch);
			#endif
			long long price;

			str_to_number(price, args[5].c_str());
			if (price <= 0)
				return;
#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
			DWORD cheque;
			str_to_number(cheque, args[6].c_str());
			if (cheque < 0)
				return;
#endif
			LPITEM item = ch->GetItem(TItemPos(window_type, pos));
			if (item)
			{
				const TItemTable * item_table = item->GetProto();

				if (item_table && (IS_SET(item_table->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MYSHOP)))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT( "??¡¤a?¡© ???????? ¡Æ©©?I¡íoA¢§?¢§?¡© ?C¢¬?C? ?o ??¢©?¢¥?¢¥?."));
					return;
				}

				if (item->IsEquipped())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT( "????A©¬?I ???????? ¡Æ©©?I¡íoA¢§?¢§?¡© ?C¢¬?C? ?o ??¢©?¢¥?¢¥?."));
					return;
				}

				if (item->isLocked())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT( "¡íc?eA©¬?I ???????? ¡Æ©©?I¡íoA¢§?¢§?¡© ?C¢¬?C? ?o ??¢©?¢¥?¢¥?."));
					return;
				}
				if (item->GetOwner() != ch)
				{
					return;
				}
#ifdef SOULBIND_SYSTEM
				if (item->IsSoulBind())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT( "You can't sell in private shop item with soul bind."));
					return;
				}
#endif
				char query[1024];
				sprintf(query, "INSERT INTO player_shop_items SET");
				sprintf(query, "%s player_id='%d'", query, ch->GetPlayerID());
				sprintf(query, "%s, shop_id='%d'", query, shop_id);
				sprintf(query, "%s, vnum='%d'", query, item->GetVnum());
				sprintf(query, "%s, count='%d'", query, item->GetCount());
				sprintf(query, "%s, price='%lld'", query, price);
#ifdef __ENABLE_CHEQUE_OFFLINE_SHOP__
				sprintf(query, "%s, cheque='%d'", query, cheque);
#endif
				sprintf(query, "%s, refine_element='%d'", query, item->GetRefineElement());

				sprintf(query, "%s, display_pos='%u'", query, display_pos);
				for (int s = 0; s < ITEM_SOCKET_MAX_NUM; s++)
				{
					sprintf(query, "%s, socket%d='%ld'", query, s, item->GetSocket(s));

				}

				for (int ia = 0; ia < ITEM_ATTRIBUTE_MAX_NUM; ia++)
				{
					const TPlayerItemAttribute& attr = item->GetAttribute(ia);
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
				ITEM_MANAGER::Instance().RemoveItem(item, "Priv shop");
				DBManager::instance().DirectQuery("UPDATE player_shop SET item_count=item_count +1 WHERE id = %d", shop_id);
			}
			else
				return;



		}
		else
			return;
		ch->LoadPrivShops();
		char buf[32];
		sprintf(buf, "refresh_shop_items %d", shop_id);
		interpret_command(ch, buf, strlen(buf));
		CharacterVectorInteractor i;
		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30000, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30000 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

#ifdef ENABLE_OFFSHOP_DECO
		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30001, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30001 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30002, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30002 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30003, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30003 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30004, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30004 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30005, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30005 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30006, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30006 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}

		if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30007, i))
		{
			CharacterVectorInteractor::iterator it = i.begin();
			while (it != i.end()) {
				LPCHARACTER pc = *it++;
				if (pc)
					if (pc->GetRaceNum() == 30007 && pc->GetPrivShop() == shop_id) {
						pc->UpdateShopItems();
						return;
					}


			}
		}
#endif
		TPacketShopUpdateItem packet;
		packet.shop_id = shop_id;
		db_clientdesc->DBPacket(HEADER_GD_SHOP_UPDATE_ITEM, 0, &packet, sizeof(packet));
		}
	}

ACMD(do_shop_refresh_items)
{
	if (ch->IsObserverMode() || ch->GetExchange())
		return;

	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	str_to_number(id, arg1);
	char szSockets[1024] = { '\0' };
	char *tempSockets = szSockets;
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		tempSockets += sprintf(tempSockets, "socket%d", i);

		if (i<ITEM_SOCKET_MAX_NUM - 1)
			tempSockets += sprintf(tempSockets, ",");
	}
	char szAttrs[1024] = { '\0' };
	char *tempAttrs = szAttrs;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; i++)
	{
		if (i < 7)
			tempAttrs += sprintf(tempAttrs, "attrtype%d,attrvalue%d", i, i);
		else
			tempAttrs += sprintf(tempAttrs, "applytype%d,applyvalue%d", i - 7, i - 7);
		if (i<ITEM_ATTRIBUTE_MAX_NUM - 1)
			tempAttrs += sprintf(tempAttrs, ",");
	}


	SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT id,vnum,count,display_pos,price,cheque,refine_element,%s,%s from player_shop_items where shop_id='%d'", szSockets, szAttrs, id));

	SQLResult * pRes = pkMsg->Get();
	BYTE bItemCount = pRes->uiNumRows;
	std::vector<TShopItemTable *> map_shop;
	ch->ChatPacket(CHAT_TYPE_COMMAND, "shop_item_clear");
	if (bItemCount>0)
	{
		bItemCount = 0;
		MYSQL_ROW row;
		int c = 0;
		while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
		{
			int col = 7;

			char attrs[1024] = { '\0' };
			char sockets[1024] = { '\0' };
			for (int i = 0; i<ITEM_SOCKET_MAX_NUM; i++)
				sprintf(sockets, "%s%s%s", sockets, row[col++], (i<ITEM_SOCKET_MAX_NUM-1 ? "|" : ""));
			//col--;

			for (int i = 0; i<ITEM_ATTRIBUTE_MAX_NUM; i++)
				sprintf(attrs, "%s%s,%s%s", attrs, row[col++], row[col++], (i<ITEM_ATTRIBUTE_MAX_NUM-1 ? "|" : ""));

			ch->ChatPacket(CHAT_TYPE_COMMAND, "shop_item %s#%s#%s#%s#%s#%s#%s#%s#%s", row[0], row[1], row[2], row[3], row[4], row[5], row[6], sockets, attrs);
		}
	}

}
ACMD(do_shop_update)
{
	if (ch->IsObserverMode() || ch->GetExchange())
		return;

	DWORD id;
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	str_to_number(id, arg1);
	if (*arg1)
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT player_id from player_shop WHERE id = %d", id));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Shop %s has been updated", arg1);
			CharacterVectorInteractor i;
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30000, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30000 && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}
#ifdef ENABLE_OFFSHOP_DECO
			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30001, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30001  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30002, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30002  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30003, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30003  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30004, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30004  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30005, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30005  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30006, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30006  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}

			if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(30007, i))
			{
				CharacterVectorInteractor::iterator it = i.begin();

				while (it != i.end()) {
					LPCHARACTER pc = *it++;
					if (pc)
						if (pc->GetRaceNum() == 30007  && pc->GetPrivShop() == id) {
							pc->UpdateShopItems();
							return;
						}


				}
			}
#endif
			TPacketShopUpdateItem packet;
			packet.shop_id = id;
			db_clientdesc->DBPacket(HEADER_GD_SHOP_UPDATE_ITEM, 0, &packet, sizeof(packet));
		}
		else
			ch->ChatPacket(CHAT_TYPE_INFO, "Shop %s does exists", arg1);
	}
}
ACMD(do_shop_delete)
{
	if (ch->IsObserverMode() || ch->GetExchange())
		return;
	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage:");
		ch->ChatPacket(CHAT_TYPE_INFO, "/delete_shop <type> <arg> ");
		ch->ChatPacket(CHAT_TYPE_INFO, "Types:");
		ch->ChatPacket(CHAT_TYPE_INFO, "		shopid - Delete shop using ID");
		ch->ChatPacket(CHAT_TYPE_INFO, "		player - Delete all player shops by player name");
		ch->ChatPacket(CHAT_TYPE_INFO, "Example:");
		ch->ChatPacket(CHAT_TYPE_INFO, "		/delete_shop player Best4ever");
		ch->ChatPacket(CHAT_TYPE_INFO, "		/delete_shop shopid 1");
		return;
	}
	if (!strcmp(arg1, "player"))
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT id from player_shop WHERE player_id=(select id from player where name='%s')", arg2));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
			{
				DWORD id;
				str_to_number(id, row[0]);
				DeleteShop(id);
			}
		}
		else
			ch->ChatPacket(CHAT_TYPE_INFO, "Player %s does have any shop", arg2);
	}
	if (!strcmp(arg1, "shopid"))
	{
		SQLMsg * pkMsg(DBManager::instance().DirectQuery("SELECT id from player_shop WHERE id='%s'", arg2));
		SQLResult * pRes = pkMsg->Get();
		if (pRes->uiNumRows>0)
		{
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(pRes->pSQLResult)) != NULL)
			{
				DWORD id;
				str_to_number(id, arg2);
				DeleteShop(id);
			}
		}
		else
			ch->ChatPacket(CHAT_TYPE_INFO, "Shop %s does exists", arg2);
	}
}
#else
ACMD(do_close_shop)
{
	if (ch->IsObserverMode())
		return;
	if (ch->GetMyShop())
	{
		ch->CloseMyShop();
		return;
	}
}
#endif

ACMD(do_set_walk_mode)
{
	ch->SetNowWalking(true);
	ch->SetWalking(true);
}

ACMD(do_set_run_mode)
{
	ch->SetNowWalking(false);
	ch->SetWalking(false);
}

ACMD(do_war)
{

	CGuild * g = ch->GetGuild();

	if (!g)
		return;


	if (g->UnderAnyWar())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ´Ù¸¥ ÀüÀï¿¡ ÂüÀü Áß ÀÔ´Ï´Ù."));
		return;
	}


	char arg1[256], arg2[256];
	DWORD type = GUILD_WAR_TYPE_FIELD; //fixme102 base int modded uint
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
		return;

	if (*arg2)
	{
		str_to_number(type, arg2);

		if (type >= GUILD_WAR_TYPE_MAX_NUM)
			type = GUILD_WAR_TYPE_FIELD;
	}


	DWORD gm_pid = g->GetMasterPID();


	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü¿¡ ´ëÇÑ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		return;
	}


	CGuild * opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·± ±æµå°¡ ¾ø½À´Ï´Ù."));
		return;
	}


	switch (g->GetGuildWarState(opp_g->GetID()))
	{
		case GUILD_WAR_NONE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµå°¡ ÀÌ¹Ì ÀüÀï Áß ÀÔ´Ï´Ù."));
					return;
				}

				int iWarPrice = KOR_aGuildWarInfo[type].iWarPrice;

				if (g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àüºñ°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return;
				}

				if (opp_g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ Àüºñ°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return;
				}
			}
			break;

		case GUILD_WAR_SEND_DECLARE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ¼±ÀüÆ÷°í ÁßÀÎ ±æµåÀÔ´Ï´Ù."));
				return;
			}
			break;

		case GUILD_WAR_RECV_DECLARE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµå°¡ ÀÌ¹Ì ÀüÀï Áß ÀÔ´Ï´Ù."));
					g->RequestRefuseWar(opp_g->GetID());
					return;
				}
			}
			break;

		case GUILD_WAR_RESERVE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ÀüÀïÀÌ ¿¹¾àµÈ ±æµå ÀÔ´Ï´Ù."));
				return;
			}
			break;

		case GUILD_WAR_END:
			return;

		default:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ÀüÀï ÁßÀÎ ±æµåÀÔ´Ï´Ù."));
			g->RequestRefuseWar(opp_g->GetID());
			return;
	}

	if (!g->CanStartWar(type))
	{

		if (g->GetLadderPoint() == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ·¹´õ Á¡¼ö°¡ ¸ðÀÚ¶ó¼­ ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			sys_log(0, "GuildWar.StartError.NEED_LADDER_POINT");
		}
		else if (g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀüÀ» ÇÏ±â À§ÇØ¼± ÃÖ¼ÒÇÑ %d¸íÀÌ ÀÖ¾î¾ß ÇÕ´Ï´Ù."), GUILD_WAR_MIN_MEMBER_COUNT);
			sys_log(0, "GuildWar.StartError.NEED_MINIMUM_MEMBER[%d]", GUILD_WAR_MIN_MEMBER_COUNT);
		}
		else
		{
			sys_log(0, "GuildWar.StartError.UNKNOWN_ERROR");
		}
		return;
	}


	if (!opp_g->CanStartWar(GUILD_WAR_TYPE_FIELD))
	{
		if (opp_g->GetLadderPoint() == 0)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ·¹´õ Á¡¼ö°¡ ¸ðÀÚ¶ó¼­ ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		else if (opp_g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµå¿ø ¼ö°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	do
	{
		if (g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµåÀåÀÌ Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	do
	{
		if (opp_g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(opp_g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµåÀåÀÌ Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	g->RequestDeclareWar(opp_g->GetID(), type);
}

ACMD(do_nowar)
{
	CGuild* g = ch->GetGuild();
	if (!g)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD gm_pid = g->GetMasterPID();

	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü¿¡ ´ëÇÑ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		return;
	}

	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·± ±æµå°¡ ¾ø½À´Ï´Ù."));
		return;
	}

	g->RequestRefuseWar(opp_g->GetID());
}

ACMD(do_detaillog)
{
	ch->DetailLog();
}

ACMD(do_monsterlog)
{
	ch->ToggleMonsterLog();
}

ACMD(do_pkmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE mode = 0;
	str_to_number(mode, arg1);

	if (mode == PK_MODE_PROTECT)
		return;

	if (ch->GetLevel() < PK_PROTECT_LEVEL && mode != 0)
		return;

	ch->SetPKMode(mode);
}

ACMD(do_messenger_auth)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	char answer = LOWER(*arg1);
	// @fixme130 AuthToAdd void -> bool
	bool bIsDenied = answer != 'y';
	bool bIsAdded = MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, bIsDenied); // DENY
	if (bIsAdded && bIsDenied)
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg2);

		if (tch)
			tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀ¸·Î ºÎÅÍ Ä£±¸ µî·ÏÀ» °ÅºÎ ´çÇß½À´Ï´Ù."), ch->GetName());
	}

}

ACMD(do_setblockmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		BYTE flag = 0;
		str_to_number(flag, arg1);
		ch->SetBlockMode(flag);
	}
}

ACMD(do_unmount)
{

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if(ch->GetWear(WEAR_COSTUME_MOUNT))
	{
		CMountSystem* mountSystem = ch->GetMountSystem();
		LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
		DWORD mobVnum = 0;
		
		if (!mountSystem && !mount)
			return;
		
#ifdef __CHANGELOOK_SYSTEM__	
		if(mount->GetTransmutation())
		{
			const TItemTable* itemTable = ITEM_MANAGER::instance().GetTable(mount->GetTransmutation());
			
			if (itemTable)
				mobVnum = itemTable->alValues[1];
			else
				mobVnum = mount->GetValue(1);
		}
		else
			mobVnum = mount->GetValue(1);
#else
		if(mount->GetValue(1) != 0)
			mobVnum = mount->GetValue(1);
#endif

		if (ch->GetMountVnum())
		{
			if(mountSystem->CountSummoned() == 0)
			{
				mountSystem->Unmount(mobVnum);
			}
		}
		return;
	}
#endif


	if (ch->UnEquipSpecialRideUniqueItem())
	{
		ch->RemoveAffect(AFFECT_MOUNT);
		ch->RemoveAffect(AFFECT_MOUNT_BONUS);

		if (ch->IsHorseRiding())
		{
			ch->StopRiding();
		}
	}
	else
	{
		ch->ChatPacket( CHAT_TYPE_INFO, LC_TEXT("ÀÎº¥Åä¸®°¡ ²Ë Â÷¼­ ³»¸± ¼ö ¾ø½À´Ï´Ù."));
	}

}

ACMD(do_observer_exit)
{
	if (ch->IsObserverMode())
	{
		if (ch->GetWarMap())
			ch->SetWarMap(NULL);

		if (ch->GetArena() != NULL || ch->GetArenaObserverMode() == true)
		{
			ch->SetArenaObserverMode(false);

			if (ch->GetArena() != NULL)
				ch->GetArena()->RemoveObserver(ch->GetPlayerID());

			ch->SetArena(NULL);
			ch->WarpSet(ARENA_RETURN_POINT_X(ch->GetEmpire()), ARENA_RETURN_POINT_Y(ch->GetEmpire()));
		}
		else
		{
			ch->ExitToSavedLocation();
		}
		ch->SetObserverMode(false);
	}
}

ACMD(do_view_equip)
{
	if (ch->GetGMLevel() <= GM_PLAYER)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		DWORD vid = 0;
		str_to_number(vid, arg1);
		LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

		if (!tch)
			return;

		if (!tch->IsPC())
			return;

		tch->SendEquipment(ch);
	}
}

ACMD(do_party_request)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ÆÄÆ¼¿¡ ¼ÓÇØ ÀÖÀ¸¹Ç·Î °¡ÀÔ½ÅÃ»À» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		if (!ch->RequestToParty(tch))
			ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

ACMD(do_party_request_accept)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->AcceptToParty(tch);
}

ACMD(do_party_request_deny)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->DenyToParty(tch);
}

ACMD(do_monarch_warpto)
{
	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ë °¡´ÉÇÑ ±â´ÉÀÔ´Ï´Ù"));
		return;
	}


	if (!ch->IsMCOK(CHARACTER::MI_WARP))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ÃÊ°£ ÄðÅ¸ÀÓÀÌ Àû¿ëÁßÀÔ´Ï´Ù."), ch->GetMCLTime(CHARACTER::MI_WARP));
		return;
	}


	const int WarpPrice = 10000;


	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±¹°í¿¡ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù. ÇöÀç : %u ÇÊ¿ä±Ý¾× : %u"), NationMoney, WarpPrice);
		return;
	}

	int x = 0, y = 0;
	char arg1[256];

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë¹ý: warpto <character name>"));
		return;
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("Å¸Á¦±¹ À¯Àú¿¡°Ô´Â ÀÌµ¿ÇÒ¼ö ¾ø½À´Ï´Ù"));
				return;
			}

			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç À¯Àú´Â %d Ã¤³Î¿¡ ÀÖ½À´Ï´Ù. (ÇöÀç Ã¤³Î %d)"), pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}

			PIXEL_POSITION pos;

			if (!SECTREE_MANAGER::instance().GetCenterPositionOfMap(pkCCI->lMapIndex, pos))
				ch->ChatPacket(CHAT_TYPE_INFO, "Cannot find map (index %d)", pkCCI->lMapIndex);
			else
			{
				//ch->ChatPacket(CHAT_TYPE_INFO, "You warp to (%d, %d)", pos.x, pos.y);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¿¡°Ô·Î ÀÌµ¿ÇÕ´Ï´Ù"), arg1);
				ch->WarpSet(pos.x, pos.y);


				CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);


				ch->SetMC(CHARACTER::MI_WARP);
			}
		}
		else if (NULL == CHARACTER_MANAGER::instance().FindPC(arg1))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "There is no one by that name");
		}

		return;
	}
	else
	{
		if (tch->GetEmpire() != ch->GetEmpire())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Å¸Á¦±¹ À¯Àú¿¡°Ô´Â ÀÌµ¿ÇÒ¼ö ¾ø½À´Ï´Ù"));
			return;
		}
		if (!IsMonarchWarpZone(tch->GetMapIndex()))
		{
			ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return;
		}
		x = tch->GetX();
		y = tch->GetY();
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¿¡°Ô·Î ÀÌµ¿ÇÕ´Ï´Ù"), arg1);
	ch->WarpSet(x, y);
	ch->Stop();


	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);


	ch->SetMC(CHARACTER::MI_WARP);
}

ACMD(do_monarch_transfer)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë¹ý: transfer <name>"));
		return;
	}

	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ë °¡´ÉÇÑ ±â´ÉÀÔ´Ï´Ù"));
		return;
	}


	if (!ch->IsMCOK(CHARACTER::MI_TRANSFER))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ÃÊ°£ ÄðÅ¸ÀÓÀÌ Àû¿ëÁßÀÔ´Ï´Ù."), ch->GetMCLTime(CHARACTER::MI_TRANSFER));
		return;
	}


	const int WarpPrice = 10000;


	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±¹°í¿¡ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù. ÇöÀç : %u ÇÊ¿ä±Ý¾× : %u"), NationMoney, WarpPrice);
		return;
	}


	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ Á¦±¹ À¯Àú´Â ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}
			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀº %d Ã¤³Î¿¡ Á¢¼Ó Áß ÀÔ´Ï´Ù. (ÇöÀç Ã¤³Î: %d)"), arg1, pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}
			if (!IsMonarchWarpZone(ch->GetMapIndex()))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}

			TPacketGGTransfer pgg;

			pgg.bHeader = HEADER_GG_TRANSFER;
			strlcpy(pgg.szName, arg1, sizeof(pgg.szName));
			pgg.lX = ch->GetX();
			pgg.lY = ch->GetY();

			P2P_MANAGER::instance().Send(&pgg, sizeof(TPacketGGTransfer));
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀ» ¼ÒÈ¯ÇÏ¿´½À´Ï´Ù."), arg1);


			CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

			ch->SetMC(CHARACTER::MI_TRANSFER);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÔ·ÂÇÏ½Å ÀÌ¸§À» °¡Áø »ç¿ëÀÚ°¡ ¾ø½À´Ï´Ù."));
		}

		return;
	}


	if (ch == tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÚ½ÅÀ» ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (tch->GetEmpire() != ch->GetEmpire())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ Á¦±¹ À¯Àú´Â ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	if (!IsMonarchWarpZone(tch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	if (!IsMonarchWarpZone(ch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	//tch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
	tch->WarpSet(ch->GetX(), ch->GetY(), ch->GetMapIndex());


	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

	ch->SetMC(CHARACTER::MI_TRANSFER);
}

ACMD(do_monarch_info)
{
	if (CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³ªÀÇ ±ºÁÖ Á¤º¸"));
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			if (n == ch->GetEmpire())
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s±ºÁÖ] : %s  º¸À¯±Ý¾× %lld "), EMPIRE_NAME(n), p->name[n], p->money[n]);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s±ºÁÖ] : %s  "), EMPIRE_NAME(n), p->name[n]);

		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ Á¤º¸"));
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s±ºÁÖ] : %s  "), EMPIRE_NAME(n), p->name[n]);

		}
	}

}

ACMD(do_elect)
{
	db_clientdesc->DBPacketHeader(HEADER_GD_COME_TO_VOTE, ch->GetDesc()->GetHandle(), 0);
}

// LUA_ADD_GOTO_INFO
struct GotoInfo
{
	std::string 	st_name;

	BYTE 	empire;
	int 	mapIndex;
	DWORD 	x, y;

	GotoInfo()
	{
		st_name 	= "";
		empire 		= 0;
		mapIndex 	= 0;

		x = 0;
		y = 0;
	}

	GotoInfo(const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void operator = (const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void __copy__(const GotoInfo& c_src)
	{
		st_name 	= c_src.st_name;
		empire 		= c_src.empire;
		mapIndex 	= c_src.mapIndex;

		x = c_src.x;
		y = c_src.y;
	}
};

ACMD(do_monarch_tax)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: monarch_tax <1-50>");
		return;
	}


	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ëÇÒ¼ö ÀÖ´Â ±â´ÉÀÔ´Ï´Ù"));
		return;
	}


	int tax = 0;
	str_to_number(tax,  arg1);

	if (tax < 1 || tax > 50)
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("1-50 »çÀÌÀÇ ¼öÄ¡¸¦ ¼±ÅÃÇØÁÖ¼¼¿ä"));

	quest::CQuestManager::instance().SetEventFlag("trade_tax", tax);


	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼¼±ÝÀÌ %d %·Î ¼³Á¤µÇ¾ú½À´Ï´Ù"));


	char szMsg[1024];

	snprintf(szMsg, sizeof(szMsg), "±ºÁÖÀÇ ¸íÀ¸·Î ¼¼±ÝÀÌ %d %% ·Î º¯°æµÇ¾ú½À´Ï´Ù", tax);
	BroadcastNotice(szMsg);

	snprintf(szMsg, sizeof(szMsg), "¾ÕÀ¸·Î´Â °Å·¡ ±Ý¾×ÀÇ %d %% °¡ ±¹°í·Î µé¾î°¡°ÔµË´Ï´Ù.", tax);
	BroadcastNotice(szMsg);


	ch->SetMC(CHARACTER::MI_TAX);
}

static const DWORD cs_dwMonarchMobVnums[] =
{
	191,
	192,
	193,
	194,
	391,
	392,
	393,
	394,
	491,
	492,
	493,
	494,
	591,
	691,
	791,
	1304,
	1901,
	2091,
	2191,
	2206,
	0,
};

ACMD(do_monarch_mob)
{
	char arg1[256];
	LPCHARACTER	tch;

	one_argument(argument, arg1, sizeof(arg1));

	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ëÇÒ¼ö ÀÖ´Â ±â´ÉÀÔ´Ï´Ù"));
		return;
	}

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: mmob <mob name>");
		return;
	}

#ifdef ENABLE_MONARCH_MOB_CMD_MAP_CHECK // @warme006
	BYTE pcEmpire = ch->GetEmpire();
	BYTE mapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(ch->GetMapIndex());
	if (mapEmpire != pcEmpire && mapEmpire != 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÚ±¹ ¿µÅä¿¡¼­¸¸ »ç¿ëÇÒ ¼ö ÀÖ´Â ±â´ÉÀÔ´Ï´Ù"));
		return;
	}
#endif


	const int SummonPrice = 5000000;


	if (!ch->IsMCOK(CHARACTER::MI_SUMMON))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ÃÊ°£ ÄðÅ¸ÀÓÀÌ Àû¿ëÁßÀÔ´Ï´Ù."), ch->GetMCLTime(CHARACTER::MI_SUMMON));
		return;
	}


	if (!CMonarch::instance().IsMoneyOk(SummonPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±¹°í¿¡ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù. ÇöÀç : %u ÇÊ¿ä±Ý¾× : %u"), NationMoney, SummonPrice);
		return;
	}

	const CMob * pkMob;
	DWORD vnum = 0;

	if (isdigit(*arg1))
	{
		str_to_number(vnum, arg1);

		if ((pkMob = CMobManager::instance().Get(vnum)) == NULL)
			vnum = 0;
	}
	else
	{
		pkMob = CMobManager::Instance().Get(arg1, true);

		if (pkMob)
			vnum = pkMob->m_table.dwVnum;
	}

	DWORD count;


	for (count = 0; cs_dwMonarchMobVnums[count] != 0; ++count)
		if (cs_dwMonarchMobVnums[count] == vnum)
			break;

	if (0 == cs_dwMonarchMobVnums[count])
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÈ¯ÇÒ¼ö ¾ø´Â ¸ó½ºÅÍ ÀÔ´Ï´Ù. ¼ÒÈ¯°¡´ÉÇÑ ¸ó½ºÅÍ´Â È¨ÆäÀÌÁö¸¦ ÂüÁ¶ÇÏ¼¼¿ä"));
		return;
	}

	tch = CHARACTER_MANAGER::instance().SpawnMobRange(vnum,
			ch->GetMapIndex(),
			ch->GetX() - number(200, 750),
			ch->GetY() - number(200, 750),
			ch->GetX() + number(200, 750),
			ch->GetY() + number(200, 750),
			true,
			pkMob->m_table.bType == CHAR_TYPE_STONE,
			true);

	if (tch)
	{

		CMonarch::instance().SendtoDBDecMoney(SummonPrice, ch->GetEmpire(), ch);


		ch->SetMC(CHARACTER::MI_SUMMON);
	}
}

static const char* FN_point_string(int apply_number)
{
	switch (apply_number)
	{
		case POINT_MAX_HP:	return LC_TEXT("ÃÖ´ë »ý¸í·Â +%d");
		case POINT_MAX_SP:	return LC_TEXT("ÃÖ´ë Á¤½Å·Â +%d");
		case POINT_HT:		return LC_TEXT("Ã¼·Â +%d");
		case POINT_IQ:		return LC_TEXT("Áö´É +%d");
		case POINT_ST:		return LC_TEXT("±Ù·Â +%d");
		case POINT_DX:		return LC_TEXT("¹ÎÃ¸ +%d");
		case POINT_ATT_SPEED:	return LC_TEXT("°ø°Ý¼Óµµ +%d");
		case POINT_MOV_SPEED:	return LC_TEXT("ÀÌµ¿¼Óµµ %d");
		case POINT_CASTING_SPEED:	return LC_TEXT("ÄðÅ¸ÀÓ -%d");
		case POINT_HP_REGEN:	return LC_TEXT("»ý¸í·Â È¸º¹ +%d");
		case POINT_SP_REGEN:	return LC_TEXT("Á¤½Å·Â È¸º¹ +%d");
		case POINT_POISON_PCT:	return LC_TEXT("µ¶°ø°Ý %d");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_PCT:	return LC_TEXT("µ¶°ø°Ý %d");
#endif
		case POINT_STUN_PCT:	return LC_TEXT("½ºÅÏ +%d");
		case POINT_SLOW_PCT:	return LC_TEXT("½½·Î¿ì +%d");
		case POINT_CRITICAL_PCT:	return LC_TEXT("%d%% È®·ü·Î Ä¡¸íÅ¸ °ø°Ý");
		case POINT_RESIST_CRITICAL:	return LC_TEXT("»ó´ëÀÇ Ä¡¸íÅ¸ È®·ü %d%% °¨¼Ò");
		case POINT_PENETRATE_PCT:	return LC_TEXT("%d%% È®·ü·Î °üÅë °ø°Ý");
		case POINT_RESIST_PENETRATE: return LC_TEXT("»ó´ëÀÇ °üÅë °ø°Ý È®·ü %d%% °¨¼Ò");
		case POINT_ATTBONUS_HUMAN:	return LC_TEXT("ÀÎ°£·ù ¸ó½ºÅÍ Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_ANIMAL:	return LC_TEXT("µ¿¹°·ù ¸ó½ºÅÍ Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_ORC:	return LC_TEXT("¿õ±ÍÁ· Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_MILGYO:	return LC_TEXT("¹Ð±³·ù Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_UNDEAD:	return LC_TEXT("½ÃÃ¼·ù Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_DEVIL:	return LC_TEXT("¾Ç¸¶·ù Å¸°ÝÄ¡ +%d%%");
		case POINT_STEAL_HP:		return LC_TEXT("Å¸°ÝÄ¡ %d%% ¸¦ »ý¸í·ÂÀ¸·Î Èí¼ö");
		case POINT_STEAL_SP:		return LC_TEXT("Å¸·ÂÄ¡ %d%% ¸¦ Á¤½Å·ÂÀ¸·Î Èí¼ö");
		case POINT_MANA_BURN_PCT:	return LC_TEXT("%d%% È®·ü·Î Å¸°Ý½Ã »ó´ë Àü½Å·Â ¼Ò¸ð");
		case POINT_DAMAGE_SP_RECOVER:	return LC_TEXT("%d%% È®·ü·Î ÇÇÇØ½Ã Á¤½Å·Â È¸º¹");
		case POINT_BLOCK:			return LC_TEXT("¹°¸®Å¸°Ý½Ã ºí·° È®·ü %d%%");
		case POINT_DODGE:			return LC_TEXT("È° °ø°Ý È¸ÇÇ È®·ü %d%%");
		case POINT_RESIST_SWORD:	return LC_TEXT("ÇÑ¼Õ°Ë ¹æ¾î %d%%");
		case POINT_RESIST_TWOHAND:	return LC_TEXT("¾ç¼Õ°Ë ¹æ¾î %d%%");
		case POINT_RESIST_DAGGER:	return LC_TEXT("µÎ¼Õ°Ë ¹æ¾î %d%%");
		case POINT_RESIST_BELL:		return LC_TEXT("¹æ¿ï ¹æ¾î %d%%");
		case POINT_RESIST_FAN:		return LC_TEXT("ºÎÃ¤ ¹æ¾î %d%%");
		case POINT_RESIST_BOW:		return LC_TEXT("È°°ø°Ý ÀúÇ× %d%%");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_CLAW:		return LC_TEXT("µÎ¼Õ°Ë ¹æ¾î %d%%");
#endif
		case POINT_RESIST_FIRE:		return LC_TEXT("È­¿° ÀúÇ× %d%%");
		case POINT_RESIST_ELEC:		return LC_TEXT("Àü±â ÀúÇ× %d%%");
		case POINT_RESIST_MAGIC:	return LC_TEXT("¸¶¹ý ÀúÇ× %d%%");
#ifdef ENABLE_MAGIC_REDUCTION_SYSTEM
		case POINT_RESIST_MAGIC_REDUCTION:	return LC_TEXT("¸¶¹ý ÀúÇ× %d%%");
#endif
		case POINT_RESIST_WIND:		return LC_TEXT("¹Ù¶÷ ÀúÇ× %d%%");
		case POINT_RESIST_ICE:		return LC_TEXT("³Ã±â ÀúÇ× %d%%");
		case POINT_RESIST_EARTH:	return LC_TEXT("´ëÁö ÀúÇ× %d%%");
		case POINT_RESIST_DARK:		return LC_TEXT("¾îµÒ ÀúÇ× %d%%");
		case POINT_REFLECT_MELEE:	return LC_TEXT("Á÷Á¢ Å¸°ÝÄ¡ ¹Ý»ç È®·ü : %d%%");
		case POINT_REFLECT_CURSE:	return LC_TEXT("ÀúÁÖ µÇµ¹¸®±â È®·ü %d%%");
		case POINT_POISON_REDUCE:	return LC_TEXT("µ¶ ÀúÇ× %d%%");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_BLEEDING_REDUCE:	return LC_TEXT("µ¶ ÀúÇ× %d%%");
#endif
		case POINT_KILL_SP_RECOVER:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã Á¤½Å·Â È¸º¹");
		case POINT_EXP_DOUBLE_BONUS:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã °æÇèÄ¡ Ãß°¡ »ó½Â");
		case POINT_GOLD_DOUBLE_BONUS:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã µ· 2¹è µå·Ó");
		case POINT_ITEM_DROP_BONUS:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã ¾ÆÀÌÅÛ 2¹è µå·Ó");
		case POINT_POTION_BONUS:	return LC_TEXT("¹°¾à »ç¿ë½Ã %d%% ¼º´É Áõ°¡");
		case POINT_KILL_HP_RECOVERY:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã »ý¸í·Â È¸º¹");
//		case POINT_SKILL:	return LC_TEXT("");
//		case POINT_BOW_DISTANCE:	return LC_TEXT("");
		case POINT_ATT_GRADE_BONUS:	return LC_TEXT("°ø°Ý·Â +%d");
		case POINT_DEF_GRADE_BONUS:	return LC_TEXT("¹æ¾î·Â +%d");
		case POINT_MAGIC_ATT_GRADE:	return LC_TEXT("¸¶¹ý °ø°Ý·Â +%d");
		case POINT_MAGIC_DEF_GRADE:	return LC_TEXT("¸¶¹ý ¹æ¾î·Â +%d");
//		case POINT_CURSE_PCT:	return LC_TEXT("");
		case POINT_MAX_STAMINA:	return LC_TEXT("ÃÖ´ë Áö±¸·Â +%d");
		case POINT_ATTBONUS_WARRIOR:	return LC_TEXT("¹«»ç¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_ASSASSIN:	return LC_TEXT("ÀÚ°´¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_SURA:		return LC_TEXT("¼ö¶ó¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_SHAMAN:		return LC_TEXT("¹«´ç¿¡°Ô °­ÇÔ +%d%%");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_ATTBONUS_WOLFMAN:	return LC_TEXT("¹«´ç¿¡°Ô °­ÇÔ +%d%%");
#endif
		case POINT_ATTBONUS_MONSTER:	return LC_TEXT("¸ó½ºÅÍ¿¡°Ô °­ÇÔ +%d%%");
		case POINT_MALL_ATTBONUS:		return LC_TEXT("°ø°Ý·Â +%d%%");
		case POINT_MALL_DEFBONUS:		return LC_TEXT("¹æ¾î·Â +%d%%");
		case POINT_MALL_EXPBONUS:		return LC_TEXT("°æÇèÄ¡ %d%%");
		case POINT_MALL_ITEMBONUS:		return LC_TEXT("¾ÆÀÌÅÛ µå·ÓÀ² %d¹è"); // @fixme180 float to int
		case POINT_MALL_GOLDBONUS:		return LC_TEXT("µ· µå·ÓÀ² %d¹è"); // @fixme180 float to int
		case POINT_MAX_HP_PCT:			return LC_TEXT("ÃÖ´ë »ý¸í·Â +%d%%");
		case POINT_MAX_SP_PCT:			return LC_TEXT("ÃÖ´ë Á¤½Å·Â +%d%%");
		case POINT_SKILL_DAMAGE_BONUS:	return LC_TEXT("½ºÅ³ µ¥¹ÌÁö %d%%");
		case POINT_NORMAL_HIT_DAMAGE_BONUS:	return LC_TEXT("ÆòÅ¸ µ¥¹ÌÁö %d%%");
		case POINT_SKILL_DEFEND_BONUS:		return LC_TEXT("½ºÅ³ µ¥¹ÌÁö ÀúÇ× %d%%");
		case POINT_NORMAL_HIT_DEFEND_BONUS:	return LC_TEXT("ÆòÅ¸ µ¥¹ÌÁö ÀúÇ× %d%%");
//		case POINT_PC_BANG_EXP_BONUS:	return LC_TEXT("");
//		case POINT_PC_BANG_DROP_BONUS:	return LC_TEXT("");
//		case POINT_EXTRACT_HP_PCT:	return LC_TEXT("");
		case POINT_RESIST_WARRIOR:	return LC_TEXT("¹«»ç°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_ASSASSIN:	return LC_TEXT("ÀÚ°´°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_SURA:		return LC_TEXT("¼ö¶ó°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_SHAMAN:	return LC_TEXT("¹«´ç°ø°Ý¿¡ %d%% ÀúÇ×");
#ifdef ENABLE_WOLFMAN_CHARACTER
		case POINT_RESIST_WOLFMAN:	return LC_TEXT("¹«´ç°ø°Ý¿¡ %d%% ÀúÇ×");
#endif
#ifdef __ELEMENT_SYSTEM__
		case POINT_ENCHANT_ELEC: return LC_TEXT("Lightning Power + %d%%");
		case POINT_ENCHANT_FIRE: return LC_TEXT("Fire Power + %d%%");
		case POINT_ENCHANT_ICE: return LC_TEXT("Ice Power + %d%%");
		case POINT_ENCHANT_WIND: return LC_TEXT("Wind Power + %d%%");
		case POINT_ENCHANT_EARTH: return LC_TEXT("Earth Power + %d%%");
		case POINT_ENCHANT_DARK: return LC_TEXT("Dark Power + %d%%");
		case POINT_ATTBONUS_CZ: return LC_TEXT("Strong against Zodiac Monsters + %d%%");
		case POINT_ATTBONUS_INSECT: return LC_TEXT("Strong against Insects + %d%%");
		case POINT_ATTBONUS_DESERT: return LC_TEXT("Strong against Desert Monsters + %d%%");
#endif
		default:					return "UNK_ID %d%%"; // @fixme180
	}
}

static bool FN_hair_affect_string(LPCHARACTER ch, char *buf, size_t bufsiz)
{
	if (NULL == ch || NULL == buf)
		return false;

	CAffect* aff = NULL;
	time_t expire = 0;
	struct tm ltm;
	int	year, mon, day;
	int	offset = 0;

	aff = ch->FindAffect(AFFECT_HAIR);

	if (NULL == aff)
		return false;

	expire = ch->GetQuestFlag("hair.limit_time");

	if (expire < get_global_time())
		return false;

	// set apply string
	offset = snprintf(buf, bufsiz, FN_point_string(aff->bApplyOn), aff->lApplyValue);

	if (offset < 0 || offset >= (int) bufsiz)
		offset = bufsiz - 1;

	localtime_r(&expire, &ltm);

	year	= ltm.tm_year + 1900;
	mon		= ltm.tm_mon + 1;
	day		= ltm.tm_mday;

	snprintf(buf + offset, bufsiz - offset, LC_TEXT(" (¸¸·áÀÏ : %d³â %d¿ù %dÀÏ)"), year, mon, day);

	return true;
}

ACMD(do_costume)
{
	char buf[768];

	const size_t bufferSize = sizeof(buf);

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	CItem* pBody = ch->GetWear(WEAR_COSTUME_BODY);
	CItem* pHair = ch->GetWear(WEAR_COSTUME_HAIR);
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	CItem* pMount = ch->GetWear(WEAR_COSTUME_MOUNT);
#endif
#ifdef ENABLE_ACCE_SYSTEM
	CItem* pAcce = ch->GetWear(WEAR_COSTUME_ACCE);
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	CItem* pWeapon = ch->GetWear(WEAR_COSTUME_WEAPON);
#endif
#ifdef __AURA_SYSTEM__
	CItem* pAura = ch->GetWear(WEAR_COSTUME_AURA);
	if (pAura)
	{
		const char* itemName = pAura->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  AURA : %s", itemName);
		if (pAura->IsEquipped() && arg1[0] == 'a')
			ch->UnequipItem(pAura);
	}
#endif
	ch->ChatPacket(CHAT_TYPE_INFO, "COSTUME status:");

	if (pHair)
	{
		const char* itemName = pHair->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  HAIR : %s", itemName);

		for (int i = 0; i < pHair->GetAttributeCount(); ++i)
		{
			const TPlayerItemAttribute& attr = pHair->GetAttribute(i);
			if (0 < attr.bType)
			{
				snprintf(buf, bufferSize, FN_point_string(attr.bType), attr.sValue);
				ch->ChatPacket(CHAT_TYPE_INFO, "     %s", buf);
			}
		}

		if (pHair->IsEquipped() && arg1[0] == 'h')
			ch->UnequipItem(pHair);
	}

	if (pBody)
	{
		const char* itemName = pBody->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  BODY : %s", itemName);

		if (pBody->IsEquipped() && arg1[0] == 'b')
			ch->UnequipItem(pBody);
	}

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (pMount)
	{
		const char* itemName = pMount->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  MOUNT : %s", itemName);

		if (pMount->IsEquipped() && arg1[0] == 'm')
			ch->UnequipItem(pMount);
	}
#endif

#ifdef ENABLE_ACCE_SYSTEM
	if (pAcce)
	{
		const char* itemName = pAcce->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  ACCE : %s", itemName);

		if (pAcce->IsEquipped() && arg1[0] == 'a')
			ch->UnequipItem(pAcce);
	}
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
	if (pWeapon)
	{
		const char* itemName = pWeapon->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  WEAPON : %s", itemName);

		if (pWeapon->IsEquipped() && arg1[0] == 'w')
			ch->UnequipItem(pWeapon);
	}
#endif
}

ACMD(do_hair)
{
	char buf[256];

	if (false == FN_hair_affect_string(ch, buf, sizeof(buf)))
		return;

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

ACMD(do_inventory)
{
	int	index = 0;
	int	count		= 1;

	char arg1[256];
	char arg2[256];

	LPITEM	item;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: inventory <start_index> <count>");
		return;
	}

	if (!*arg2)
	{
		index = 0;
		str_to_number(count, arg1);
	}
	else
	{
		str_to_number(index, arg1); index = MIN(index, INVENTORY_MAX_NUM);
		str_to_number(count, arg2); count = MIN(count, INVENTORY_MAX_NUM);
	}

	for (int i = 0; i < count; ++i)
	{
		if (index >= INVENTORY_MAX_NUM)
			break;

		item = ch->GetInventoryItem(index);

		ch->ChatPacket(CHAT_TYPE_INFO, "inventory [%d] = %s",
						index, item ? item->GetName() : "<NONE>");
		++index;
	}
}

//gift notify quest command
ACMD(do_gift)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift");
}

#ifdef ENABLE_CUBE_RENEWAL_WORLDARD
ACMD(do_cube)
{

	const char *line;
	char arg1[256], arg2[256], arg3[256];
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
	{
		return;
	}

	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			Cube_open(ch);
			break;

		default:
			return;
	}
}
#else
ACMD(do_cube)
{
	if (!ch->CanDoCube())
		return;

	dev_log(LOG_DEB0, "CUBE COMMAND <%s>: %s", ch->GetName(), argument);
	int cube_index = 0, inven_index = 0;
	const char *line;

	int inven_type = 0;
	
	char arg1[256], arg2[256], arg3[256];
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
	{
		// print usage
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: cube open");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube close");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube add <inveltory_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube delete <cube_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube list");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube cancel");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube make [all]");
		return;
	}

	const std::string& strArg1 = std::string(arg1);

	// r_info (request information)
	// /cube r_info     ==> (Client -> Server) CoAc NPC¡Æ¢® ¢¬¢¬¥ìe ¨ùo AO¢¥A ¡¤©ö¨öACC ¢¯aA¡í
	//					    (Server -> Client) /cube r_list npcVNUM resultCOUNT 123,1/125,1/128,1/130,5
	//
	// /cube r_info 3   ==> (Client -> Server) CoAc NPC¡Æ¢® ¢¬¢¬¥ìe¨ùo AO¢¥A ¡¤©ö¨öACC A©¬ 3©ö©ªA¡Æ ¨ú¨¡AIAUA¡í ¢¬¢¬¥ìa¢¥A ¥ì¡Í CE¢¯aCN A¢´¨¬¢¬¢¬| ¢¯aA¡í
	// /cube r_info 3 5 ==> (Client -> Server) CoAc NPC¡Æ¢® ¢¬¢¬¥ìe¨ùo AO¢¥A ¡¤©ö¨öACC A©¬ 3©ö©ªA¡Æ ¨ú¨¡AIAU¨¬IAI AIEA 5¡Æ©øAC ¨ú¨¡AIAUA¡í ¢¬¢¬¥ìa¢¥A ¥ì¡Í CE¢¯aCN Ac¡¤a A¢´¨¬¢¬¢¬| ¢¯aA¡í
	//					   (Server -> Client) /cube m_info startIndex count 125,1|126,2|127,2|123,5&555,5&555,4/120000@125,1|126,2|127,2|123,5&555,5&555,4/120000
	//
	if (strArg1 == "r_info")
	{
		if (0 == arg2[0])
			Cube_request_result_list(ch);
		else
		{
			if (isdigit(*arg2))
			{
				int listIndex = 0, requestCount = 1;
				str_to_number(listIndex, arg2);

				if (0 != arg3[0] && isdigit(*arg3))
					str_to_number(requestCount, arg3);

				Cube_request_material_info(ch, listIndex, requestCount);
			}
		}

		return;
	}

	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			Cube_open(ch);
			break;

		case 'c':	// close
			Cube_close(ch);
			break;

		case 'l':	// list
			Cube_show_list(ch);
			break;

		case 'a':	// add cue_index inven_index
			{
				if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3))
					return;

				str_to_number(cube_index, arg2);
				str_to_number(inven_index, arg3);
				Cube_add_item (ch, cube_index, inven_index);
			}
			break;

		case 'd':	// delete
			{
				if (0 == arg2[0] || !isdigit(*arg2))
					return;

				str_to_number(cube_index, arg2);
				Cube_delete_item (ch, cube_index);
			}
			break;

		case 'm':	// make
			if (0 != arg2[0])
			{
				while (true == Cube_make(ch))
					dev_log (LOG_DEB0, "cube make success");
			}
			else
				Cube_make(ch);
			break;

		default:
			return;
	}
}
#endif

ACMD(do_in_game_mall)
{
	if (LC_IsEurope() == true)
	{
		char country_code[3];

		switch (LC_GetLocalType())
		{
			case LC_GERMANY:	country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_FRANCE:		country_code[0] = 'f'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_ITALY:		country_code[0] = 'i'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_SPAIN:		country_code[0] = 'e'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_UK:			country_code[0] = 'e'; country_code[1] = 'n'; country_code[2] = '\0'; break;
			case LC_TURKEY:		country_code[0] = 't'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_POLAND:		country_code[0] = 'p'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_PORTUGAL:	country_code[0] = 'p'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_GREEK:		country_code[0] = 'g'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_RUSSIA:		country_code[0] = 'r'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_DENMARK:	country_code[0] = 'd'; country_code[1] = 'k'; country_code[2] = '\0'; break;
			case LC_BULGARIA:	country_code[0] = 'b'; country_code[1] = 'g'; country_code[2] = '\0'; break;
			case LC_CROATIA:	country_code[0] = 'h'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_MEXICO:		country_code[0] = 'm'; country_code[1] = 'x'; country_code[2] = '\0'; break;
			case LC_ARABIA:		country_code[0] = 'a'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_CZECH:		country_code[0] = 'c'; country_code[1] = 'z'; country_code[2] = '\0'; break;
			case LC_ROMANIA:	country_code[0] = 'r'; country_code[1] = 'o'; country_code[2] = '\0'; break;
			case LC_HUNGARY:	country_code[0] = 'h'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_NETHERLANDS: country_code[0] = 'n'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_USA:		country_code[0] = 'u'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_CANADA:	country_code[0] = 'c'; country_code[1] = 'a'; country_code[2] = '\0'; break;
#ifdef __MULTI_LANGUAGE_SYSTEM__
			case LC_EUROPE: country_code[0] = 'e'; country_code[1] = 'u'; country_code[2] = '\0'; break;
#endif
			default:
				if (test_server == true)
				{
#ifdef __MULTI_LANGUAGE_SYSTEM__
					country_code[0] = 'p'; country_code[1] = 't'; country_code[2] = '\0';
#else
					country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0';
#endif
				}
				break;
		}

		char buf[512+1];
		char sas[33];
		MD5_CTX ctx;
		const char sas_key[] = "GF9001";

		snprintf(buf, sizeof(buf), "%u%u%s", ch->GetPlayerID(), ch->GetAID(), sas_key);

		MD5Init(&ctx);
		MD5Update(&ctx, (const unsigned char *) buf, strlen(buf));
#ifdef __FreeBSD__
		MD5End(&ctx, sas);
#else
		static const char hex[] = "0123456789abcdef";
		unsigned char digest[16];
		MD5Final(digest, &ctx);
		int i;
		for (i = 0; i < 16; ++i) {
			sas[i+i] = hex[digest[i] >> 4];
			sas[i+i+1] = hex[digest[i] & 0x0f];
		}
		sas[i+i] = '\0';
#endif

		snprintf(buf, sizeof(buf), "mall http://%s/ishop?pid=%u&c=%s&sid=%d&sas=%s",
				g_strWebMallURL.c_str(), ch->GetPlayerID(), country_code, g_server_id, sas);

		ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
	}
}


ACMD(do_dice)
{
	char arg1[256], arg2[256];
	int start = 1, end = 100;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (*arg1 && *arg2)
	{
		start = atoi(arg1);
		end = atoi(arg2);
	}
	else if (*arg1 && !*arg2)
	{
		start = 1;
		end = atoi(arg1);
	}

	end = MAX(start, end);
	start = MIN(start, end);

	int n = number(start, end);

#ifdef ENABLE_DICE_SYSTEM
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_DICE_INFO, LC_TEXT("%s´ÔÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_DICE_INFO, LC_TEXT("´ç½ÅÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), n, start, end);
#else
	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_INFO, LC_TEXT("%s´ÔÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ç½ÅÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), n, start, end);
#endif
}

#ifdef ENABLE_NEWSTUFF
ACMD(do_click_safebox)
{
	if ((ch->GetGMLevel() <= GM_PLAYER) && (ch->GetDungeon() || ch->GetWarMap()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot open the safebox in dungeon or at war."));
		return;
	}

	ch->SetSafeboxOpenPosition();
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
}
ACMD(do_force_logout)
{
	LPDESC pDesc=DESC_MANAGER::instance().FindByCharacterName(ch->GetName());
	if (!pDesc)
		return;
	pDesc->DelayedDisconnect(0);
}
#endif

ACMD(do_click_mall)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
}

ACMD(do_ride)
{
	dev_log(LOG_DEB0, "[DO_RIDE] start");

	if (ch->IsDead() || ch->IsStun())
	{
		return;
	}

	if (ch->GetMapIndex() == 113)
		return;

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
	if (ch->IsPolymorphed() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Nu poti folosi un mount atat timp cat esti transformat.");
		return;
	}

	if(ch->GetWear(WEAR_COSTUME_MOUNT))
	{
		CMountSystem* mountSystem = ch->GetMountSystem();
		LPITEM mount = ch->GetWear(WEAR_COSTUME_MOUNT);
		DWORD mobVnum = 0;
		
		if (!mountSystem && !mount)
		{
			return;
		}
		
		#ifdef __CHANGELOOK_SYSTEM__
		if (mount->GetTransmutation())
		{
			const TItemTable* itemTable = ITEM_MANAGER::instance().GetTable(mount->GetTransmutation());
			if (itemTable)
			{
				mobVnum = itemTable->alValues[1];
			}
			else
			{
				mobVnum = mount->GetValue(1);
			}
		}
		else
		{
			mobVnum = mount->GetValue(1);
		}
		#else
		if (mount->GetValue(1) != 0)
		{
			mobVnum = mount->GetValue(1);
		}
		#endif

		if (ch->GetMountVnum())
		{
			if(mountSystem->CountSummoned() == 0)
			{
				mountSystem->Unmount(mobVnum);
			}
		}
		else
		{
			if(mountSystem->CountSummoned() == 1)
			{
				mountSystem->Mount(mobVnum, mount);
			}
		}
		
		return;
	}
#endif

	if (ch->IsHorseRiding())
	{
		ch->StopRiding();
		return;
	}

	if (ch->GetHorse() != NULL)
	{
		ch->StartRiding();
		return;
	}

	for (BYTE i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM item = ch->GetInventoryItem(i);
		if (NULL == item)
		{
			continue;
		}

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_MOUNT)
#else
		if (item->GetType() == ITEM_COSTUME)
#endif
		{
			ch->UseItem(TItemPos (INVENTORY, i));
			return;
		}
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
}

#ifdef ENABLE_HIDE_COSTUME_SYSTEM
ACMD (do_costume_hide)
{
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));
	
	if (0 == arg1[0])
		return;

	int slot = atoi(arg1);

	ch->FuncHideCostume(slot);
}
#endif

#ifdef ENABLE_EXIT_FAST
ACMD(do_exit_fast)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "exitfastclientitachi");
	ch->GetDesc()->DelayedDisconnect(1);
}
#endif

#ifdef ENABLE_OFFSHOP_DECO
//Shop Decoration
ACMD(do_open_decoration){
	std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT activacion FROM player.offline_shop_decoration WHERE id = %u LIMIT 1", ch->GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0) {
		ch ->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Paquete de cachemira] Para usar esta funcion debes comprar el paquete de cachemira en la tienda de objetos."));
		return;
	}
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenDecoration");
}
ACMD(do_shop_decoration){
	const char *var;
	char arg1[255],arg2[255];
	var = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	int arg1_1 = atoi(arg1);
	int arg2_1 = atoi(arg2);

	std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT activacion FROM player.offline_shop_decoration WHERE id = %u LIMIT 1", ch->GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0){				
		ch->ChatPacket(CHAT_TYPE_INFO, "Error");
		return;	
	}
	/*
	{
		std::auto_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM player.player_shop WHERE player_id = %u", ch->GetPlayerID()));
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		BYTE bResult = 0;
		str_to_number(bResult, row[0]);

		if (bResult)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Error.");
			return;
		}
	}
	*/
	if ((arg1_1 >= 30000 && arg1_1 <= 30007)||(arg2_1 >= 0 && arg2_1 <= 5)){
		ch->OfflineDecoration(arg1_1);
		ch->OfflineDecoration1(arg2_1);
	}
	else{
		ch->ChatPacket(CHAT_TYPE_INFO, "Error.");
		return;
	}	
}
//Shop Decoration
#endif

ACMD(do_shop_open){
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenPrivateShop");
}

#if defined(__SORT_INVENTORY_ITEMS__) && defined(__SPECIAL_INVENTORY_SYSTEM__)
ACMD(do_sort_special_inventory)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE type = 0;
	str_to_number(type, arg1);

	ch->SortSpecialInventoryItems(type);
}
#endif

ACMD(do_fishgame)
{
	LPITEM rod = ch->GetWear(WEAR_WEAPON);
	if (!rod)
		return;
	if (ch->m_pkFishingEvent)
	{
		char arg1[256];
		one_argument(argument, arg1, sizeof(arg1));
		int arg1_1 = atoi(arg1);
		if (arg1_1 == 58236)
		{
			++ch->fishGame;
		}
		else if (arg1_1 == 78531)
		{
			if (rod->GetType() == ITEM_ROD)
			{
				ch->fishing_take();
			}
		}
		else if (arg1_1 == 90295)
		{
			ch->m_pkFishingEvent = NULL;
			rod->SetSocket(2, 0);
			fishing::FishingFail(ch); // :)
		}
	}

}

ACMD(do_stat_val)
{
	char	arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	int val = 0;
	str_to_number(val, arg2);
	
	if (!*arg1 || val <= 0)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change your state as long as you are transformed."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;
	
	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;
	
	if (val > ch->GetPoint(POINT_STAT))
		val = ch->GetPoint(POINT_STAT);
	
	if (ch->GetRealPoint(idx) + val > MAX_STAT)
		val = MAX_STAT - ch->GetRealPoint(idx);

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + val);
	ch->SetPoint(idx, ch->GetPoint(idx) + val);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
		ch->PointChange(POINT_MAX_HP, 0);
	else if (idx == POINT_HT)
		ch->PointChange(POINT_MAX_SP, 0);

	ch->PointChange(POINT_STAT, -val);
	ch->ComputePoints();
}
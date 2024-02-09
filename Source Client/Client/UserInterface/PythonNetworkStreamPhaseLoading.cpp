#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "Packet.h"
#include "PythonApplication.h"
#include "NetworkActorManager.h"

#include "AbstractPlayer.h"

#include "../eterPack/EterPackManager.h"

void CPythonNetworkStream::EnableChatInsultFilter(bool isEnable)
{
	m_isEnableChatInsultFilter=isEnable;
}

void CPythonNetworkStream::__FilterInsult(char* szLine, UINT uLineLen)
{
	m_kInsultChecker.FilterInsult(szLine, uLineLen);
}

bool CPythonNetworkStream::IsChatInsultIn(const char* c_szMsg)
{
	if (m_isEnableChatInsultFilter)
		return false;

	return IsInsultIn(c_szMsg);
}

bool CPythonNetworkStream::IsInsultIn(const char* c_szMsg)
{
	return m_kInsultChecker.IsInsultIn(c_szMsg, strlen(c_szMsg));
}

#if defined(__BL_LOADING_TIP__)
CPythonNetworkStream::CTIPMANAGER::~CTIPMANAGER()
{
	for (const std::string* sTip : vTipVnum)
		delete sTip;

	for (const STIPGROUP* pTip : vTipGroup)
		delete pTip;
}

CPythonNetworkStream::CTIPMANAGER::STIPGROUP::STIPGROUP(std::vector<long>&& m_MapIdx, std::vector<uint16_t>&& m_TipIdx)
	: vMapIdx(std::move(m_MapIdx)), vTipIdx(std::move(m_TipIdx))
{
}

bool CPythonNetworkStream::CTIPMANAGER::STIPGROUP::IsMapExist(const long MapIndex) const
{
	return (std::find(vMapIdx.begin(), vMapIdx.end(), MapIndex) != vMapIdx.end());
}

bool CPythonNetworkStream::CTIPMANAGER::STIPGROUP::IsIndexExist(const uint16_t Index) const
{
	return (std::find(vTipIdx.begin(), vTipIdx.end(), Index) != vTipIdx.end());
}

bool CPythonNetworkStream::CTIPMANAGER::InitList(const char* FileName)
{
	CTextFileLoader* pkTextFileLoader = CTextFileLoader::Cache(FileName);

	if (!pkTextFileLoader)
		return false;

	if (pkTextFileLoader->IsEmpty())
		return false;

	pkTextFileLoader->SetTop();

	for (DWORD i = 0; i < pkTextFileLoader->GetChildNodeCount(); ++i) {
		CTextFileLoader::CGotoChild GotoChild(pkTextFileLoader, i);
		CTokenVector* tv;

		std::vector<long> vMapIdx;
		if (pkTextFileLoader->GetTokenVector("map_index", &tv))
			for (CTokenVector::const_iterator it = tv->begin(); it != tv->end(); ++it)
				vMapIdx.emplace_back(std::stol(*it));

		std::vector<uint16_t> vTipIdx;
		if (pkTextFileLoader->GetTokenVector("tip_vnum", &tv))
			for (CTokenVector::const_iterator it = tv->begin(); it != tv->end(); ++it)
				vTipIdx.emplace_back(std::stoi(*it));

		vTipGroup.push_back(new STIPGROUP(std::move(vMapIdx), std::move(vTipIdx)));
	}

	return true;
}

bool CPythonNetworkStream::CTIPMANAGER::InitVnum(const char* FileName)
{
	const VOID* pvData;
	CMappedFile kFile;
	if (!CEterPackManager::Instance().Get(kFile, FileName, &pvData))
		return false;

	CMemoryTextFileLoader kTextFileLoader;
	kTextFileLoader.Bind(kFile.Size(), pvData);

	CTokenVector kTokenVector;
	for (DWORD i = 0; i < kTextFileLoader.GetLineCount(); ++i)
	{
		if (kTextFileLoader.SplitLineByTab(i, &kTokenVector) == false)
			continue;

		std::string* sTip = new std::string(std::move(kTokenVector.at(1)));
		vTipVnum.push_back(sTip);

		const uint16_t uIdx = std::stoi(kTokenVector.at(0));

		for (STIPGROUP* pTip : vTipGroup) {
			if (pTip->IsIndexExist(uIdx))
				pTip->vLocalTipVnum.push_back(sTip);
		}
	}

	return true;
}

std::string* CPythonNetworkStream::CTIPMANAGER::GetTip(const long lMapIndex) const
{
	if (vTipGroup.empty())
		return nullptr;
	
	auto it = std::find_if(vTipGroup.begin(), vTipGroup.end(), 
		[lMapIndex](const STIPGROUP* pTipGroup) { return pTipGroup->IsMapExist(lMapIndex); });

	TIPVEC* vecTip = nullptr;
	
	if (it != vTipGroup.end()) {
		// we find the group with special map index
		vecTip = &(*it)->vLocalTipVnum;
	}
	else
	{
		// we cannot find the group with special map index
		// we will get tip from global("normal") group
		vecTip = &vTipGroup.at(0)->vLocalTipVnum;
	}

	if (vecTip && vecTip->empty() == false) {
		const long idx = random_range(0, vecTip->size() - 1);
		return vecTip->at(idx);
	}

	return nullptr;
}
#endif

bool CPythonNetworkStream::LoadInsultList(const char* c_szInsultListFileName)
{
	CMappedFile file;
	const VOID* pvData;
	if (!CEterPackManager::Instance().Get(file, c_szInsultListFileName, &pvData))
		return false;

	CMemoryTextFileLoader kMemTextFileLoader;
	kMemTextFileLoader.Bind(file.Size(), pvData);

	m_kInsultChecker.Clear();
	for (DWORD dwLineIndex=0; dwLineIndex<kMemTextFileLoader.GetLineCount(); ++dwLineIndex)
	{
		const std::string& c_rstLine=kMemTextFileLoader.GetLineString(dwLineIndex);
		m_kInsultChecker.AppendInsult(c_rstLine);
	}
	return true;
}

bool CPythonNetworkStream::LoadConvertTable(DWORD dwEmpireID, const char* c_szFileName)
{
	if (dwEmpireID<1 || dwEmpireID>=4)
		return false;

	CMappedFile file;
	const VOID* pvData;
	if (!CEterPackManager::Instance().Get(file, c_szFileName, &pvData))
		return false;

	DWORD dwEngCount=26;
	DWORD dwHanCount=(0xc8-0xb0+1)*(0xfe-0xa1+1);
	DWORD dwHanSize=dwHanCount*2;
	DWORD dwFileSize=dwEngCount*2+dwHanSize;

	if (file.Size()<dwFileSize)
		return false;

	char* pcData=(char*)pvData;

	STextConvertTable& rkTextConvTable=m_aTextConvTable[dwEmpireID-1];
	memcpy(rkTextConvTable.acUpper, pcData, dwEngCount);pcData+=dwEngCount;
	memcpy(rkTextConvTable.acLower, pcData, dwEngCount);pcData+=dwEngCount;
	memcpy(rkTextConvTable.aacHan, pcData, dwHanSize);

	return true;
}

bool CPythonNetworkStream::LoadLocaleQuestVnum(const char* c_szLocaleQuestFileName)
{
	CMappedFile file;
	const VOID* pvData;

	if (!CEterPackManager::Instance().Get(file, c_szLocaleQuestFileName, &pvData))
	{
		Tracef("CPythonNetworkStream::LoadLocaleQuestVnum(c_szLocaleQuestFileName=%s) - Load Error", c_szLocaleQuestFileName);
		return false;
	}

	CMemoryTextFileLoader kMemTextFileLoader;
	kMemTextFileLoader.Bind(file.Size(), pvData);

	m_kMap_dwID_strLocaleQuest.clear();

	CTokenVector TokenVector;
	for (DWORD i = 0; i < kMemTextFileLoader.GetLineCount(); ++i)
	{
		if (!kMemTextFileLoader.SplitLineByTab(i, &TokenVector))
			continue;

		const char* c_szComment = "#";
		if (TokenVector[0].compare(0, 1, c_szComment) == 0 || TokenVector.size() != 2)
			continue;

		m_kMap_dwID_strLocaleQuest.insert(std::make_pair(atoi(TokenVector[0].c_str()), TokenVector[1]));
	}

	return true;
}

const char* CPythonNetworkStream::GetLocaleQuestVnum(DWORD dwVnum)
{
	std::map<DWORD, std::string>::iterator it = m_kMap_dwID_strLocaleQuest.find(dwVnum);

	static char s_szChat[1024 + 1] = "";
	if (it != m_kMap_dwID_strLocaleQuest.end())
	{
		sprintf(s_szChat, "%s", it->second.c_str());
	}

	return s_szChat;
}

// Loading ---------------------------------------------------------------------------
void CPythonNetworkStream::LoadingPhase()
{
	TPacketHeader header;

	if (!CheckPacket(&header))
		return;

#if defined(_DEBUG) && defined(ENABLE_PRINT_RECV_PACKET_DEBUG)
	Tracenf("RECV HEADER : %u , phase %s ", header, m_strPhase.c_str());
#endif

	switch (header)
	{
		case HEADER_GC_PHASE:
			if (RecvPhasePacket())
				return;
			break;

		case HEADER_GC_MAIN_CHARACTER:
			if (RecvMainCharacter())
				return;
			break;

		// SUPPORT_BGM
		case HEADER_GC_MAIN_CHARACTER2_EMPIRE:
			if (RecvMainCharacter2_EMPIRE())
				return;
			break;

		case HEADER_GC_MAIN_CHARACTER3_BGM:
			if (RecvMainCharacter3_BGM())
				return;
			break;

		case HEADER_GC_MAIN_CHARACTER4_BGM_VOL:
			if (RecvMainCharacter4_BGM_VOL())
				return;
			break;

		// END_OF_SUPPORT_BGM

		case HEADER_GC_CHARACTER_UPDATE:
			if (RecvCharacterUpdatePacket())
				return;
			break;

		case HEADER_GC_PLAYER_POINTS:
			if (__RecvPlayerPoints())
				return;
			break;

		case HEADER_GC_PLAYER_POINT_CHANGE:
			if (RecvPointChange())
				return;
			break;

		case HEADER_GC_ITEM_SET:
			if (RecvItemSetPacket())
				return;
			break;

		case HEADER_GC_PING:
			if (RecvPingPacket())
				return;
			break;

		case HEADER_GC_QUICKSLOT_ADD:
			if (RecvQuickSlotAddPacket())
				return;
			break;

		case HEADER_GC_HYBRIDCRYPT_KEYS:
			RecvHybridCryptKeyPacket();
			return;
			break;

		case HEADER_GC_HYBRIDCRYPT_SDB:
			RecvHybridCryptSDBPacket();
			return;
			break;

#ifdef ENABLE_GROWTH_PET_SYSTEM
		case HEADER_GC_PET_SET:
			if (RecvPetSet())
				return;
			break;
#endif

		default:
			GamePhase();
			return;
			break;
	}

	RecvErrorPacket(header);
}

void CPythonNetworkStream::SetLoadingPhase()
{
	if ("Loading"!=m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Loading Phase ##");
	Tracen("");

	m_strPhase = "Loading";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::LoadingPhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveLoadingPhase);

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.Clear();

	CFlyingManager::Instance().DeleteAllInstances();
	CEffectManager::Instance().DeleteAllInstances();

	__DirectEnterMode_Initialize();
}

bool CPythonNetworkStream::RecvMainCharacter()
{
	TPacketGCMainCharacter MainChrPacket;
	if (!Recv(sizeof(TPacketGCMainCharacter), &MainChrPacket))
		return false;

	m_dwMainActorVID = MainChrPacket.dwVID;
	m_dwMainActorRace = MainChrPacket.wRaceNum;
	m_dwMainActorEmpire = 0;
	m_dwMainActorSkillGroup = MainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.SetName(MainChrPacket.szName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", MainChrPacket.lX, MainChrPacket.lY));

	//Tracef(" >> RecvMainCharacter\n");

	SendClientVersionPacket();
	return true;
}

// SUPPORT_BGM
bool CPythonNetworkStream::RecvMainCharacter2_EMPIRE()
{
	TPacketGCMainCharacter2_EMPIRE mainChrPacket;
	if (!Recv(sizeof(mainChrPacket), &mainChrPacket))
		return false;

	m_dwMainActorVID = mainChrPacket.dwVID;
	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	//Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	SendClientVersionPacket();
	return true;
}

bool CPythonNetworkStream::RecvMainCharacter3_BGM()
{
	TPacketGCMainCharacter3_BGM mainChrPacket;
	if (!Recv(sizeof(mainChrPacket), &mainChrPacket))
		return false;

	m_dwMainActorVID = mainChrPacket.dwVID;
	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szUserName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	__SetFieldMusicFileName(mainChrPacket.szBGMName);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	//Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	SendClientVersionPacket();
	return true;
}

bool CPythonNetworkStream::RecvMainCharacter4_BGM_VOL()
{
	TPacketGCMainCharacter4_BGM_VOL mainChrPacket;
	if (!Recv(sizeof(mainChrPacket), &mainChrPacket))
		return false;

	m_dwMainActorVID = mainChrPacket.dwVID;
	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer=CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szUserName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	__SetFieldMusicFileInfo(mainChrPacket.szBGMName, mainChrPacket.fBGMVol);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	//Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	SendClientVersionPacket();
	return true;
}


static std::string	gs_fieldMusic_fileName;
static float		gs_fieldMusic_volume = 1.0f / 5.0f * 0.1f;

void CPythonNetworkStream::__SetFieldMusicFileName(const char* musicName)
{
	gs_fieldMusic_fileName = musicName;
}

void CPythonNetworkStream::__SetFieldMusicFileInfo(const char* musicName, float vol)
{
	gs_fieldMusic_fileName = musicName;
	gs_fieldMusic_volume = vol;
}

const char* CPythonNetworkStream::GetFieldMusicFileName()
{
	return gs_fieldMusic_fileName.c_str();
}

float CPythonNetworkStream::GetFieldMusicVolume()
{
	return gs_fieldMusic_volume;
}
// END_OF_SUPPORT_BGM


bool CPythonNetworkStream::__RecvPlayerPoints()
{
	TPacketGCPoints PointsPacket;

	if (!Recv(sizeof(TPacketGCPoints), &PointsPacket))
		return false;

	for (DWORD i = 0; i < POINT_MAX_NUM; ++i)
		CPythonPlayer::Instance().SetStatus(i, PointsPacket.points[i]);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshStatus", Py_BuildValue("()"));
	return true;
}

void CPythonNetworkStream::StartGame()
{
	m_isStartGame=TRUE;
}

bool CPythonNetworkStream::SendEnterGame()
{
	TPacketCGEnterFrontGame EnterFrontGamePacket;

	EnterFrontGamePacket.header = HEADER_CG_ENTERGAME;

	if (!Send(sizeof(EnterFrontGamePacket), &EnterFrontGamePacket))
	{
		Tracen("Send EnterFrontGamePacket");
		return false;
	}

	if (!SendSequence())
		return false;

	__SendInternalBuffer();
	return true;
}


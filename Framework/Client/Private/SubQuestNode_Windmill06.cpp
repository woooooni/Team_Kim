#include "stdafx.h"
#include "SubQuestNode_Windmill06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

CSubQuestNode_Windmill06::CSubQuestNode_Windmill06()
{
}

HRESULT CSubQuestNode_Windmill06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("ǳ�� ����");
	m_strQuestContent = TEXT("��¥ �ҳฦ ã��");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("����� ��Ʋ!");
	m_strNextQuestContent = TEXT("����� ��Ʋ�� �ҳฦ ȥ������");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill06.json");

	for (const auto& talkDesc : Load) 
	{
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Windmill06::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pGeekGirl = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Engineer_Dummy");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pGeekGirl);

	/* ī�޶� Ÿ�� ���� */
	// CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");

	//if (nullptr != pTarget)
	//{

		// �ӽ� �ּ�
		//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		//if (nullptr != pActionCam)
		//{
		//	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);
		//	pActionCam->Start_Action_Talk(); //���� ȥ�ڸ� null
		//}


	//}

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Windmill06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			//if (nullptr != pActionCam)
			//	pActionCam->Finish_Action_Talk();

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill06::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Windmill06::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_00_KuuSay_Hey~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_01_GGSay_Eh.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pGeekGirl->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pGeekGirl->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_02_Hehehe.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_03_GGSay_EEE~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pGeekGirl->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pGeekGirl->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_04_What.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_05_GGSay_Happy1.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pGeekGirl->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pGeekGirl->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle03"));
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_06_EAAAAAA!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSEndOfDonDonKing01"));
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_07_GGSay_Play!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pGeekGirl->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pGeekGirl->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_08_EEEEEE!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	case 9:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_06_09_GGSay_Happy2.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pGeekGirl->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pGeekGirl->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		break;
	}
}

CSubQuestNode_Windmill06* CSubQuestNode_Windmill06::Create()
{
	CSubQuestNode_Windmill06* pInstance = new CSubQuestNode_Windmill06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill06::Free()
{
	__super::Free();
}

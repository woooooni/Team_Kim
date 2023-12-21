#include "stdafx.h"
#include "StelliaState_SpinTail.h"

#include "Stellia.h"

CStelliaState_SpinTail::CStelliaState_SpinTail(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpinTail::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpinTail::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill01"));
}

void CStelliaState_SpinTail::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_SpinTail::Exit_State()
{
}

CStelliaState_SpinTail* CStelliaState_SpinTail::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpinTail* pInstance = new CStelliaState_SpinTail(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpinTail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpinTail::Free()
{
	__super::Free();
}


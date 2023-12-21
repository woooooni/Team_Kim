#include "stdafx.h"
#include "StelliaState_Attack2.h"

#include "Stellia.h"

CStelliaState_Attack2::CStelliaState_Attack2(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Attack2::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Attack2::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Attack02"));
}

void CStelliaState_Attack2::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}
}

void CStelliaState_Attack2::Exit_State()
{
}

CStelliaState_Attack2* CStelliaState_Attack2::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Attack2* pInstance = new CStelliaState_Attack2(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Attack2::Free()
{
	__super::Free();
}


#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Large_Finish.h"

CState_SwordMan_Neutral_Pick_Large_Finish::CState_SwordMan_Neutral_Pick_Large_Finish(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Large_Finish::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    m_pCharacter = dynamic_cast<CCharacter*>(m_pStateMachineCom->Get_Owner());

    if (nullptr == m_pCharacter)
        return E_FAIL;
    
    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Large_Finish::Enter_State(void* pArg)
{
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Large_Finish::Tick_State(_float fTimeDelta)
{
    
}

void CState_SwordMan_Neutral_Pick_Large_Finish::Exit_State()
{

}


CState_SwordMan_Neutral_Pick_Large_Finish* CState_SwordMan_Neutral_Pick_Large_Finish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Large_Finish* pInstance = new CState_SwordMan_Neutral_Pick_Large_Finish(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Large_Finish");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Large_Finish::Free()
{
    __super::Free();
}

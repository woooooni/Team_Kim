#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_SwordMan_Neutral_Pick_Large_Enter.h"

CState_SwordMan_Neutral_Pick_Large_Enter::CState_SwordMan_Neutral_Pick_Large_Enter(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Neutral_Pick_Large_Enter::Initialize(const list<wstring>& AnimationList)
{
    
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    return S_OK;
}

void CState_SwordMan_Neutral_Pick_Large_Enter::Enter_State(void* pArg)
{
    m_pCharacter->Disappear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_SwordMan_Neutral_Pick_Large_Enter::Tick_State(_float fTimeDelta)
{
    if (nullptr != m_pCharacter->Get_Target())
    {
        CTransform* pTargetTransform = m_pCharacter->Get_Target()->Get_Component<CTransform>(L"Com_Transform");
        if (nullptr != pTargetTransform)
        {
            Vec4 vHandCenterPosition = {};
            Vec4 vLeftHandPosition = (m_pModelCom->Get_SocketLocalMatrix(0) * m_pTransformCom->Get_WorldMatrix()).Translation();
            Vec4 vRightHandPosition = (m_pModelCom->Get_SocketLocalMatrix(1) * m_pTransformCom->Get_WorldMatrix()).Translation();

            vHandCenterPosition = (vLeftHandPosition + vRightHandPosition) / 2.f;

            pTargetTransform->Set_State(CTransform::STATE_POSITION, vHandCenterPosition);
        }
    }
    else
    {
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_IDLE);
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::NEUTRAL_PICK_LARGE_IDLE);
}

void CState_SwordMan_Neutral_Pick_Large_Enter::Exit_State()
{

}


CState_SwordMan_Neutral_Pick_Large_Enter* CState_SwordMan_Neutral_Pick_Large_Enter::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Neutral_Pick_Large_Enter* pInstance = new CState_SwordMan_Neutral_Pick_Large_Enter(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Neutral_Pick_Large_Enter");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Neutral_Pick_Large_Enter::Free()
{
    __super::Free();
}

#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "Animation.h"
#include "State_SwordMan_Battle_Dash.h"
#include "Motion_Trail.h"

CState_SwordMan_Battle_Dash::CState_SwordMan_Battle_Dash(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_SwordMan_Battle_Dash::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;

    
    
    return S_OK;
}

void CState_SwordMan_Battle_Dash::Enter_State(void* pArg)
{
    // 마우스 방향으로 구르기
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);

    m_MotionTrailDesc.fAlphaSpeed = 2.f;
    m_MotionTrailDesc.fBlurPower = 0.1f;
    m_MotionTrailDesc.vBloomPower = { 0.f ,0.f, 0.f };
    m_MotionTrailDesc.vRimColor = { 1.f, 1.f, 0.f, 1.f };
    m_MotionTrailDesc.fMotionTrailTime = 0.1f;

    m_pCharacter->Generate_MotionTrail(m_MotionTrailDesc);
}

void CState_SwordMan_Battle_Dash::Tick_State(_float fTimeDelta)
{
    if (m_pModelCom->Get_Progress() <= 0.5f)
    {
        m_pTransformCom->Move(m_pTransformCom->Get_Look(), 8.f, fTimeDelta);
    }
    else if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_Progress() >= 0.85f)
    {
        Input(fTimeDelta);
        m_pCharacter->Stop_MotionTrail();
    }

    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_SwordMan_Battle_Dash::Exit_State()
{
    m_iCurrAnimIndex = 0;
}


void CState_SwordMan_Battle_Dash::Input(_float fTimeDelta)
{
    if(KEY_HOLD(KEY::W) || KEY_HOLD(KEY::A) || KEY_HOLD(KEY::S) || KEY_HOLD(KEY::D))
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

CState_SwordMan_Battle_Dash* CState_SwordMan_Battle_Dash::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_SwordMan_Battle_Dash* pInstance = new CState_SwordMan_Battle_Dash(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_Battle_Dash");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_SwordMan_Battle_Dash::Free()
{
    __super::Free();
}

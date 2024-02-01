#include "stdafx.h"
#include "StelliaState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Stellia.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Particle_Manager.h"
#include "Decal.h"

_uint CStelliaState_Base::m_iAtkIndex = 0;

CStelliaState_Base::CStelliaState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CStelliaState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pStellia = dynamic_cast<CStellia*>(m_pOwner);

	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fRunSpeed = 6.f;
	m_fSprintSpeed = 12.f;
	m_fRage3AroundSpeed = 40.f;

	m_fAroundDist = 30.f;


	/* 공격 패턴만 따로 모아놓기. (후에 순차적 혹은 랜덤으로 전환하기 위해) */
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK1);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_SPINTAIL);
	m_vecAtkState.push_back(CStellia::STELLIA_JUMPSTAMP);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_LASER);
	m_vecAtkState.push_back(CStellia::STELLIA_CHARGE);

	return S_OK;
}

void CStelliaState_Base::Enter_State(void* pArg)
{
}

void CStelliaState_Base::Tick_State(_float fTimeDelta)
{
	/* Dead */
	if (m_pStellia->Get_Stat().fHp <= 0.f)
		m_pStateMachineCom->Change_State(CStellia::STELLIA_DEAD);
}

void CStelliaState_Base::Exit_State()
{
}

void CStelliaState_Base::Init_Pattern()
{
	m_vecAtkState.clear();

	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_SPINTAIL);
	m_vecAtkState.push_back(CStellia::STELLIA_JUMPSTAMP);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK1);
	m_vecAtkState.push_back(CStellia::STELLIA_TRIPLELASER);
	m_vecAtkState.push_back(CStellia::STELLIA_CHARGE);
	m_vecAtkState.push_back(CStellia::STELLIA_BIGBANG);

}

_bool CStelliaState_Base::State_Wait(_float fDestTime, _float fTimeDelta)
{
	/* 기안티와 다르게 루프 애니메이션이 아니기에 종료 시점으로 처리. */
	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_Stellia.ao|Stellia_Stand02"))
	{
		m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand02"));
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= fDestTime)
	{
		m_fTime = fDestTime - m_fTime;
		return true;
	}

	return false;
}

void CStelliaState_Base::Rage1_Tick(_float fTimeDelta)
{
	if (m_pStellia->Get_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE))
	{
		if (m_pStellia->Get_AccDamage() >= m_pStellia->Get_DestDamage())
		{
			_float fStunTime = 15.f;
			m_pStellia->Reset_RageAccDamage();
			m_pStellia->Set_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE, false);
			m_pStateMachineCom->Change_State(CStellia::STELLIA_COUNTERSTART, &fStunTime);
		}
	}
}

void CStelliaState_Base::Generate_Explosion(_uint iCount)
{
	CTransform* pTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
	{
		MSG_BOX("Transform Get Failed.");
		return;
	}

	for (_uint i = 0; i < iCount; ++i)
	{
		Vec4 vExplosionPos = pTransform->Get_Position();
		vExplosionPos.x += GI->RandomFloat(-10.f, 10.f);
		vExplosionPos.y = m_pStellia->Get_OriginPos().y + 0.5f;
		vExplosionPos.z += GI->RandomFloat(-10.f, 10.f);

		CGameObject* pExplosion = nullptr;

		if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, L"Prorotype_GameObject_Stellia_Explosion", m_pStellia, &pExplosion)))
		{
			MSG_BOX("Add Icicle Failed.");
			return;
		}

		CTransform* pIcicleTransform = pExplosion->Get_Component<CTransform>(L"Com_Transform");

		pIcicleTransform->Set_State(CTransform::STATE_POSITION, vExplosionPos);
	}
}

void CStelliaState_Base::Free()
{
	__super::Free();

	Safe_Release(m_pDecal);
}

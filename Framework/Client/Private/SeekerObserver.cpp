#include "stdafx.h"
#include "SeekerObserver.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CSeekerObserver::CSeekerObserver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CSeekerObserver::CSeekerObserver(const CSeekerObserver& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CSeekerObserver::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerObserver::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CSeekerObserver::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSeekerObserver::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CSeekerObserver::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CSeekerObserver::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CSeekerObserver::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CSeekerObserver::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CSeekerObserver::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -8.f, 0.f, 0.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SeekerObserver"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerObserver::Ready_States()
{
	m_strObjectTag = TEXT("SeekerObserver");

	m_tStat.fSpeed = 2.5f;
	m_tStat.bHasMove = false;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerObserver.ao|SeekerObserver_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerObserver.ao|SeekerObserver_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	// SKM_SeekerKing.ao|SeekerCat_Walk
	strAnimationName.push_back(L"SKM_SeekerObserver.ao|SeekerObserver_Stand");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerObserver.ao|SeekerObserver_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CSeekerObserver::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerObserver::Ready_Sockets()
{
	return S_OK;
}

CSeekerObserver* CSeekerObserver::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSeekerObserver* pInstance = new CSeekerObserver(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSeekerObserver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSeekerObserver::Clone(void* pArg)
{
	CSeekerObserver* pInstance = new CSeekerObserver(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSeekerObserver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSeekerObserver::Free()
{
	__super::Free();
}
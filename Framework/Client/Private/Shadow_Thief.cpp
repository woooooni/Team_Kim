#include "stdafx.h"
#include "Shadow_Thief.h"

#include "GameInstance.h"

#include "Shadow_ThiefBT.h"

CShadow_Thief::CShadow_Thief(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CShadow_Thief::CShadow_Thief(const CShadow_Thief& rhs)
	: CMonster(rhs)
{
}

HRESULT CShadow_Thief::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CShadow_Thief::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CShadow_Thief::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::NUM_5))
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK];
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];
	}
	if (KEY_TAP(KEY::V) || KEY_TAP(KEY::B))
	{
		m_tStat.fHp -= 10.f;
	}

	// << : Test 
	if (KEY_TAP(KEY::HOME))
	{
		_uint iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex();
		m_pModelCom->Set_Animation(iCurAnimIndex + 1);
	}
	else if (KEY_TAP(KEY::DEL))
	{
		_int iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex() - 1;
		if (iCurAnimIndex < 0)
			iCurAnimIndex = 0;
		m_pModelCom->Set_Animation(iCurAnimIndex);
	}
	// >> 

	__super::Tick(fTimeDelta);
}

void CShadow_Thief::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CShadow_Thief::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CShadow_Thief::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CShadow_Thief::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CShadow_Thief::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CShadow_Thief::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CShadow_Thief::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-4.f, 1.f, 4.f, 1.f));
	m_vOriginPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	/* 로밍 경로(임시) */
	m_vecRoamingArea.push_back(XMVectorSet(-3.f, 1.f, 1.f, 1.f));
	m_vecRoamingArea.push_back(XMVectorSet(-1.f, 1.f, 6.f, 1.f));
	m_vecRoamingArea.push_back(m_vOriginPos);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Shadow_Thief"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	//CRigidBody::RIGID_BODY_DESC RigidDesc;
	//RigidDesc.pNavigation = m_pNavigationCom;
	//RigidDesc.pTransform = m_pTransformCom;
	//
	//
	//RigidDesc.PhysXDesc.vOffsetPos = { 0.f, 0.f, 0.f };
	//RigidDesc.PhysXDesc.vExtents = { 5.f, 5.f, 10.f };
	//
	//RigidDesc.PhysXDesc.eColliderType = PHYSX_COLLIDER_TYPE::BOX;
	//RigidDesc.PhysXDesc.eRigidType = PHYSX_RIGID_TYPE::DYNAMIC;
	//
	//RigidDesc.PhysXDesc.bLockAngle_X = true;
	//RigidDesc.PhysXDesc.bLockAngle_Y = false;
	//RigidDesc.PhysXDesc.bLockAngle_Z = true;
	//
	//RigidDesc.PhysXDesc.bKinematic = false;
	//RigidDesc.PhysXDesc.fAngularDamping = 30.f;
	//RigidDesc.PhysXDesc.fDensity = 1.f;
	//
	//
	//RigidDesc.PhysXDesc.fStaticFriction = 0.f;
	//RigidDesc.PhysXDesc.fDynamicFriction = 1.f;
	//RigidDesc.PhysXDesc.fRestitution = 0.f;
	//
	//RigidDesc.PhysXDesc.fMaxVelocity = 10.f;
	//RigidDesc.PhysXDesc.pGameObject = this;
	//RigidDesc.PhysXDesc.bKinematic = true;
	//
	///* For. Com_RigidBody*/
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CShadow_Thief::Ready_States()
{
	m_pBTCom = CShadow_ThiefBT::Create(m_pDevice, m_pContext, this);

	m_tStat.fMaxHp = 100;
	m_tStat.fHp = 100;

	return S_OK;
}

HRESULT CShadow_Thief::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);
	
	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.f;
	ColliderDesc.tSphere = tSphere;
	
	ColliderDesc.pOwner = this;
	ColliderDesc.pNode = nullptr;
	ColliderDesc.pOwnerTransform = m_pTransformCom;
	ColliderDesc.ModePivotMatrix = m_pModelCom->Get_PivotMatrix();
	ColliderDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);
	ColliderDesc.bLockAngle_X = false;
	ColliderDesc.bLockAngle_Y = false;
	ColliderDesc.bLockAngle_Z = false;
	
	ColliderDesc.fAngularDamping = 0.f;
	ColliderDesc.fDensity = 1.f;
	
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
		return E_FAIL;



	//CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	//ZeroMemory(&OBBDesc, sizeof OBBDesc);
	//
	//BoundingOrientedBox OBBBox;
	//ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));
	//
	//XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYawFromVector(XMVectorSet(0.f, 0.f, 0.f, 1.f)));
	//OBBBox.Extents = { 500.f, 200.f, 500.f };
	//
	//OBBDesc.tBox = OBBBox;
	//
	//OBBDesc.pOwner = this;
	//OBBDesc.pNode = nullptr;
	//OBBDesc.pOwnerTransform = m_pTransformCom;
	//OBBDesc.ModePivotMatrix = m_pModelCom->Get_PivotMatrix();
	//OBBDesc.vOffsetPosition = Vec3(0.f, 250.f, 0.f);
	//OBBDesc.bLockAngle_X = true;
	//OBBDesc.bLockAngle_Y = true;
	//OBBDesc.bLockAngle_Z = true;
	//
	//OBBDesc.fAngularDamping = 0.f;
	//OBBDesc.fDensity = 1.f;
	//
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
	//	return E_FAIL;

	return S_OK;
}

CShadow_Thief* CShadow_Thief::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CShadow_Thief* pInstance = new CShadow_Thief(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CShadow_Thief");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShadow_Thief::Clone(void* pArg)
{
	CShadow_Thief* pInstance = new CShadow_Thief(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CShadow_Thief");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_Thief::Free()
{
	__super::Free();

}

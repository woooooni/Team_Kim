#include "RigidBody.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "..\Public\PhysX_Body.h"

CPhysX_Body::CPhysX_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
	
}

CPhysX_Body::CPhysX_Body(CPhysX_Body& rhs)
	: CComponent(rhs)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

HRESULT CPhysX_Body::Initialize_Prototype()
{

#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;
#endif
	return S_OK;
}

HRESULT CPhysX_Body::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PHYSX_BODY_DESC* pDesc = static_cast<PHYSX_BODY_DESC*>(pArg);
	m_pTransformCom = pDesc->pTransform;

	if (nullptr == m_pTransformCom)
		return E_FAIL;

	Safe_AddRef(m_pTransformCom);

	m_eColliderType = pDesc->PhysXDesc.eColliderType;
	m_eRigidType = pDesc->PhysXDesc.eRigidType;

	if (m_eRigidType != PHYSX_RIGID_TYPE::DYNAMIC)
		return E_FAIL;

	
	m_pXBody = GI->Add_Dynamic_Actor(pDesc->PhysXDesc);
	if (nullptr == m_pXBody)
		return E_FAIL;

#ifdef _DEBUG
	XMStoreFloat3(&m_vStartPosition, XMLoadFloat3(&pDesc->PhysXDesc.vStartPosition) + m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pOriginal_OBB = new BoundingOrientedBox(m_vStartPosition, _float3(pDesc->PhysXDesc.vExtents.x * 0.5f,
		pDesc->PhysXDesc.vExtents.y * 0.5f,
		pDesc->PhysXDesc.vExtents.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));
	m_pOBB = new BoundingOrientedBox(*m_pOriginal_OBB);


	m_pOriginal_Sphere = new BoundingSphere(m_vStartPosition, pDesc->PhysXDesc.fRadius);
	m_pSphere = new BoundingSphere(*m_pOriginal_Sphere);
#endif

	m_bFirst = true;

	return S_OK;
}


void CPhysX_Body::Update_PhysX_Body(_float fTimeDelta)
{
#ifdef _DEBUG
	m_bCollision = false;
#endif // DEBUG
	if (m_pOwner->Is_Dead())
		return;

	if (false == m_bFirst && m_pXBody->isSleeping())
		return;

	PxTransform PxTransform = m_pXBody->getGlobalPose();
	_matrix WorldMatrix;

	_float3 vScale = m_pTransformCom->Get_Scale();

	WorldMatrix.r[CTransform::STATE_RIGHT] = XMVector3Normalize(XMVectorSet(PxTransform.q.getBasisVector0().x, PxTransform.q.getBasisVector0().y, PxTransform.q.getBasisVector0().z, 0.f)) * vScale.x;
	WorldMatrix.r[CTransform::STATE_UP] = XMVector3Normalize(XMVectorSet(PxTransform.q.getBasisVector1().x, PxTransform.q.getBasisVector1().y, PxTransform.q.getBasisVector1().z, 0.f)) * vScale.y;
	WorldMatrix.r[CTransform::STATE_LOOK] = XMVector3Normalize(XMVectorSet(PxTransform.q.getBasisVector2().x, PxTransform.q.getBasisVector2().y, PxTransform.q.getBasisVector2().z, 0.f)) * vScale.z;
	WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(PxTransform.p.x, PxTransform.p.y, PxTransform.p.z, 1.f);
	
	
	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
#ifdef _DEBUG
	_matrix RenderMatrix = WorldMatrix;

	m_pOriginal_OBB->Transform(*m_pOBB, RenderMatrix);
	m_pOriginal_Sphere->Transform(*m_pSphere, RenderMatrix);
#endif

	WorldMatrix.r[CTransform::STATE_POSITION] = XMVector3TransformCoord(XMLoadFloat3(&m_vStartPosition), WorldMatrix);
	
	
	

	m_bFirst = false;
}



#ifdef _DEBUG
HRESULT CPhysX_Body::Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());


	m_pEffect->SetView(GI->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW));
	m_pEffect->SetProjection(GI->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();
	_float4 vColor = _float4(1.f, 0.f, 0.f, 1.f);

	if (m_eColliderType == PHYSX_COLLIDER_TYPE::BOX)
		DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&vColor));
	else if (m_eColliderType == PHYSX_COLLIDER_TYPE::SPHERE)
		DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&vColor));

	m_pBatch->End();

	return S_OK;
}

#endif


void CPhysX_Body::Set_Sleep(_bool bSleep)
{
	m_pXBody->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, bSleep);
}

_bool CPhysX_Body::Is_Sleep()
{
	return m_pXBody->isSleeping();
}

void CPhysX_Body::Add_Force(_vector vDir, _float fForce, _bool bClear)
{
	if (true == bClear)
		m_pXBody->clearForce();

	_float3 vForce = {};
 	XMStoreFloat3(&vForce, XMVector3Normalize(vDir) * fForce);
	m_pXBody->addForce(PxVec3(vForce.x, vForce.y, vForce.z), PxForceMode::eFORCE);
}

void CPhysX_Body::Add_Velocity(_vector vDir, _float fForce, _bool bClear)
{
	PxVec3 vVelocity = { 0.f, 0.f, 0.f };

	if (false == bClear)
		vVelocity += m_pXBody->getLinearVelocity();

	_float3 vInputVelocity = {};
	XMStoreFloat3(&vInputVelocity, vDir * fForce);
	vVelocity += PxVec3(vInputVelocity.x, vInputVelocity.y, vInputVelocity.z);

	m_pXBody->setLinearVelocity(vVelocity);
}



void CPhysX_Body::Set_Velocity(_float3 vVelocity)
{
	m_pXBody->setLinearVelocity(PxVec3(vVelocity.x, vVelocity.y, vVelocity.z));
}

_float3 CPhysX_Body::Get_Velocity()
{
	PxVec3 vPhysXVel = m_pXBody->getLinearVelocity();
	_float3 vVelocity = { vPhysXVel.x, vPhysXVel.y, vPhysXVel.z };
	return vVelocity;
}



CPhysX_Body* CPhysX_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPhysX_Body* pInstance = new CPhysX_Body(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Crate Failed. : CPhysX_Body");
		return nullptr;
	}
	return pInstance;
}

CComponent* CPhysX_Body::Clone(void* pArg)
{
	CPhysX_Body* pInstance = new CPhysX_Body(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CPhysX_Body");
		return nullptr;
	}

	return pInstance;
}

void CPhysX_Body::Free()
{
	__super::Free();
	// m_pXBody는 PhysXManager에서 자동으로 삭제되낟.
	
#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}
	Safe_Release(m_pInputLayout);
	

	Safe_Delete(m_pOriginal_OBB);
	Safe_Delete(m_pOBB);

	Safe_Delete(m_pOriginal_Sphere);
	Safe_Delete(m_pSphere);
#endif

	Safe_Release(m_pTransformCom);
}


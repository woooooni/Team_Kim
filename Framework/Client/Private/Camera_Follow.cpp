#include "stdafx.h"
#include "Camera_Follow.h"

#include "GameInstance.h"

#include "Camera_Manager.h"

CCamera_Follow::CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Follow::CCamera_Follow(const CCamera_Follow & rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Follow::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Camera */
	{
		m_tLerpDist.fCurValue = Cam_Dist_Follow_Default;

		/* 팔로우 카메라에서 룩앳 오프셋을 사용하는 일은 없다. 타겟 오프셋만을 사용한다.*/
		m_vLookAtOffset = Vec4::UnitW;
		m_vTargetOffset = Vec4{ 1.2f, 1.3f, 0.f, 1.f };

		m_vMouseSensitivity = Vec2{ 0.2f, 0.35f };
	}

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::Tick(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Calculate_WorldPosition(fTimeDelta));
	m_pTransformCom->LookAt(Calculate_Look(fTimeDelta));
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Follow::Render()
{
	return S_OK;
}

HRESULT CCamera_Follow::Ready_Components()
{
	return S_OK;
}

Vec4 CCamera_Follow::Calculate_WorldPosition(_float fTimeDelta)
{
	/* 구면 로컬 위치 (반지름 1 기준) */
	Vec4 vLocalSpherical = Calculate_LoaclSphericalPosition(fTimeDelta);

	/* 디스턴스 반영 */
	vLocalSpherical *= m_tLerpDist.fCurValue;
	
	/* 카메라 목표 월드 위치  */
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	Vec4 vWorldGoal = vLocalSpherical
		+ Vec4(pTargetTransform->Get_Position());											 /* 타겟 포지션 */
		+ Calculate_ReleativePosition(m_vTargetOffset, pTargetTransform->Get_WorldMatrix()); /* 타겟의 회전을 반영한 오프셋 */

	vWorldGoal.w = 1.f;

	/* 댐핑 적용 월드 위치 (카메라의 현재 위치와 목표위치를 댐핑 계수에 따라 보간한다)*/
	if (m_tDampingDesc.bDamping)
		return Calculate_DampingPosition(vWorldGoal);

	return vWorldGoal;
}

Vec4 CCamera_Follow::Calculate_LoaclSphericalPosition(_float fTimeDelta)
{
	_long	MouseMove = 0l;

	if (MouseMove = GI->Get_DIMMoveState(DIMM_X))
		m_vAngle.x += MouseMove * m_vMouseSensitivity.y * fTimeDelta * -1.f;

	if (MouseMove = GI->Get_DIMMoveState(DIMM_Y))
	{
		m_vAngle.y += MouseMove * m_vMouseSensitivity.x * fTimeDelta;

		if (m_vAngle.y <= 0.7f) /* Min : 0.f */
		{
			m_vAngle.y = 0.7f;
		}
		else if (2.5f < m_vAngle.y) /* Max : 3.14*/
		{
			m_vAngle.y = 2.5f;
		}
	}

	/* 구면 좌표계(극좌표계) -> 왼손 직교 좌표계 */
	Vec4 vCamLocal
	{
		1.f * sinf(m_vAngle.y) * cosf(m_vAngle.x),	// x = r * sin(위도 앙각) * cos(경도 방위각)
		1.f * cosf(m_vAngle.y),						// y = r * cos(위도 앙각)
		1.f * sinf(m_vAngle.y) * sinf(m_vAngle.x),	// z = r * sin(위도 앙각) * sin(경도 방위각)
		0.f
	};

	return vCamLocal;
}

Vec4 CCamera_Follow::Calculate_Look(_float fTimeDelta)
{
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	Vec4 vPosition_LookAtObject = Vec4(pTargetTransform->Get_Position());

	Vec4 vPosition_Offset = Calculate_ReleativePosition(m_vTargetOffset, m_pTransformCom->Get_WorldMatrix()); /* 카메라의 회전을 반영한 타겟 오프셋 */

	return Vec4(vPosition_LookAtObject + vPosition_Offset).OneW();
}

Vec4 CCamera_Follow::Calculate_ReleativePosition(Vec4 vPos, Matrix matWorld)
{
	/* 월드형렬에서 회전상태만을 반영하여 포지션을 적용한다. */

	/* 행렬의 포지션 초기화 */
	matWorld.Translation(Vec3::Zero);

	/* 행렬의 라업룩 정규화 */
	matWorld.Right(XMVector3Normalize(matWorld.Right()));
	matWorld.Up(XMVector3Normalize(matWorld.Up()));
	matWorld.Backward(XMVector3Normalize(matWorld.Backward()));
	
	/* 행렬의 회전 적용된 포지션 값 */
	Vec3 vRelativePos = XMVector3TransformCoord(vPos, matWorld);

	return Vec4(vRelativePos.x, vRelativePos.y, vRelativePos.z, 1.f);
}

Vec4 CCamera_Follow::Calculate_DampingPosition(Vec4 vGoalPos)
{
	if (!m_tDampingDesc.bSet) /* 댐핑이 켜졌지만, 최초 세팅이 안 된 경우 세팅 한다. */
	{
		m_tDampingDesc.bSet = true;
		m_tDampingDesc.vCurPos = vGoalPos;

		return m_tDampingDesc.vCurPos;
	}
	else /* 이전에 세팅이 이루어 졌다면 댐핑 계산을 적용한다. */
	{
		Vec4 vDist = (vGoalPos.ZeroW() - m_tDampingDesc.vCurPos.ZeroW()) * m_tDampingDesc.fDampingCoefficient;

		m_tDampingDesc.vCurPos += vDist;

		return m_tDampingDesc.vCurPos.OneW();
	}
	
	return vGoalPos;
}

CCamera_Follow * CCamera_Follow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Follow::Clone(void* pArg)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}

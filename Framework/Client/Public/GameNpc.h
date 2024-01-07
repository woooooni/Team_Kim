#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CRigidBody;
class CPipeLine;
class CStateMachine;
class CNavigation;
class CHierarchyNode;
class CPhysX_Controller;
END


BEGIN(Client)
class CGameNpc abstract : public CGameObject
{
public:
	enum NPC_STATE { NPC_IDLE, NPC_TALK, NPC_MOVE_ONEWAY, NPC_MOVE_TWOWAY, NPC_END };

public:
	typedef struct tagNpcStat
	{
		_uint iLevel = 1;

		_uint iAtt = 1;
		_uint iDef = 1;

		_int iHp = 100;
		_int iMaxHp = 100;

		_float fSpeed = 0.f;

		// 툴에서 헷갈리지 않게.
		_bool bHasMove = true;
		_bool bHasTalk = true;
		_bool bHasIdle = true;
		_bool bIsUnique = false;
	}NPC_STAT;

protected:
	CGameNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CGameNpc(const CGameNpc& rhs);
	virtual ~CGameNpc() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

public:
	virtual void Set_Infinite(_float fInfiniteTime, _bool bInfinite);
	_bool Is_Infinite() { return m_bInfinite; }

public:
	const NPC_STAT& Get_Stat() { return m_tStat; }
	void Set_Stat(const NPC_STAT& StatDesc) { m_tStat = StatDesc; }

	vector<_vector>* Get_RoamingArea() { return &m_vecRoaming; }
	_vector			 Get_RoamingIndex(_int iIndex) { return m_vecRoaming[iIndex]; }
	void			 Set_RoamingArea(vector<_vector> vecRoaming) { m_vecRoaming = vecRoaming; }

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States() PURE;
	virtual HRESULT Ready_Colliders();
	virtual void On_Damaged(const COLLISION_INFO& tInfo);

private:
	// 렌더러에 넘겨줄 애니메이션 인스턴싱 정보.
	ANIMODEL_INSTANCE_DESC m_AnimInstanceDesc = {};

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	class CShader* m_pShaderCom = nullptr;
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CModel* m_pModelCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	class CStateMachine* m_pStateCom = nullptr;
	class CNavigation* m_pNavigationCom = nullptr;
	class CPhysX_Controller* m_pControllerCom = nullptr;

	class CWeapon* m_pWeapon = nullptr;

protected:
	vector<class CHierarchyNode*>		m_Sockets;
	typedef vector<CGameObject*>		Sockets;

protected:
	_float m_fAccInfinite = 0.f;
	_float m_fInfiniteTime = 5.f;
	_bool m_bInfinite = false;

	NPC_STAT m_tStat = {};

	vector<_vector> m_vecRoaming = {};
	_vector			m_vInitPos = {};

	// 블룸 효과
	_float3 m_vBloomPower = _float3(1.f, 1.f, 1.f);

	// 일반, 유니크 구분
	_bool m_bIsUnique = false;

public:
	virtual void Free() override;
};

END

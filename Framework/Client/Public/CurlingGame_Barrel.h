#pragma once

#include "Client_Defines.h"
#include "CurlingGame_Prop.h"

BEGIN(Client)

class CCurlingGame_Barrel final : public CCurlingGame_Prop
{
public:
	typedef struct tagElasticCollisionDesc
	{
		_bool bSet = false;
		Vec3 vVelocity = Vec3::Zero;

		void Set(const Vec3 v)
		{
			vVelocity = v;
			bSet = true;
		}

		void Clear()
		{
			vVelocity = Vec3::Zero;
			bSet = false;
		}

	}ELASTIC_COLL_DESC;

private:
	CCurlingGame_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCurlingGame_Barrel(const CCurlingGame_Barrel& rhs);
	virtual ~CCurlingGame_Barrel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	void PutDown();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Colliders() override;

private:
	HRESULT Calculate_ElasticCollision(CGameObject* pOther);
	HRESULT Calculate_ActionAndReaction(class CCurlingGame_Wall* pWall);

private:
	ELASTIC_COLL_DESC	m_tElasticColDesc	= {};
	LERP_FLOAT_DESC		m_tHeightLerpDesc	= {};

	_uint				m_iNumCol			= 0;

public:
	static CCurlingGame_Barrel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;

public:
	virtual void Free() override;
};

END


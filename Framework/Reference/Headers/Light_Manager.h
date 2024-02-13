#pragma once

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);

public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);

	HRESULT Add_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp);
	HRESULT Set_ShadowLight(_uint iLevelIndex, _vector vEye, _vector vAt, _vector vUp);

	HRESULT Add_ShadowProj(_uint iLevelIndex, _float fFovAngleY, _float fAspectRatio, _float fNearZ, _float fFarZ);

	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


public:
	_float4x4 Get_ShadowLightViewMatrix(_uint iLevelIndex);
	_float4x4 Get_ShadowLightViewMatrix_Inverse(_uint iLevelIndex);

	_float4x4 Get_ShadowLightProjMatrix(_uint iLevelIndex);

	XMMATRIX GetLightViewMatrix();
public:
	list<class CLight*>* Get_LightList() { return &m_Lights; }


public:
	HRESULT Reset_Lights();
	HRESULT	Add_Sun(class CGameObject* pSun);

	void SetMainSun(_bool appear) { m_bDisappear = appear; }
	const _bool MainSunAppear() const { return m_bDisappear; }

	Vec4 Get_SunScreenPos();
	const class CGameObject* Get_Sun() const { return m_pSun; }

	

private:
	list<class CLight*>			m_Lights;
	typedef list<class CLight*>	LIGHTS;
	class  CGameObject* m_pSun = nullptr;

private:
	map<_uint, _float4x4> m_ShadowViewMatrix;
	map<_uint, _float4x4> m_ShadowProjMatrix;

private:
	_bool m_bDisappear = false;


public:
	void CascadeShadowGen(ID3D11DeviceContext* pContext);
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

private:
	ID3D11Texture2D* m_pCascadeDepthStencilRT = nullptr;
	ID3D11DepthStencilView* m_pCascadeDepthStencilDSV = nullptr;
	ID3D11ShaderResourceView* m_pCascadeDepthStencilSRV = nullptr;

public:
	virtual void Free() override;
};

END
#pragma once

/* 렌더타겟들을 생성해서 모아둔다. */
#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

private:
	HRESULT Ready_Shadow_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY);
	HRESULT Ready_Minimap_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY);
	HRESULT Ready_UI_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY);
	HRESULT	Ready_Cascade_Shadow_DSV(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY);

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, _uint iWinSizeX, _uint iWinSizeY);
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vColor, _uint iArraySize = 1);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	
	HRESULT Bind_SRV(class CShader* pShader, const wstring& strTargetTag, const _char* pConstantName);

	/* strMRTTag에 해당하는 list에 담겨있는 타겟들을 장치에 바인딩한다. */
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, _bool bClear = true);
	HRESULT Begin_Minimap_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, _bool bClear = true);
	HRESULT Begin_Shadow_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag);
	HRESULT Begin_UI_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, _bool bClear = true, _bool bStencil = true);
	HRESULT Begin_CascadeShadow_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag);

	HRESULT Clear_RenderTarget(const wstring& strTargetTag);

	/* 다시 원래 상태로 복구한다. */
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

public:
	// Cascade 
	ID3D11DepthStencilView* Get_Cascade_DSV() { return m_pCascadeDSV; }
	ID3D11ShaderResourceView* Get_Cascasd_SRV() { return m_pCascadeSRV; }
	ID3D11DepthStencilView* Get_BackBuffer() { return m_pDSV; }
	ID3D11RenderTargetView** Get_PrevRTVs() { return m_pPrevRTVs; }

	void SetRenderTarget(ID3D11DeviceContext* pContext, int nCascadeIndex);
	void ClearCascadeDepthBuffer(ID3D11DeviceContext* pContext);

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif

private:
	map<const wstring, class CRenderTarget*>			m_RenderTargets;	

	/* 장치에 동시에 바인딩되어야하는 타겟들을 미리 묶어두겠다. */
	map<const wstring, list<class CRenderTarget*>>		m_MRTs;

private:
	// ID3D11RenderTargetView*					m_pBackBufferRTV = { nullptr };
	ID3D11RenderTargetView*					m_pPrevRTVs[8];
	ID3D11DepthStencilView*					m_pDSV = { nullptr };
	ID3D11DepthStencilView*					m_pMinimapDSV = { nullptr };
	ID3D11DepthStencilView*					m_pShadowDSV = { nullptr };
	ID3D11DepthStencilView*                 m_pUIDSV = { nullptr };

	ID3D11DepthStencilView*					m_pCascadeDSV = nullptr;
	ID3D11ShaderResourceView*				m_pCascadeSRV = nullptr;

	_int m_nTextureWidth = 0;
	_int m_nTextureHeight = 0;
private:
	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring & strMRTTag);


public:
	virtual void Free() override;
};

END
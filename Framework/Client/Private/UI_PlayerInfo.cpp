#include "stdafx.h"
#include "UI_PlayerInfo.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_PlayerInfo::CUI_PlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_PlayerInfo")
{
}

CUI_PlayerInfo::CUI_PlayerInfo(const CUI_PlayerInfo& rhs)
	: CUI(rhs)
{
}

void CUI_PlayerInfo::Set_Active(_bool bActive)
{
	if (bActive)
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(true);
		}
	}
	else
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(false);
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_PlayerInfo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerInfo::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Make_Child(51.f, -11.f, 125.f, 16.f, TEXT("Prototype_GameObject_UI_Player_HPBar"));

	return S_OK;
}

void CUI_PlayerInfo::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_PlayerInfo::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// Todo : Player구조가 정리되면 Set_Level로 빼서 UIManager로 연동하자.

		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			CRenderer::TEXT_DESC LevelDesc;
			LevelDesc.strText = L"5"; // Temp
			LevelDesc.strFontTag = L"Default_Bold";
			LevelDesc.vPosition = m_vDefaultPosition;
			LevelDesc.vColor = m_vShadowColor;
			LevelDesc.vScale = { 0.8f, 0.8f };
			m_pRendererCom->Add_Text(LevelDesc); // 그림자

			LevelDesc.vPosition = _float2(m_vDefaultPosition.x - 3.f, m_vDefaultPosition.y - 3.f);
			LevelDesc.vColor = m_vLevelColor;
			m_pRendererCom->Add_Text(LevelDesc); // Real Text

			// 전투력 외곽선
			CRenderer::TEXT_DESC  TextDesc;
			TextDesc.strText = L"전투력"; // Temp
			TextDesc.strFontTag = L"Default_Medium";
			TextDesc.vPosition = _float2(m_vTextPosition.x - 1.f, m_vTextPosition.y);
			TextDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
			TextDesc.vScale = { 0.38f, 0.38f };
			m_pRendererCom->Add_Text(TextDesc);
			TextDesc.vPosition = _float2(m_vTextPosition.x + 1.f, m_vTextPosition.y);
			m_pRendererCom->Add_Text(TextDesc);
			TextDesc.vPosition = _float2(m_vTextPosition.x, m_vTextPosition.y - 1.f);
			m_pRendererCom->Add_Text(TextDesc);
			TextDesc.vPosition = _float2(m_vTextPosition.x, m_vTextPosition.y + 1.f);
			m_pRendererCom->Add_Text(TextDesc);

			// 전투력
			TextDesc.vPosition = m_vTextPosition;
			TextDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(TextDesc);

			// Todo : 전투력을 받아오게끔 구조 변경 필요함.
			// 전투력 숫자 외곽선
			CRenderer::TEXT_DESC Power;
			Power.strText = L"111111";
			Power.strFontTag = L"Default_Medium";
			Power.vPosition = _float2(m_vNumPosition.x - 2.f, m_vNumPosition.y);
			Power.vColor = _float4(0.f, 0.f, 0.f, 1.f);
			Power.vScale = { 0.38f, 0.38f };
			m_pRendererCom->Add_Text(Power);
			Power.vPosition = _float2(m_vNumPosition.x + 2.f, m_vNumPosition.y);
			m_pRendererCom->Add_Text(Power);
			Power.vPosition = _float2(m_vNumPosition.x, m_vNumPosition.y - 2.f);
			m_pRendererCom->Add_Text(Power);
			Power.vPosition = _float2(m_vNumPosition.x, m_vNumPosition.y + 2.f);
			m_pRendererCom->Add_Text(Power);

			//
			Power.vPosition = _float2(200.f, 56.f);
			Power.vColor = m_vPowerColor;
			m_pRendererCom->Add_Text(Power);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_PlayerInfo::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_PlayerInfo::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_PlayerInfo::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_PlayerInfo::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_PlayerInfo* CUI_PlayerInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerInfo* pInstance = new CUI_PlayerInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PlayerInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerInfo::Clone(void* pArg)
{
	CUI_PlayerInfo* pInstance = new CUI_PlayerInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PlayerInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerInfo::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}

#include "stdafx.h"
#include "UIDamage_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include <filesystem>
#include "FileUtils.h"
#include "Utils.h"

#include "UI_Damage_Skill.h"
#include "UI_Damage_General.h"
#include "UI_Damage_Critical.h"


IMPLEMENT_SINGLETON(CUIDamage_Manager)

CUIDamage_Manager::CUIDamage_Manager()
{

}

HRESULT CUIDamage_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	
	return S_OK;
}

void CUIDamage_Manager::Tick(_float fTimeDelta)
{

}

HRESULT CUIDamage_Manager::Ready_DamageNumberPrototypes()
{
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Blue"), // Water -> Weakness
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::BLUE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Red"), // Fire -> PlayerDamage
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::RED), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_WhiteGold"), // Light
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::WHITEGOLD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Green"), // Wood
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::GREEN), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Purple"), // Dark
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::PURPLE), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Gold"),
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::GOLD), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_GoldWithRed"), // Strong
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::GOLD_WITHRED), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_White"),
		CUI_Damage_Skill::Create(m_pDevice, m_pContext, CUI_Damage_Skill::UI_DAMAGEFONT::WHITE), LAYER_UI)))
		return E_FAIL;

	// Critical
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Critical"),
		CUI_Damage_Critical::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	// General
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_General"),
		CUI_Damage_General::Create(m_pDevice, m_pContext, CUI_Damage_General::UI_DMG_FONTTYPE::DMG_GENERALATTACK), LAYER_UI)))
		return E_FAIL;

	// Miss
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_DamageNumber_Miss"),
		CUI_Damage_General::Create(m_pDevice, m_pContext, CUI_Damage_General::UI_DMG_FONTTYPE::DMG_MISS), LAYER_UI)))
		return E_FAIL;

	return S_OK;
}

_int CUIDamage_Manager::Calculate_DamageColor(ELEMENTAL_TYPE eMonsterType, ELEMENTAL_TYPE eWeaponType, _int iDamage)
{
	if (ELEMENTAL_TYPE::BASIC <= eMonsterType || ELEMENTAL_TYPE::BASIC <= eWeaponType)
		return -1;

	if (999999 < iDamage)
	{
		iDamage = 999999;
		// 혹은 return -1;
	}

	UI_DAMAGEFONTCOLOR eColor = UI_DAMAGEFONTCOLOR::FONT_DAMAGECOLOR_END;

	switch (eMonsterType)
	{
	case ELEMENTAL_TYPE::FIRE: // 불 몬스터
		if (ELEMENTAL_TYPE::WATER == eWeaponType) // 물 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else if (ELEMENTAL_TYPE::WOOD == eWeaponType) // 나무 무기 -> Weakness
		{
			eColor = UI_DAMAGEFONTCOLOR::FONT_BLUE;
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::WATER: // 물 몬스터
		if (ELEMENTAL_TYPE::WOOD == eWeaponType) // 나무 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else if (ELEMENTAL_TYPE::FIRE == eWeaponType) // 불 무기 -> Weakness
		{
			eColor = UI_DAMAGEFONTCOLOR::FONT_BLUE;
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::WOOD: // 나무 몬스터
		if (ELEMENTAL_TYPE::FIRE == eWeaponType) // 불 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else if (ELEMENTAL_TYPE::WATER == eWeaponType) // 물 무기 -> Weakness
		{
			eColor = UI_DAMAGEFONTCOLOR::FONT_BLUE;
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::LIGHT: // 빛 몬스터
		if (ELEMENTAL_TYPE::DARK == eWeaponType) // 어둠 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;

	case ELEMENTAL_TYPE::DARK: // 어둠 몬스터
		if (ELEMENTAL_TYPE::LIGHT == eWeaponType) // 빛 무기 -> Strong
		{
			if (m_iMaxStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_GOLD; // 기본 공격
		}
		else
		{
			if (m_iStandard <= iDamage)
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD; //Critical Damage
			else
				eColor = UI_DAMAGEFONTCOLOR::FONT_WHITE; // 기본 공격
		}
		break;
	}

	return _int(eColor);
}

HRESULT CUIDamage_Manager::Create_PlayerDamageNumber(CTransform* pTransformCom, _int iDamage)
{
	CUI_Damage_Skill::DAMAGE_DESC DamageDesc = {};
	ZeroMemory(&DamageDesc, sizeof(CUI_Damage_Skill::DAMAGE_DESC));

	DamageDesc.pTargetTransform = pTransformCom;
	DamageDesc.iDamage = iDamage;

	CGameObject* pNumber = nullptr;
	if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_DamageNumber_Red"), &DamageDesc, &pNumber)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_MonsterDamageNumber(CTransform* pTransformCom, UI_DAMAGEFONTCOLOR eType, _int iDamage)
{
	if (UI_DAMAGEFONTCOLOR::FONT_DAMAGECOLOR_END <= eType)
		return E_FAIL;

	CGameObject* pNumber = nullptr;
	CUI_Damage_Skill::DAMAGE_DESC DamageDesc = {};
	ZeroMemory(&DamageDesc, sizeof(CUI_Damage_Skill::DAMAGE_DESC));

	DamageDesc.pTargetTransform = pTransformCom;
	DamageDesc.iDamage = iDamage;
	if (m_iMaxStandard <= iDamage)
		DamageDesc.bCritical = true;
	else
		DamageDesc.bCritical = false;

	switch (eType)
	{
	case UI_DAMAGEFONTCOLOR::FONT_BLUE:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Blue"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case UI_DAMAGEFONTCOLOR::FONT_WHITE:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_White"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case UI_DAMAGEFONTCOLOR::FONT_WHITE_AND_GOLD:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_WhiteGold"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case UI_DAMAGEFONTCOLOR::FONT_GOLD:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_Gold"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;

	case UI_DAMAGEFONTCOLOR::FONT_GOLD_AND_RED:
		if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
			TEXT("Prototype_GameObject_UI_DamageNumber_GoldWithRed"), &DamageDesc, &pNumber)))
			return E_FAIL;
		break;
	}

	if (nullptr == pNumber)
		return E_FAIL;

	return S_OK;
}

//HRESULT CUIDamage_Manager::Create_CommonDamageNumber(CTransform* pTransformCom, _int iDamage)
//{
//	CGameObject* pNumber = nullptr;
//	CUI_Damage_General::GENERAL_DESC DamageDesc = {};
//
//	ZeroMemory(&DamageDesc, sizeof(CUI_Damage_General::GENERAL_DESC));
//
//	DamageDesc.pTargetTransform = pTransformCom;
//	DamageDesc.iDamage = iDamage;
//
//	if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
//		TEXT("Prototype_GameObject_UI_DamageNumber_General"), &DamageDesc, &pNumber)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CUIDamage_Manager::Create_Critical(_uint eDamageFontType, _float2 vPosition)
{
	//enum UI_DAMAGEFONT { BLUE, GOLD_WITHRED, GREEN, PURPLE, RED, WHITE, WHITEGOLD, GOLD, DAMAGEFOND_END };
	CUI_Damage_Critical::UI_CRITICALFONT eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICALFONT_END;

	if (CUI_Damage_Skill::UI_DAMAGEFONT::BLUE == eDamageFontType)
	{
		eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_BLUE;
	}
	else if (CUI_Damage_Skill::UI_DAMAGEFONT::GOLD_WITHRED)
	{
		eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_YELLOW;
//		eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_REDBLUR;
	}
//	else if (CUI_Damage_Skill::UI_DAMAGEFONT::WHITE)
//	{
//		eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_BLUE;
//	}
	else if (CUI_Damage_Skill::UI_DAMAGEFONT::WHITEGOLD)
	{
		eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_REDBLUR;
	}
	else
		return E_FAIL;

	CGameObject* pCritical = nullptr;
	CUI_Damage_Critical::CRITICAL_DESC CriticalDesc = {};
	ZeroMemory(&CriticalDesc, sizeof(CUI_Damage_Critical::CRITICAL_DESC));

	CriticalDesc.vPosition = vPosition;
	// 	enum UI_CRITICALFONT { CRITICAL_BLUE, CRITICAL_YELLOW, CRITICAL_RED, CRITICAL_REDBLUR, CRITICALFONT_END };
	CriticalDesc.eType = CUI_Damage_Critical::UI_CRITICALFONT::CRITICAL_BLUE;

	if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_DamageNumber_Critical"), &CriticalDesc, &pCritical)))
		return E_FAIL;

	if (nullptr == pCritical)
		return E_FAIL;

	return S_OK;
}

HRESULT CUIDamage_Manager::Create_Miss(CTransform* pTransformCom)
{
	CGameObject* pMiss = nullptr;

	CUI_Damage_General::GENERAL_DESC MissDesc = {};
	ZeroMemory(&MissDesc, sizeof(CUI_Damage_General::GENERAL_DESC));

	MissDesc.pTargetTransform = pTransformCom;
	MissDesc.iDamage = 0; // 아무값이나 넣어도 상관없음. 반영X

	if (FAILED(GI->Add_GameObject(_uint(GI->Get_CurrentLevel()), LAYER_TYPE::LAYER_UI,
		TEXT("Prototype_GameObject_UI_DamageNumber_Miss"), &MissDesc, &pMiss)))
		return E_FAIL;

	if (nullptr == pMiss)
		return E_FAIL;

	return S_OK;
}

void CUIDamage_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

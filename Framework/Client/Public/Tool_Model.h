#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Model final : public CTool
{
private:
	CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Tick_Model(_float fTimeDelta);
	void Tick_Animation(_float fTimeDelta);
	void Tick_Socket(_float fTimeDelta);
	void Tick_Event(_float fTimeDelta);
	void Tick_Costume(_float fTimeDelta);

	void Tick_Dummys(_float fTimeDelta);

private:
	HRESULT Render_DebugDraw();

private:
	/* In Initialize */
	HRESULT Ready_DebugDraw();
	HRESULT Ready_Dummy();
	HRESULT Ready_WeaponPrototypes();

	/* In Impoty Animation */
	HRESULT Ready_SocketTransforms();

private:
	void Reset_Transform();
	const _bool Is_Exception();

	HRESULT Clear_ToolAnimationData();

	Vec3 Calculate_SocketPosition();
	Matrix Calculate_SocketWorldMatrix();

private:
	wstring m_strFilePath = L"";
	wstring m_strFileName = L"";

	class CDummy* m_pDummy = nullptr;

#pragma region Animation

	_bool m_bAllAnimLoop = TRUE;

#pragma endregion


#pragma region Socket

	/* Bone */
	_int m_iCurBoneIndex = 0;

	/* ������Ÿ�� ����*/
	vector<class CPart*> m_Weapons;
	_int m_iCurWeaponIndex = -1;

	/* ���� */
	vector<ANIM_TRANSFORM_CACHES>	m_AnimTransformsCaches;/* ��� �� ��� �� */
	vector<wstring>					m_AddedTransformNames; /* �𵨿� �߰��� �� �̸� ���*/

	_int m_iSocketIndex = -1; /* �� ����Ʈ �ڽ������� �ε��� == ���� Ʈ������ ���Ϳ����� �ε��� */

	/* Render Index */
	_int m_iRenderSocketIndex = 0;

#pragma endregion


#pragma region Debug Draw 

	Vec4 m_vColor = { 0.f, 0.f, 1.f, 1.f };
	BasicEffect* m_pEffect = nullptr;
	BoundingSphere* m_pSphere = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;

#pragma endregion

public:
	static CTool_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
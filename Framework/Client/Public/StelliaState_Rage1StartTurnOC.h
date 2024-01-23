#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage1StartTurnOC final : public CStelliaState_Base
{
private:
	CStelliaState_Rage1StartTurnOC(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage1StartTurnOC() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	Vec4 m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;

public:
	static CStelliaState_Rage1StartTurnOC* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END


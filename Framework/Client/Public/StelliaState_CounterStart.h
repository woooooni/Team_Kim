#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_CounterStart final : public CStelliaState_Base
{
private:
	CStelliaState_CounterStart(CStateMachine* pStateMachine);
	virtual ~CStelliaState_CounterStart() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool  m_bIsSlow = false;
	_float m_fTime = 0.f;
	_float m_fSlowTime = 0.f;

	_bool m_bIsTimeSlep = false;

public:
	static CStelliaState_CounterStart* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END


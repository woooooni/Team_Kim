#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_NoisySnowField01 final : public CQuestNode_Base
{
private:
	CSubQuestNode_NoisySnowField01();
	virtual ~CSubQuestNode_NoisySnowField01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pBella = nullptr;

public:
	static CSubQuestNode_NoisySnowField01* Create();
	virtual void Free() override;
};


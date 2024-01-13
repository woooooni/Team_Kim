#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_SnowField02_2 final : public CQuestNode_Base
{
private:
	CMainQuestNode_SnowField02_2();
	virtual ~CMainQuestNode_SnowField02_2() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pKuu = nullptr;

public:
	static CMainQuestNode_SnowField02_2* Create();
	virtual void Free() override;
};


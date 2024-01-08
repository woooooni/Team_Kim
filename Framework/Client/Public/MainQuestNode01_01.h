#pragma once

#include "QuestNode_Base.h"

BEGIN(Client)

class CMainQuestNode01_01 final : public CQuestNode_Base
{
private:
	CMainQuestNode01_01();
	virtual ~CMainQuestNode01_01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	class CKuu* m_pKuu = nullptr;

public:
	static CMainQuestNode01_01* Create();
	virtual void Free() override;
};

END